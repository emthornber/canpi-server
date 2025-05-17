#ifndef GPIO_H
#define GPIO_H

#include <log4cpp/Category.hh>
#include <pigpiod_if2.h>
#include <string>
#include <memory>

enum Mode
{
    GP_IN = PI_INPUT,
    GP_OUT = PI_OUTPUT,
};

enum Level
{
    GP_OFF = PI_OFF,
    GP_ON = PI_ON,
};
/** GPIO Class
 * @brief Singleton wrapper class for pigpio_if2 C library
 * @details The class provides access to the pigpio daemon and, thence, to the
 * GPIO pins.
 */

class gpio
{
public:
    static gpio *Library(log4cpp::Category *logger);
    virtual ~gpio();

    int setdir_gpio(unsigned pin_num, Mode dir);   // Set GPIO Direction
    int setval_gpio(unsigned pin_num, Level val);  // Set GPIO Value (output pins)
    int getval_gpio(unsigned pin_num, Level &val); // Get GPIO Value (input/ output pins)

private:
    static gpio *_library;
    log4cpp::Category *logger;
    const char *err_to_text(int);
    const char *level_to_text(int);
    const char *mode_to_text(int);
    gpio();

    int pi; // ID returned by pigpio_start()
};

#endif // GPIO_H
