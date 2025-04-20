#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class Flights {
public:
    string FlightID;
    string Origin;
    string OriginSF;
    string Destination;
    string DestinationSF;
    string DepartureTime;
    string ArrivalTime;

    void display() const {
        cout << "\nAvailable Flights:\n";
        cout << "Flight ID: " << FlightID << ", Origin: " << Origin << " (" << OriginSF << ")"
             << ", Destination: " << Destination << " (" << DestinationSF << ")"
             << ", Departure Time: " << DepartureTime << ", Arrival Time: " << ArrivalTime << endl;
    }
};

class Passenger {
public:
    string name;
    int age;

    Passenger(string n, int a) {
        name = n;
        age = a;
    }
};

int main() {
    vector<Flights> flights;
    int n;

    while (true) {
        cout<<"..WelCome to Flight Booking System..."<<endl;
        cout << "\nEnter 1 For View Flights\n";
        cout << "Enter 2 For View Tickets\n";
        cout << "Enter 0 For Exit..\n";
        cin >> n;

        if (n == 0) {
            break;
        }

        switch (n) {
            case 1: {
                ifstream file("flights.txt");
                if (!file.is_open()) {
                    cout << "Error: Could not open file." << endl;
                    continue;
                }

                string searchOrigin, searchDestination;
                cout << "Enter origin to search: ";
                cin >> searchOrigin;
                cout << "Enter destination to search: ";
                cin >> searchDestination;

                Flights temp;
                flights.clear(); // Clear previous results
                while (file >> temp.FlightID >> temp.Origin >> temp.OriginSF >> temp.Destination >> temp.DestinationSF >> temp.DepartureTime >> temp.ArrivalTime) {
                    if ((temp.Origin == searchOrigin || temp.OriginSF == searchOrigin) &&
                        (temp.Destination == searchDestination || temp.DestinationSF == searchDestination)) {
                        flights.push_back(temp);
                    }
                }

                file.close();

                if (flights.empty()) {
                    cout << "Flight not found.\n";
                    break;
                }

                cout << "Matching result(s):\n";
                for (size_t i = 0; i < flights.size(); ++i) {
                    cout << "[" << i + 1 << "] ";
                    flights[i].display();
                }

                char ch;
                cout << "Do you want to book a flight? (Y/N): ";
                cin >> ch;

                if (ch == 'Y' || ch == 'y') {
                    int choice, ps;
                    cout << "Select flight by number (e.g., 1, 2...): ";
                    cin >> choice;
                    if (choice < 1 || choice > flights.size()) {
                        cout << "Invalid choice.\n";
                        break;
                    }

                    Flights selectedFlight = flights[choice - 1];

                    cout << "Enter number of passengers: ";
                    cin >> ps;

                    vector<Passenger> passengers;

                    for (int i = 0; i < ps; ++i) {
                        string pname;
                        int page;
                        cout << "Enter name of Passenger " << i + 1 << ": ";
                        cin >> pname;
                        cout << "Enter age of Passenger " << i + 1 << ": ";
                        cin >> page;
                        passengers.push_back(Passenger(pname, page));
                    }

                    // Save to file
                    ofstream ticketFile("passenger_tickets.txt", ios::app);
                    if (!ticketFile.is_open()) {
                        cout << "Error opening ticket file.\n";
                        break;
                    }

                    ticketFile << "\n------------------------------\n";
                    ticketFile << "Flight Ticket Details:\n";
                    ticketFile << "Flight ID: " << selectedFlight.FlightID << endl;
                    ticketFile << "Origin: " << selectedFlight.Origin << " (" << selectedFlight.OriginSF << ")\n";
                    ticketFile << "Destination: " << selectedFlight.Destination << " (" << selectedFlight.DestinationSF << ")\n";
                    ticketFile << "Departure Time: " << selectedFlight.DepartureTime << ", Arrival Time: " << selectedFlight.ArrivalTime << "\n";
                    ticketFile << "Passenger(s):\n";

                    for (const auto &p : passengers) {
                        ticketFile << "- Name: " << p.name << ", Age: " << p.age << "\n";
                    }
                    ticketFile << "------------------------------\n";

                    ticketFile.close();

                    cout << "Ticket successfully booked and saved!\n";
                }

                break;
            }

            case 2: {
                ifstream ticketFile("passenger_tickets.txt");
                if (!ticketFile.is_open()) {
                    cout << "No tickets found.\n";
                    break;
                }

                string line;
                while (getline(ticketFile, line)) {
                    cout << line << endl;
                }

                ticketFile.close();
                break;
            }

            default:
                cout << "Invalid option. Try again.\n";
        }
    }

    return 0;
}
