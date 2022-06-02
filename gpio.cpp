#include <lgpio.h>
#include "gpio.h"
#include "config.h"

Gpio::Gpio(QObject *parent) : QObject(parent)
{
    // initialization gpio
    m_handle = lgGpiochipOpen(CHIP);
    if (m_handle < 0)
        throw lguErrorText(m_handle);
    int init_state = 0;
    int error = 0;
    for (int pin : LEDS)
        if ((error = lgGpioClaimOutput(m_handle, LFLAGS, pin, init_state)) < 0)
            throw lguErrorText(error);
}

Gpio::~Gpio()
{
    // switch leds off
    int state = 0;
    for (int pin : LEDS)
        lgGpioWrite(m_handle, pin, state);
    lgGpiochipClose(m_handle);
}

void Gpio::set(int pin, int state)
{
    lgGpioWrite(m_handle, pin, state);
}

void Gpio::set(int pattern)
{
        // 1011
        // 0001
        //------ AND
        // 0001
        int n = 0;
        bool value;
        unsigned int check = 0b0001;
        for(auto pin : LEDS)
        {
            // n stelle von pattern ausmaskieren
            value = check & pattern>>n; // bit shift to the right;
            lgGpioWrite(m_handle, pin, value);
            n++;
        }
}
