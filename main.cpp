#include <iostream>   // for input and output
#include <windows.h>  
#include <fstream>   // file Handling
#include <vector>    // for storing flight and booking data
#include <string>   // for string manipulation
#include <sstream>  // for string manipulation
#include <ctime>    // for generating unique ticket IDs
#include <iomanip>  // for formatting output
#include <filesystem>   // for creating directories

using namespace std;
namespace fs = std::filesystem;

class Flights {         // for stroing flight details
protected:
string FlightID, Origin, OriginSF, Destination, DestinationSF;
string DepartureTime, ArrivalTime, FlightPrice;

public:


Flights() = default;

Flights(string id, string origin, string originSF,
string destination, string destinationSF, string departure,
string arrival, string price) {
    FlightID = id;
    Origin = origin;
    OriginSF = originSF;
    Destination = destination;
    DestinationSF = destinationSF;
    DepartureTime = departure;
    ArrivalTime = arrival;
    FlightPrice = price;
}

void display() const {          // display flight details
    cout << left << setw(12) << FlightID
    << setw(15) << (Origin + " (" + OriginSF + ")")
    << setw(20) << (Destination + " (" + DestinationSF + ")")
    << setw(15) << DepartureTime
    << setw(15) << ArrivalTime
    << "₹" << FlightPrice << endl;
}

// getmethods

string getFlightID() const { return FlightID; }
string getOrigin() const { return Origin; }
string getOriginSF() const { return OriginSF; }
string getDestination() const { return Destination; }
string getDestinationSF() const { return DestinationSF; }
string getDepartureTime() const { return DepartureTime; }
string getArrivalTime() const { return ArrivalTime; }
string getFlightPrice() const { return FlightPrice; }

// operator oveloading
friend ifstream& operator>>(ifstream& in, Flights& f) {
    in >> f.FlightID >> f.Origin >> f.OriginSF
    >> f.Destination >> f.DestinationSF
    >> f.DepartureTime >> f.ArrivalTime >> f.FlightPrice;
    return in;
}
};

class Booking : public Flights {       // for storing booking details
    string PassengerName, Gender;      // inherited from Flights class
    int Age;
    string TicketID;

    public:
    Booking(string name, string gender, int age, const Flights& flight, const string& ticketID = "")
    : PassengerName(name), Gender(gender), Age(age), TicketID(ticketID) {
        FlightID = flight.getFlightID();
        Origin = flight.getOrigin();
        OriginSF = flight.getOriginSF();
        Destination = flight.getDestination();
        DestinationSF = flight.getDestinationSF();
        DepartureTime = flight.getDepartureTime();
        ArrivalTime = flight.getArrivalTime();
        FlightPrice = flight.getFlightPrice();
    }
    

    string getPassengerName() const { return PassengerName; }
    string getGender() const { return Gender; }
    int getAge() const { return Age; }
    string getTicketID() const { return TicketID; }
    void setTicketID(const string& id) { TicketID = id; }   // set methhod
};

void DisplayMenu() {                                    // display menu
    cout << "\n========= Main Menu =========\n";
    cout << "1. Search & Book Flights\n";
    cout << "2. View All Tickets\n";
    cout << "3. Cancel Booking\n";
    cout << "4. Exit\n";
    cout << "=============================\n";
    cout << "Enter your choice (1-4): ";
}

int Counter() {                                           // function to generate unique ticket ID

    const string counterFile = "ticket_counter.txt";
    int counter = 1000;

    ifstream in (counterFile);
    if (in) {
        in >> counter;
        in.close();
    }

    ofstream out(counterFile);
    if (out) {
        out << counter + 1;
        out.close();
    }

    return counter;
}

void GenerateTicket(const vector<Booking>& bookings, const string& ticketID) {
    if (bookings.empty()) {
        cout << "No bookings available to generate a ticket.\n";
        return;
    }

    fs::create_directory("tickets");   // create directory to store tickets
    
    string fileName = "ticket_" + ticketID + ".txt";    // create file name using ticket ID

    ofstream ticket("tickets/" + fileName);   // create file to store ticket details
    if (!ticket) {
        cout << "Error creating ticket file.\n";
        return;
    }
    
    ticket << "--------------- India Airlines Ticket ---------------\n";            // print flight details in file
    ticket << "Ticket ID     : " << bookings[0].getTicketID() << "\n\n";
    ticket << "Flight Details:\n";
    ticket << "-----------------------------------------------------\n";
    ticket << "Flight ID     : " << bookings[0].getFlightID() << "\n";
    ticket << "From          : " << bookings[0].getOrigin() << " (" << bookings[0].getOriginSF() << ")\n";
    ticket << "To            : " << bookings[0].getDestination() << " (" << bookings[0].getDestinationSF() << ")\n";
    ticket << "Departure     : " << bookings[0].getDepartureTime() << "\n";
    ticket << "Arrival       : " << bookings[0].getArrivalTime() << "\n";
    ticket << "Price (each)  : ₹" << bookings[0].getFlightPrice() << "\n\n";
    
    ticket << "Passenger(s):\n";                                // print passenger details in file
    for (int i = 0; i < bookings.size(); ++i) {
        ticket << i + 1 << ". " << bookings[i].getPassengerName()
        << " | Gender: " << bookings[i].getGender()
        << " | Age: " << bookings[i].getAge() << "\n";
    }
    
    ticket.close();
    cout << "Ticket successfully generated: " << fileName << "\n"; 
}

void BookFlights(vector<Flights>& flights, vector<Booking>& bookings, Flights& selectedFlight) {
    
    int ch2;                                                                      // which flgiht to book
    cout << "\nEnter the flight number to book (1-" << flights.size() << "): ";
    cin >> ch2;
    if (ch2 < 1 || ch2 > flights.size()) {
        cout << "Invalid selection. Returning to the main menu.\n";
        return;
    }
    selectedFlight = flights[ch2 - 1];          // get selected flight details
    
    int num;                                                  // taking passenger details 
    cout << "Enter the number of passengers: ";
    cin >> num;
    
    cin.ignore();
    bookings.clear();
    
    // generate unique ticket ID

    int ticketCounter = Counter();  // function call to generate unique ticket ID by updating the counter
    stringstream ss;                
    ss << "TKT" << time(0) << ticketCounter;    
    string ticketID = ss.str();                 // generate ticket ID using current time and counter
    
    for (int i = 0; i < num; ++i) {
        string name, gender;
        int age;
        cout << "\nPassenger " << i + 1 << " Name: ";
        getline(cin, name);
        cout << "Gender (M/F/O): ";
        cin >> gender;
        cout << "Age: ";
        cin >> age;
        cin.ignore();
        
        bookings.emplace_back(name, gender, age, selectedFlight, ticketID);     // stroing passenger details in booking vector
    }
    
    
    GenerateTicket(bookings, ticketID);  // call function to generate ticket
    
    cout << "\nBooking successful! " << num << " passenger(s) booked for flight " << selectedFlight.getFlightID() << ".\n";
}

void SearchFlights(vector<Flights>& flights, vector<Booking>& bookings, Flights& selectedFlight) { // Search flights based on user input

    ifstream file("flights.txt");                                                // create a file to store flight details
    if (!file) {
        cout << "Error: Could not open flights.txt. Please ensure the file exists.\n";
        return;
    }
    string searchOrigin, searchDestination;         // prompt user to enter flight details
    cout << "\nEnter origin city or code (e.g., DEL): ";
    cin >> searchOrigin;
    cout << "Enter destination city or code (e.g., BOM): ";
    cin >> searchDestination;
    
    cout << "\nSearching for flights...\n";                 
    Flights temp;
    flights.clear();
    while (file >> temp) {              // read flight details from file    
        if ((temp.getOrigin() == searchOrigin || temp.getOriginSF() == searchOrigin) &&
            (temp.getDestination() == searchDestination || temp.getDestinationSF() == searchDestination)) {
                flights.push_back(temp);
        }
    }
    
    if (flights.empty()) {
        cout << "\nNo matching flights found. Please try again.\n";
        return;
    }

    cout << "\nAvailable Flights:\n";
    cout << left << setw(15) << "FlightID"              // show available flights
    << setw(15) << "Origin"
    << setw(20) << "Destination"
    << setw(15) << "Departure"
    << setw(15) << "Arrival"
    << "Price\n";
    int i = 0;
    for (const auto& f : flights) {
        cout << ++i << ". ";
        f.display();
    }

    file.close();

    BookFlights(flights, bookings, selectedFlight);
    
} 

void SaveBookingsToFile(const vector<vector<Booking>>& allBookings) {   // save booking details to file
    ofstream file("bookings.txt");
    if (!file) {
        cout << "Error opening file to save bookings.\n";
        return;
    }

    for (const auto& group : allBookings) {  // iterate through each group of bookings
        for (const auto& b : group) {       // iterate through each booking in the group
            file << b.getPassengerName() << " "
                 << b.getGender() << " "
                 << b.getAge() << " "
                 << b.getFlightID() << " "
                 << b.getOrigin() << " "
                 << b.getOriginSF() << " "
                 << b.getDestination() << " "
                 << b.getDestinationSF() << " "
                 << b.getDepartureTime() << " "
                 << b.getArrivalTime() << " "
                 << b.getFlightPrice() << " "
                 << b.getTicketID() << "\n";

        }
        file << "--- END ---\n";
    }

    file.close();
}

void LoadBookingsFromFile(vector<vector<Booking>>& allBookings) {   // load booking details from file
    ifstream file("bookings.txt");
    if (!file) {
        cout << "No previous bookings found.\n";
        return;
    }

    vector<Booking> bookingGroup;
    string line;

    while (getline(file, line)) {
        if (line == "--- END ---") {
            allBookings.push_back(bookingGroup);
            bookingGroup.clear();
            continue;
        }

        stringstream ss(line);
        string name, gender, flightID, origin, originSF, destination, destinationSF, departure, arrival, price, ticketID;
        int age;

        ss >> name >> gender >> age >> flightID >> origin >> originSF >> destination >> destinationSF >> departure >> arrival >> price >> ticketID;

        Flights flight(flightID, origin, originSF, destination, destinationSF, departure, arrival, price);
        bookingGroup.emplace_back(name, gender, age, flight, ticketID);
    }

    if (!bookingGroup.empty()) {
        allBookings.push_back(bookingGroup); // Add any remaining group
    }

    file.close();
}

void ViewTickets() {            // display all tickets
    
    string ticketsDir = "tickets/";
    if (!fs::exists(ticketsDir) || fs::is_empty(ticketsDir)) {
        cout << "No ticekts available." << endl;
        return;
    }

    cout << "\nDisplaying all tickets:\n";
    for (const auto& entry : fs::directory_iterator(ticketsDir)) {          // iterate through each file in the directory
        if (entry.is_regular_file()) {
            cout << "\nTicket File: " << entry.path().filename() << "\n";
            ifstream ticket(entry.path());
            if (ticket.is_open()) {
                string line;
                while (getline(ticket, line)) {
                    cout << line << endl;
                }
                ticket.close();
            } else {
                cout << "Error opening ticket file: " << entry.path().filename() << "\n";
            }
        }
    }
}

void CancelBooking(vector<vector<Booking>>& allBookings) {      // cancel booking
    if (allBookings.empty()) {
        cout << "No bookings available to cancel.\n";
        return;
    }

    cout << "Available Tickets:\n";
    for (int i = 0; i < allBookings.size(); ++i) {
        cout << i + 1 << ". Ticket " << i + 1 << " (" << allBookings[i].size() << " passenger(s))\n";
    }

    int choice;
    cout << "Enter the ticket number to cancel (1-" << allBookings.size() << "): ";
    cin >> choice;

    if (choice < 1 || choice > allBookings.size()) {
        cout << "Invalid choice.\n";
        return;
    }

    string ticketID = allBookings[choice - 1][0].getTicketID();     // get ticket ID of the selected booking
    string ticketFileName = "tickets/ticket_" + ticketID + ".txt";

    if (fs::exists(ticketFileName)) {
        fs::remove(ticketFileName);
        cout << "Ticket file " << ticketFileName << " deleted successfully.\n";
    } else {
        cout << "Ticket file " << ticketFileName << " not found.\n";
    }

    allBookings.erase(allBookings.begin() + (choice - 1));
    SaveBookingsToFile(allBookings); // Update the file
    cout << "Booking canceled successfully.\n";
}

int main() {
    SetConsoleOutputCP(CP_UTF8);            // for rupee symbol

    vector<Flights> flights;
    Flights selectedFlight;
    vector<vector<Booking>> allBookings; 

    LoadBookingsFromFile(allBookings);          // load previous bookings from file

    cout << "\n========= Welcome to India Airlines =========\n";

    while (true) {
        DisplayMenu();          // display menu

        int ch1;
        cin >> ch1;

        switch (ch1) {
            case 1: {
                vector<Booking> bookings;
                SearchFlights(flights, bookings, selectedFlight);
                if (!bookings.empty()) {
                    allBookings.push_back(bookings);
                    SaveBookingsToFile(allBookings);   // save bookings to file
                }
                break;
            }
            case 2:
                ViewTickets();    // view all tickets
                break;
            case 3:
                CancelBooking(allBookings); // cancel booking
                break;
            case 4:
                cout << "\nThank you for choosing India Airlines!\n";
                return 0;
            default:
                cout << "Invalid option. Try again.\n";
        }
    }
}
