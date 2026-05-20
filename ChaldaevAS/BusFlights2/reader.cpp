#include "reader.h"

#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>



bool fileExist(const string& filepath) {

	ifstream file(filepath);

	if (!file.is_open()) return false;

	return true;
}

int countNonEmptyLines(const string& filepath) {
	ifstream file(filepath);
	if (!file.is_open()) return -1;

	int count = 0;
	string line;
	while (getline(file, line)) {
		if (!line.empty()) ++count;
	}

	return count;
}

int countStops(const string& stopsString) {
	if (stopsString.empty()) return 0;

	istringstream ss(stopsString);

	int count = 0;
	string stop;
	while (getline(ss, stop, ',')) ++count;

	return count;
}

Flight* allocateFlights(int count) {
	if (count <= 0) return nullptr;

	return new Flight[count];
}

string* allocateAndFillStops(const string& stopsString, int& count) {
	if (stopsString.empty()) {
		count = 0;
		return nullptr;
	}

	int stopCount = countStops(stopsString);
	if (stopCount == 0) {
		count = 0;
		return nullptr;
	}

	string* stopsArray = new string[stopCount];

	istringstream ss(stopsString);
	string stop;
	int inx = 0;
	while (getline(ss, stop, ',')) stopsArray[inx++] = stop;

	count = inx;
	return stopsArray;
}

time_t parseTime(const string& datatimeString) {
	tm t = {};
	istringstream ss(datatimeString);

	ss >> get_time(&t, "%d.%m.%Y %H:%M");


	if (ss.fail()) return -1;

	return mktime(&t);
}

void parseAndFillFlights(Flight* flights, int maxCount, const string& filename, int& outCount) {
	ifstream file(filename);

	if (!file.is_open()) {
		outCount = 0;
		return;
	}

	int idx = 0;
	string line;

	int number;
	int stopCount;
	time_t departure;
	int seats;


	while (getline(file, line) && idx < maxCount) {
		if (line.empty()) continue;

		istringstream lineStream(line);
		string token;

		if (!getline(lineStream, token, ';')) continue;
		number = stoi(token);

		if (!getline(lineStream, token, ';')) continue;
		string* stops = allocateAndFillStops(token, stopCount);

		if (!getline(lineStream, token, ';')) {
			delete[] stops;
			continue;
		}

		departure = parseTime(token);
		if (departure == -1) {
			delete[] stops;
			continue;
		}

		if (!getline(lineStream, token, ';')) {
			delete[] stops;
			continue;
		}
		seats = stoi(token);
		if (seats < 0) seats = 0;

		flights[idx].number = number;
		flights[idx].freeSeats = seats;
		flights[idx].stopCount = stopCount;
		flights[idx].stops = stops;
		flights[idx].departure = departure;

		++idx;
	}

	outCount = idx;
}

void freeAllFlights(Flight* flights, int count) {
	if (!flights) return;

	for (int i = 0; i < count; ++i) {
		delete[] flights[i].stops;
		flights[i].stops = nullptr;
	}

	delete[] flights;
}
