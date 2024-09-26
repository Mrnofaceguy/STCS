#include <iostream>
#include <fstream>   
#include <string>
#include <random> 
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <iomanip>

#include "C:\Users\Mrnofaceguy\Documents\GitHub\STCS2\implementation\TCF\tcf.c++"
// Global variables from other files
extern int therm_01, therm_02, therm_03, therm_04;
extern bool htr_01, htr_02, htr_03, htr_04;
extern int set_01, set_02, set_03, set_04;
bool auto_control; // Auto control status

void displayStatus() {
    std::cout << "\n--- System Status ---\n";
    std::cout << "Temperature Sensors:\n";
    std::cout << "Sensor 1: " << therm_01 << "°C\n";
    std::cout << "Sensor 2: " << therm_02 << "°C\n";
    std::cout << "Sensor 3: " << therm_03 << "°C\n";
    std::cout << "Sensor 4: " << therm_04 << "°C\n";
    std::cout << "Heater Status:\n";
    std::cout << "Heater 1: " << (htr_01 ? "ON" : "OFF") << "\n";
    std::cout << "Heater 2: " << (htr_02 ? "ON" : "OFF") << "\n";
    std::cout << "Heater 3: " << (htr_03 ? "ON" : "OFF") << "\n";
    std::cout << "Heater 4: " << (htr_04 ? "ON" : "OFF") << "\n";
    std::cout << "Auto Control: " << (auto_control ? "Enabled" : "Disabled") << "\n";
}

void toggleAutoControl() {
    auto_control = !auto_control;
    std::cout << "Auto control " << (auto_control ? "enabled." : "disabled.") << "\n";
}

void manualControl() {
    int heater, action;
    std::cout << "Enter heater number (1-4) to override: ";
    std::cin >> heater;
    std::cout << "Enter action (1 to turn ON, 0 to turn OFF): ";
    std::cin >> action;

    switch (heater) {
        case 1: htr_01 = action; break;
        case 2: htr_02 = action; break;
        case 3: htr_03 = action; break;
        case 4: htr_04 = action; break;
        default: std::cout << "Invalid heater number\n"; return;
    }
    std::cout << "Heater " << heater << (action ? " turned ON.\n" : " turned OFF.\n");
}

void setTemperatureLimits() {
    int sensor, min_temp, max_temp;
    std::cout << "Enter sensor number (1-4) to set limits: ";
    std::cin >> sensor;
    std::cout << "Enter minimum temperature: ";
    std::cin >> min_temp;
    std::cout << "Enter maximum temperature: ";
    std::cin >> max_temp;

    switch (sensor) {
        case 1: set_01 = max_temp; break;
        case 2: set_02 = max_temp; break;
        case 3: set_03 = max_temp; break;
        case 4: set_04 = max_temp; break;
        default: std::cout << "Invalid sensor number\n"; return;
    }
    std::cout << "Limits for Sensor " << sensor << " set. Min: " << min_temp << " Max: " << max_temp << "\n";
}

void menu() {
    int choice;
    while (true) {
        std::cout << "\n--- Menu ---\n";
        std::cout << "1. Display Status\n";
        std::cout << "2. Toggle Auto Control\n";
        std::cout << "3. Manual Heater Control\n";
        std::cout << "4. Set Temperature Limits\n";
        std::cout << "5. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1: displayStatus(); break;
            case 2: toggleAutoControl(); break;
            case 3: manualControl(); break;
            case 4: setTemperatureLimits(); break;
            case 5: return; // Exit the menu
            default: std::cout << "Invalid choice, try again.\n"; break;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1)); // For smooth loop
    }
}



int main(){
    std::thread t1(control); 
    std::thread t2(inicialize); 
    std::thread t3(menu); 
    t1.join();
    t2.join();
    t3.join();
}