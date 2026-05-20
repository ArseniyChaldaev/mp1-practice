#include <string>
#include "reader.h"
#include "ui.h"
#include <iostream>
#include "searcher.h"




void runProgramm() {
	int choice = -1;
	string filepath;
	Flight* flights = nullptr;
	int flightCount = 0;
	string targetDestination;

	do {
		clearScreen();
		show_main_menu();
		cin >> choice;

		switch (choice) {


		case 1: {
			clearScreen();
			show_file_menu();
			cin.ignore();
			getline(cin, filepath);

			targetDestination.clear();

			if (filepath == "BACK") {
				filepath.clear();
				cout << "Filepath input cancelled.\n";
				break;
			}

			if (!fileExist(filepath)) {
				cout << "Error: file not found!\n";
			}

			if (flights != nullptr) {
				freeAllFlights(flights, flightCount);
				flights = nullptr;
			}

			int lines = countNonEmptyLines(filepath);
			if (lines <= 0) {
				cout << "Error: file is empty or unreadable.\n";
				break;
			}

			flights = allocateFlights(lines);
			if (!flights) {
				cout << "Error: memory allocation failed.\n";
				break;
			}

			parseAndFillFlights(flights, lines, filepath, flightCount);
			cout << "Loaded " << flightCount << " flights.\n";

			break;
		}

		case 2: {
			clearScreen();
			show_destination_menu();
			cin.ignore();
			getline(cin, targetDestination);
			if (targetDestination == "BACK") {
				targetDestination.clear();
				cout << "Destination selection cancelled.\n";
			}
			else {
				cout << "Destination set to: " << targetDestination << endl;
			}
			break;
		}

		case 3: {
			clearScreen();
			if (!flights || flightCount == 0) {
				cout << "No flights loaded. Please select file first (option 1).\n";
				break;
			}
			if (targetDestination.empty()) {
				cout << "No destination selected. Please select destination (option 2).\n";
				break;
			}

			time_t now = time(nullptr);
			int idx = findNearestFlight(flights, flightCount, targetDestination, now);

			if (idx == -1) {
				cout << "No future flights to " << targetDestination << " found.\n";
			}
			else {
				printFlightInfo(flights[idx], targetDestination, now);
			}
			break;
		}
		case 0: {
			if (flights != nullptr) {
				freeAllFlights(flights, flightCount);
				flights = nullptr;
			}
			break;
		}
		default: cout << "Invalid choice. Try again.\n";
		}
	} while (choice != 0);
}
