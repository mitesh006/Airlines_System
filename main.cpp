#include <iostream>
#include <fstream>
#include <vector>
#include <string> 

using namespace std;

class Flights {
    
    public :
    string FlightID;
    string Origin;
    string OriginSF;
    string Destination;
    string DestinationSF;
    string DepartureTime;
    string ArrivalTime;
    string FlightPrice;


    void display() const {
        cout << "Flight ID: " << FlightID << ", Origin: " << Origin << " (" << OriginSF << ") "
        << ", Destination: " << Destination << " (" << DestinationSF << ") " 
        << ", Departure Time: " << DepartureTime
        << ", Arrival Time: " << ArrivalTime 
        << ", Price: " << FlightPrice << endl;
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
    while (file >> temp.FlightID >> temp.Origin >> temp.OriginSF >> temp.Destination >> temp.DestinationSF >> temp.DepartureTime >> temp.ArrivalTime >> temp.FlightPrice) {
        if ((temp.Origin == searchOrigin || temp.OriginSF == searchOrigin)&& (temp.Destination == searchDestination || temp.DestinationSF == searchDestination)) {
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
