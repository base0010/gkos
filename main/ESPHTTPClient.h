#pragma once
#ifndef _ESPHTTPClient_H
#define _ESPHTTPClient_H

#include <string.h>
#include <stdlib.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <esp_event.h>
#include <tcpip_adapter.h>
#include <esp_http_client.h>
#include <string>
#include <Arduino.h>
#include <stddef.h>
#include <stdio.h>

class ESPHTTPClient{
#define MAX_HTTP_RX_BUFFER 512
    private:


    public:
      esp_err_t _http_event_handler(esp_http_client_event_t *evt);

      static void http_perform_as_stream_reader( const esp_http_client_config_t *config);

      static void web3_rawTx(const char *rawTx);

      static void http_async(const esp_http_client_config_t *config, const char *postData);
};

#endif
