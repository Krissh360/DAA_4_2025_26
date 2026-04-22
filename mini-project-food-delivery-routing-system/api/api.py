from flask_cors import CORS
from flask import Flask, request, jsonify
import subprocess
import os

BASE_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
FRONTEND_DIR = os.path.join(BASE_DIR, "frontend")
ENGINE_EXE = os.path.join(BASE_DIR, "engine", "core.exe")

app = Flask(__name__, static_folder=FRONTEND_DIR, static_url_path="")
CORS(app)

def run_cpp(args):
    try:
        args = [str(a) for a in args]  # force strings
        out = subprocess.check_output([ENGINE_EXE] + args, text=True)
        return out
    except Exception as e:
        return "Error: " + str(e)


@app.route("/", methods=["GET"])
def frontend_index():
    return app.send_static_file("index.html")


# -------- Locations --------
@app.route("/api/locations", methods=["GET"])
def locations():
    locs = [
        "Sector-17","Sector-22","Sector-8","Sector-35","Panjab University",
        "Sector-43","Elante Mall","PGIMER","Industrial Area Phase 1","Sector-9"
    ]
    return jsonify({"locations": locs})

@app.route("/api/restaurants")
def api_restaurants():
    out = run_cpp(["list_restaurants"])
    items = [x.strip() for x in out.split("\n") if x.strip() != ""]
    return jsonify({"restaurants": items})


# -------- Signup --------
@app.route("/api/signup/user", methods=["POST"])
def api_signup_user():
    d = request.json
    out = run_cpp(["signup_user", d["name"], d["username"], d["email"], str(d["password"])])
    return jsonify({"result": out})

@app.route("/api/signup/admin", methods=["POST"])
def api_signup_admin():
    d = request.json
    out = run_cpp(["signup_admin", d["name"], str(d["password"])])
    return jsonify({"result": out})

@app.route("/api/signup/rider", methods=["POST"])
def api_signup_rider():
    d = request.json
    out = run_cpp(["signup_rider", d["name"], str(d["password"]), str(d["location"])])
    return jsonify({"result": out})

# -------- Login --------
@app.route("/api/login/user", methods=["POST"])
def api_login_user():
    d = request.json
    # ✅ use email instead of username
    out = run_cpp(["login_user", d["email"], str(d["password"])])
    return jsonify({"result": out})

@app.route("/api/login/admin", methods=["POST"])
def api_login_admin():
    d = request.json
    out = run_cpp(["login_admin", d["name"], str(d["password"])])
    return jsonify({"result": out})

@app.route("/api/login/rider", methods=["POST"])
def api_login_rider():
    d = request.json
    out = run_cpp([
        "login_rider",
        d["name"],
        str(d["password"])
    ])
    return jsonify({"result": out})



# -------- Admin actions --------
@app.route("/api/admin/users", methods=["GET"])
def api_view_users():
    out = run_cpp(["view_users"])
    return jsonify({"result": out})

@app.route("/api/admin/riders", methods=["GET"])
def api_view_riders():
    out = run_cpp(["view_riders"])
    return jsonify({"result": out})

@app.route("/api/admin/orders", methods=["GET"])
def api_view_orders():
    out = run_cpp(["view_orders"])
    return jsonify({"result": out})

@app.route("/api/admin/remove_user", methods=["POST"])
def api_remove_user():
    d = request.json
    out = run_cpp(["remove_user", str(d["id"])])
    return jsonify({"result": out})

@app.route("/api/admin/remove_rider", methods=["POST"])
def api_remove_rider():
    d = request.json
    out = run_cpp(["remove_rider", str(d["id"])])
    return jsonify({"result": out})

@app.route("/api/orders/pending")
def api_pending():
    out = run_cpp(["pending_orders"])
    return jsonify({"result": out})

@app.route("/api/orders/accept", methods=["POST"])
def api_accept():
    d = request.json
    out = run_cpp([
        "accept_order",
        str(d["riderId"]),
        str(d["orderId"])
    ])
    return jsonify({"result": out})


# -------- Orders --------
@app.route("/api/place_order", methods=["POST"])
def api_place_order():
    d = request.json
    try:
        out = run_cpp([
            "place_order",
            str(d["username"]),      # make sure string
            str(d["destination"]),   # core expects the user destination first
            str(d["restaurant"])     # followed by the restaurant location index
        ])
        return jsonify({"result": out})
    except Exception as e:
        return jsonify({"result": f"Error: {e}"})

@app.route("/api/orders/accept", methods=["POST"])
def api_orders_accept():
    d = request.json
    try:
        out = run_cpp([
            "accept_order",
            str(d["riderId"]),
            str(d["orderId"])
        ])
        return jsonify({"result": out})
    except Exception as e:
        return jsonify({"result": f"Error: {e}"})



@app.route("/api/route", methods=["GET"])
def api_route():
    src = request.args.get("src")
    dest = request.args.get("dest")
    out = run_cpp(["show_route", src, dest])
    return jsonify({"result": out})

if __name__ == "__main__":
    app.run(debug=True)
