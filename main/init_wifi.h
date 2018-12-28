#include "freertos/event_groups.h"

#ifndef INIT_WIFI
#define INIT_WIFI

void init_wifi(void);
int getCONNECTED_BIT(void);
EventGroupHandle_t getWIFIEventGroup(void);

#endif
