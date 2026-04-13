#include <errno.h>
#include <sstream>
#include <unordered_map>
#include <unistd.h>
#include "gpio.hpp"

// ----------------------------------------------------------------------------
// CANPiCAP / CANPiZero subsystems
// These classes know nothing about GPIO internals — they just use the
// IInputPin / IOutputPin interfaces.
// ----------------------------------------------------------------------------

/**
 * Controls a single LED
 * Drives an output pin high for on, low for off.
 */
void LEDController::setOn() { pin_->write(true); }
void LEDController::setOff() { pin_->write(false); }

bool LEDController::isOn() const { return pin_->read(); }
int LEDController::pinNumber() const { return pin_->pinNumber(); }

/**
 * Reads a push button state
 * The input pin reads high when the button is pressed.
 */
bool PushButtonSensor::isPressed() const { return !pin_->read(); }
int PushButtonSensor::pinNumber() const { return pin_->pinNumber(); }