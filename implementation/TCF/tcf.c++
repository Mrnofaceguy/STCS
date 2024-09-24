#include <iostream>
#include <fstream>   
#include <string>
#include <random> 
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <iomanip>
#include "C:\Users\Mrnofaceguy\Documents\GitHub\STCS2\implementation\TSL\tsl.c++"

int frequency=5;
int Kp;
int Ki;
int Kd;
int set_01=0;
int set_02=0;
int set_03=0;
int set_04=0;
void control(){
     
    const std::chrono::milliseconds interval(1000 / frequency); 
    while (true) {
        if(therm_01>=set_01||therm_01>=20){
            htr01(false);
        }
        if(therm_01<=-20){
            htr01(true);
        }
        if(therm_02>=set_02||therm_02>=20){
            htr02(false);
        }
        if(therm_02<=-20){
            htr02(true);
        }
        if(therm_03>=set_03||therm_03>=20){
            htr03(false);
        }
        if(therm_03<=-20){
            htr03(true);
        }
        if(therm_04>=set_04||therm_04>=20){
            htr04(false);
        }
        if(therm_04<=-20){
            htr04(true);
        }

        this_thread::sleep_for(interval);
    }
}

int main(){
    std::thread t1(control); // Start function1 in a separate thread
    std::thread t2(inicialize); // Start function2 in a separate thread
    t1.join();
    t2.join();


    
    

}
