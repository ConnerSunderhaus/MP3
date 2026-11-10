// FuelLog.cpp, Conner Sunderhaus, 11/10/26
// this program caculates the distance, gallons, 
// cost, and gallons per mile based on odometer 
// readings and money spent, outputting them into a neat table.

#include <array>
#include <iomanip>
#include <string>

using namespace std;

int indexOfLargest(double values[], int count);
int indexOfSmallest(double values[], int count);
double sumOf(double values[], int count);
double costPerMile(double cost, double miles);
double milesPerGallon(double miles, double gallons);
double fillUpCost(double amount, double pricePerUnit);
double toGallons(double amount, char unit);

const int FILL_UPS = 20;

void main() {

}


double toGallons(double amount, char unit) {
	if (unit == 'G' || unit == 'g') {
		amount *= 3.785;
	}
	return amount;
}

double fillUpCost(double amount, double pricePerUnit) {
	double cost = amount * pricePerUnit;
	return cost;
}

double milesPerGallon(double miles, double gallons) {
	double mpg = miles * gallons;
	return mpg;
}

double costPerMile(double cost, double miles) {

}

double sumOf(double values[], int count) {

}

int indexOfSmallest(double values[], int count) {

}

int indexOfLargest(double values[], int count) {

}