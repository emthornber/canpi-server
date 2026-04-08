#pragma once

/**
 * Proxy for a GPIO output pin.
 * Acquired from IGpio::makeOutputPin().
 * The proxy keeps the parent IGpio object alive via a shared_ptr internally.
 */
class IOutputPin {
public:
    virtual ~IOutputPin() = default;

    /// Drive the pin high (true) or low (false).
    virtual void write(bool value) = 0;

    /// Read back the last value driven onto the pin.
    virtual bool read() const = 0;

    /// The BCM pin number this proxy represents.
    virtual int pinNumber() const = 0;
};