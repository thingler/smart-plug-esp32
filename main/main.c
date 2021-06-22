#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"

#include "nvs_flash.h"
#include "driver/gpio.h"

#include "init_wifi.h"
#include "handle_aws_iot.h"

#include "cJSON.h"

static const char *TAG = "main";
static char *topic = CONFIG_AWS_IOT_TOPIC;

#define GPIO_SWITCH 32

static void subCallBack(char *message) {
    cJSON *json = cJSON_Parse(message);    
    cJSON *action = cJSON_GetObjectItem(json, "action");
    cJSON *thingID = cJSON_GetObjectItem(json, "thingID");

    if (!action || !thingID) {
        ESP_LOGI(TAG, "Not a smartplug action message");
        cJSON_Delete(json);
        return;
    }

    char *actionVal = action->valuestring;
    char *thingIDVal = thingID->valuestring;

    if (strcmp(thingIDVal, (char *) CONFIG_AWS_CLIENT_ID) != 0) {
        ESP_LOGI(TAG, "Action is not for this smartplug");
        cJSON_Delete(json);
        return;
    }
    
    const char switchOn[] = "on";
    const char switchOff[] = "off";
    
    if (strcmp(actionVal, switchOn) == 0) {
        ESP_LOGI(TAG, "Turning switch on");
        gpio_set_level(GPIO_SWITCH, true);
    }
    if (strcmp(actionVal, switchOff) == 0) {
        ESP_LOGI(TAG, "Turning switch off");
        gpio_set_level(GPIO_SWITCH, false);
    }
    cJSON_Delete(json);
}

static void aws_iot_task(void *param)
{
    connectIOT();
    subscribeIOT(&subCallBack, topic);
}

void app_main()
{
    // Initialize NVS.
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );

    // Configure switch
    gpio_pad_select_gpio(GPIO_SWITCH);
    // set the correct direction
    gpio_set_direction(GPIO_SWITCH, GPIO_MODE_OUTPUT);
    // Turn swich off
    gpio_set_level(GPIO_SWITCH, false);

    init_wifi();
    xTaskCreatePinnedToCore(&aws_iot_task, "aws_iot_task", 9216, NULL, 5, NULL, 1);
    // publishIOT((char *) "Thing is connected", topic);

    cJSON *json;
    json=cJSON_CreateObject();
    cJSON_AddStringToObject(json, "message", "Thing is connected");
    cJSON_AddStringToObject(json, "thingID", CONFIG_AWS_CLIENT_ID);
    char *connMsg = cJSON_Print(json);
    publishIOT(connMsg, topic);
}
