
# ✈️ C++ Airline Booking System

A command-line airline ticket booking system developed in C++, supporting flight search, booking, ticket generation, viewing, and cancellation using file handling and object-oriented design.

---

## 📦 Features

- 🔍 Search flights by origin and destination
- 🎟️ Book multiple passengers in one go
- 📁 Generates a ticket file for each booking group in the `tickets/` folder
- ❌ Cancel a booking using the unique Ticket ID
- 💾 Persistent data via `flights.txt`, `bookings.txt`, and `ticket_counter.txt`

---

## ✅ Prerequisites

Ensure you have the following installed on your system:

- **GCC/G++ compiler (version 9 or later)**  
  Confirm with:
  ```bash
  g++ --version
  ```

- **DLL dependencies (for Windows builds):**
  These should be in the same folder as the `airlines.exe`:
  - `libgcc_s_seh-1.dll`
  - `libstdc++-6.dll`
  - `libwinpthread-1.dll`

---

## 🚀 Installation & Running

1. **Compile the program (if not using the provided executable):**
   ```bash
   g++ -std=c++17 -o airlines.exe main.cpp
   ```

2. **Run the executable:**
   ```bash
   ./airlines.exe
   ```

3. **Ensure the following files are present in the root directory:**
   - `flights.txt` – list of available flights
   - `ticket_counter.txt` – used to generate unique ticket IDs
   - `bookings.txt` – stores all past bookings
   - `tickets/` – stores generated ticket files

---

## 🔧 Project Structure

```
.
├── airlines.exe              # Compiled executable
├── bookings.txt              # Stores all bookings persistently
├── flights.txt               # Master list of available flights
├── ticket_counter.txt        # Maintains global ticket counter across runs
├── tickets/                  # Contains ticket files for each group booking
│   └── ticket_TKT<timestamp><count>.txt
├── main.cpp                  # Source code file
├── README.md                 # This documentation
└── *.dll                     # Required runtime DLLs for Windows
```

---

## 🧠 Concepts Used

### 📁 File Handling

- **Reading flights** from `flights.txt` using file streams
- **Saving and loading bookings** to/from `bookings.txt`
- **Ticket generation** to `tickets/ticket_<ticketID>.txt`
- **Tracking ticket number** with `ticket_counter.txt` for unique filenames

### 👪 Inheritance

- `Flights` class encapsulates flight info.
- `Booking` class inherits flight info and adds passenger details.
- This demonstrates how inheritance allows modular extension of base classes.

### 📚 STL (Standard Template Library)

- **Vectors:** 
  - Used to store lists of flights and bookings dynamically.
  - Simplifies memory management and iteration.

- **File Streams (`ifstream`, `ofstream`)** used for persistent storage.

- **String manipulation, `stringstream`, `getline`** used for formatted I/O and ticket ID generation.

---

## 💡 Notes

- Ticket ID generation uses a mix of `time(0)` and a counter from `ticket_counter.txt`.
- Bookings for multiple passengers in one session share the same ticket ID and are grouped under one ticket file.
- Tickets can be canceled by entering the exact Ticket ID.

---

## 📷 Screenshot

![Airline Booking System Screenshot](976ea1c6-6621-4dfe-9737-3e3885182753.png)

---

## 🛠️ Future Improvements

- GUI version using Qt or SFML
- Integration with a real database (e.g., SQLite)
- User login and account management

---

## 👨‍💻 Author

Built as a C++ project to demonstrate file handling, class inheritance, and STL usage in a practical system.
