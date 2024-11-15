#ifndef GPIO_H
#define GPIO_H

#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

using std::ifstream;
using std::ofstream;
using std::string;

/* GPIO Class
 * Purpose: Each object instantiated from this class will control a GPIO pin
 * The GPIO pin number must be passed to the overloaded class constructor
 */
class gpio
{
public:
    // gpio();  // create a GPIO object that controls GPIO4 (default
    gpio(string x); // create a GPIO object that controls GPIOx, where x is passed to this constructor
    virtual ~gpio();
    // int export_gpio(); // exports GPIO
    // int unexport_gpio(); // unexport GPIO
    int setdir_gpio(string dir);  // Set GPIO Direction
    int setval_gpio(string val);  // Set GPIO Value (output pins)
    int getval_gpio(string &val); // Get GPIO Value (input/ output pins)
    string get_pin();             // return the GPIO number associated with the instance of an object
private:
    unsigned pin;  // GPIO number associated with the instance of an object
    unsigned mode; // Mode of pin usage - INPUT, OUTPUT
    // bool exported=false;
};

#endif // GPIO_H
