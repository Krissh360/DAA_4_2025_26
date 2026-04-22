#include "core.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <string>

static const int INF = 999;
static const int V = 10;

// ---- locations (unchanged from your program) ----
static std::vector<std::string> g_locations = {
    "Sector-17","Sector-22","Sector-8","Sector-35","Panjab University",
    "Sector-43","Elante Mall","PGIMER","Industrial Area Phase 1","Sector-9"
};
const std::vector<std::string>& list_locations(){ return g_locations; }



// ---- restaurant list (names mapped to location indices from your console app) ----
// case 1: Oven fresh  -> index 1 (Sector-22)
// case 3: Pizza Hut    -> index 3 (Sector-35)
// case 5: KFC          -> index 5 (Sector-43)
// case 7: Pashtun      -> index 7 (PGIMER)
// case 9: Mcdonalds    -> index 9 (Sector-9)
static std::vector<std::pair<std::string,int>> g_restaurants = {
    {"Oven Fresh",1},{"Pizza Hut",3},{"KFC",5},{"Pashtun",7},{"Mcdonalds",9}
};
const std::vector<std::pair<std::string,int>>& list_restaurants(){ return g_restaurants; }

std::string get_restaurant_names(){
    std::ostringstream o;
    for(const auto &r : g_restaurants){
        o << r.first << "\n";
    }
    return o.str();
}
// ---- graph ----
static int costMatrix[V][V] = {
    {999, 4, 999, 999, 999, 999, 999, 7, 999, 999},
    {4, 999, 8, 999, 999, 999, 999, 999, 999, 3},
    {999, 8, 999, 7, 999, 4, 999, 999, 2, 999},
    {999, 999, 7, 999, 9, 14, 999, 999, 999, 999},
    {999, 999, 999, 9, 999, 10, 999, 999, 999, 999},
    {999, 999, 4, 14, 10, 999, 999, 999, 999, 999},
    {999, 999, 999, 999, 999, 2, 999, 1, 6, 999},
    {8, 11, 999, 999, 999, 999, 1, 999, 7, 9},
    {999, 999, 2, 999, 999, 999, 6, 7, 999, 1},
    {999, 3, 999, 999, 999, 999, 999, 999, 1, 999}
};

int getIndex(const std::string& s){ for(int i=0;i<(int)g_locations.size();++i) if(g_locations[i]==s) return i; return -1; }
std::string getLocation(int i){ return (i>=0 && i<(int)g_locations.size())? g_locations[i] : "INVALID"; }

// ---- dijkstra (same logic) ----
static int distArr[V], parentArr[V]; static bool visitedArr[V];
static void initD(const std::string& src){ int s=getIndex(src); for(int i=0;i<V;i++){distArr[i]=INF;visitedArr[i]=false;parentArr[i]=i;} if(s>=0&&s<V) distArr[s]=0;}
static int nearest(){ int node=-1,m=INF; for(int i=0;i<V;i++) if(!visitedArr[i]&&distArr[i]<m){m=distArr[i];node=i;} return node; }
static void dijkstra(){ for(int i=0;i<V;i++){ int u=nearest(); if(u==-1) break; visitedArr[u]=true; for(int v=0;v<V;v++) if(costMatrix[u][v]!=INF && distArr[u]+costMatrix[u][v]<distArr[v]){ distArr[v]=distArr[u]+costMatrix[u][v]; parentArr[v]=u; } } }
static int eta_from_km(double km){ return (int)((km/25.0)*60.0); }

RouteResult compute_route(const std::string& s, const std::string& t){
    RouteResult r{};
    int src=getIndex(s), dst=getIndex(t);
    if(src<0||dst<0){ r.routeLine="Invalid source/destination"; r.distanceKm=-1; r.etaMin=-1; return r; }
    initD(s); dijkstra();
    std::vector<std::string> path; int cur=dst;
    while(cur!=src && cur!=-1){ path.push_back(getLocation(cur)); if(parentArr[cur]==cur) break; cur=parentArr[cur]; }
    std::reverse(path.begin(), path.end());
    std::ostringstream oss; oss<<s; for(auto& p:path) oss<<" -> "<<p;
    r.routeLine=oss.str(); r.distanceKm=distArr[dst]; r.etaMin=eta_from_km(r.distanceKm); return r;
}

// ---- storage ----
struct User{ int id; std::string name,username,email; int pass; };
struct Admin{ int id; std::string name; int pass; };
struct Rider{ int id; std::string name; int pass; int location; };
struct Order{ int id; std::string username; int userLoc; int restLoc; int riderId; std::string status; int etaRU; }; // RU ETA

static std::vector<User> g_users; static std::vector<Admin> g_admins; static std::vector<Rider> g_riders; static std::vector<Order> g_orders;
static int g_user_key=0,g_admin_key=0,g_rider_key=0,g_order_key=0;

static std::string path_users   ="../storage/users.txt";
static std::string path_admins  ="../storage/admins.txt";
static std::string path_riders  ="../storage/riders.txt";
static std::string path_orders  ="../storage/orders.txt";

static void split(const std::string& s,std::vector<std::string>& out){ out.clear(); std::stringstream ss(s); std::string t; while(std::getline(ss,t,'|')) out.push_back(t); }

void init_system(){
    g_users.clear(); g_admins.clear(); g_riders.clear(); g_orders.clear();
    g_user_key=g_admin_key=g_rider_key=g_order_key=0;
    // users
    if(std::ifstream f{path_users}){ std::string line; while(std::getline(f,line)){ if(line.empty()) continue; std::vector<std::string> t; split(line,t); if(t.size()==5){ User u{std::stoi(t[0]),t[1],t[2],t[3],std::stoi(t[4])}; g_users.push_back(u); g_user_key=std::max(g_user_key,u.id);} } }
    // admins
    if(std::ifstream f{path_admins}){ std::string line; while(std::getline(f,line)){ if(line.empty()) continue; std::vector<std::string> t; split(line,t); if(t.size()==3){ Admin a{std::stoi(t[0]),t[1],std::stoi(t[2])}; g_admins.push_back(a); g_admin_key=std::max(g_admin_key,a.id);} } }
    // riders
    if(std::ifstream f{path_riders}){ std::string line; while(std::getline(f,line)){ if(line.empty()) continue; std::vector<std::string> t; split(line,t); if(t.size()==4){ Rider r{std::stoi(t[0]),t[1],std::stoi(t[2]),std::stoi(t[3])}; g_riders.push_back(r); g_rider_key=std::max(g_rider_key,r.id);} } }
    // orders
    if(std::ifstream f{path_orders}){ std::string line; while(std::getline(f,line)){ if(line.empty()) continue; std::vector<std::string> t; split(line,t); if(t.size()==7){ Order o{std::stoi(t[0]),t[1],std::stoi(t[2]),std::stoi(t[3]),std::stoi(t[4]),t[5],std::stoi(t[6])}; g_orders.push_back(o); g_order_key=std::max(g_order_key,o.id);} } }
}

void save_system(){
    { std::ofstream f(path_users,std::ios::trunc); for(auto& u:g_users) f<<u.id<<"|"<<u.name<<"|"<<u.username<<"|"<<u.email<<"|"<<u.pass<<"\n"; }
    { std::ofstream f(path_admins,std::ios::trunc); for(auto& a:g_admins) f<<a.id<<"|"<<a.name<<"|"<<a.pass<<"\n"; }
    { std::ofstream f(path_riders,std::ios::trunc); for(auto& r:g_riders) f<<r.id<<"|"<<r.name<<"|"<<r.pass<<"|"<<r.location<<"\n"; }
    { std::ofstream f(path_orders,std::ios::trunc); for(auto& o:g_orders) f<<o.id<<"|"<<o.username<<"|"<<o.userLoc<<"|"<<o.restLoc<<"|"<<o.riderId<<"|"<<o.status<<"|"<<o.etaRU<<"\n"; }
}

// ---- helpers ----
static bool user_exists(const std::string& u){ for(auto& x:g_users) if(x.username==u) return true; return false; }
static bool rider_exists(const std::string& n){ for(auto& x:g_riders) if(x.name==n) return true; return false; }
static bool admin_exists(const std::string& n){ for(auto& x:g_admins) if(x.name==n) return true; return false; }

// ---- auth ----
std::string signup_user(const std::string& name,const std::string& username,const std::string& email,int pass){
    if(user_exists(username)) return "USER ALREADY EXISTS\n";
    int id=++g_user_key; g_users.push_back({id,name,username,email,pass}); save_system();
    std::ostringstream o; o<<"Sign up Successful! User ID: "<<id<<"\n"; return o.str();
}
std::string signup_admin(const std::string& name,int pass){
    if(admin_exists(name)) return "ADMIN ALREADY EXISTS\n";
    int id=++g_admin_key; g_admins.push_back({id,name,pass}); save_system();
    std::ostringstream o; o<<"Sign up Successful! Admin ID: "<<id<<"\n"; return o.str();
}
std::string signup_rider(const std::string& name,int pass,int loc){
    if(loc<0||loc>=V) return "Invalid location index (0-9)\n";
    if(rider_exists(name)) return "RIDER ALREADY EXISTS\n";
    int id=++g_rider_key; g_riders.push_back({id,name,pass,loc}); save_system();
    std::ostringstream o; o<<"Sign up Successful! Rider ID: "<<id<<" | Start: "<<getLocation(loc)<<"\n"; return o.str();
}
std::string login_user(const std::string &email, const std::string &password){
    std::ifstream fin("../storage/users.txt");
    if(!fin.is_open())
        return "Error: Could not open users file.";

    std::string line;
    while(std::getline(fin, line)){
        std::stringstream ss(line);
        std::string part;
        std::vector<std::string> parts;

        while(std::getline(ss, part, '|')){
            parts.push_back(part);
        }

        // Expected format: id | name | username | email | password
        if(parts.size() >= 5){
            if(parts[3] == email && parts[4] == password){
                return "Login Successful";
            }
        }
    }

    return "Login Failed";
}
std::string login_admin(const std::string &name, const std::string &password){
    std::ifstream fin("../storage/admins.txt");
    if(!fin.is_open())
        return "Error: Could not open admins file";

    std::string line;
    while(std::getline(fin, line)){
        std::stringstream ss(line);
        std::string part;
        std::vector<std::string> parts;
        while(std::getline(ss, part, '|')){
            parts.push_back(part);
        }
        // Format: id | name | password
        if(parts.size() >= 3){
            if(parts[1] == name && parts[2] == password){
                return "Login Successful";
            }
        }
    }
    return "Login Failed";
}

std::string login_rider(const std::string &name, const std::string &password){
    std::ifstream fin("../storage/riders.txt");
    if(!fin.is_open())
        return "Error: Could not open riders file";

    std::string line;
    while(std::getline(fin, line)){
        std::stringstream ss(line);
        std::string part;
        std::vector<std::string> parts;

        while(std::getline(ss, part, '|')){
            parts.push_back(part);
        }

        // Format: id | name | pass | location
        if(parts.size() >= 4){
            if(parts[1] == name && parts[2] == password){
                return "Login Successful";
            }
        }
    }
    return "Login Failed";
}



// ---- admin views/actions ----
std::string view_users(){ std::ostringstream o; o<<"---- USERS ----\n"; for(auto& u:g_users) o<<"ID:"<<u.id<<" | "<<u.name<<" | "<<u.username<<" | "<<u.email<<"\n"; return o.str(); }
std::string view_riders(){ std::ostringstream o; o<<"---- RIDERS ----\n"; for(auto& r:g_riders) o<<"ID:"<<r.id<<" | "<<r.name<<" | Loc:"<<getLocation(r.location)<<"\n"; return o.str(); }
std::string view_orders(){ std::ostringstream o; o<<"---- ORDERS ----\n"; for(auto& d:g_orders){ o<<"OID:"<<d.id<<" | user:"<<d.username<<" | from:"<<getLocation(d.restLoc)<<" | to:"<<getLocation(d.userLoc)<<" | status:"<<d.status<<" | riderId:"<<d.riderId<<" | ETA:"<<d.etaRU<<"m\n"; } return o.str(); }
std::string remove_user(int id){ auto it=std::remove_if(g_users.begin(),g_users.end(),[&](auto& u){return u.id==id;}); bool ok=(it!=g_users.end()); g_users.erase(it,g_users.end()); save_system(); return ok?"USER DELETED SUCCESSFULLY\n":"USER NOT FOUND\n"; }
std::string remove_rider(int id){ auto it=std::remove_if(g_riders.begin(),g_riders.end(),[&](auto& r){return r.id==id;}); bool ok=(it!=g_riders.end()); g_riders.erase(it,g_riders.end()); save_system(); return ok?"RIDER DELETED SUCCESSFULLY\n":"RIDER NOT FOUND\n"; }

// ---- orders (web flow) ----
// Place order now creates a PENDING order with ETA = restaurant->user only
std::string place_order(const std::string& username,int userLocIndex,int restaurantLocIndex){
    if(userLocIndex<0||userLocIndex>=V || restaurantLocIndex<0||restaurantLocIndex>=V) return "Invalid indices\n";
    if(!user_exists(username)) return "User not found. Please signup/login first.\n";
    auto rr = compute_route(getLocation(restaurantLocIndex), getLocation(userLocIndex));
    int oid = ++g_order_key;
    g_orders.push_back({oid, username, userLocIndex, restaurantLocIndex, -1, "PENDING", rr.etaMin});
    save_system();

    // find restaurant name by loc index
    std::string rname="Restaurant";
    for(auto& p: g_restaurants) if(p.second==restaurantLocIndex){ rname=p.first; break; }

    std::ostringstream o;
    o << "Thank you for ordering from " << rname << ". Estimated time of delivery: "
      << rr.etaMin << " minutes.";
    return o.str();
}

std::string list_pending_orders(){
    std::ostringstream o;
    o<<"---- PENDING ORDERS ----\n";
    for(auto& x: g_orders){
        if(x.status=="PENDING"){
            o<<"OID:"<<x.id<<" | user:"<<x.username
             <<" | from:"<<getLocation(x.restLoc)<<" | to:"<<getLocation(x.userLoc)
             <<" | ETA:"<<x.etaRU<<"m\n";
        }
    }
    return o.str();
}

std::string accept_order(int riderId,int orderId){
    // find order
    auto it = std::find_if(g_orders.begin(), g_orders.end(), [&](const Order& od){return od.id==orderId;});
    if(it==g_orders.end()) return "Order not found\n";
    if(it->status!="PENDING") return "Order already accepted/completed\n";
    // ensure rider exists
    auto rt = std::find_if(g_riders.begin(), g_riders.end(), [&](const Rider& r){return r.id==riderId;});
    if(rt==g_riders.end()) return "Rider not found\n";

    it->status="ACCEPTED"; it->riderId=riderId; save_system();

    auto rr = compute_route(getLocation(it->restLoc), getLocation(it->userLoc));
    std::ostringstream o;
    o<<"Calculating the shortest path from "<<getLocation(it->restLoc)<<" to "<<getLocation(it->userLoc)<<".\n"
     <<"Shortest path - "<<rr.routeLine<<"\n"
     <<"Estimated time for delivery - "<<rr.etaMin<<" minutes.";
    return o.str();
}
