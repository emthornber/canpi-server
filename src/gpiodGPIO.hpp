#pragma once

#include <gpio/IGpio.hpp>
#include <filesystem>
#include <memory>
#include <string>

// Forward declaration — keeps <gpiod.hpp> out of this header so that
// consumers of GpiodGpio.hpp do not need libgpiod on their include path.
namespace gpiod { class chip; }

/**
 * GPIO implementation for Linux using libgpiod (the modern character-device
 * interface that replaced the legacy sysfs GPIO interface).
 *
 * This is the implementation to use on Debian Trixie and later.
 * Install the development package with:
 *   sudo apt install libgpiod-dev
 *
 * Must be created via GpiodGpio::create() so that pin proxies can safely
 * hold a shared_ptr back to this object.  The constructor is private to
 * enforce this — std::make_shared cannot call a private constructor.
 *
 * Example:
 *   // Open the default chip with a consumer tag for diagnostics
 *   auto gpio = GpiodGpio::create();
 *
 *   // Or specify a chip and consumer explicitly
 *   auto gpio = GpiodGpio::create("/dev/gpiochip4", "model-railway");
 */
class GpiodGpio : public IGpio, public std::enable_shared_from_this<GpiodGpio> {
public:
    /**
     * The only correct way to construct a GpiodGpio.
     * @param chipPath  Path to the GPIO character device. Defaults to
     *                  /dev/gpiochip0, which is the primary chip on the RPi.
     * @param consumer  A short label attached to every line request, visible
     *                  in gpioinfo output. Useful for diagnostics.
     */
    static std::shared_ptr<GpiodGpio> create(
        const std::filesystem::path& chipPath = "/dev/gpiochip0",
        const std::string& consumer = "railway");

    // Destructor is declared here and defined in the .cpp so that the
    // compiler sees the full definition of gpiod::chip when generating
    // the unique_ptr destructor.
    ~GpiodGpio() override;

    std::unique_ptr<IInputPin>  makeInputPin(int pin)  override;
    std::unique_ptr<IOutputPin> makeOutputPin(int pin) override;

private:
    GpiodGpio(const std::filesystem::path& chipPath, const std::string& consumer);

    std::unique_ptr<gpiod::chip> chip_;
    std::string consumer_;
};