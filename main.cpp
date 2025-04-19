#include <iostream>
#include <fstream>
#include <vector>
#include <string> 

using namespace std;

class Flights {
    
    public :
    string FlightID;
    string Origin;
    string Destination;
    string DepartureTime;
    string ArrivalTime;


    void display() const {
        cout << "Flight ID: " << FlightID << ", Origin: " << Origin
             << ", Destination: " << Destination << ", Departure Time: " << DepartureTime
             << ", Arrival Time: " << ArrivalTime << endl;
    }

};

int main() {
    ifstream file("flights.txt");
    vector<Flights> flights;

    if (!file.is_open()) {
        cout << "Error: Could not open file." << endl;
        return 1;
    }

    string searchOrigin, searchDestination;
    cout << "Enter origin to search: ";
    cin >> searchOrigin;
    cout << "Enter destination to search: ";
    cin >> searchDestination;


    Flights temp;
    while (file >> temp.FlightID >> temp.Origin >> temp.Destination >> temp.DepartureTime >> temp.ArrivalTime) {
        if (temp.Origin == searchOrigin && temp.Destination == searchDestination) {
            flights.push_back(temp);
        }
    }

    if (flights.empty()) {
        cout << "Flight not found." << endl;
    } else {
        cout << "Matching flight(s):\n";
        for (const auto &f : flights) {
            f.display();
        }
    }

    file.close();
    return 0;
}
