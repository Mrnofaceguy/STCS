#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>
#include <cstdlib>
#include <conio.h>  // For _kbhit() and _getch() on Windows

#ifdef _WIN32
    #define CLEAR_SCREEN "cls"
#else
    #define CLEAR_SCREEN "clear"
#endif

// Global Variables
int therm_01 = 25, therm_02 = 30, therm_03 = 20, therm_04 = 35;
bool htr_01 = false, htr_02 = false, htr_03 = false, htr_04 = false;
bool auto_control = true;
int set_01 = 28, set_02 = 32, set_03 = 22, set_04 = 36;
std::atomic<int> update_frequency(5); // Default frequency 5 Hz
std::atomic<bool> exit_program(false); // To control program termination
std::atomic<bool> displaying_menu(false); // To avoid input during display

std::mutex mtx;

// Function to clear the terminal screen
void clearScreen() {
    system(CLEAR_SCREEN);
}

// Function to display the system status (temperature, heaters, auto control)
void displayStatus() {
    std::lock_guard<std::mutex> lock(mtx);  // Ensure thread-safe access
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
    std::cout << "Update Frequency: " << update_frequency.load() << " Hz\n";
}

// Function to simulate temperature updates and auto control
void updateTemperatures() {
    while (!exit_program.load()) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            // Simulate temperature changes
            therm_01 += (rand() % 3 - 1);
            therm_02 += (rand() % 3 - 1);
            therm_03 += (rand() % 3 - 1);
            therm_04 += (rand() % 3 - 1);
            
            // Auto-control logic (e.g., turning heaters on or off based on temperature)
            if (auto_control) {
                htr_01 = therm_01 < set_01;
                htr_02 = therm_02 < set_02;
                htr_03 = therm_03 < set_03;
                htr_04 = therm_04 < set_04;
            }
        }

        // Sleep for the current update frequency
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / update_frequency.load()));
    }
}

// Function to adjust update frequency based on user input
void changeFrequency() {
    std::cout << "Enter new update frequency (Hz): ";
    int freq;
    std::cin >> freq;

    if (freq > 0) {
        update_frequency.store(freq);
        std::cout << "Update frequency set to " << freq << " Hz.\n";
    } else {
        std::cout << "Invalid frequency value. Please enter a positive number.\n";
    }
}

// Function to toggle auto control
void toggleAutoControl() {
    std::lock_guard<std::mutex> lock(mtx);
    auto_control = !auto_control;
    std::cout << "Auto control " << (auto_control ? "enabled." : "disabled.") << "\n";
}

// Manual control of heaters
void manualControl() {
    int heater, action;
    std::cout << "Enter heater number (1-4) to override: ";
    std::cin >> heater;
    std::cout << "Enter action (1 to turn ON, 0 to turn OFF): ";
    std::cin >> action;

    std::lock_guard<std::mutex> lock(mtx);
    switch (heater) {
        case 1: htr_01 = action; break;
        case 2: htr_02 = action; break;
        case 3: htr_03 = action; break;
        case 4: htr_04 = action; break;
        default: std::cout << "Invalid heater number\n"; return;
    }
    std::cout << "Heater " << heater << (action ? " turned ON.\n" : " turned OFF.\n");
}

// Function to set temperature limits
void setTemperatureLimits() {
    int sensor, min_temp, max_temp;
    std::cout << "Enter sensor number (1-4) to set limits: ";
    std::cin >> sensor;
    std::cout << "Enter minimum temperature: ";
    std::cin >> min_temp;
    std::cout << "Enter maximum temperature: ";
    std::cin >> max_temp;

    std::lock_guard<std::mutex> lock(mtx);
    switch (sensor) {
        case 1: set_01 = max_temp; break;
        case 2: set_02 = max_temp; break;
        case 3: set_03 = max_temp; break;
        case 4: set_04 = max_temp; break;
        default: std::cout << "Invalid sensor number\n"; return;
    }
    std::cout << "Limits for Sensor " << sensor << " set. Min: " << min_temp << " Max: " << max_temp << "\n";
}

// Function for live temperature display
void temperatureDisplay() {
    while (!exit_program.load()) {
        if (!displaying_menu.load()) {
            clearScreen();  // Clear the screen before each display
            displayStatus();  // Show the current status
            std::this_thread::sleep_for(std::chrono::milliseconds(1000 / update_frequency.load()));  // Sleep based on update frequency
        }
    }
}

// Main function for user interaction
void menu() {
    while (!exit_program.load()) {
        if (_kbhit()) {
            int choice = _getch();
            displaying_menu.store(true);  // Stop updating the display during menu interaction
            clearScreen();  // Clear the screen before handling input

            std::cout << "\n--- Menu ---\n";
            std::cout << "1. Display Status\n";
            std::cout << "2. Toggle Auto Control\n";
            std::cout << "3. Manual Heater Control\n";
            std::cout << "4. Set Temperature Limits\n";
            std::cout << "5. Change Update Frequency\n";
            std::cout << "6. Exit\n";
            std::cout << "Enter your choice: ";

            switch (choice) {
                case '1': displayStatus(); break;
                case '2': toggleAutoControl(); break;
                case '3': manualControl(); break;
                case '4': setTemperatureLimits(); break;
                case '5': changeFrequency(); break;
                case '6': exit_program.store(true); break;
                default: break; // Ignore invalid keys
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(1000));  // To allow user to read output
            displaying_menu.store(false);  // Resume updating the display after menu interaction
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Rest the CPU
    }
}

int main() {
    // Start the temperature update thread
    std::thread updateThread(updateTemperatures);

    // Start a separate thread for live temperature display
    std::thread displayThread(temperatureDisplay);

    // Handle user input asynchronously
    menu();

    // Join threads before exiting
    updateThread.join();
    displayThread.join();

    return 0;
}
