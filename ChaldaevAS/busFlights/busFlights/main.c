// C:\Users\Arseniy\Documents\busFlight.txt

#define _CRT_SECURE_NO_WARNINGS
#ifdef _WIN32
#define strcasecmp _stricmp
#endif
#include "reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define MAX_INPUT 512

int main() {
	BusFlight* flights = NULL;
	int count = 0;

	printf("------[Bus flights]------\n");

	while (1) {
		char filepath[MAX_INPUT];
		printf("\nEnter the path to the bus route file: ");
		if (fgets(filepath, sizeof(filepath), stdin) == NULL) {
			printf("Enter error!\n");
			continue;
		}

		filepath[strcspn(filepath, "\n")] = '\0';

		flights = read_flights(filepath, &count);
		if (flights == NULL) continue;

		printf("Loaded %d flights.\n", count);
		break;
	}

	while (1) {
		char target[MAX_INPUT];
		int best_index = -1;
		time_t best_time = 0;
		time_t now = time(NULL);
		int i = 0;
		int j = 0;

		printf("\nEnter your destination (or '0' to exit): ");

		if (fgets(target, sizeof(target), stdin) == NULL) {
			printf("Enter error!\n");
			continue;
		}
		target[strcspn(target, "\n")] = '\0';
		if (strcmp(target, "0") == 0) {
			break;
		}

		for (i = 0; i < count; i++) {
			int has_stop = 0;
			for (j = 0; j < flights[i].stop_count; j++) {
				if (strcasecmp(flights[i].stops[j], target) == 0) {
					has_stop = 1;
					break;
				}
			}

			if (!has_stop) continue;
			if (flights[i].free_seats <= 0) continue;

			time_t dep = mktime(&flights[i].departure);
			if (dep == -1) continue;
			if (difftime(dep, now) < 0) continue;

			if (best_index == -1 || dep < best_time) {
				best_index = i;
				best_time = dep;
			}

		}
		if (best_index == -1) {
			printf("No available flights to '%s' with free seats.\n", target);
		}
		else {
			BusFlight* best = &flights[best_index];
			char time_string[80];
			strftime(time_string, sizeof(time_string), "%Y-%m-%d %H:%M", &best->departure);
			printf("\n=== NEAREST FLIGHT ===\n");
			printf("Route number:   %d\n", best->number);
			printf("Departure time: %s\n", time_string);
			printf("Free seats:     %d\n", best->free_seats);
			printf("======================\n");
		}
	}

	free_flights(flights, count);
	return 0;
}
