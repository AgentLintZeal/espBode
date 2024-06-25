#ifndef _ESP_AWG_H_
#define _ESP_AWG_H_

#include "esp_config.h"

#define CMD_LEN 128

typedef struct
{
    uint8_t ch1Output;
    uint8_t ch2Output;
    uint8_t ch1Wave;
    uint8_t ch2Wave;
    uint32_t ch1Freq;
    uint32_t ch2Freq;
    uint32_t ch1Ampl;
    uint32_t ch2Ampl;
    uint32_t ch1Phase;
    uint32_t ch2Phase;
    uint32_t ch1Offset;
    uint32_t ch2Offset;
} SDeviceState;

class espAWG
{
  public:
    // Set to true once initDevice has been called
    bool isInitialized = false;

    // Used to set default AWG settings
    virtual void initDevice();

    // Set the wave type for a given channel - use EWaveType
    virtual void setCh1Wave(uint8_t wave);
    virtual void setCh2Wave(uint8_t wave);

    // Turn a channel off (0) or on (1)
    virtual void setCh1Output(uint32_t output);
    virtual void setCh2Output(uint32_t output);

    // Set frequency in Hz
    virtual void setCh1Freq(uint32_t frequency);
    virtual void setCh2Freq(uint32_t frequency);

    // Amplitude in mV - ex: 12.345V = 12345
    virtual void setCh1Ampl(uint32_t ampl);
    virtual void setCh2Ampl(uint32_t ampl);

    // Phase in 0.1 deg - ex: 12.5 deg = 125
    virtual void setCh1Phase(uint32_t phase);
    virtual void setCh2Phase(uint32_t phase);

    // Offset in mV - ex: 12.345V = 12345
    virtual void setCh1Offset(int32_t offset);
    virtual void setCh2Offset(int32_t offset);

  protected:
    // Writes out the command built from individual class methods above
    void writeData();

    // Looks for reply from AWG after command is sent
    void getResponse();

    SDeviceState gDeviceState;
    char command[CMD_LEN];
};

#endif // _ESP_AWG_H_