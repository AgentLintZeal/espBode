#ifndef _ESP_PARSER_H_
#define _ESP_PARSER_H_

#include "esp_config.h"

#ifdef AWG_TYPE_JDS8000
#include "esp_awg_jds8000.h"
extern espAWGJDS8000 awg;
#endif

extern volatile char *gReadBuffer;

void handleScopeMsg(char *msg, uint8_t len);

#endif
