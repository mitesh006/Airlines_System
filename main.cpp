#include <iostream>
#include <fstream>
#include <vector>
#include <string> 

using namespace std;

class Flights {
    
    string FlightID;
    string Origin;
    string OriginSF;
    string Destination;
    string DestinationSF;
    string DepartureTime;
    string ArrivalTime;
    string FlightPrice;
    
    public :

    void display() const {
        cout << "Flight ID: " << FlightID << ", Origin: " << Origin << " (" << OriginSF << ") "
        << ", Destination: " << Destination << " (" << DestinationSF << ") " 
        << ", Departure Time: " << DepartureTime
        << ", Arrival Time: " << ArrivalTime 
        << ", Price: " << FlightPrice << endl;
    }

    string getOrigin() const { return Origin; }
    string getOriginSF() const { return OriginSF; }
    string getDestination() const { return Destination; }
    string getDestinationSF() const { return DestinationSF; }

    friend ifstream& operator>> (ifstream& in, Flights& f) {
        in >> f.FlightID >> f.Origin >> f.OriginSF
           >> f.Destination >> f.DestinationSF
           >> f.DepartureTime >> f.ArrivalTime >> f.FlightPrice;
        return in;
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
    while (file >> temp) {
        if ((temp.getOrigin() == searchOrigin || temp.getOriginSF() == searchOrigin) && 
            (temp.getDestination() == searchDestination || temp.getDestinationSF() == searchDestination)) {
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
