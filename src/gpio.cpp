#include "gpio.h"
#include <unordered_map>

namespace
{
    const std::unordered_map<int, std::string> err_strings = {
        {PI_BAD_GPIO, "PI_BAD_GPIO"},
        {PI_BAD_MODE, "PI_BAD_MODE"},
        {PI_BAD_LEVEL, "PI_BAD_LEVEL"},
        {PI_NOT_PERMITTED, "PI_NOT_PERMITTED "},
    };
    const std::unordered_map<int, std::string> mode_strings = {
        {static_cast<int>(Mode::GP_IN), "PI_INPUT"},
        {static_cast<int>(Mode::GP_OUT), "PI_OUTPUT"},
    };
    const std::unordered_map<int, std::string> level_strings = {
        {static_cast<int>(Level::GP_OFF), "PI_OFF"},
        {static_cast<int>(Level::GP_ON), "PI_ON"},
    };

    /**
     * @brief Return hash value of hash key allowing for missing key
     * @param table address of hash table
     * @param key hash key
     * @return
     */
    std::string state_to_text(const std::unordered_map<int, std::string> &table, int key)
    {
        auto it = table.find(key);
        return it != table.end() ? it->second : "UNKNOWN";
    }
}

gpio *gpio::_library = 0;

/**
 * @brief singleton ctor
 * @return
 */
gpio *gpio::Library(log4cpp::Category *log4cpp)
{
    if (_library == 0)
    {
        _library = new gpio;
        _library->logger = log4cpp;
    }
    return _library;
}

gpio::gpio() : pi(pigpio_start(nullptr, nullptr))
{
    if (pi < 0)
    {
        throw std::runtime_error("Failed to connect to pigpio daemon");
    }
}
/**
 * @brief dtor
 */
gpio::~gpio()
{
    // Terminate connection to pigpio daemon and releases resources used by the
    // library.
    logger->debug("[gpio] pigpio_if2 library stopped");
    pigpio_stop(pi);
}

/**
 * @brief set mode for a GPIO pin
 * @param pin_num pin number 0 - 53
 * @param dir mode PI_INPUT or PI_OUTPUT
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
            pin_num, err_to_string(result), result);
    }
    else
    {
        logger->debug("[gpio] Pin %d set to mode %s", pin_num, mode_to_string(dir));
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
            pin_num, err_to_string(result), result);
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
            pin_num, err_to_string(result), result);
        return result;
    }
}

// Utility routines
/**
 * @brief Convert an error number to text
 * @param errnum Error number as defined in pigpio.h
 * @return
 */
std::string gpio::err_to_string(int errnum)
{
    return state_to_text(err_strings, errnum);
}

/**
 * @brief Convert a level number to text
 * @param level level number as defined in pigpio.h
 * @return
 */
std::string gpio::level_to_string(int level)
{
    return state_to_text(level_strings, level);
}

/**
 * @brief Convert a mode number to text
 * @param mode mode number as defined in pigpio.h
 * @return
 */
std::string gpio::mode_to_string(int mode)
{
    return state_to_text(mode_strings, mode);
}