#include "GpiodGpio.hpp"

#include <gpiod.h>

#include <gpio/IInputPin.h>
#include <gpio/IOutputPin.h>

// ---------------------------------------------------------------------------
// Pin proxy implementations — internal to this translation unit
// ---------------------------------------------------------------------------

namespace
{

    class GpiodInputPin : public IInputPin
    {
    public:
        GpiodInputPin(std::shared_ptr<GpiodGpio> gpio, gpiod::line_request request, int pin)
            : gpio_(std::move(gpio)), request_(std::move(request)), pin_(pin) {}

        bool read() const override
        {
            return request_.get_value(pin_) == gpiod::line::value::ACTIVE;
        }

        int pinNumber() const override { return pin_; }

    private:
        std::shared_ptr<GpiodGpio> gpio_;
        mutable gpiod::line_request request_; // mutable: get_value() is non-const in libgpiod
        int pin_;
    };

    class GpiodOutputPin : public IOutputPin
    {
    public:
        GpiodOutputPin(std::shared_ptr<GpiodGpio> gpio, gpiod::line_request request, int pin)
            : gpio_(std::move(gpio)), request_(std::move(request)), pin_(pin) {}

        void write(bool value) override
        {
            request_.set_value(
                pin_,
                value ? gpiod::line::value::ACTIVE : gpiod::line::value::INACTIVE);
        }

        bool read() const override
        {
            return request_.get_value(pin_) == gpiod::line::value::ACTIVE;
        }

        int pinNumber() const override { return pin_; }

    private:
        std::shared_ptr<GpiodGpio> gpio_;
        mutable gpiod::line_request request_; // mutable: get_value() is non-const in libgpiod
        int pin_;
    };

} // namespace

// ---------------------------------------------------------------------------
// GpiodGpio
// ---------------------------------------------------------------------------

GpiodGpio::GpiodGpio(const std::filesystem::path &chipPath, const std::string &consumer)
    : chip_(std::make_unique<gpiod::chip>(chipPath)), consumer_(consumer)
{
}

// Defined here rather than in the header so that the compiler sees the full
// definition of gpiod::chip (via gpiod.hpp above) when instantiating the
// unique_ptr destructor.
GpiodGpio::~GpiodGpio() = default;

std::shared_ptr<GpiodGpio> GpiodGpio::create(
    const std::filesystem::path &chipPath,
    const std::string &consumer)
{
    return std::shared_ptr<GpiodGpio>(new GpiodGpio(chipPath, consumer));
}

std::unique_ptr<IInputPin> GpiodGpio::makeInputPin(int pin)
{
    auto request = chip_->prepare_request()
                       .set_consumer(consumer_)
                       .add_line_settings(
                           pin,
                           gpiod::line_settings().set_direction(gpiod::line::direction::INPUT))
                       .do_request();

    return std::make_unique<GpiodInputPin>(shared_from_this(), std::move(request), pin);
}

std::unique_ptr<IOutputPin> GpiodGpio::makeOutputPin(int pin)
{
    auto request = chip_->prepare_request()
                       .set_consumer(consumer_)
                       .add_line_settings(
                           pin,
                           gpiod::line_settings().set_direction(gpiod::line::direction::OUTPUT))
                       .do_request();

    return std::make_unique<GpiodOutputPin>(shared_from_this(), std::move(request), pin);
}