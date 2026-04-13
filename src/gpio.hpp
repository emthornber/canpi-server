#pragma once

#include <log4cpp/Category.hh>
#include <gpiod.h>
#include <string>
#include <memory>
#include <errno.h>
#include "gpio/IGpio.hpp"
#include "gpio/IInputPin.hpp"
#include "gpio/IOutputPin.hpp"

#include "GpiodGpio.hpp"

/**
 * Controls a single LED
 * Drives an output pin high for on, low for off.
 */
class LEDController
{
public:
    LEDController() = default;
    explicit LEDController(std::unique_ptr<IOutputPin> pin)
        : pin_(std::move(pin)) {}

    void setOn();
    void setOff();

    bool isOn() const;
    int pinNumber() const;

private:
    std::unique_ptr<IOutputPin> pin_;
};

/**
 * Reads a push button state
 * The input pin reads high when the button is pressed.
 */
class PushButtonSensor
{
public:
    PushButtonSensor() = default;
    explicit PushButtonSensor(std::unique_ptr<IInputPin> pin)
        : pin_(std::move(pin)) {}

    bool isPressed() const;
    int pinNumber() const;

private:
    std::unique_ptr<IInputPin> pin_;
};
