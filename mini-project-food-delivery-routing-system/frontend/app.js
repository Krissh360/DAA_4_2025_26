const API = "http://localhost:5000/api";

const RESTAURANT_LOCATION_BY_NAME = {
  "Oven Fresh": 1,
  "Pizza Hut": 3,
  "KFC": 5,
  "Pashtun": 7,
  "Mcdonalds": 9
};

async function api(path, opts = {}) {
  let response;

  try {
    response = await fetch(API + path, {
      headers: { "Content-Type": "application/json" },
      ...opts
    });
  } catch (error) {
    throw new Error("Unable to reach the API. Make sure the Flask server is running on http://localhost:5000.");
  }

  const raw = await response.text();
  let data = {};

  try {
    data = raw ? JSON.parse(raw) : {};
  } catch (error) {
    data = { result: raw || `Request failed with status ${response.status}.` };
  }

  if (!response.ok) {
    throw new Error(data.result || `Request failed with status ${response.status}.`);
  }

  return data;
}

function byId(id) {
  return document.getElementById(id);
}

function val(id) {
  const element = byId(id);
  return element ? element.value.trim() : "";
}

function safeMessage(value) {
  return String(value || "").trim();
}

function inferTone(message = "") {
  const text = message.toLowerCase();

  if (
    text.includes("error") ||
    text.includes("failed") ||
    text.includes("invalid") ||
    text.includes("not found") ||
    text.includes("already") ||
    text.includes("could not")
  ) {
    return "danger";
  }

  if (
    text.includes("successful") ||
    text.includes("thank you") ||
    text.includes("accepted") ||
    text.includes("deleted")
  ) {
    return "success";
  }

  if (text.includes("pending")) {
    return "warning";
  }

  return "info";
}

function setStatus(id, message, tone = inferTone(message)) {
  const element = byId(id);
  if (!element) {
    return;
  }

  if (!message) {
    element.hidden = true;
    element.textContent = "";
    delete element.dataset.tone;
    return;
  }

  element.hidden = false;
  element.dataset.tone = tone;
  element.textContent = message;
}

function setOutput(id, message, tone = "info") {
  const element = byId(id);
  if (!element) {
    return;
  }

  element.dataset.tone = tone;
  element.textContent = message || "No data available.";
}

function showToast(message, tone = inferTone(message), detail = "") {
  if (!message) {
    return;
  }

  let stack = byId("toast_stack");
  if (!stack) {
    stack = document.createElement("div");
    stack.id = "toast_stack";
    stack.className = "toast-stack";
    document.body.appendChild(stack);
  }

  const toast = document.createElement("div");
  toast.className = "toast";
  toast.dataset.tone = tone;

  const title = document.createElement("div");
  title.textContent = message;
  toast.appendChild(title);

  if (detail) {
    const note = document.createElement("small");
    note.textContent = detail;
    toast.appendChild(note);
  }

  stack.appendChild(toast);

  window.setTimeout(() => {
    toast.remove();
    if (!stack.childElementCount) {
      stack.remove();
    }
  }, 4200);
}

async function withButtonState(buttonId, loadingLabel, task) {
  const button = byId(buttonId);
  const originalText = button ? button.textContent : "";

  if (button) {
    button.disabled = true;
    button.textContent = loadingLabel;
  }

  try {
    return await task();
  } finally {
    if (button) {
      button.disabled = false;
      button.textContent = originalText;
    }
  }
}

function ensureFields(ids, statusId) {
  const firstMissing = ids.find((id) => !val(id));
  if (!firstMissing) {
    return true;
  }

  const message = "Please fill in all required fields before continuing.";
  setStatus(statusId, message, "warning");
  showToast("Incomplete form", "warning", message);
  byId(firstMissing)?.focus();
  return false;
}

function ensureNumeric(id, statusId, label) {
  const value = val(id);
  if (/^\d+$/.test(value)) {
    return true;
  }

  const message = `${label} must use digits only.`;
  setStatus(statusId, message, "warning");
  showToast("Invalid input", "warning", message);
  byId(id)?.focus();
  return false;
}

function clearFields(ids) {
  ids.forEach((id) => {
    const element = byId(id);
    if (element) {
      element.value = "";
    }
  });
}

function setLoginMode(mode) {
  const panels = document.querySelectorAll("[data-auth-panel]");
  const buttons = document.querySelectorAll("[data-auth-button]");

  panels.forEach((panel) => {
    panel.classList.toggle("is-active", panel.dataset.authPanel === mode);
  });

  buttons.forEach((button) => {
    button.classList.toggle("is-active", button.dataset.authButton === mode);
  });

  setStatus("login_feedback", "");
}

function setSignupMode(mode) {
  const panels = document.querySelectorAll("[data-signup-panel]");
  const buttons = document.querySelectorAll("[data-signup-button]");

  panels.forEach((panel) => {
    panel.classList.toggle("is-active", panel.dataset.signupPanel === mode);
  });

  buttons.forEach((button) => {
    button.classList.toggle("is-active", button.dataset.signupButton === mode);
  });
}

function setEntryMode(mode, syncHash = true) {
  const panels = document.querySelectorAll("[data-entry-panel]");
  const buttons = document.querySelectorAll("[data-entry-button]");

  panels.forEach((panel) => {
    panel.classList.toggle("is-active", panel.dataset.entryPanel === mode);
  });

  buttons.forEach((button) => {
    button.classList.toggle("is-active", button.dataset.entryButton === mode);
  });

  if (syncHash) {
    const hash = mode === "signup" ? "#register" : "#login";
    if (window.location.hash !== hash) {
      window.history.replaceState(null, "", hash);
    }
  }
}

function showEntry(mode) {
  setEntryMode(mode);
}

function showUser() {
  setLoginMode("user");
}

function showRider() {
  setLoginMode("rider");
}

function showAdmin() {
  setLoginMode("admin");
}

function showSignupUser() {
  setSignupMode("user");
}

function showSignupAdmin() {
  setSignupMode("admin");
}

function showSignupRider() {
  setSignupMode("rider");
}

document.addEventListener("DOMContentLoaded", initPage);

function initPage() {
  const page = document.body.dataset.page;

  if (page === "auth") {
    const mode = window.location.hash.toLowerCase() === "#register" ? "signup" : "login";
    setEntryMode(mode, false);
    setLoginMode("user");
    setSignupMode("user");
    loadLocations("sr_loc");
  }

  if (page === "login") {
    setLoginMode("user");
  }

  if (page === "signup") {
    setSignupMode("user");
    loadLocations("sr_loc");
  }

  if (page === "user-dashboard") {
    loadRestaurants("ud_rest");
    loadLocations("ud_dest");
  }

  if (page === "rider-dashboard") {
    viewPending();
  }
}

async function loadLocations(id) {
  const select = byId(id);
  if (!select) {
    return;
  }

  try {
    const data = await api("/locations");
    select.innerHTML = "";

    data.locations.forEach((name, index) => {
      const option = document.createElement("option");
      option.value = index;
      option.textContent = name;
      select.appendChild(option);
    });
  } catch (error) {
    showToast("Unable to load locations", "danger", error.message);
  }
}

async function loadRestaurants(id) {
  const select = byId(id);
  if (!select) {
    return;
  }

  try {
    const data = await api("/restaurants");
    select.innerHTML = "";

    data.restaurants.forEach((restaurant) => {
      const name = typeof restaurant === "string" ? restaurant : restaurant.name;
      const locationIndex = typeof restaurant === "string"
        ? RESTAURANT_LOCATION_BY_NAME[name]
        : restaurant.locationIndex;

      const option = document.createElement("option");
      option.value = locationIndex ?? "";
      option.textContent = name;
      select.appendChild(option);
    });
  } catch (error) {
    setStatus("ud_status", error.message, "danger");
    showToast("Unable to load restaurants", "danger", error.message);
  }
}

async function signupUser() {
  if (!ensureFields(["su_name", "su_username", "su_email", "su_pass"], "su_status")) {
    return;
  }

  if (!ensureNumeric("su_pass", "su_status", "Password")) {
    return;
  }

  await withButtonState("btn-signup-user", "Creating user...", async () => {
    try {
      const body = JSON.stringify({
        name: val("su_name"),
        username: val("su_username"),
        email: val("su_email"),
        password: Number(val("su_pass"))
      });

      const data = await api("/signup/user", { method: "POST", body });
      const message = safeMessage(data.result);
      const tone = inferTone(message);

      setStatus("su_status", message, tone);
      showToast(tone === "success" ? "User account created" : "User signup finished", tone, message);

      if (tone === "success") {
        clearFields(["su_name", "su_username", "su_email", "su_pass"]);
      }
    } catch (error) {
      setStatus("su_status", error.message, "danger");
      showToast("User signup failed", "danger", error.message);
    }
  });
}

async function signupAdmin() {
  if (!ensureFields(["sa_name", "sa_pass"], "sa_status")) {
    return;
  }

  if (!ensureNumeric("sa_pass", "sa_status", "Password")) {
    return;
  }

  await withButtonState("btn-signup-admin", "Creating admin...", async () => {
    try {
      const body = JSON.stringify({
        name: val("sa_name"),
        password: Number(val("sa_pass"))
      });

      const data = await api("/signup/admin", { method: "POST", body });
      const message = safeMessage(data.result);
      const tone = inferTone(message);

      setStatus("sa_status", message, tone);
      showToast(tone === "success" ? "Admin account created" : "Admin signup finished", tone, message);

      if (tone === "success") {
        clearFields(["sa_name", "sa_pass"]);
      }
    } catch (error) {
      setStatus("sa_status", error.message, "danger");
      showToast("Admin signup failed", "danger", error.message);
    }
  });
}

async function signupRider() {
  if (!ensureFields(["sr_name", "sr_pass"], "sr_status")) {
    return;
  }

  if (!ensureNumeric("sr_pass", "sr_status", "Password")) {
    return;
  }

  await withButtonState("btn-signup-rider", "Creating rider...", async () => {
    try {
      const body = JSON.stringify({
        name: val("sr_name"),
        password: Number(val("sr_pass")),
        location: Number(byId("sr_loc").value)
      });

      const data = await api("/signup/rider", { method: "POST", body });
      const message = safeMessage(data.result);
      const tone = inferTone(message);

      setStatus("sr_status", message, tone);
      showToast(tone === "success" ? "Rider account created" : "Rider signup finished", tone, message);

      if (tone === "success") {
        clearFields(["sr_name", "sr_pass"]);
      }
    } catch (error) {
      setStatus("sr_status", error.message, "danger");
      showToast("Rider signup failed", "danger", error.message);
    }
  });
}

async function loginUser() {
  if (!ensureFields(["lu_email", "lu_pass"], "login_feedback")) {
    return;
  }

  await withButtonState("btn-login-user", "Signing in...", async () => {
    try {
      const body = JSON.stringify({
        email: val("lu_email"),
        password: val("lu_pass")
      });

      const data = await api("/login/user", { method: "POST", body });
      const message = safeMessage(data.result);

      if (message === "Login Successful") {
        setStatus("login_feedback", "Login successful. Redirecting to the user dashboard...", "success");
        showToast("Login successful", "success", "Opening the user dashboard.");
        window.setTimeout(() => {
          window.location.href = "user-dashboard.html";
        }, 550);
        return;
      }

      setStatus("login_feedback", message, inferTone(message));
      showToast("User login failed", inferTone(message), message);
    } catch (error) {
      setStatus("login_feedback", error.message, "danger");
      showToast("User login failed", "danger", error.message);
    }
  });
}

async function loginAdmin() {
  if (!ensureFields(["la_name", "la_pass"], "login_feedback")) {
    return;
  }

  await withButtonState("btn-login-admin", "Signing in...", async () => {
    try {
      const body = JSON.stringify({
        name: val("la_name"),
        password: val("la_pass")
      });

      const data = await api("/login/admin", { method: "POST", body });
      const message = safeMessage(data.result);

      if (message === "Login Successful") {
        setStatus("login_feedback", "Login successful. Redirecting to the admin dashboard...", "success");
        showToast("Login successful", "success", "Opening the admin dashboard.");
        window.setTimeout(() => {
          window.location.href = "admin-dashboard.html";
        }, 550);
        return;
      }

      setStatus("login_feedback", message, inferTone(message));
      showToast("Admin login failed", inferTone(message), message);
    } catch (error) {
      setStatus("login_feedback", error.message, "danger");
      showToast("Admin login failed", "danger", error.message);
    }
  });
}

async function loginRider() {
  if (!ensureFields(["lr_name", "lr_pass"], "login_feedback")) {
    return;
  }

  await withButtonState("btn-login-rider", "Signing in...", async () => {
    try {
      const body = JSON.stringify({
        name: val("lr_name"),
        password: val("lr_pass")
      });

      const data = await api("/login/rider", { method: "POST", body });
      const message = safeMessage(data.result);

      if (message === "Login Successful") {
        setStatus("login_feedback", "Login successful. Redirecting to the rider dashboard...", "success");
        showToast("Login successful", "success", "Opening the rider dashboard.");
        window.setTimeout(() => {
          window.location.href = "rider-dashboard.html";
        }, 550);
        return;
      }

      setStatus("login_feedback", message, inferTone(message));
      showToast("Rider login failed", inferTone(message), message);
    } catch (error) {
      setStatus("login_feedback", error.message, "danger");
      showToast("Rider login failed", "danger", error.message);
    }
  });
}

async function placeOrder() {
  if (!ensureFields(["ud_username"], "ud_status")) {
    return;
  }

  await withButtonState("btn-place-order", "Placing order...", async () => {
    try {
      const body = JSON.stringify({
        username: val("ud_username"),
        restaurant: Number(byId("ud_rest").value),
        destination: Number(byId("ud_dest").value)
      });

      const data = await api("/place_order", { method: "POST", body });
      const message = safeMessage(data.result);
      const tone = inferTone(message);

      setStatus("ud_status", message, tone);
      showToast(tone === "success" ? "Order placed" : "Order update", tone, message);
    } catch (error) {
      setStatus("ud_status", error.message, "danger");
      showToast("Order failed", "danger", error.message);
    }
  });
}

async function viewPending() {
  await withButtonState("btn-view-pending", "Refreshing...", async () => {
    try {
      const data = await api("/orders/pending");
      const message = safeMessage(data.result) || "No pending orders yet.";
      setOutput("rd_list", message, "info");
    } catch (error) {
      setOutput("rd_list", error.message, "danger");
      showToast("Unable to load pending orders", "danger", error.message);
    }
  });
}

async function acceptOrder() {
  if (!ensureFields(["rd_riderId", "rd_orderId"], "rd_status")) {
    return;
  }

  await withButtonState("btn-accept-order", "Accepting...", async () => {
    try {
      const body = JSON.stringify({
        riderId: Number(val("rd_riderId")),
        orderId: Number(val("rd_orderId"))
      });

      const data = await api("/orders/accept", { method: "POST", body });
      const message = safeMessage(data.result);
      const tone = inferTone(message);

      setStatus("rd_status", message, tone);
      showToast(tone === "success" ? "Order accepted" : "Order update", tone, message);

      if (tone === "success") {
        viewPending();
      }
    } catch (error) {
      setStatus("rd_status", error.message, "danger");
      showToast("Unable to accept order", "danger", error.message);
    }
  });
}

async function loadAdminView(path, buttonId) {
  await withButtonState(buttonId, "Loading...", async () => {
    try {
      const data = await api(path);
      setOutput("ad_out", safeMessage(data.result), "info");
    } catch (error) {
      setOutput("ad_out", error.message, "danger");
      showToast("Unable to load admin data", "danger", error.message);
    }
  });
}

async function viewUsers() {
  await loadAdminView("/admin/users", "btn-view-users");
}

async function viewRiders() {
  await loadAdminView("/admin/riders", "btn-view-riders");
}

async function viewOrders() {
  await loadAdminView("/admin/orders", "btn-view-orders");
}

async function removeUser() {
  if (!ensureFields(["ad_uid"], "ad_status")) {
    return;
  }

  await withButtonState("btn-remove-user", "Removing...", async () => {
    try {
      const body = JSON.stringify({ id: Number(val("ad_uid")) });
      const data = await api("/admin/remove_user", { method: "POST", body });
      const message = safeMessage(data.result);
      const tone = inferTone(message);

      setStatus("ad_status", message, tone);
      setOutput("ad_out", message, tone);
      showToast(tone === "success" ? "User removed" : "User removal finished", tone, message);
    } catch (error) {
      setStatus("ad_status", error.message, "danger");
      setOutput("ad_out", error.message, "danger");
      showToast("Unable to remove user", "danger", error.message);
    }
  });
}

async function removeRider() {
  if (!ensureFields(["ad_rid"], "ad_status")) {
    return;
  }

  await withButtonState("btn-remove-rider", "Removing...", async () => {
    try {
      const body = JSON.stringify({ id: Number(val("ad_rid")) });
      const data = await api("/admin/remove_rider", { method: "POST", body });
      const message = safeMessage(data.result);
      const tone = inferTone(message);

      setStatus("ad_status", message, tone);
      setOutput("ad_out", message, tone);
      showToast(tone === "success" ? "Rider removed" : "Rider removal finished", tone, message);
    } catch (error) {
      setStatus("ad_status", error.message, "danger");
      setOutput("ad_out", error.message, "danger");
      showToast("Unable to remove rider", "danger", error.message);
    }
  });
}
