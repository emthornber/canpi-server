#include "gpio.h"

/* gpio::gpio()
{
    this->pin = "4"; //GPIO4 is default
} */

gpio::gpio(string gnum)
{
    this->pin = gnum; // Instatiate GPIOClass object for GPIO pin number "gnum"
}

gpio::~gpio()
{
    if (exported)
    {
        // unexport_gpio();
    }
}

int gpio::export_gpio()
{
    string export_str = "/sys/class/gpio/export";
    ofstream exportgpio(export_str.c_str()); // Open "export" file. Convert C++ string to C string. Required for all Linux pathnames
    // if (exportgpio < 0){
    if (exportgpio.fail() || exportgpio.bad())
    {
        std::cout << " OPERATION FAILED: Unable to export GPIO" << this->pin << " ." << std::endl;
        return -1;
    }
    exportgpio << this->pin; // write GPIO number to export
    exportgpio.close();      // close export file
    exported = true;
    return 0;
}

int gpio::unexport_gpio()
{
    string unexport_str = "/sys/class/gpio/unexport";
    ofstream unexportgpio(unexport_str.c_str()); // Open unexport file
    // if (unexportgpio < 0){
    if (unexportgpio.fail() || unexportgpio.bad())
    {
        std::cout << " OPERATION FAILED: Unable to unexport GPIO" << this->pin << " ." << std::endl;
        return -1;
    }

    unexportgpio << this->pin; // write GPIO number to unexport
    unexportgpio.close();      // close unexport file
    return 0;
}

int gpio::setdir_gpio(string dir)
{

    string setdir_str = "/sys/class/gpio/gpio" + this->pin + "/direction";
    ofstream setdirgpio(setdir_str.c_str()); // open direction file for gpio
    // if (setdirgpio < 0){
    if (setdirgpio.fail() || setdirgpio.bad())
    {
        std::cout << " OPERATION FAILED: Unable to set direction of GPIO" << this->pin << " ." << std::endl;
        return -1;
    }

    setdirgpio << dir;  // write direction to direction file
    setdirgpio.close(); // close direction file
    return 0;
}

int gpio::setval_gpio(string val)
{

    string setval_str = "/sys/class/gpio/gpio" + this->pin + "/value";
    ofstream setvalgpio(setval_str.c_str()); // open value file for gpio
    // if (setvalgpio < 0){
    if (setvalgpio.fail() || setvalgpio.bad())
    {
        std::cout << " OPERATION FAILED: Unable to set the value of GPIO" << this->pin << " ." << std::endl;
        return -1;
    }

    setvalgpio << val;  // write value to value file
    setvalgpio.close(); // close value file
    return 0;
}

int gpio::getval_gpio(string &val)
{

    string getval_str = "/sys/class/gpio/gpio" + this->pin + "/value";
    // std::cout << "Reading the value of " << getval_str << std::endl;
    ifstream getvalgpio(getval_str.c_str()); // open value file for gpio
    // if (getvalgpio < 0){
    if (getvalgpio.fail() || getvalgpio.bad())
    {
        std::cout << " OPERATION FAILED: Unable to get value of GPIO" << this->pin << " ." << std::endl;
        return -1;
    }

    getvalgpio >> val; // read gpio value
    // std::cout << "GPIO value is " << val << std::endl;
    if (val != "0")
        val = "1";
    else
        val = "0";

    getvalgpio.close(); // close the value file
    return 0;
}

string gpio::get_pin()
{

    return this->pin;
}
