//Author: Cameron Rajbally
/*References:
* 1)C++ Programming. Publisher: Cengage. Author: D.S. Malik.
* 2)StackOverflow.com
* 3)CodeCamp.com
*/


#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

using namespace std;

// Structure to hold traffic data
struct TrafficData {
    string time;
    string vehicleType;
    int speed;
};

// Function to read the traffic data file and return a vector of TrafficData
vector<TrafficData> readTrafficData(const string& filename) {
    vector<TrafficData> data;
    ifstream file(filename);
    string line;

    // Check if the file opens successfully
    if (!file.is_open()) {
        cerr << "Error opening file!!!: " << filename << endl;
        return data;
    }

    // Skip the first line (header)
    getline(file, line);  // Read and ignore the header

    // Process the rest of the file
    while (getline(file, line)) {
        stringstream ss(line);
        TrafficData entry;
        string speed;

        getline(ss, entry.time, ';');
        getline(ss, entry.vehicleType, ';');
        getline(ss, speed, ';');

        try {
            entry.speed = stoi(speed);  // Convert speed string to integer
            data.push_back(entry);
        }
        catch (const invalid_argument&) {
            cerr << "Invalid speed value in line: " << line << ". Skipping entry." << endl;
        }
    }

    file.close();
    return data;
}

// Report 1: Traffic Summary Report
void generateTrafficSummary(const vector<TrafficData>& data) {
    map<string, int> vehicleCount;
    map<string, int> vehiclesPerDay;

    for (const auto& entry : data) {
        vehicleCount[entry.vehicleType]++;
        string date = entry.time.substr(0, 10);  // Extract date
        vehiclesPerDay[date]++;
    }

    int totalVehicles = data.size();
    cout << "Traffic Summary Report:" << endl;
    cout << "-----------------------" << endl;
    cout << "Total vehicles: " << totalVehicles << endl;

    // Vehicle count per day
    for (const auto& day : vehiclesPerDay) {
        cout << day.first << ": " << day.second << " vehicles" << endl;
    }

    // Vehicle type distribution
    cout << "Vehicle distribution:" << endl;
    for (const auto& type : vehicleCount) {
        cout << "- " << type.first << ": " << type.second << " ("
            << (type.second * 100.0 / totalVehicles) << "%)" << endl;
    }
}

// Report 2: Peak Traffic Times Report
void generatePeakTrafficReport(const vector<TrafficData>& data) {
    map<string, map<int, int>> vehiclesPerHourPerDay;
    map<string, int> totalVehiclesPerDay;

    for (const auto& entry : data) {
        string date = entry.time.substr(0, 10);
        int hour = stoi(entry.time.substr(11, 2));

        vehiclesPerHourPerDay[date][hour]++;
        totalVehiclesPerDay[date]++;
    }

    // Peak traffic per hour and day
    string peakHourTime;
    int peakHourVehicles = 0;

    for (const auto& dayData : vehiclesPerHourPerDay) {
        for (const auto& hourData : dayData.second) {
            if (hourData.second > peakHourVehicles) {
                peakHourVehicles = hourData.second;
                peakHourTime = dayData.first + " " + to_string(hourData.first) + ":00 - " + to_string(hourData.first + 1) + ":00";
            }
        }
    }

    // Peak traffic per day
    string peakDay;
    int peakDayVehicles = 0;
    for (const auto& day : totalVehiclesPerDay) {
        if (day.second > peakDayVehicles) {
            peakDayVehicles = day.second;
            peakDay = day.first;
        }
    }

    // Output the peak traffic report
    cout << "Peak Traffic Times Report:" << endl;
    cout << "--------------------------" << endl;
    cout << "Peak traffic time (overall): " << peakHourTime << " with " << peakHourVehicles << " vehicles" << endl;

    // Peak traffic by day
    for (const auto& dayData : vehiclesPerHourPerDay) {
        string peakHourForDay;
        int maxVehiclesForDay = 0;
        for (const auto& hourData : dayData.second) {
            if (hourData.second > maxVehiclesForDay) {
                maxVehiclesForDay = hourData.second;
                peakHourForDay = to_string(hourData.first) + ":00 - " + to_string(hourData.first + 1) + ":00";
            }
        }
        cout << "  - " << dayData.first << ": " << peakHourForDay << ", " << maxVehiclesForDay << " vehicles" << endl;
    }
}

// Report 3: Congestion Level Report
void generateCongestionReport(const vector<TrafficData>& data) {
    map<string, map<int, int>> vehiclesPerHourPerDay;
    map<string, int> totalVehiclesPerDay;
    int totalVehicles = 0;

    for (const auto& entry : data) {
        string date = entry.time.substr(0, 10);
        int hour = stoi(entry.time.substr(11, 2));

        vehiclesPerHourPerDay[date][hour]++;
        totalVehiclesPerDay[date]++;
        totalVehicles++;
    }

    double averageVehiclesPerDay = totalVehicles / totalVehiclesPerDay.size();

    cout << "Congestion Level Report:" << endl;
    cout << "------------------------" << endl;

    string mostCongestedDay;
    int maxVehicles = 0;
    string mostCongestedPeriod;
    int maxHourVehicles = 0;

    for (const auto& dayData : vehiclesPerHourPerDay) {
        string day = dayData.first;
        int totalDayVehicles = totalVehiclesPerDay[day];

        if (totalDayVehicles > maxVehicles) {
            maxVehicles = totalDayVehicles;
            mostCongestedDay = day;
        }

        for (const auto& hourData : dayData.second) {
            int hour = hourData.first;
            int vehiclesAtHour = hourData.second;

            if (vehiclesAtHour > maxHourVehicles) {
                maxHourVehicles = vehiclesAtHour;
                mostCongestedPeriod = day + " " + to_string(hour) + ":00 - " + to_string(hour + 1) + ":00";
            }
        }
    }

    cout << "Average congestion level: "
        << (averageVehiclesPerDay < 100 ? "Low" : (averageVehiclesPerDay < 250 ? "Moderate" : "High"))
        << endl;

    cout << "Most congested period: " << mostCongestedPeriod << ", with " << maxHourVehicles << " vehicles" << endl;

    cout << "Congestion breakdown by day:" << endl;
    for (const auto& dayData : totalVehiclesPerDay) {
        cout << "  - " << dayData.first << ": "
            << (dayData.second < 100 ? "Low" : (dayData.second < 250 ? "Moderate" : "High"))
            << " (" << dayData.second << " vehicles)" << endl;
    }
}

// Menu to let user to choose a report to generate
void displayMenu() {
    cout << "\nTraffic Report Menu:" << endl;
    cout << "1. Traffic Summary Report" << endl;
    cout << "2. Peak Traffic Times Report" << endl;
    cout << "3. Congestion Level Report" << endl;
    cout << "4. Exit" << endl;
    cout << "Enter your choice: ";
}

int main() {
    string filename = "C:\\Data\\trafficdata.txt";  //file path for text data

    vector<TrafficData> data = readTrafficData(filename);

    int choice;
    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
        case 1:
            generateTrafficSummary(data);
            break;
        case 2:
            generatePeakTrafficReport(data);
            break;
        case 3:
            generateCongestionReport(data);
            break;
        case 4:
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Invalid choice. Please select a valid option." << endl;
        }
    } while (choice != 4);

    return 0;
}
