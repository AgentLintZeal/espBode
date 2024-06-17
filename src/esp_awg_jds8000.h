#ifndef _ESP_JDS8000_H_
#define _ESP_JDS8000_H_

#include "esp_awg.h"

#ifdef AWG_TYPE_JDS8000
typedef enum
{
    EWaveType_Sine = 0,
    EWaveType_Square = 1,
    EWaveType_Pulse = 2,
    EWaveType_Triangle = 3,
    EWaveType_Ramp = 4,
    EWaveType_CMOS = 5,
    EWaveType_DC = 6,
    EWaveType_DevSine = 7,
    EWaveType_HalfWave = 8,
    EWaveType_FullWave = 9,
    EWaveType_PosLadder = 10,
    EWaveType_NegLadder = 11,
    EWaveType_PosTrap = 12,
    EWaveType_NegTrap = 13,
    EWaveType_Noise = 14,
    EWaveType_ExpRise = 15,
    EWaveType_ExpFall = 16,
    EWaveType_LogRise = 17,
    EWaveType_LogFall = 18,
    EWaveType_Sinc = 19,
    EWaveType_Multitone = 20,
    EWaveType_Lorentz = 21,

    // Above 21 is user-definable
    EWaveType_Last
} EWaveType;
#endif

class espAWGJDS8000 : public espAWG
{
  public:
    void setCh1Wave(uint8_t wave);
    void setCh2Wave(uint8_t wave);

    /* 0 - off; 1 - on */
    void setCh1Output(uint32_t output);
    void setCh2Output(uint32_t output);

    /* Set frequency in Hz */
    void setCh1Freq(uint32_t frequency);
    void setCh2Freq(uint32_t frequency);

    /* Ampl is in mV: 12.345V = 12345 */
    void setCh1Ampl(uint32_t ampl);
    void setCh2Ampl(uint32_t ampl);

    /* Phase is in 0.1 deg: 12.5 deg = 125 */
    void setCh1Phase(uint32_t phase);
    void setCh2Phase(uint32_t phase);

    /* Offset is in mV: 12.345V = 12345 */
    void setCh1Offset(int32_t offset);
    void setCh2Offset(int32_t offset);

    // Can be used to set some default parameters
    void initDevice(void);
};

#endif // _ESP_JDS8000_H_
