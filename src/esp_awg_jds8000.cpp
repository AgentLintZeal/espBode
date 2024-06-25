#include "esp_awg_jds8000.h"
#include <string.h>

// Used to set default AWG settings
void espAWGJDS8000::initDevice()
{
    espAWG::initDevice();

    // Setup sync between channels 1 & 2 - the oscilloscope expects this
    strcpy(command, ":w25=1,1,1,1,1,0.\r\n");
    command[21] = 0;
    writeData();
}

// Set the wave type for channel 1 - use EWaveType
void espAWGJDS8000::setCh1Wave(uint8_t wave)
{
    espAWG::setCh1Wave(wave);

    snprintf(command, CMD_LEN, ":w11=%01u.\r\n", wave);
    writeData();
}

// Set the wave type for channel 2 - use EWaveType
void espAWGJDS8000::setCh2Wave(uint8_t wave)
{
    espAWG::setCh2Wave(wave);

    snprintf(command, CMD_LEN, ":w12=%01u.\r\n", wave);
    writeData();
}

// Turn channels 1 & 2 off (0) or on (1)
void espAWGJDS8000::setChOutput(uint32_t output1, uint32_t output2)
{
    espAWG::setCh1Output(output1);
    espAWG::setCh2Output(output2);

    snprintf(command, CMD_LEN, ":w10=%01u,%01u.\r\n", gDeviceState.ch1Output, gDeviceState.ch2Output);
    writeData();
}

// Turn channel 1 off (0) or on (1)
void espAWGJDS8000::setCh1Output(uint32_t output)
{
    // The JDS8000 doesn't sync channel output state, so always turn both on or off
    setChOutput(output, output);
}

// Turn channel 2 off (0) or on (1)
void espAWGJDS8000::setCh2Output(uint32_t output)
{
    // The JDS8000 doesn't sync channel output state, so always turn both on or off
    setChOutput(output, output);
}

// Set channel 1 frequency in Hz
void espAWGJDS8000::setCh1Freq(uint32_t frequency)
{
    espAWG::setCh1Freq(frequency);

    snprintf(command, CMD_LEN, ":w13=%u,0.\r\n", frequency);
    writeData();
}

// Set channel 2 frequency in Hz
void espAWGJDS8000::setCh2Freq(uint32_t frequency)
{
    espAWG::setCh2Freq(frequency);

    snprintf(command, CMD_LEN, ":w14=%u,0.\r\n", frequency);
    writeData();
}

// Set channel 1 amplitude in mV - ex: 12.345V = 12345
void espAWGJDS8000::setCh1Ampl(uint32_t ampl)
{
    espAWG::setCh1Ampl(ampl);

    snprintf(command, CMD_LEN, ":w15=%u.\r\n", ampl);
    writeData();
}

// Set channel 2 amplitude in mV - ex: 12.345V = 12345
void espAWGJDS8000::setCh2Ampl(uint32_t ampl)
{
    espAWG::setCh2Ampl(ampl);

    snprintf(command, CMD_LEN, ":w16=%u.\r\n", ampl);
    writeData();
}

// Set channel 1 phase in 0.1 deg - ex: 12.5 deg = 125
void espAWGJDS8000::setCh1Phase(uint32_t phase)
{
    uint32_t modPhase = phase % 36000;

    espAWG::setCh2Phase(modPhase);
    espAWG::setCh1Phase(modPhase);

    // CH2 is always set 1st, then CH1 as a pair
    snprintf(command, CMD_LEN, ":w22=%u.\r\n", modPhase);
    writeData();
    snprintf(command, CMD_LEN, ":w21=%u.\r\n", modPhase);
    writeData();
}

// Set channel 2 phase in 0.1 deg - ex: 12.5 deg = 125
void espAWGJDS8000::setCh2Phase(uint32_t phase)
{
    setCh1Phase(phase);
}

// Set channel 1 offset in mV - ex: 12.345V = 12345
void espAWGJDS8000::setCh1Offset(int32_t offset)
{
    if (offset > 9999)
        offset = 9999;
    if (offset < -9999)
        offset = -9999;

    espAWG::setCh1Offset(offset);

    snprintf(command, CMD_LEN, ":w17=%d.\r\n", offset + 1000);
    writeData();
}

// Set channel 2 offset in mV - ex: 12.345V = 12345
void espAWGJDS8000::setCh2Offset(int32_t offset)
{
    if (offset > 9999)
        offset = 9999;
    if (offset < -9999)
        offset = -9999;

    espAWG::setCh2Offset(offset);

    snprintf(command, CMD_LEN, ":w18=%d.\r\n", offset + 1000);
    writeData();
}