# Airlines System

The Airlines System project is a C++ application that showcases key programming concepts such as **file handling**, **inheritance**, and the **Standard Template Library (STL)** (particularly vectors). This project is designed to manage and simulate various airline system functionalities.

## Prerequisites

Before running the project, ensure your system meets the following requirements:

- **GCC Compiler**: Version 9.0 or higher.  
  Install GCC using the following command:
  ```bash
  sudo apt update
  sudo apt install gcc
  ```
- **DLL Files**: Ensure that the required `.dll` files for the application are present in the executable's directory (if applicable for Windows users). These `.dll` files should be included automatically during the build process. If any are missing, please check the repository's `dll` folder or reach out to the maintainer.

## Installation

Follow these steps to set up and run the project:

1. Clone the repository:
   ```bash
   git clone https://github.com/mitesh006/Airlines_System.git
   ```

2. Navigate to the project directory:
   ```bash
   cd Airlines_System
   ```

3. Build the project:
   ```bash
   g++ -std=c++17 -o AirlinesSystem main.cpp
   ```

   Replace `main.cpp` with the actual entry file if it differs.

4. Run the executable:
   ```bash
   ./AirlinesSystem
   ```

   On Windows:
   ```cmd
   AirlinesSystem.exe
   ```

## Concepts Used

This project demonstrates the following C++ programming concepts:

1. **File Handling**:
   - Reading and writing data to files for persistent storage.
   - Management of airline data, including flight schedules, passenger information, and ticket details.

2. **Inheritance**:
   - Implementation of base and derived classes to model different entities of the airline system (e.g., flights,
