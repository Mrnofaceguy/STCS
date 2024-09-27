#include <iostream>
#include <fstream>   
#include <string>
#include <random> 
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <iomanip>
#include <sstream>

using namespace std;

int therm_01;
int therm_02;
int therm_03;
int therm_04;
bool htr_01;
bool htr_02;
bool htr_03;
bool htr_04;
string period;

void check_period() {
    auto now = std::chrono::steady_clock::now();
    auto duration = now.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

    uint8_t lowerBits = static_cast<uint8_t>(millis & 0xFF);
    if ((lowerBits >= 0x00 && lowerBits <= 0x1F) || (lowerBits >= 0x60 && lowerBits <= 0xFF)) {
        period = "normal";
    } else if (lowerBits >= 0x20 && lowerBits <= 0x3F) {
        period = "eclipse";
    } else if (lowerBits >= 0x40 && lowerBits <= 0x5F) {
        period = "SUN_EXPOSURE";
    }
}

void htr01(bool b) { htr_01 = b; }
void htr02(bool b) { htr_02 = b; }
void htr03(bool b) { htr_03 = b; }
void htr04(bool b) { htr_04 = b; }

int temp(int i) {
    if (i == 1) return therm_01;
    if (i == 2) return therm_02;
    if (i == 3) return therm_03;
    if (i == 4) return therm_04;
    return 0; // Default return
}

bool heater(int i) {
    if (i == 1) return htr_01;
    if (i == 2) return htr_02;
    if (i == 3) return htr_03;
    if (i == 4) return htr_04;
    return false; // Default return
}

std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm* tm_now = std::localtime(&now_time);
    std::ostringstream oss;
    oss << std::put_time(tm_now, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

void cycle() {
    int on, off;
    if (period == "normal") {
        on = 1;
        off = 1;
    } else if (period == "eclipse") {
        on = 4;
        off = 7;
    } else if (period == "SUN_EXPOSURE") {
        on = 7;
        off = 1;
    }

    if (htr_01) {
        therm_01 += on;
    } else {
        therm_01 -= off;
    }
    if (htr_02) {
        therm_02 += on;
    } else {
        therm_02 -= off;
    }
    if (htr_03) {
        therm_03 += on;
    } else {
        therm_03 -= off;
    }
    if (htr_04) {
        therm_04 += on;
    } else {
        therm_04 -= off;
    }

    // Log current state
    std::ofstream file("log.txt", std::ios::app);
    file << getCurrentTimestamp() << " " << therm_01 << " " << therm_02 << " " << therm_03 << " " << therm_04 << std::endl
         << htr_01 << " " << htr_02 << " " << htr_03 << " " << htr_04 << std::endl
         << period << std::endl << std::endl << "---------//---------" ;
    file.close();
}

void inicialize() {
    srand(time(0));

    // Initialize temperatures
    therm_01 = (rand() % 13) - 5;
    therm_02 = (rand() % 13) - 5;
    therm_03 = (rand() % 13) - 5;
    therm_04 = (rand() % 13) - 5;

    htr_01 = false;
    htr_02 = false;
    htr_03 = false;
    htr_04 = false;
    period = "normal";

    // Debugging output
    std::cout << "Initializing temperatures: "
              << therm_01 << " " << therm_02 << " "
              << therm_03 << " " << therm_04 << std::endl;

    const int frequency = 5; 
    const std::chrono::milliseconds interval(1000 / frequency); 
    for (int i = 0; i < 5; ++i) {  // Run for 5 iterations for testing
        check_period();
        cycle();
        std::this_thread::sleep_for(interval); // Pause for the interval
        std::cout << "Current temperatures: " 
                  << therm_01 << " " << therm_02 << " "
                  << therm_03 << " " << therm_04 << std::endl;
        std::cout << "Current period: " << period << std::endl;
    }
}
