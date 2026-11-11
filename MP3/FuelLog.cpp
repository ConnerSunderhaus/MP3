// FuelLog.cpp, Conner Sunderhaus, 11/10/26
// this program caculates the distance, gallons, 
// cost, and gallons per mile based on odometer 
// readings and money spent, outputting them into a neat table.

#include <array>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

int indexOfLargest(double values[], int count);
int indexOfSmallest(double values[], int count);
double sumOf(double values[], int count);
double costPerMile(double cost, double miles);
double milesPerGallon(double miles, double gallons);
double fillUpCost(double amount, double pricePerUnit);
double toGallons(double amount, char unit);
double averageOf(double amt1[], int size);

const int MAX_FILL_UPS = 20;

void main() {
	double cost[MAX_FILL_UPS];
	double fuelAmt[MAX_FILL_UPS];
	char fuelUnit[MAX_FILL_UPS];
	int odometer[MAX_FILL_UPS + 1];
	double miles[MAX_FILL_UPS];
	double gallons[MAX_FILL_UPS];
	double mpg[MAX_FILL_UPS];
	double cpg[MAX_FILL_UPS];
	double cpm[MAX_FILL_UPS];
	int fillUps = 0;
	double budget;

		cout << "Enter starting odometer reading: ";
		cin >> odometer[0];
		cout << "Enter number of fill-ups: ";
		cin >> fillUps;

		int leg = fillUps - 1;

		if (fillUps > 20) {
			cout << "Error -- you can only enter 20 fill-ups";
			return;
		}

		for (int i = 0; i < fillUps; i++) {
			cout << "Fill-up " << i + 1 << " (odometer, amount, G or L, price, total): ";
			cin >> odometer[i + 1] >> fuelAmt[i] >> fuelUnit[i] >> cpg[i] >> cost[i];
		}

		for (int i = 0; i < fillUps; i++) {
			miles[i] = odometer[i + 1] - odometer[i];
			gallons[i] = toGallons(fuelAmt[i], fuelUnit[i]);
			mpg[i] = milesPerGallon(miles[i], gallons[i]);
			cpm[i] = costPerMile(cost[i], miles[i]);
		}
		
		cout << "Enter fuel budget for the trip: ";
		cin >> budget;

		cout << "Leg   Miles   Gallons   MPG     Cost  Cost/mile" << endl;
		for (int i = 0; i < fillUps; i++) {
			printf("%3d%8.0f%10.2f%6.1f  $%6.3f  $%7.2f\n", i + 1, miles[i], gallons[i], cost[i], cpg[i], cpm[i]);
		}
		printf("Total miles: %.0f\n", sumOf(miles, fillUps));
		printf("Total gallons: %.0f\n", sumOf(gallons, fillUps));
		printf("Overall MPG: %.3f\n", averageOf(mpg, fillUps));
		printf("Total fuel cost: %.2f\n", sumOf(cost, fillUps));
		printf("Cheapest leg per mile: Leg %d\n", indexOfSmallest(cpm, fillUps) + 1);
		printf("Most expensive leg per mile: Leg %d\n", indexOfLargest(cpm, fillUps) + 1);
		printf("%.2f budget used out of $%.2f\n", sumOf(cost, fillUps), budget);
}

double averageOf(double amt1[], int size) {
	double avg = 0;
	for (int i = 0; i < size; i++) {
		avg = sumOf(amt1, size) / size;
	}
	return avg;
}

double toGallons(double amount, char unit) {
	if (unit == 'L' || unit == 'l') {
		amount /= 3.785;
	}
	return amount;
}

double fillUpCost(double amount, double pricePerUnit) {
	double cost = amount * pricePerUnit;
	return cost;
}

double milesPerGallon(double miles, double gallons) {
	double mpg = miles / gallons;
	return mpg;
}

double costPerMile(double cost, double miles) {
	double total = cost / miles;
	return total;
}

double sumOf(double values[], int count) {
	int total = 0;
	for (int i = 0; i < count; i++) {
		total += values[i];
	}
	return total;
}

int indexOfSmallest(double values[], int count) {
	int smallest = 0;
	for (int i = 0; i < count; i++) {
		if (smallest > values[i]) {
			smallest = i;
		}
	}
	return smallest;
}

int indexOfLargest(double values[], int count) {
	int largest = 0;
	for (int i = 0; i < count; i++) {
		if (largest < values[i]) {
			largest = i;
		}
	}
	return largest;
}