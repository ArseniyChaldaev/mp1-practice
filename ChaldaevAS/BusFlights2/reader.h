#ifndef READER_H
#define READER_H

#include <string>
#include <ctime>
using namespace std;


struct Flight {
	int number;
	int stopCount;
	string* stops;
	time_t departure;
	int freeSeats;
};

bool fileExist(const string& filepath);

int countNonEmptyLines(const string& filepath);
int countStops(const string& stopsString);

Flight* allocateFlights(int count);
string* allocateAndFillStops(const string& stopsString, int& count);

time_t parseTime(const string& datatimeString);

void parseAndFillFlights(Flight* flights, int maxCount, const string& filename, int& outCount);


void freeAllFlights(Flight* flights, int count);

#endif