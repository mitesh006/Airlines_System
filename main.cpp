#include <iostream>
#include <windows.h>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

class Flights {
protected:
string FlightID, Origin, OriginSF, Destination, DestinationSF;
string DepartureTime, ArrivalTime, FlightPrice;

public:
Flights() = default;

void display() const {
    cout << left << setw(12) << FlightID
    << setw(15) << (Origin + " (" + OriginSF + ")")
    << setw(20) << (Destination + " (" + DestinationSF + ")")
    << setw(15) << DepartureTime
    << setw(15) << ArrivalTime
    << "₹" << FlightPrice << endl;
}

string getFlightID() const { return FlightID; }
string getOrigin() const { return Origin; }
string getOriginSF() const { return OriginSF; }
string getDestination() const { return Destination; }
string getDestinationSF() const { return DestinationSF; }
string getDepartureTime() const { return DepartureTime; }
string getArrivalTime() const { return ArrivalTime; }
string getFlightPrice() const { return FlightPrice; }

friend ifstream& operator>>(ifstream& in, Flights& f) {
    in >> f.FlightID >> f.Origin >> f.OriginSF
    >> f.Destination >> f.DestinationSF
    >> f.DepartureTime >> f.ArrivalTime >> f.FlightPrice;
    return in;
}
};

class Booking : public Flights {
    string PassengerName, Gender;
    int Age;
    
    public:
    Booking(string name, string gender, int age, const Flights& flight)
    : PassengerName(name), Gender(gender), Age(age) {
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
};

void DisplayMenu() {
    cout << "\n========= Main Menu =========\n";
    cout << "1. Search & Book Flights\n";
    cout << "2. View All Tickets\n";
    cout << "3. Cancel Booking\n";
    cout << "4. Exit\n";
    cout << "=============================\n";
    cout << "Enter your choice (1-4): ";
}

int Counter() {

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

void GenerateTicket(const vector<Booking>& bookings) {
    if (bookings.empty()) {
        cout << "No bookings available to generate a ticket.\n";
        return;
    }

    fs::create_directory("tickets");
    
    int ticketCounter = Counter();
    stringstream ss;
    ss << "TKT" << time(0) << ticketCounter;
    string ticketID = ss.str();
    
    string fileName = "ticket_" + ticketID + ".txt";
    ofstream ticket("tickets/" + fileName);
    if (!ticket) {
        cout << "Error creating ticket file.\n";
        return;
    }
    
    ticket << "--------------- India Airlines Ticket ---------------\n";
    ticket << "Ticket ID     : " << ticketID << "\n\n";
    ticket << "Flight Details:\n";
    ticket << "-----------------------------------------------------\n";
    ticket << "Flight ID     : " << bookings[0].getFlightID() << "\n";
    ticket << "From          : " << bookings[0].getOrigin() << " (" << bookings[0].getOriginSF() << ")\n";
    ticket << "To            : " << bookings[0].getDestination() << " (" << bookings[0].getDestinationSF() << ")\n";
    ticket << "Departure     : " << bookings[0].getDepartureTime() << "\n";
    ticket << "Arrival       : " << bookings[0].getArrivalTime() << "\n";
    ticket << "Price (each)  : ₹" << bookings[0].getFlightPrice() << "\n\n";
    
    ticket << "Passenger(s):\n";
    for (int i = 0; i < bookings.size(); ++i) {
        ticket << i + 1 << ". " << bookings[i].getPassengerName()
        << " | Gender: " << bookings[i].getGender()
        << " | Age: " << bookings[i].getAge() << "\n";
    }
    
    ticket.close();
    cout << "Ticket successfully generated: " << fileName << "\n";
}

void SearchAndBookFlights(vector<Flights>& flights, vector<Booking>& bookings, Flights& selectedFlight) {
    ifstream file("flights.txt");
    if (!file) {
        cout << "Error: Could not open flights.txt. Please ensure the file exists.\n";
        return;
    }

    string searchOrigin, searchDestination;
    cout << "\nEnter origin city or code (e.g., DEL): ";
    cin >> searchOrigin;
    cout << "Enter destination city or code (e.g., BOM): ";
    cin >> searchDestination;
    
    Flights temp;
    flights.clear();
    while (file >> temp) {
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
    cout << left << setw(12) << "FlightID"
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
    
    int ch2;
    cout << "\nEnter the flight number to book (1-" << flights.size() << "): ";
    cin >> ch2;
    if (ch2 < 1 || ch2 > flights.size()) {
        cout << "Invalid selection. Returning to the main menu.\n";
        return;
    }
    selectedFlight = flights[ch2 - 1];
    
    int num;
    cout << "Enter the number of passengers: ";
    cin >> num;
    
    cin.ignore();
    bookings.clear();
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
        
        bookings.emplace_back(name, gender, age, selectedFlight);
    }
    
    cout << "\nBooking successful! " << num << " passenger(s) booked for flight " << selectedFlight.getFlightID() << ".\n";
    GenerateTicket(bookings);
    
}

void SaveBookingsToFile(const vector<vector<Booking>>& allBookings) {
    ofstream file("bookings.txt");
    if (!file) {
        cout << "Error opening file to save bookings.\n";
        return;
    }

    for (const auto& group : allBookings) {
        for (const auto& b : group) {
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
                 << b.getFlightPrice() << "\n";
        }
        file << "--- END ---\n";
    }

    file.close();
}

void LoadBookingsFromFile(vector<vector<Booking>>& allBookings) {
    ifstream file("bookings.txt");
    if (!file) {
        cout << "No previous bookings found.\n";
        return;
    }

    vector<Booking> bookingGroup;
    string name, gender, flightID, origin, originSF, destination, destinationSF, departure, arrival, price;
    int age;

    Flights flight;
    flight = Flights(); // Reset object

    while (file >> name >> gender >> age && file >> flight) {
        

        bookingGroup.emplace_back(name, gender, age, flight);

        string separator;
        getline(file, separator); // Read to end of line
        if (separator.find("--- END ---") != string::npos) {
            allBookings.push_back(bookingGroup);
            bookingGroup.clear();
        }
    }

    file.close();
}

void ViewTickets() {
    
    string ticketsDir = "tickets/";
    if (!fs::exists(ticketsDir) || fs::is_empty(ticketsDir)) {
        cout << "No ticekts available." << endl;
        return;
    }

    cout << "\nDisplaying all tickets:\n";
    for (const auto& entry : fs::directory_iterator(ticketsDir)) {
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

void CancelBooking(vector<vector<Booking>>& allBookings) {

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

    stringstream ss;
    ss << "TKT" << time(0) << (1000 + choice - 1);
    string ticketFileName = "tickets/ticket_" + ss.str() + ".txt";

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
    SetConsoleOutputCP(CP_UTF8);

    vector<Flights> flights;
    Flights selectedFlight;
    vector<vector<Booking>> allBookings; 

    LoadBookingsFromFile(allBookings); 

    cout << "\n========= Welcome to India Airlines =========\n";

    while (true) {
        DisplayMenu();

        int ch1;
        cin >> ch1;

        switch (ch1) {
            case 1: {
                vector<Booking> bookings;
                SearchAndBookFlights(flights, bookings, selectedFlight);
                if (!bookings.empty()) {
                    allBookings.push_back(bookings);
                    SaveBookingsToFile(allBookings);
                }
                break;
            }
            case 2:
                ViewTickets();
                break;
            case 3:
                CancelBooking(allBookings);
                break;
            case 4:
                cout << "\nThank you for choosing India Airlines!\n";
                return 0;
            default:
                cout << "Invalid option. Try again.\n";
        }
    }
}