#pragma once

/**
 * Proxy for a GPIO input pin.
 * Acquired from IGpio::makeInputPin().
 * The proxy keeps the parent IGpio object alive via a shared_ptr internally.
 */
class IInputPin
{
public:
    virtual ~IInputPin() = default;

    /// Read the current logic level of the pin.
    virtual bool read() const = 0;

    /// The BCM pin number this proxy represents.
    virtual int pinNumber() const = 0;
};