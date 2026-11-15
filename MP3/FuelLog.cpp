#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

// Required function prototypes from original MP3 specification
double toGallons(double amount, char unit);
double fillUpCost(double amount, double pricePerUnit);
double milesPerGallon(double miles, double gallons);
double costPerMile(double cost, double miles);
double sumOf(double values[], int count);
int indexOfSmallest(double values[], int count);
int indexOfLargest(double values[], int count);

// Required function prototypes from MP3 Change Request (Exchange Rates)
int dayOfYear(int month, int day);
int loadRates(string fileName, double rates[], int size);
void fillMissingRates(double rates[], int size);

const int MAX_FILLUPS = 20;
const int DAYS_IN_YEAR = 366;

int main() {
    double rates[DAYS_IN_YEAR];
    if (loadRates("rates2026.csv", rates, DAYS_IN_YEAR) == -1) {
        cout << "Error: Could not open rates2026.csv." << endl;
        return 1;
    }
    fillMissingRates(rates, DAYS_IN_YEAR);

    int month[MAX_FILLUPS];
    int day[MAX_FILLUPS];
    double odometer[MAX_FILLUPS + 1];
    double fuelAmt[MAX_FILLUPS];
    char fuelUnit[MAX_FILLUPS];
    double pricePerUnit[MAX_FILLUPS];
    double totalSale[MAX_FILLUPS];
    string moneyUnit[MAX_FILLUPS];

    double miles[MAX_FILLUPS];
    double gallons[MAX_FILLUPS];
    double mpg[MAX_FILLUPS];
    double cost[MAX_FILLUPS];
    double cpm[MAX_FILLUPS];

    int fillUps = 0;
    double budget = 0.0;

    cout << "Enter starting odometer reading: ";
    cin >> odometer[0];

    cout << "Enter number of fill-ups: ";
    cin >> fillUps;
    while (fillUps < 1 || fillUps > MAX_FILLUPS) {
        cout << "Error: Number of fill-ups must be between 1 and " << MAX_FILLUPS << ". Please try again: ";
        cin >> fillUps;
    }

    for (int i = 0; i < fillUps; i++) {
        bool valid = false;
        while (!valid) {
            cout << "Fill-up " << i + 1
                << " (month, day, odometer, amount, G or L, price per unit, total sale, CAD or USD): ";
            cin >> month[i] >> day[i] >> odometer[i + 1] >> fuelAmt[i]
                >> fuelUnit[i] >> pricePerUnit[i] >> totalSale[i] >> moneyUnit[i];

            // Normalize unit letter and currency string to uppercase
            if (fuelUnit[i] == 'g') fuelUnit[i] = 'G';
            if (fuelUnit[i] == 'l') fuelUnit[i] = 'L';
            for (size_t c = 0; c < moneyUnit[i].length(); c++) {
                if (moneyUnit[i][c] >= 'a' && moneyUnit[i][c] <= 'z') {
                    moneyUnit[i][c] = moneyUnit[i][c] - 'a' + 'A';
                }
            }

            int doy = dayOfYear(month[i], day[i]);

            if (doy == -1) {
                cout << "Error: " << month[i] << "/" << day[i]
                    << " is not a valid 2026 date. Please re-enter Fill-up " << i + 1 << "." << endl;
            }
            else if (odometer[i + 1] <= odometer[i]) {
                cout << "Error: Odometer reading must be greater than the previous reading ("
                    << odometer[i] << "). Please re-enter Fill-up " << i + 1 << "." << endl;
            }
            else if (fuelUnit[i] != 'G' && fuelUnit[i] != 'L') {
                cout << "Error: Fuel unit must be G (gallons) or L (liters). Please re-enter Fill-up "
                    << i + 1 << "." << endl;
            }
            else if (moneyUnit[i] != "CAD" && moneyUnit[i] != "USD") {
                cout << "Error: Money unit must be CAD or USD. Please re-enter Fill-up "
                    << i + 1 << "." << endl;
            }
            else if ((moneyUnit[i] == "CAD" || fuelUnit[i] == 'L') && rates[doy] == -1.0) {
                cout << "Error: No exchange rate available for " << month[i] << "/" << day[i]
                    << "/2026. Please re-enter Fill-up " << i + 1 << "." << endl;
            }
            else {
                valid = true;
            }
        }
    }

    cout << "Enter fuel budget for the trip: ";
    cin >> budget;

    // Compute per-leg arrays after reading the entire log
    for (int i = 0; i < fillUps; i++) {
        miles[i] = odometer[i + 1] - odometer[i];
        gallons[i] = toGallons(fuelAmt[i], fuelUnit[i]);
        mpg[i] = milesPerGallon(miles[i], gallons[i]);

        int doy = dayOfYear(month[i], day[i]);
        if (moneyUnit[i] == "CAD") {
            cost[i] = totalSale[i] * rates[doy];
            pricePerUnit[i] = pricePerUnit[i] * rates[doy];
        }
        else {
            cost[i] = totalSale[i];
        }

        cpm[i] = costPerMile(cost[i], miles[i]);
    }

    // Print report using printf for every line of the report
    printf("\nLeg     Miles   Gallons     MPG       Cost   Cost/mile\n");
    for (int i = 0; i < fillUps; i++) {
        printf("%3d  %8.1f  %8.2f  %6.1f   $%7.2f     $%6.3f\n",
            i + 1, miles[i], gallons[i], mpg[i], cost[i], cpm[i]);
    }

    double totalMiles = sumOf(miles, fillUps);
    double totalGallons = sumOf(gallons, fillUps);
    double overallMpg = milesPerGallon(totalMiles, totalGallons);
    double totalCost = sumOf(cost, fillUps);
    double overallCpm = costPerMile(totalCost, totalMiles);

    printf("\nTotal miles: %.1f\n", totalMiles);
    printf("Total gallons: %.2f\n", totalGallons);
    printf("Overall MPG: %.1f\n", overallMpg);
    printf("Total fuel cost: $%.2f\n", totalCost);
    printf("Overall cost per mile: $%.3f\n", overallCpm);
    printf("Cheapest leg per mile: Leg %d\n", indexOfSmallest(cpm, fillUps) + 1);
    printf("Most expensive leg per mile: Leg %d\n", indexOfLargest(cpm, fillUps) + 1);

    if (totalCost < budget) {
        printf("Under budget by $%.2f\n", budget - totalCost);
    }
    else if (totalCost > budget) {
        printf("Over budget by $%.2f\n", totalCost - budget);
    }
    else {
        printf("Exactly on budget by $0.00\n");
    }

    return 0;
}

double toGallons(double amount, char unit) {
    if (unit == 'L' || unit == 'l') {
        amount /= 3.785;
    }
    return amount;
}

double fillUpCost(double amount, double pricePerUnit) {
    return amount * pricePerUnit;
}

double milesPerGallon(double miles, double gallons) {
    return miles / gallons;
}

double costPerMile(double cost, double miles) {
    return cost / miles;
}

double sumOf(double values[], int count) {
    double total = 0.0;
    for (int i = 0; i < count; i++) {
        total += values[i];
    }
    return total;
}

int indexOfSmallest(double values[], int count) {
    int smallest = 0;
    for (int i = 1; i < count; i++) {
        if (values[i] < values[smallest]) {
            smallest = i;
        }
    }
    return smallest;
}

int indexOfLargest(double values[], int count) {
    int largest = 0;
    for (int i = 1; i < count; i++) {
        if (values[i] > values[largest]) {
            largest = i;
        }
    }
    return largest;
}

int dayOfYear(int month, int day) {
    int daysInMonth[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (month < 1 || month > 12) {
        return -1;
    }
    if (day < 1 || day > daysInMonth[month]) {
        return -1;
    }
    int doy = 0;
    for (int m = 1; m < month; m++) {
        doy += daysInMonth[m];
    }
    doy += day;
    return doy;
}

int loadRates(string fileName, double rates[], int size) {
    for (int i = 0; i < size; i++) {
        rates[i] = -1.0;
    }

    ifstream inFile(fileName);
    if (!inFile.is_open()) {
        return -1;
    }

    string header;
    getline(inFile, header); // Skip header line ("Month,Day,Rate")

    int count = 0;
    int month = 0;
    int day = 0;
    double rate = 0.0;
    char comma1 = ',', comma2 = ',';

    while (inFile >> month >> comma1 >> day >> comma2 >> rate) {
        int doy = dayOfYear(month, day);
        if (doy >= 1 && doy < size) {
            rates[doy] = rate;
            count++;
        }
    }

    inFile.close();
    return count;
}

void fillMissingRates(double rates[], int size) {
    int firstDay = -1;
    int lastDay = -1;

    for (int i = 1; i < size; i++) {
        if (rates[i] != -1.0) {
            if (firstDay == -1) {
                firstDay = i;
            }
            lastDay = i;
        }
    }

    if (firstDay != -1 && lastDay != -1) {
        for (int i = firstDay + 1; i <= lastDay; i++) {
            if (rates[i] == -1.0) {
                rates[i] = rates[i - 1];
            }
        }
    }
}