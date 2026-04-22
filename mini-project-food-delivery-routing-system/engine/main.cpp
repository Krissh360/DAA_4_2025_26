#include "core.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) { std::cout << "No mode provided\n"; return 0; }
    std::string mode = argv[1];
    init_system();

    try {
        if (mode == "signup_user" && argc==6)       std::cout << signup_user(argv[2],argv[3],argv[4],std::stoi(argv[5]));
        else if (mode == "signup_admin" && argc==4) std::cout << signup_admin(argv[2],std::stoi(argv[3]));
        else if (mode == "signup_rider" && argc==5) std::cout << signup_rider(argv[2],std::stoi(argv[3]),std::stoi(argv[4]));
        else if (mode == "login_user"  && argc==4)  std::cout << login_user(argv[2], argv[3]);
        else if (mode == "login_admin" && argc==4)  std::cout << login_admin(argv[2], argv[3]);
        else if (mode == "login_rider" && argc==4)
        std::cout << login_rider(argv[2], argv[3]);
        else if (mode == "place_order" && argc==5) {
        std::cout << place_order(argv[2], std::stoi(argv[3]), std::stoi(argv[4]));
}
        else if (mode == "view_users")              std::cout << view_users();
        else if (mode == "view_riders")             std::cout << view_riders();
        else if (mode == "remove_user" && argc==3)  std::cout << remove_user(std::stoi(argv[2]));
        else if (mode == "remove_rider"&& argc==3)  std::cout << remove_rider(std::stoi(argv[2]));
        else if (mode == "view_orders")             std::cout << view_orders();                
        else if (mode == "pending_orders")          std::cout << list_pending_orders();
        else if (mode == "pending_orders")     std::cout << list_pending_orders();   
        else if (mode == "accept_order" && argc==4)
        std::cout << accept_order(std::stoi(argv[2]), std::stoi(argv[3]));     
        else if (mode == "accept_order" && argc==4) std::cout << accept_order(std::stoi(argv[2]),std::stoi(argv[3]));
        else if (mode == "list_restaurants") {
           std::cout << get_restaurant_names();
           return 0;
}


        else std::cout << "Invalid mode or wrong args\n";
    } catch(...) { std::cout << "Error processing request\n"; }

    return 0;
}
