#include "esp_config.h"
#include "esp_network.h"
#include "esp_parser.h"

WiFiServer rpcServer(RPC_PORT);
WiFiServer lxiServer(LXI_PORT);

uint8_t wifiTimeout = 0;
bool wifiConnected = false;

#if defined(DEBUG_TELNET) && !defined(DEBUG_UART) && !defined(ESP32)
ESPTelnet telnet;
#endif

/* Uncomment to test connection to AWG only */
// #define AWG_TESTING

void setup()
{
    // For a non-ESP32, this is the port that talks to the AWG
    Serial.begin(115200);
#ifdef ESP32
    /* For ESP32, Serial2 talks to the AWG. Serial connects to IDE Terminal
        or other terminal application (usually via USB) */
    Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2); // Hardware Serial of ESP32
#endif

#ifndef AWG_TESTING
    // We start by connecting to a WiFi network
    DEBUG_LN();
    DEBUG("-> Connecting to ");
    DEBUG(WIFI_SSID);

#ifdef STATIC_IP // static assigned IP
    IPAddress ip(ESP_IP);
    IPAddress mask(ESP_MASK);
    IPAddress gateway(ESP_GW);
    WiFi.config(ip, gateway, mask);
#endif

#ifdef WIFI_MODE_CLIENT // DHCP assigned IP in Client (device) mode
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PSK);
#elif WIFI_MODE_AP // Devices connect to MCU as an Access Point
    WiFi.softAP(WIFI_SSID, WIFI_PSK);
    DEBUG("-> SoftAP IP");
    DEBUG(WiFi.softAPIP());
#else
#error PLEASE SELECT WIFI_MODE_AP OR WIFI_MODE_CLIENT!
#endif

#endif
}

// Main look that only tests connection between MCU and AWG
void awgTest()
{
    espAWGJDS8000 awg;

    DEBUG_LN("--> Sync Channels")
    awg.initDevice();

    for (uint8_t wave = 0; wave < EWaveType_Last; wave++)
    {
        // Step through defined waves
        DEBUG("--> Wave: ")
        DEBUG_LN(wave)
        awg.setCh1Wave(wave);

        // Step through channel output combinations
        switch (wave % 4)
        {
        case 1: {
            DEBUG("--> Output: ")
            DEBUG_LN("On Off")
            awg.setChOutput(1, 0);
            break;
        }
        case 2: {
            DEBUG("--> Output: ")
            DEBUG_LN("Off On")
            awg.setChOutput(0, 1);
            break;
        }
        case 3: {
            DEBUG("--> Output: ")
            DEBUG_LN("On On")
            awg.setChOutput(1, 1);
            break;
        }
        default: {
            DEBUG("--> Output: ")
            DEBUG_LN("Off Off")
            awg.setChOutput(0, 0);
            break;
        }
        }

        // Step through frequencies in kHz
        DEBUG("--> Frequency: ")
        DEBUG_LN(wave * 1000000)
        awg.setCh1Freq(wave * 1000000);

        // Step through amplitude in mV
        DEBUG("--> Amplitude: ")
        DEBUG_LN(wave * 100)
        awg.setCh1Ampl(wave * 100);

        // Step through phase in degrees
        DEBUG("--> Phase: ")
        DEBUG_LN(wave * 500)
        awg.setCh1Phase(wave * 500);

        delay(1000);
    }
}

// Main loop that connects Siglent Oscilloscope to AWG
void processRequest()
{
    // check WiFi connection
#ifdef WIFI_MODE_CLIENT
    if (WiFi.status() != WL_CONNECTED)
    {
        // cleanly disconnect from WiFi
#if defined(DEBUG_TELNET) && !defined(DEBUG_UART) && !defined(ESP32)
        telnet.stop();
#endif
        rpcServer.stop();
        lxiServer.stop();
        wifiConnected = false;

        if (wifiTimeout++ > 10)
        {
            wifiTimeout = 0;

            DEBUG_LN();
            DEBUG_LN("-> WiFi Reconnecting");

            WiFi.disconnect();
            WiFi.begin(WIFI_SSID, WIFI_PSK);
        }

        delay(500);
        DEBUG(".");
        return;
    }
#endif

    // new wifi connection
    if (!wifiConnected)
    {
        wifiConnected = true;
        wifiTimeout = 0;

        DEBUG_LN();
        DEBUG("-> WiFi connected to IP address: ");
        DEBUG_LN(WiFi.localIP());

        rpcServer.begin();
        lxiServer.begin();

#if defined(DEBUG_TELNET) && !defined(DEBUG_UART) && !defined(ESP32)
        telnet.begin();
#endif
    }

#if defined(DEBUG_TELNET) && !defined(DEBUG_UART) && !defined(ESP32)
    // check telnet connection
    telnet.loop();
#endif

    // check for rpc client
    WiFiClient rpcClient;
    rpcClient.setTimeout(1000);
    rpcClient = rpcServer.available();
    if (rpcClient)
    {
        DEBUG_LN();
        DEBUG_LN("-> RPC connection");

        handlePacket(rpcClient);
        rpcClient.stop();

        DEBUG_LN("-> RPC disconnecting");
    }

    // check for lxi client
    WiFiClient lxiClient;
    lxiClient.setTimeout(1000);
    lxiClient = lxiServer.available();
    if (lxiClient)
    {
        DEBUG_LN();
        DEBUG_LN("-> LXI connection");

        // handle lxi connection
        while (!handlePacket(lxiClient))
        {
#if defined(DEBUG_TELNET) && !defined(DEBUG_UART) && !defined(ESP32)
            telnet.loop();
#endif
            yield();
        }

        lxiClient.stop();

        DEBUG_LN("-> LXI disconnecting");
    }
}

void loop()
{
#ifdef AWG_TESTING
    awgTest();
#else
    processRequest();
#endif
}