#ifndef READER_H
#define READER_H

#include <time.h>

typedef struct {
    int number;
    int stop_count;
    char** stops;
    struct tm departure;
    int free_seats;
} BusFlight;

BusFlight* read_flights(const char* filepath, int* out_count);
void free_flights(BusFlight* flights, int count);

#endif