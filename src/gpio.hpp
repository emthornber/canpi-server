#ifndef GPIO_H
#define GPIO_H

#include <log4cpp/Category.hh>
#include <gpiod.h>
#include <string>
#include <memory>
#include <errno.h>

enum Mode
{
    GP_IN = GPIOD_LINE_DIRECTION_INPUT,
    GP_OUT = GPIOD_LINE_DIRECTION_OUTPUT,
};

enum Level
{
    GP_OFF = GPIOD_LINE_VALUE_INACTIVE,
    GP_ON = GPIOD_LINE_VALUE_ACTIVE,
};

/** GPIO Chip Class
 * @brief Singleton wrapper class for access to the GPIO Chip.
 * @details The class provides access to the gpio chip via the libgpiod C library
 */

class GPIOChip
{
public:
    static GPIOChip *Chip(log4cpp::Category *logger);
    virtual ~GPIOChip();

protected:
    GPIOChip();

private:
    static GPIOChip *_chip;
    log4cpp::Category *logger;
    const char *CHIP_PATH = "/dev/gpiochip0";
    struct gpiod_chip *chip = nullptr;
    int counter = 0;
    const char *err_to_text(int);
    const char *level_to_text(int);
    const char *mode_to_text(int);
};

/** GPIO Pin Class
 * @brief Wrapper class for access to a GPIO pin.
 * @details The class provides access to a GPIO pin via the libgpiod C library
 */
class GPIOPin
{
public:
    GPIOPin(unsigned int offset, Mode direction, log4cpp::Category *log4cpp);
    virtual ~GPIOPin();

    int configure_output_pin(unsigned int offset);
    int setdir_gpio(unsigned pin_num, Mode dir);   // Set GPIO Direction
    int setval_gpio(unsigned pin_num, Level val);  // Set GPIO Value (output pins)
    int getval_gpio(unsigned pin_num, Level &val); // Get GPIO Value (input/ output pins)
    int setdir(Mode dir);
    int setval(Level val);
    int getval(Level &val);

protected:
    GPIOPin();

private:
    GPIOChip *chip;
    unsigned int offset;
    Mode direction;
    struct gpiod_request_config *request_config = nullptr;
    struct gpiod_line_config *line_config = nullptr;
    struct gpiod_line_settings *line_settings = nullptr;
    struct gpiod_line_request *line_request = nullptr;
};

#endif // GPIO_H
