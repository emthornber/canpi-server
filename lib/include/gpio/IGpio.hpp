#pragma once

#include <memory>

#include "IInputPin.hpp"
#include "IOutputPin.hpp"

/**
 * Factory interface for acquiring GPIO pin proxies.
 *
 * Create a concrete implementation (RealGpio or MockGpio) via std::make_shared,
 * and pass the shared_ptr to any subsystem that needs to create pin proxies.
 * Each proxy will internally hold a shared_ptr back to this object, keeping it
 * alive for at least as long as any pin is alive.
 *
 * Example:
 *   auto gpio = std::make_shared<MockGpio>();
 *   auto motorPin = gpio->makeOutputPin(17);
 *   auto sensorPin = gpio->makeInputPin(5);
 */
class IGpio
{
public:
    virtual ~IGpio() = default;

    /**
     * Acquire a proxy for a GPIO input (read-only) pin.
     * @param pin  BCM pin number.
     * @return     Unique ownership of the pin proxy.
     */
    virtual std::unique_ptr<IInputPin> makeInputPin(int pin) = 0;

    /**
     * Acquire a proxy for a GPIO output (read-write) pin.
     * @param pin  BCM pin number.
     * @return     Unique ownership of the pin proxy.
     */
    virtual std::unique_ptr<IOutputPin> makeOutputPin(int pin) = 0;
};