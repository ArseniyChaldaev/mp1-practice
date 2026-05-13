#define _CRT_SECURE_NO_WARNINGS
#include "reader.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>


int count_lines(const char* filepath) {
	FILE* file = fopen(filepath, "r");
	if (!file) return -1;

	int lines = 0;
	char line[1024];

	while (fgets(line, sizeof(line), file)) {
		if (line[0] == '\n' || line[0] == '\r') continue;
		lines++;
	}
	fclose(file);
	return lines;
}

int count_stops(const char* stop_token) {
	char copy_stop_token[1024];
	int count = 0;
	char* token;

	if (stop_token == NULL) return 0;

	strcpy(copy_stop_token, stop_token);
	token = strtok(copy_stop_token, ",");

	while (token != NULL) {
		count++;
		token = strtok(NULL, ",");
	}

	return count;
}

char** parse_stop_token(const char* stop_token, int* out_count) {
	int i = 0;
	int j = 0;
	int count = count_stops(stop_token);
	char copy_stop_token[1024];
	char* minor_token;

	if (count == 0) {
		printf("\nCounting stops error\n");
		*out_count = 0;
		return 0;
	}

	*out_count = count;

	strcpy(copy_stop_token, stop_token);

	char** stops = malloc(sizeof(char*) * count);
	if (!stops) {
		printf("\nMemory allocation (in stops) error!\n");
		return 0;
	}

	minor_token = strtok(copy_stop_token, ",");
	while (i < count && minor_token != NULL) {
		stops[i] = malloc(strlen(minor_token) + 1);
		if (!stops[i]) {
			printf("\nMemory allocation (in token stop) error!\n");
			for (j = 0; j < i; ++j) free(stops[j]);
			free(stops);
			*out_count = 0;
			return 0;

		}
		strcpy(stops[i], minor_token);
		i++;
		minor_token = strtok(NULL, ",");

	}
	return stops;

}

int parse_datetime(const char* time_token, struct tm* out_tm) {
	if (!time_token || !out_tm) return 0;

	int year, month, day, hour, min;
	if (sscanf(time_token, "%d-%d-%d %d:%d", &year, &month, &day, &hour, &min) != 5) {
		printf("\nParsing time error\n");
		return 0;
	}

	out_tm->tm_year = year - 1900;
	out_tm->tm_mon = month - 1;
	out_tm->tm_mday = day;
	out_tm->tm_hour = hour;
	out_tm->tm_min = min;
	out_tm->tm_sec = 0;

	return 1;
}

BusFlight* read_flights(const char* filepath, int* out_count) {
	int i = 0;
	char buffer[1024];
	char copy_buffer[1024];


	int lines = count_lines(filepath);
	if (lines <= 0) {
		printf("\nFile reading error!\n");
		return NULL;
	}
	BusFlight* flights = malloc(sizeof(BusFlight) * lines);
	if (!flights) {
		printf("\nMemory allocation error!\n");
		return NULL;
	}

	FILE* file = fopen(filepath, "r");
	if (!file) {
		printf("\nOpen file error!\n");
		free(flights);
		return NULL;
	}

	while (i < lines && fgets(buffer, sizeof(buffer), file)) {
		if (buffer[0] == '\n' || buffer[0] == '\r') continue;
		strcpy(copy_buffer, buffer);

		char* num = strtok(copy_buffer, ";");
		char* stops = strtok(NULL, ";");
		char* time = strtok(NULL, ";");
		char* seats = strtok(NULL, ";");

		flights[i].number = atoi(num);
		flights[i].free_seats = atoi(seats);

		flights[i].stops = parse_stop_token(stops, &flights[i].stop_count);

		if (flights[i].stops == NULL) {
			free_flights(flights, i);
			fclose(file);
			return NULL;
		}

		if (!parse_datetime(time, &flights[i].departure)) {
			free_flights(flights, i);
			fclose(file);
			return NULL;
		}
		i++;
	}
	fclose(file);
	*out_count = lines;
	return flights;
}

void free_flights(BusFlight* flights, int count) {
	int i = 0;
	int j = 0;
	if (!flights) return;
	for (i = 0; i < count; i++) {
		for (j = 0; j < flights[i].stop_count; j++) {
			free(flights[i].stops[j]);
		}
		free(flights[i].stops);
	}
	free(flights);
}