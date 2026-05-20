#define _CRT_SECURE_NO_WARNINGS
#include <iomanip>
#include <iostream>
#include "reader.h"

using namespace std;



int findNearestFlight(Flight* flights, int flightCount, string& destination, time_t now) {
	int bestIndex = -1;
	long long bestDiff = 0;

	for (int i = 0; i < flightCount; ++i) {
		bool found = false;
		for (int s = 0; s < flights[i].stopCount; ++s) {
			if (flights[i].stops[s] == destination) {
				found = true;
				break;
			}
		}
		if (!found) continue;

		time_t dep = flights[i].departure;
		if (dep <= now) continue;
		long long diff = dep - now;
		if (bestIndex == -1 || diff < bestDiff) {
			bestIndex = i;
			bestDiff = diff;
		}
	}
	return bestIndex;
}

void printFlightInfo(const Flight& flight, const string& destination, time_t now) {
	cout << "Nearest flight to " << destination << ":\n";
	cout << "  Flight number: " << flight.number << endl;
	cout << "  Departure: " << ctime(&flight.departure); // ctime уже добавляет \n
	cout << "  Free seats: " << flight.freeSeats << endl;

	long long diff = flight.departure - now;
	long long minutes = diff / 60;
	long long hours = minutes / 60;
	minutes %= 60;
	cout << "  Time left: " << hours << "h " << minutes << "m\n";
}
