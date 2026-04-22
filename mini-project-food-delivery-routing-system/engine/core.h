#pragma once
#include <string>
#include <vector>

struct RouteResult {
    std::string routeLine;
    int distanceKm;
    int etaMin;
};

// lifecycle
void init_system();
void save_system();

// signup / login
std::string signup_user(const std::string& name, const std::string& username,
                        const std::string& email, int pass);
std::string signup_admin(const std::string& name, int pass);
std::string signup_rider(const std::string& name, int pass, int location);

std::string login_user(const std::string &email, const std::string &password);
std::string login_admin(const std::string &name, const std::string &password);
std::string login_rider(const std::string &name, const std::string &password);

// admin views/actions
std::string view_users();
std::string view_riders();
std::string remove_user(int id);
std::string remove_rider(int id);
std::string view_orders();                 // NEW: admin order history

// restaurants / locations
const std::vector<std::string>& list_locations();
const std::vector<std::pair<std::string,int>>& list_restaurants(); // name, locationIndex
std::string get_restaurant_names();


// orders (web workflow)
std::string place_order(const std::string& username, int userLocIndex, int restaurantLocIndex);
// user popup already contains ETA (restaurant->user)

// rider workflow
std::string list_pending_orders();                     // human-readable list
std::string accept_order(int riderId, int orderId);    // returns popup text with route+ETA

// helpers
int getIndex(const std::string& src);
std::string getLocation(int index);
RouteResult compute_route(const std::string& source, const std::string& dest);
