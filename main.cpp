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

void GenerateTicket(const vector<Booking>& bookings) {
    if (bookings.empty()) {
        cout << "No bookings available to generate a ticket.\n";
        return;
    }

    ofstream ticket("ticket.txt");
    if (!ticket) {
        cout << "Error creating ticket file.\n";
        return;
    }

    static int ticketCounter = 1000;
    stringstream ss;
    ss << "TKT" << time(0) << ticketCounter++;
    string ticketID = ss.str();

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
    cout << "Ticket successfully generated: ticket.txt\n";
}

void DisplayMenu() {
    cout << "\nMenu:\n";
    cout << "1. Search & Book Flights\n";
    cout << "2. View Ticket\n";
    cout << "3. Exit\n";
    cout << "Enter choice: ";
}

void SearchAndBookFlights(vector<Flights>& flights, vector<Booking>& bookings, Flights& selectedFlight) {
    ifstream file("flights.txt");
    if (!file) {
        cout << "Error: Could not open flights.txt\n";
        return;
    }

    string searchOrigin, searchDestination;
    cout << "\nEnter origin city or code      : ";
    cin >> searchOrigin;
    cout << "Enter destination city or code : ";
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
        cout << "\nNo matching flights found.\n";
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
    cout << "\nEnter flight number to book: ";
    cin >> ch2;
    if (ch2 < 1 || ch2 > flights.size()) {
        cout << "Invalid selection.\n";
        return;
    }
    selectedFlight = flights[ch2 - 1];

    int num;
    cout << "Number of passengers: ";
    cin >> num;

    cin.ignore();
    bookings.clear();
    for (int i = 0; i < num; ++i) {
        string name, gender;
        int age;
        cout << "\nPassenger " << i + 1 << " Name   : ";
        getline(cin, name);
        cout << "Gender (M/F/O)       : ";
        cin >> gender;
        cout << "Age                  : ";
        cin >> age;
        cin.ignore();

        bookings.emplace_back(name, gender, age, selectedFlight);
    }
}

void ViewTicket(const vector<Booking>& bookings) {
    GenerateTicket(bookings);
    try {
        ifstream ticket("ticket.txt");
        if (!ticket.is_open()) {
            throw runtime_error("ticket.txt not found. Please book first.");
        }
        cout << "\n--------- Your Ticket ---------\n";
        string line;
        while (getline(ticket, line)) {
            cout << line << endl;
        }
        ticket.close();
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);

    vector<Flights> flights;
    Flights selectedFlight;
    vector<Booking> bookings;

    cout << "\n========= Welcome to India Airlines =========\n";

    while (true) {
        DisplayMenu();

        int ch1;
        cin >> ch1;

        switch (ch1) {
            case 1:
                SearchAndBookFlights(flights, bookings, selectedFlight);
                break;

            case 2:
                ViewTicket(bookings);
                break;

            case 3:
                cout << "\nThank you for choosing India Airlines!\n";
                return 0;

            default:
                cout << "Invalid option. Try again.\n";
        }
    }
}