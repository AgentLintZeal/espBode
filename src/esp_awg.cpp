#include "esp_awg.h"
#include <string.h>

#include "esp_config.h"

// Used to set default AWG settings
void espAWG::initDevice()
{
    isInitialized = true;
}

// Set the wave type for channel 1 - use EWaveType
void espAWG::setCh1Wave(uint8_t wave)
{
    gDeviceState.ch1Wave = wave;
}

// Set the wave type for channel 1 - use EWaveType
void espAWG::setCh2Wave(uint8_t wave)
{
    gDeviceState.ch2Wave = wave;
}

// Turn channel 1 off (0) or on (1)
void espAWG::setCh1Output(uint32_t output)
{
    gDeviceState.ch1Output = output;
}

// Turn channel 2 off (0) or on (1)
void espAWG::setCh2Output(uint32_t output)
{
    gDeviceState.ch2Output = output;
}

// Set channel 1 frequency in Hz
void espAWG::setCh1Freq(uint32_t frequency)
{
    gDeviceState.ch1Freq = frequency;
}

// Set channel 2 frequency in Hz
void espAWG::setCh2Freq(uint32_t frequency)
{
    gDeviceState.ch2Freq = frequency;
}

// Set channel 1 amplitude in mV - ex: 12.345V = 12345
void espAWG::setCh1Ampl(uint32_t ampl)
{
    gDeviceState.ch1Ampl = ampl;
}

// Set channel 2 amplitude in mV - ex: 12.345V = 12345
void espAWG::setCh2Ampl(uint32_t ampl)
{
    gDeviceState.ch2Ampl = ampl;
}

// Set channel 1 phase in 0.1 deg - ex: 12.5 deg = 125
void espAWG::setCh1Phase(uint32_t phase)
{
    gDeviceState.ch1Phase = phase;
}

// Set channel 2 phase in 0.1 deg - ex: 12.5 deg = 125
void espAWG::setCh2Phase(uint32_t phase)
{
    gDeviceState.ch2Phase = phase;
}

// Set channel 1 offset in mV - ex: 12.345V = 12345
void espAWG::setCh1Offset(int32_t offset)
{
    gDeviceState.ch1Offset = offset;
}

// Set channel 2 offset in mV - ex: 12.345V = 12345
void espAWG::setCh2Offset(int32_t offset)
{
    gDeviceState.ch2Offset = offset;
}

// Writes out the command built from individual class methods above
void espAWG::writeData()
{
    DEBUG("   [AWG:writeData] ");

#ifdef ESP32
    // send command to IDE terminal
    Serial.write((uint8_t *)command, strlen(command));
    // send command to awg
    Serial2.write((uint8_t *)command, strlen(command));
#else
    // send command to awg
    Serial.write((uint8_t *)command, strlen(command));
#endif

    // echo via telnet
#if defined(DEBUG_TELNET) && !defined(DEBUG_UART) && !defined(ESP32)
    telnet.print(command);
#endif

    getResponse();
}

// Looks for reply from AWG after command is sent
void espAWG::getResponse()
{
    DEBUG("   [AWG:getResponse] ");

    uint16_t timeout = 0;
    uint8_t responsePos = 0;
    while (timeout++ < 1000)
    {
#ifdef ESP32
        if (Serial2.available() > 0)
        {
            char c = Serial2.read();
#else
        if (Serial.available() > 0)
        {
            char c = Serial.read();
#endif
            if (c == '\n')
            {
                command[responsePos++] = 0;
                timeout = 0xFFFF;
                break;
            }
            else
            {
                command[responsePos++] = c;
            }
        }

        delay(1);
    }

    command[responsePos] = 0;

    // write response
    if (timeout == 0xFFFF) // flag value for response complete
    {
        DEBUG_LN(command);
    }
    else
    {
        DEBUG_LN("TIMEOUT");
    }
}