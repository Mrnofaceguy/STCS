#include <iostream>
#include <fstream>   
#include <string>
#include <random> 
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <iomanip>
#include "C:\\Users\\Mrnofaceguy\\Documents\\GitHub\\STCS2\\implementation\\TSL\\tsl.c++"

// Declare frequency as an external variable
extern int frequency;  // This should match the definition in vui.c++

// Control running state
bool control_running = true;  // Flag to control the control thread

int Kp;
int Ki;
int Kd;
int set_01 = 0;
int set_02 = 0;
int set_03 = 0;
int set_04 = 0;

void control() {
    while (control_running) {  // Check the running state
        // Control logic to maintain temperatures within the desired range
        if (therm_01 >= set_01 || therm_01 >= 20) {
            htr01(false);
        }
        if (therm_01 <= -20) {
            htr01(true);
        }
        if (therm_02 >= set_02 || therm_02 >= 20) {
            htr02(false);
        }
        if (therm_02 <= -20) {
            htr02(true);
        }
        if (therm_03 >= set_03 || therm_03 >= 20) {
            htr03(false);
        }
        if (therm_03 <= -20) {
            htr03(true);
        }
        if (therm_04 >= set_04 || therm_04 >= 20) {
            htr04(false);
        }
        if (therm_04 <= -20) {
            htr04(true);
        }

        // Sleep for control frequency
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / frequency));
    }
}

void initializeTemperatures() {
    srand(time(0));

    // Initialize temperatures within the desired range of -7 to 5 degrees
    therm_01 = (rand() % 13) - 7; // Random value between -7 and 5
    therm_02 = (rand() % 13) - 7; // Random value between -7 and 5
    therm_03 = (rand() % 13) - 7; // Random value between -7 and 5
    therm_04 = (rand() % 13) - 7; // Random value between -7 and 5

    // Initializing heaters to off
    htr_01 = false;
    htr_02 = false;
    htr_03 = false;
    htr_04 = false;
    period = "normal";

    std::cout << "Temperatures initialized to: "
              << therm_01 << " " << therm_02 << " "
              << therm_03 << " " << therm_04 << std::endl;

    // Start the control loop in a separate thread
    std::thread controlThread(control);
    controlThread.detach();  // Detach so it runs independently
}
