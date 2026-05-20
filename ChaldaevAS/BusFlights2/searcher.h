#ifndef SEARCHER_H
#define SEARCHER_H

#include "reader.h"
#include <ctime>

int findNearestFlight(Flight* flights, int flightCount, std::string& destination, time_t now);

void printFlightInfo(const Flight& flight, const std::string& destination, time_t now);

#endif