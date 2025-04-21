#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <ctime>

using namespace std;

class Flights {
protected:
    string FlightID;
    string Origin;
    string OriginSF;
    string Destination;
    string DestinationSF;
    string DepartureTime;
    string ArrivalTime;
    string FlightPrice;

public:
    Flights() : FlightID(""), Origin(""), OriginSF(""), Destination(""), 
    DestinationSF(""), DepartureTime(""), ArrivalTime(""), FlightPrice("") {}

    void display() const {
        cout << "Flight ID: " << FlightID << ", Origin: " << Origin << " (" << OriginSF << ") "
             << ", Destination: " << Destination << " (" << DestinationSF << ") "
             << ", Departure Time: " << DepartureTime
             << ", Arrival Time: " << ArrivalTime
             << ", Price: " << FlightPrice << endl;
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

    string PassengerName;
    string Gender;
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


    void GenerateTicket(const vector<Booking>& bks) {

        vector <Booking> bookings;
        bookings = bks;

        if (bookings.empty()) {
            cout << "No bookings available to generate a ticket." << endl;
            return;
        }

        ofstream ticket("ticket.txt");
        if (!ticket) {
            cout << "Error creating ticket file." << endl;
            return;
        }

        static int ticketCounter = 1000;
        stringstream ss;
        ss << "TKT" << time(0) << ticketCounter++;
        string ticketID = ss.str();

        ticket << "---------- India Airlines Ticket ----------\n";
        ticket << "Ticket ID     : " << ticketID << "\n";
        ticket << "Flight Details:\n";
        ticket << "-------------------------------------------\n";
        ticket << "Flight ID     : " << bookings[0].getFlightID() << " (" << bookings[0].getOriginSF()
               << " to " << bookings[0].getDestinationSF() << ")\n";
        ticket << "From          : " << bookings[0].getOrigin() << endl;
        ticket << "To            : " << bookings[0].getDestination() << endl;
        ticket << "Departure     : " << bookings[0].getDepartureTime() << endl;
        ticket << "Arrival       : " << bookings[0].getArrivalTime() << endl;
        ticket << "Price         : " << bookings[0].getFlightPrice() << endl;
        ticket << "\nPassenger(s):\n";

        for (int i = 0; i < bookings.size(); ++i) {
            ticket << i + 1 << ". Name: " << bookings[i].getPassengerName()
                   << ", Gender: " << bookings[i].getGender()
                   << ", Age: " << bookings[i].getAge() << "\n";
        }

        ticket.close();
    }

int main() {
    vector<Flights> flights;
    Flights selectedFlight;
    vector<Booking> bookings;

    cout << "India AirLines" << endl;

    int ch1;

    while (1) {
        cout << "1. Search Flights" << endl;
        cout << "2. View Tickets" << endl;
        cout << "3. Exit" << endl;

        cin >> ch1;

        switch (ch1) {
            case 1: {
                ifstream file("flights.txt");
                if (!file.is_open()) {
                    cout << "Error: Could not open file." << endl;
                    return 1;
                }

                string searchOrigin, searchDestination;
                cout << "Enter origin      : ";
                cin >> searchOrigin;
                cout << "Enter destination : ";
                cin >> searchDestination;

                Flights temp;
                while (file >> temp) {
                    if ((temp.getOrigin() == searchOrigin || temp.getOriginSF() == searchOrigin) &&
                        (temp.getDestination() == searchDestination || temp.getDestinationSF() == searchDestination)) {
                        flights.push_back(temp);
                    }
                }

                if (flights.empty()) {
                    cout << "Flight not found." << endl;
                    break;
                } else {
                    cout << "Matching flight(s):\n";
                    int i = 0;
                    for (const auto& f : flights) {
                        cout << ++i << ". ";
                        f.display();
                    }
                }

                file.close();

                try {
                    cout << "Enter the flight number to book (1-" << flights.size() << "): ";
                    int ch2;
                    cin >> ch2;
                    if (ch2 < 1 || ch2 > flights.size()) {
                        throw runtime_error("Invalid flight number selected.");
                    }
                    selectedFlight = flights[ch2 - 1];
                } catch (const exception& e) {
                    cout << "Error: " << e.what() << endl;
                    break;
                }

                int n;
                cout << "Enter number of passengers: ";
                cin >> n;

                string name, gen;
                int ag;

                for (int i = 0; i < n; i++) {
                    cout << i + 1 << "." << endl;
                    cout << "Passenger's Name   : ";
                    cin.ignore();
                    getline(cin, name);
                    cout << "Passenger's Gender : ";
                    cin >> gen;
                    cout << "Passenger's Age    : ";
                    cin >> ag;

                    Booking b(name, gen, ag, selectedFlight);
                    bookings.push_back(b);
                }

                break;
            }

            case 2: {

                GenerateTicket(bookings);

                try {
                    ifstream ticket("ticket.txt");
                    if (!ticket.is_open()) {
                        throw runtime_error("Ticket file not found. Book a flight first.");
                    }
                    string line;
                    while (getline(ticket, line)) {
                        cout << line << endl;
                    }
                    ticket.close();
                } catch (const exception& e) {
                    cout << "Error: " << e.what() << endl;
                }
                break;
            }

            case 3:
                return 0;
        }
    }
}

