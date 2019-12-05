#include "ESPHTTPClient.h"


#define MAX_HTTP_RX_BUFFER 512

static const char *TAG = "HTTP_CLIENT";

esp_err_t ESPHTTPClient::_http_event_handler(esp_http_client_event_t *evt)
{
    switch (evt->event_id)
    {
    case HTTP_EVENT_ERROR:
        ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
        break;
    case HTTP_EVENT_ON_CONNECTED:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
        break;
    case HTTP_EVENT_HEADER_SENT:
        ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
        break;
    case HTTP_EVENT_ON_HEADER:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
        break;
    case HTTP_EVENT_ON_DATA:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
        if (!esp_http_client_is_chunked_response(evt->client))
        {
            // Write out data
            // printf("%.*s", evt->data_len, (char*)evt->data);
        }

        break;
    case HTTP_EVENT_ON_FINISH:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
        break;
    case HTTP_EVENT_DISCONNECTED:
        ESP_LOGD(TAG, "HTTP_EVENT_DISCONNECTED");
        break;
    }
    return ESP_OK;
}
//HOTSPOT


// const esp_http_client_config_t web3RPC;
// web3RPC.url = web3URL.c_str();

static char buffer[MAX_HTTP_RX_BUFFER + 1];

void ESPHTTPClient::http_perform_as_stream_reader(const esp_http_client_config_t *config)
{
    // char *buffer = malloc(MAX_HTTP_RX_BUFFER + 1);
    //buffer = {0}?
    if (buffer == NULL)
    {
        ESP_LOGE(TAG, "Cannot malloc http receive buffer");
        return;
    }

    esp_http_client_handle_t client = esp_http_client_init(config);
    esp_err_t err;
    if ((err = esp_http_client_open(client, 0)) != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err));
        // free(buffer);
        return;
    }
    int content_length = esp_http_client_fetch_headers(client);
    int total_read_len = 0, read_len;
    if (total_read_len < content_length && content_length <= MAX_HTTP_RX_BUFFER)
    {
        read_len = esp_http_client_read(client, buffer, content_length);
        if (read_len <= 0)
        {
            ESP_LOGE(TAG, "Error read data");
        }
        buffer[read_len] = 0;
        ESP_LOGD(TAG, "read_len = %d", read_len);
    }
    ESP_LOGI(TAG, "HTTP Stream reader Status = %d, content_length = %d",
             esp_http_client_get_status_code(client),
             esp_http_client_get_content_length(client));
    esp_http_client_close(client);
    esp_http_client_cleanup(client);
    // free(buffer);
}

 void ESPHTTPClient::http_async(const esp_http_client_config_t *config, const char* postData){
     Serial.println("HTTPASYNC");

     esp_http_client_handle_t client = esp_http_client_init(config);

     Serial.println("HTTP CLIENT CREATED");
     esp_err_t err;

     esp_http_client_set_method(client, HTTP_METHOD_POST);
     esp_http_client_set_post_field(client, postData, strlen(postData));


     Serial.println("POST CREATED");

     while (1)
     {
         err = esp_http_client_perform(client);
         Serial.println("PERFORMED");

         if (err != ESP_ERR_HTTP_EAGAIN)
         {
             break;
         }
    }

    if (err == ESP_OK) {
        ESP_LOGI(TAG, "HTTPS Status = %d, content_length = %d",
                esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
    } else {
        ESP_LOGE(TAG, "Error perform http request %s", esp_err_to_name(err));
    }
    // esp_http_client_cleanup(client);
    vTaskDelay(100 * portTICK_PERIOD_MS);

    Serial.println("Stream Read Start");
    esp_http_client_close(client);
    esp_http_client_cleanup(client);

    ESPHTTPClient::http_perform_as_stream_reader(config);
}


void ESPHTTPClient::web3_rawTx(const char *rawTx)
{
    //async dosent work for http
    std::string web3URL = "http://10.42.0.1:8000";
//     const esp_http_client_config_t web3RPC = {
//         url : web3URL.c_str(),
//         host : NULL,
//         port : NULL,
//         username : NULL,
//         password : NULL,
//         auth_type : esp_http_client_auth_type_t{},
//         path : NULL,
//         query : NULL,
//         cert_pem : NULL,
//         method : esp_http_client_method_t{},
//         timeout_ms : 3000,
//         disable_auto_redirect : false,
//         max_redirection_count : 0,
//         event_handler : NULL,
//         transport_type : esp_http_client_transport_t{},
//         buffer_size : 512,
//         user_data : NULL,
//         is_async : false
//     }; 

//     Serial.println("RAWTX");

     // const esp_http_client_config_t web3RPC = {
     //     url : web3URL.c_str(),
     //     port : 8000,
     //     timeout_ms : 5000,
     //     is_async : true
     // };
//      ESPHTTPClient::http_async(&web3RPC, rawTx);
}
