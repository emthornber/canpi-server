#include "gpio.h"
#include <errno.h>
#include <sstream>
#include <unordered_map>

namespace
{
    const ::std::unordered_map<int, ::std::string> err_strings = {
        {GPIO_BAD_PIN, "GPIO_BAD_PIN"},
        {GPIO_BAD_MODE, "GPIO_BAD_MODE"},
        {GPIO_BAD_LEVEL, "GPIO_BAD_LEVEL"},
        {GPIO_NOT_PERMITTED, "GPIO_NOT_PERMITTED "},
    };
    const ::std::unordered_map<int, ::std::string> mode_strings = {
        {static_cast<int>(Mode::GP_IN), "GPIO_INPUT"},
        {static_cast<int>(Mode::GP_OUT), "GPIO_OUTPUT"},
    };
    const ::std::unordered_map<int, ::std::string> level_strings = {
        {static_cast<int>(Level::GP_OFF), "GPIO_OFF"},
        {static_cast<int>(Level::GP_ON), "GPIO_ON"},
    };

    /**
     * @brief Return hash value of hash key allowing for missing key
     * @param table address of hash table
     * @param key hash key
     * @return
     */
    const char *state_to_text(const ::std::unordered_map<int, ::std::string> &table, int key)
    {
        auto it = table.find(key);
        return it != table.end() ? it->second.c_str() : "UNKNOWN";
    }
}

GPIOChip *GPIOChip::_chip = 0;

/**
 * @brief singleton ctor
 * @return
 */
GPIOChip *GPIOChip::Chip(log4cpp::Category *log4cpp)
{
    if (_chip == 0)
    {
        _chip = new GPIOChip;
        _chip->logger = log4cpp;
        _chip->counter = 0;
    }
    // Increment the counter for the number of users of the GPIO Chip
    _chip->counter++;

    return _chip;
}

GPIOChip::GPIOChip()
{
    chip = gpiod_chip_open(CHIP_PATH);
    if (!chip)
    {
        logger->error("[gpio] Failed to open gpiochip at %s (Error: %d)", CHIP_PATH, errno);
    }
}
/**
 * @brief dtor
 */
GPIOChip::~GPIOChip()
{
    _chip->counter--;
    if (_chip->counter <= 0)
    {
        gpiod_chip_close(chip);
        delete _chip;
        _chip = nullptr;
        logger->debug("[gpio] chip closed");
    }
    else
    {
        logger->debug("[gpio] chip still in use by %d pins", _chip->counter);
    }
}

GPIOPin::GPIOPin(unsigned int offset, Mode direction, log4cpp::Category *log4cpp)
{
    chip = GPIOChip::Chip(log4cpp);
    this->offset = offset;
    setdir(direction);
}
/**
 * @brief set mode for a GPIO pin
 * @param pin_num pin number 0 - 53
 * @param dir mode GPIO_INPUT or GPIO_OUTPUT
 * @return
 */
int gpio::setdir_gpio(unsigned pin_num, Mode dir)
{
    // set_mode returns 0 on success
    int result = set_mode(pi, pin_num, dir);
    if (result)
    {
        logger->error(
            "[gpio] setdir_gpio error for pin %d - %s (%d)",
            pin_num, err_to_text(result), result);
    }
    else
    {
        logger->debug("[gpio] Pin %d set to mode %s", pin_num, mode_to_text(dir));
    }
    return result;
}

/**
 * @brief set value of a GPIO pin
 * @param pin_num pin number 0 - 53
 * @param val 0 or 1
 * @return
 */
int gpio::setval_gpio(unsigned pin_num, Level val)
{
    int result = gpio_write(pi, pin_num, val);
    if (result)
    {
        logger->error(
            "[gpio] setval_gpio error for pin %d - %s (%d)",
            pin_num, err_to_text(result), result);
    }
    return result;
}

/**
 * @brief get value of a GPIO pin
 * @param pin_num pin number 0 - 53
 * @param val
 * @return
 */
int gpio::getval_gpio(unsigned pin_num, Level &val)
{
    int result = gpio_read(pi, pin_num);
    if (result >= 0)
    {
        val = result ? Level::GP_ON : Level::GP_OFF;
        return 0;
    }
    else
    {
        logger->error(
            "[gpio] getval_gpio error for pin %d - %s (%d)",
            pin_num, err_to_text(result), result);
        return result;
    }
}

// Utility routines
/**
 * @brief Convert an error number to text
 * @param errnum Error number as defined in pigpio.h
 * @return
 */
const char *gpio::err_to_text(int errnum)
{
    return state_to_text(err_strings, errnum);
}

/**
 * @brief Convert a level number to text
 * @param level level number as defined in pigpio.h
 * @return
 */
const char *gpio::level_to_text(int level)
{
    return state_to_text(level_strings, level);
}

/**
 * @brief Convert a mode number to text
 * @param mode mode number as defined in pigpio.h
 * @return
 */
const char *gpio::mode_to_text(int mode)
{
    return state_to_text(mode_strings, mode);
}