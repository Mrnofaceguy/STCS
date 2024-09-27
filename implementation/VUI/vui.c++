#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include "C:\\Users\\Mrnofaceguy\\Documents\\GitHub\\STCS2\\implementation\\TCF\\tcf.c++"  // Include tcf.c++

// Global variables for auto control and update frequency
bool auto_control = true;       // Controls whether the heating is automatic
int frequency = 5;              // Default frequency for control logic
int update_frequency = 5;       // Display update frequency

#ifdef _WIN32
    #define CLEAR_SCREEN "cls"
#else
    #define CLEAR_SCREEN "clear"
#endif

// Function to clear the terminal screen
void clearScreen() {
    system(CLEAR_SCREEN);
}

// Function to display the system status (temperature, heaters, auto control)
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
    std::cout << "Update Frequency: " << update_frequency << " Hz\n";
    std::cout << "Target Temperatures:\n";
    std::cout << "Target 1: " << set_01 << "°C\n";
    std::cout << "Target 2: " << set_02 << "°C\n";
    std::cout << "Target 3: " << set_03 << "°C\n";
    std::cout << "Target 4: " << set_04 << "°C\n";
}

// Function to run temperature updates in the background
void updateTemperatures() {
    while (true) {
        check_period();  // Check the current period
        cycle();  // Call the cycle function from tcf.c++
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / frequency));  // Update at the specified frequency
    }
}

// Function to display temperatures live for 2 seconds
void showTemperaturesForTwoSeconds() {
    auto start = std::chrono::high_resolution_clock::now();

    while (true) {
        clearScreen();  // Clear the screen before each display
        displayStatus();  // Show the current status

        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / update_frequency));  // Refresh at the update frequency

        // Check if 2 seconds have passed
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = now - start;
        if (elapsed.count() >= 2.0) {
            break;  // Exit loop after 2 seconds
        }
    }
}

// Function to toggle auto control
void toggleAutoControl() {
    auto_control = !auto_control;  // Toggle the auto control state
    std::cout << "Auto control " << (auto_control ? "enabled." : "disabled.") << "\n";
}

// Function to set update frequency
void setUpdateFrequency() {
    int new_frequency;
    std::cout << "Enter the new update frequency for control (Hz): ";
    std::cin >> new_frequency;

    // Validate the input frequency (for example, between 1 Hz and 10 Hz)
    if (new_frequency >= 1 && new_frequency <= 10) {
        frequency = new_frequency; // Update the frequency variable in tcf.c++
        std::cout << "Control update frequency set to " << frequency << " Hz.\n";
    } else {
        std::cout << "Invalid frequency. Please enter a value between 1 and 10 Hz.\n";
    }
}

// Function to set target temperatures
void setTargetTemperatures() {
    std::cout << "Enter target temperature for Sensor 1 (-20 to 20): ";
    std::cin >> set_01;

    std::cout << "Enter target temperature for Sensor 2 (-20 to 20): ";
    std::cin >> set_02;

    std::cout << "Enter target temperature for Sensor 3 (-20 to 20): ";
    std::cin >> set_03;

    std::cout << "Enter target temperature for Sensor 4 (-20 to 20): ";
    std::cin >> set_04;

    // Validate the input temperatures
    if (set_01 < -20 || set_01 > 20 || 
        set_02 < -20 || set_02 > 20 || 
        set_03 < -20 || set_03 > 20 || 
        set_04 < -20 || set_04 > 20) {
        std::cout << "Invalid target temperatures. They must be between -20 and 20 degrees.\n";
    } else {
        std::cout << "Target temperatures set to:\n";
        std::cout << "Sensor 1: " << set_01 << "°C\n";
        std::cout << "Sensor 2: " << set_02 << "°C\n";
        std::cout << "Sensor 3: " << set_03 << "°C\n";
        std::cout << "Sensor 4: " << set_04 << "°C\n";
    }
}

// Main function for user interaction
void menu() {
    while (true) {
        int choice;
        std::cout << "\n--- Menu ---\n";
        std::cout << "1. Display Live Temperatures for 2 Seconds\n";
        std::cout << "2. Toggle Auto Control\n";
        std::cout << "3. Set Update Frequency\n";
        std::cout << "4. Set Target Temperatures\n";  // New option to set target temperatures
        std::cout << "5. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1: 
                showTemperaturesForTwoSeconds(); 
                break;
            case 2: 
                toggleAutoControl();  // Call the toggle function
                break;
            case 3:
                setUpdateFrequency();  // Call the set frequency function
                break;
            case 4:
                setTargetTemperatures();  // Call the set target temperatures function
                break;
            case 5: 
                std::cout << "Exiting program...\n";
                control_running = false;  // Set the control flag to false
                return;  // Exit the menu
            default: 
                std::cout << "Invalid choice, try again.\n"; 
                break;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));  // Give a slight pause before returning to the menu
    }
}

int main() {
    std::cout << "Initializing system...\n";
    initializeTemperatures();  // Call the renamed initialize function
    std::cout << "System initialized.\n";

    // Start the background temperature update thread
    std::thread updateThread(updateTemperatures);

    // Run the main menu for user interaction
    menu();

    // Join the update thread before exiting
    updateThread.join();

    return 0;
}
