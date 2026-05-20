#include <iostream>
#include "ui.h"

using namespace std;



void show_main_menu() {
	cout << "_________________________________\n";
	cout << "|                               |\n";
	cout << "|      BUS FLIGHT SEARCHER      |\n";
	cout << "|_______________________________|\n";
	cout << "|   0. Exit programm            |\n";
	cout << "|                               |\n";
	cout << "|   1. To select file           |\n";
	cout << "|   2. Select a destination     |\n";
	cout << "|   3. Show the nearest flights |\n";
	cout << "|_______________________________|\n";
	cout << "| Choose an option: ";

}

void clearScreen() {
	cout << "\033[2J\033[1;1H" << flush;
}

void show_file_menu() {
	cout << "_________________________________\n";
	cout << "|                               |\n";
	cout << "|   1. To select file           |\n";
	cout << "|_______________________________|\n\n";
	cout << "Write filepath or 'BACK' to return to main menu: ";
}

void show_destination_menu() {
	cout << "_________________________________\n";
	cout << "|                               |\n";
	cout << "|   2. Select a destination     |\n";
	cout << "|_______________________________|\n\n";
	cout << "Write destination or 'BACK' to return to main menu: ";
}