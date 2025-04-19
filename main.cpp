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


    void display() const {
        cout<<"\n Available Flights::"<<endl;

        cout << "Flight ID: " << FlightID << ", Origin: " << Origin << " (" << OriginSF << ")" 
        << ", Destination: " << Destination << " " << DestinationSF << ", Departure Time: " << DepartureTime
        << ", Arrival Time: " << ArrivalTime << endl;
    }

};

int main() {
    vector<Flights> flights;
    int n;
    
    cout<<"Enter 1 For View Flights"<<endl;
    cout<<"Enter 2 For View Tickets"<<endl;
    cout<<"Enter 0 For Exit.."<<endl;

while(true){
    switch(n){
 case 1:
 char ch;int ps;
 ifstream file("flights.txt");

    cout << "Flight Booking" << endl;
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
    while (file >> temp.FlightID >> temp.Origin >> temp.OriginSF >> temp.Destination >> temp.DestinationSF >> temp.DepartureTime >> temp.ArrivalTime) {
        if ((temp.Origin == searchOrigin || temp.OriginSF == searchOrigin) && temp.Destination == searchDestination) {
            flights.push_back(temp);
        }
    }

    if (flights.empty()) {
        cout << "Flight not found." << endl;
    } else {
        cout << "Matching result(s):\n";
        for (const auto &f : flights) {
            f.display();
        }
    }
 file.close();
 cout<<"Do You Want Book Flight- If Yes Press 'Y' Or if Not Press 'N'";
 cin>>ch;
 cout<<"Enter Number OF Passenger::";
 cin>>ps;
 if(ch=='y'|| ch=='Y')
 {
    int i=0;
 while(ps!=0){
    string name;int age;
    cout<<"Enter Details Of Passener"<<i+1;
    cin>>

 }
 }




    }
}

    return 0;
}
