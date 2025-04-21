#include <iostream>
#include <windows.h>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <ctime>
#include <iomanip>
using namespace std;

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

void GenerateTicket(const vector<Booking>& bookings) {
    if (bookings.empty()) {
        cout << "No bookings available to generate a ticket.\n";
        return;
    }
    
    static int ticketCounter = 1000;
    stringstream ss;
    ss << "TKT" << time(0) << ticketCounter++;
    string ticketID = ss.str();
    
    string fileName = "ticket_" + ticketID + ".txt";
    ofstream ticket(fileName);
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
        cout << "Error saving bookings to file.\n";
        return;
    }
    
    for (const auto& bookingGroup : allBookings) {
        for (const auto& booking : bookingGroup) {
            file << booking.getPassengerName() << " "
            << booking.getGender() << " "
            << booking.getAge() << " "
            << booking.getFlightID() << " "
            << booking.getOrigin() << " "
            << booking.getDestination() << " "
            << booking.getDepartureTime() << " "
            << booking.getArrivalTime() << " "
            << booking.getFlightPrice() << "\n";
        }
        file << "---\n"; // Separator for different bookings
    }
    file.close();
}

void LoadBookingsFromFile(vector<vector<Booking>>& allBookings, const vector<Flights>& flights) {
ifstream file("bookings.txt");
if (!file) {
    cout << "No previous bookings found.\n";
    return;
}

vector<Booking> bookingGroup;
string name, gender, flightID, origin, destination, departure, arrival, price;
int age;
while (file >> name >> gender >> age >> flightID >> origin >> destination >> departure >> arrival >> price) {
    Flights flight;
    flight.getFlightID() = flightID;
    flight.getOrigin() = origin;
        flight.getDestination() = destination;
        flight.getDepartureTime() = departure;
        flight.getArrivalTime() = arrival;
        flight.getFlightPrice() = price;
        
        bookingGroup.emplace_back(name, gender, age, flight);
        
        if (file.peek() == '-') { // End of a booking group
            allBookings.push_back(bookingGroup);
            bookingGroup.clear();
            file.ignore(256, '\n'); // Skip the separator line
        }
    }
    file.close();
}

void ViewAllTickets(const vector<vector<Booking>>& allBookings) {
    if (allBookings.empty()) {
        cout << "No tickets available.\n";
        return;
    }

    for (int i = 0; i < allBookings.size(); ++i) {
        cout << "\nTicket " << i + 1 << ":\n";
        ifstream ticket("ticket.txt");
        if (ticket.is_open()) {
            string line;
            while (getline(ticket, line)) {
                cout << line << endl;
            }
            ticket.close();
        }
    }
}

void CancelBooking(vector<vector<Booking>>& allBookings) {
    if (allBookings.empty()) {
        cout << "No bookings to cancel.\n";
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

    allBookings.erase(allBookings.begin() + (choice - 1));
    SaveBookingsToFile(allBookings); // Update the file
    cout << "Booking canceled successfully.\n";
}

int main() {
    SetConsoleOutputCP(CP_UTF8);

    vector<Flights> flights;
    Flights selectedFlight;
    vector<vector<Booking>> allBookings;

    LoadBookingsFromFile(allBookings, flights); // Load previous bookings

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
                    SaveBookingsToFile(allBookings); // Save after each booking
                }
                break;
            }
            case 2:
                ViewAllTickets(allBookings);
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