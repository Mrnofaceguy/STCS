#include <iostream>
#include <fstream>   
#include <string>
#include <random> 
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <iomanip>
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
void inicialize(){

    srand(time(0));
    
    therm_01 = (rand() % 13)-5;
    therm_02 = (rand() % 13)-5;
    therm_03 = (rand() % 13)-5;
    therm_04 = (rand() % 13)-5;
    htr_01 = false;
    htr_02 = false;
    htr_03 = false;
    htr_04 = false;
    period = "normal";
    //cout << therm_01 << " " << therm_02 << " " << therm_03 << " " << therm_04 << endl;
}
void htr01(bool b){
    htr_01=b;
}
void htr02(bool b){
    htr_02=b;
}
void htr03(bool b){
    htr_03=b;
}
void htr04(bool b){
    htr_04=b;
}
int temp(int i){
    if(i==1){
        return therm_01;
    }
    if(i==2){
        return therm_02;
    }
    if(i==3){
        return therm_03;
    }
    if(i==4){
        return therm_04;
    }
}
bool heater(int i){
    if(i==1){
        return htr_01;
    }
    if(i==2){
        return htr_02;
    }
    if(i==3){
        return htr_03;
    }
    if(i==4){
        return htr_04;
    }
}
std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm *tm_now = std::localtime(&now_time);

    // Format the time as YYYY-MM-DD HH:MM:SS
    std::ostringstream oss;
    oss << std::put_time(tm_now, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}
void cycle(){
    int on;
    int off;
    if(period=="normal"){
        on=1;
        off=1;
    }
    if(period=="eclipse"){
        on=4;
        off=7;
    }
    if(period=="SUN_EXPOSURE"){
        on=7;
        off=1;
    }
    if(htr_01){
        therm_01+=on;
    }
    else{
        therm_01-=off;
    }
    if(htr_02){
        therm_02+=on;
    }
    else{
        therm_02-=off;
    }
    if(htr_03){
        therm_03+=on;
    }
    else{
        therm_03-=off;
    }
    if(htr_04){
        therm_04+=on;
    }
    else{
        therm_04-=off;
    }
    std::ofstream file("log.txt", std::ios::app);
 

    file << getCurrentTimestamp() << " " << endl << therm_01 << " " << therm_02 << " " << therm_03 << " " << therm_04 << endl 
    << htr_01 << " " << htr_02 << " " << htr_03 << " " << htr_04  << endl
    << period << endl << endl << "---------//---------" ;
    file.close();

}
void write(){
    ofstream file("status.txt", std::ios::trunc);
    file << therm_01 << endl << therm_02 << endl << therm_03 << endl << therm_04 << endl << endl 
    << htr_01 << endl << htr_02 << endl << htr_03 << endl << htr_04 << endl << endl
    << period << endl;
    file.close();
}
void check_period(){
    auto now = std::chrono::steady_clock::now();
    auto duration = now.time_since_epoch();

    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

    uint8_t lowerBits = static_cast<uint8_t>(millis & 0xFF);
    if ((lowerBits >= 0x00 && lowerBits <= 0x1F) || (lowerBits >= 0x60 && lowerBits <= 0xFF)) {
        period = "normal";
    }
    if (lowerBits >= 0x20 && lowerBits <= 0x3F ) {
        period = "eclipse";
    }
    if (lowerBits >= 0x40 && lowerBits <= 0x5F ) {
        period = "SUN_EXPOSURE";
    }
}
void read(){
    ifstream file("status.txt");
        
    file >> therm_01 >> therm_02 >> therm_03 >> therm_04;

    int temp_htr_01, temp_htr_02, temp_htr_03, temp_htr_04;
    file >> temp_htr_01 >> temp_htr_02 >> temp_htr_03 >> temp_htr_04;

    htr_01 = static_cast<bool>(temp_htr_01);
    htr_02 = static_cast<bool>(temp_htr_02);
    htr_03 = static_cast<bool>(temp_htr_03);
    htr_04 = static_cast<bool>(temp_htr_04);

    file >> period;
        
    file.close();

}
int main(){
    inicialize();

    const int frequency = 5; 
    const std::chrono::milliseconds interval(1000 / frequency); 
    while (true) {
        check_period();
        cycle();
        //cout << therm_01 << " " << therm_02 << " " << therm_03 << " " << therm_04 << endl << htr_01 << " " << htr_02 << " " << htr_03 << " " << htr_04 << endl << period << endl;

        this_thread::sleep_for(interval);
    }
    //cout << therm_01 << " " << therm_02 << " " << therm_03 << " " << therm_04 << endl << htr_01 << " " << htr_02 << " " << htr_03 << " " << htr_04 << endl << period << endl;
    return 0;
}
