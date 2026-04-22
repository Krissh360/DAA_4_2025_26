# Food Delivery System
A **C++ based food delivery system** that optimizes delivery routes using **Dijkstra's algorithm**. Built with an **object-oriented programming (OOP) approach**, the system features a **command-line interface (CLI)** and provides dedicated functionalities for **users, riders, and administrators** to efficiently manage food delivery operations.




## Features

### User Interface
- User registration and login system
- Restaurant selection from multiple options:
  - Cheezious
  - Pizza Hut
  - KFC
  - Hardees
  - McDonald's
- Real-time order placement
- Location-based delivery tracking
- Multiple restaurant selection in a single order

### Rider Interface
- Rider registration and login
- Order queue management
- Optimized delivery route visualization
- Real-time order status updates
- Current location tracking

### Admin Interface
- Admin registration and login
- User management (view/remove users)
- Rider management (view/remove riders)
- System oversight capabilities

### Route Optimization
- Implements Dijkstra's algorithm for shortest path finding
- Optimizes routes between:
  - Rider to restaurants
  - Restaurants to user location
- Handles multiple pickup locations efficiently
- Coverage across different sectors (G-10, F-10, E-10)

## Technical Implementation

### Data Structures Used
- Linked Lists (for user, rider, and admin management)
- Queue (for order management)
- Arrays and Matrices (for location mapping)
- Graph (for route optimization)

### Location System
Covers 10 different locations across Islamabad:
- G-10/1 to G-10/4
- F-10/1 to F-10/4
- E-10/1 to E-10/2

### Security Features
- Password-protected accounts
- Unique IDs for users, riders, and admins
- Session management
## Installation

### Prerequisites
- C++ compiler (GCC recommended)
- Git (for cloning the repository)
- Terminal/Console access

### Steps to Run

1. Clone the repository 

```bash
git clone https://github.com/rafaybbx/food-delivery-system.git
```

2. Navigate to the project directory

```bash
cd food-delivery-system
```

3. Compile the program

```bash
g++ main.cpp -o ood_delivery
```
4. Run the executable
```bash
./food_delivery # For Linux/Mac
food_delivery.exe # For Windows
```
## How to Use

1. Run the program and select from the main menu:
   - Signup
   - Login
   - Exit

2. Based on your role (User/Rider/Admin), log in with credentials

3. For Users:
   - Select restaurants
   - Choose delivery location
   - Place order
   - Track delivery

4. For Riders:
   - View assigned orders
   - Follow optimized delivery routes
   - Complete deliveries

5. For Admins:
   - Manage users and riders
   - System oversight

## Technical Requirements

- C++ compiler
- Standard Template Library (STL)
- Minimum 512MB RAM
- Terminal/Console support

## Troubleshooting

Common issues and solutions:

1. Compilation errors:
   - Ensure you have a C++ compiler installed
   - Check if all required headers are available
   - Verify file paths are correct

2. Runtime errors:
   - Check input format matches requirements
   - Verify sufficient system memory
   - Ensure proper permissions for file operations

3. Display issues:
   - Adjust console window size
   - Check terminal compatibility
   - Verify character encoding

## Future Improvements

- Payment system integration
- Real-time GPS tracking
- Restaurant partner portal
- Rating system
- Order history tracking
- Multiple simultaneous orders handling

## Contributing

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## Contributors

Abdul Rafay(rafybbx)

## License

This project is licensed under the **MIT License**. See the [LICENSE](LICENSE) file for details.  

## Acknowledgments

- CSC211 Data Structures and Algorithms (CUI)
- Graph theory concepts
- C++ Standard Template Library
