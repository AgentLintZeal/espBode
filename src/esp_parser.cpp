#include "esp_config.h"

#ifdef AWG_TYPE_JDS8000
#include "esp_awg_jds8000.h"
espAWGJDS8000 awg;
#endif

#include "esp_parser.h"

volatile char *gReadBuffer = NULL;

/* Function parses number from the msg string expects natural a number. */
static uint32_t parseNumber(char *msg)
{
    uint32_t number = 0;

    // Will stop at the first non-numeric character
    while (0 <= (msg[0] - '0') && (msg[0] - '0') <= 9)
    {
        number *= 10;
        number += msg[0] - '0';
        msg++;
    }

    // returns a positive whole number
    return number;
}

/* Similar to parseNumber, but handles also decimal '.' and '-' sign.
    Return value is multiplied by dec_count to include the decimal part:
        msg         dec_count       result
        123.456     4               1234560
        -1.2        2               -120
        12346.9388  3               12346938 */
static int32_t parseDecimal(char *msg, uint8_t dec_count = 3)
{
    uint8_t dot = 0;
    int32_t number = 0;

    int32_t multiplier = 1;
    for (uint8_t i = 0; i < dec_count; i++)
    {
        multiplier *= 10;
    }

    if (msg[0] == '-')
    {
        multiplier *= -1;
        msg++;
    }

    while (((0 <= (msg[0] - '0') && (msg[0] - '0') <= 9) || msg[0] == '.') && dot < (dec_count + 1))
    {
        if (msg[0] == '.')
        {
            if (dot)
                break;
            dot = 1;
            msg++;
            continue;
        }
        if (dot)
        {
            multiplier /= 10;
            dot++;
        }
        number *= 10;
        number += msg[0] - '0';
        msg++;
    }

    return number * multiplier;
}

/* TODO: This handle function doesn't implement the commands correctly, even though they are interpreted enough
    to get by.The manual of Siglent AWG commands can be found at:
        https://siglentna.com/wp-content/uploads/dlm_uploads/2024/06/SDG_Programming-Guide_PG02-E05C.pdf */
void handleScopeMsg(char *msg, uint8_t len)
{
    bool isReadRequest = false;

    // write line to terminal
    char msgString[256];
    for (uint8_t i = 0; i < len; i++)
    {
        if (msg[i] == '\n')
        {
            msgString[i] = 0;
        }
        else
        {
            msgString[i] = msg[i];
        }
    }
    msgString[len] = 0;

    DEBUG("   [parser:handleScopeMsg] msg = ");
    DEBUG_LN(msgString);

    uint8_t selectedChannel = 1;
    while (len > 0)
    {
        /* Siglent Bode Plot II commands start with a channel selector and are followed by
            one or more sections, each having a set of parameters and values. Sections are
            delimited with semi-colons, parameters and values are comma delimited.

            Some examples:

            - C1:OUTP LOAD,50;      channel 1 OUTP section write request to set the load (output impeadance)to 50 ohms.
            - C2:BSWV FRQ,5000000,  channel 2 BSWV section write request to set the frequency to 5000.000 Hz.

            It is also possible to receive a command with a question mark, which denotes a read
            request, rather than a write request.

            For example:

            - C1:BSWV?  channel 1 BSWV section read request for all parameters and values.
        */

        // ID request message, we preapare answer in gReadBuffer
        if (0 == strncmp(msg, "IDN-SGLT-PRI?", 13))
        {
            DEBUG_LN("   [parser:handleScopeMsg] IDN-SGLT-PRI? (ID Request)");

            isReadRequest = true;
            if (gReadBuffer != NULL)
            {
                free((void *)gReadBuffer); /* Prevent memory leaks */
            }

            // This is where we have the microcontroller pretend to be the Siglent AWG
            gReadBuffer = (char *)malloc(strlen(ID) + 1);
            strcpy((char *)gReadBuffer, ID);
            break;
        }

        // Channel 1 selector prefix
        if (0 == strncmp(msg, "C1:", 3))
        {
            msg += 3;
            len -= 3;

            DEBUG_LN("   [parser:handleScopeMsg] C1: selected");
            selectedChannel = 1;
        }

        // Channel 2 selector prefix
        if (0 == strncmp(msg, "C2:", 3))
        {
            msg += 3;
            len -= 3;

            DEBUG_LN("   [parser:handleScopeMsg] C2: selected");
            selectedChannel = 2;
        }

        // BSWV read request message, we preapare answer in gReadBuffer
        if (0 == strncmp(msg, "BSWV?", 5))
        {
            DEBUG_LN("   [parser:handleScopeMsg] BSWV? (Basic Wave Request)");

            isReadRequest = true;
            if (gReadBuffer != NULL)
            {
                free((void *)gReadBuffer); /* Prevent memory leaks */
            }

            gReadBuffer = (char *)malloc(strlen(ID) + 1);

            // Gather up a bunch of stuff here, send ID for now
            strcpy((char *)gReadBuffer, ID);
            break;
        }

        // Entering Basic Wave section (BSWV)
        if (0 == strncmp(msg, "BSWV ", 5))
        {
            msg += 5;
            len -= 5;

            DEBUG_LN("   [parser:handleScopeMsg] BSWV section");
        }

        // WVTP only appears under BSWV - signal wave type (based on internal wave index - see EWaveType)
        if (0 == strncmp(msg, "WVTP,", 5))
        {
            msg += 5;
            len -= 5;

            DEBUG("   [parser:handleScopeMsg] WVTP");
            if (0 == strncmp(msg, "SINE,", 5))
            {
                DEBUG_LN(": SINE");

                msg += 5;
                len -= 5;

                selectedChannel == 1 ? awg.setCh1Wave(EWaveType_Sine) : awg.setCh2Wave(EWaveType_Sine);
            }
            else
            {
                // Add other wave types here
                DEBUG_LN(": NOT SINE");
            }
        }

        // PHSE only appears under BSWV - signal phase in degrees (channel 2 relative to channel 1)
        if (0 == strncmp(msg, "PHSE,", 5))
        {
            msg += 5;
            len -= 5;

            int32_t value = parseDecimal(msg, 2);
            DEBUG("   [parser:handleScopeMsg] PHSE:");
            DEBUG_LN(value);

            selectedChannel == 1 ? awg.setCh1Phase(value) : awg.setCh2Phase(value);
        }

        // FRQ only appears under BSWV - signal Frequency in Hertz (Hz)
        if (0 == strncmp(msg, "FRQ,", 4))
        {
            msg += 4;
            len -= 4;

            int32_t value = parseDecimal(msg);
            DEBUG("   [parser:handleScopeMsg] FRQ:");
            DEBUG_LN(value);

            selectedChannel == 1 ? awg.setCh1Freq(value) : awg.setCh2Freq(value);
        }

        // AMP only appears under BSWV - signal amplitude in Volts peak-to-peak (Vpp)
        if (0 == strncmp(msg, "AMP,", 4))
        {
            msg += 4;
            len -= 4;

            int32_t value = parseDecimal(msg);
            DEBUG("   [parser:handleScopeMsg] AMP:");
            DEBUG_LN(value);

            selectedChannel == 1 ? awg.setCh1Ampl(value) : awg.setCh2Ampl(value);
        }

        // OFST only appears under BSWV - signal offset in Volts
        if (0 == strncmp(msg, "OFST,", 5))
        {
            msg += 5;
            len -= 5;

            int32_t value = parseDecimal(msg, 2);
            DEBUG("   [parser:handleScopeMsg] OFST:");
            DEBUG_LN(value);

            selectedChannel == 1 ? awg.setCh1Offset(value) : awg.setCh2Offset(value);
        }

        // Entering channel Output section (OUTP)
        if (0 == strncmp(msg, "OUTP ", 5))
        {
            msg += 5;
            len -= 5;

            DEBUG_LN("   [parser:handleScopeMsg] OUTP section");
        }

        // LOAD only appears under OUTP - set output channel impeadance
        if (0 == strncmp(msg, "LOAD,", 5))
        {
            msg += 5;
            len -= 5;

            int32_t value = parseNumber(msg);
            DEBUG("   [parser:handleScopeMsg] LOAD: ");
            DEBUG(value);
            DEBUG_LN(" Ohm");

            // Not supported; hardware fixed at 50 Ohm - do nothing
        }

        // ON only appears under OUTP - set channel output on
        if (0 == strncmp(msg, "ON", 2))
        {
            msg += 2;
            len -= 2;

            DEBUG_LN("   [parser:handleScopeMsg] ON");
            selectedChannel == 1 ? awg.setCh1Output(1) : awg.setCh2Output(1);
        }

        // OFF only appears under OUTP - set channel output off
        if (0 == strncmp(msg, "OFF", 3))
        {
            msg += 3;
            len -= 3;

            DEBUG_LN("   [parser:handleScopeMsg] OFF");
            selectedChannel == 1 ? awg.setCh1Output(0) : awg.setCh2Output(0);
        }

        // Unknown string, skip one character
        msg++;
        len--;
    }
}