#include "esp_awg_jds8000.h"
#include <string.h>

void espAWGJDS8000::setCh1Wave(uint8_t wave)
{
    espAWG::setCh1Wave(wave);

    snprintf(command, CMD_LEN, ":w11=%01u.\r\n", wave);
    writeData();
}

void espAWGJDS8000::setCh2Wave(uint8_t wave)
{
    espAWG::setCh2Wave(wave);

    snprintf(command, CMD_LEN, ":w12=%01u.\r\n", wave);
    writeData();
}

void espAWGJDS8000::setCh1Output(uint32_t output)
{
    espAWG::setCh1Output(output);

    snprintf(command, CMD_LEN, ":w10=%01u,%01u.\r\n", gDeviceState.ch1Output, gDeviceState.ch2Output);
    writeData();
}

void espAWGJDS8000::setCh2Output(uint32_t output)
{
    espAWG::setCh2Output(output);

    snprintf(command, CMD_LEN, ":w10=%01u,%01u.\r\n", gDeviceState.ch1Output, gDeviceState.ch2Output);
    writeData();
}

/* Set frequency in Hz */
void espAWGJDS8000::setCh1Freq(uint32_t frequency)
{
    espAWG::setCh1Freq(frequency);

    snprintf(command, CMD_LEN, ":w13=%u,0.\r\n", frequency);
    writeData();
}

/* Set frequency in Hz */
void espAWGJDS8000::setCh2Freq(uint32_t frequency)
{
    espAWG::setCh2Freq(frequency);

    snprintf(command, CMD_LEN, ":w14=%u,0.\r\n", frequency);
    writeData();
}

/* Ampl is in mV: 12.345V = 12345 */
void espAWGJDS8000::setCh1Ampl(uint32_t ampl)
{
    espAWG::setCh1Ampl(ampl);

    snprintf(command, CMD_LEN, ":w15=%u.\r\n", ampl);
    writeData();
}

void espAWGJDS8000::setCh2Ampl(uint32_t ampl)
{
    espAWG::setCh2Ampl(ampl);

    snprintf(command, CMD_LEN, ":w16=%u.\r\n", ampl);
    writeData();
}

/* Phase is in 0.1deg: 12.5deg = 125 */
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

void espAWGJDS8000::setCh2Phase(uint32_t phase)
{
    setCh1Phase(phase);
}

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