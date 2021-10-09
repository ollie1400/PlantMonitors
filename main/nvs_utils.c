#include "nvs_utils.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_log.h"

#define SENSOR_NAME_KEY "sname"

typedef uint32_t nvs_handle_t;

static const char *TAG = "nvs_utils";

void clearNVS()
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(nvs_flash_erase());
}

uint8_t initNVS()
{
    ESP_LOGI(TAG, "initNVS");
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    if (err != ESP_OK)
    {
        ESP_LOGI(TAG, "NVS error: %s\n", esp_err_to_name(err));
        return false;
    }
    else
    {
        ESP_LOGI(TAG, "NVS OK");
        return true;
    }
}

uint8_t tryReadSSIDPW(char *ssid, char *pwd)
{
    ESP_LOGI(TAG, "tryReadSSIDPW");
    nvs_handle_t storage;
    esp_err_t err = nvs_open("store", NVS_READWRITE, &storage);
    if (err != ESP_OK)
    {
        ESP_LOGI(TAG, "nvs_open: %s\n", esp_err_to_name(err));
        return false;
    }
    else
    {
        // Read ssid
        size_t length = MAX_SSID_LENGTH;
        err = nvs_get_str(storage, "ssid", ssid, &length);
        switch (err)
        {
        case ESP_OK:
            ESP_LOGI(TAG, "SSID was: %s\n", ssid);
            break;
        default:
            ESP_LOGI(TAG, "nvs_get_str(ssid): %s\n", esp_err_to_name(err));
            nvs_close(storage);
            return false;
        }

        if (length > MAX_SSID_LENGTH)
        {
            ESP_LOGI(TAG, "Stored SSID too long");
            nvs_close(storage);
            return false;
        }

        // read pwd
        length = MAX_PWD_LENGTH;
        err = nvs_get_str(storage, "pwd", pwd, &length);
        switch (err)
        {
        case ESP_OK:
            ESP_LOGI(TAG, "pwd was: %s\n", pwd);
            break;
        default:
            ESP_LOGI(TAG, "nvs_get_str(pwd): %s\n", esp_err_to_name(err));
            nvs_close(storage);
            return false;
        }

        if (length > MAX_PWD_LENGTH)
        {
            ESP_LOGI(TAG, "Stored PWD too long");
            nvs_close(storage);
            return false;
        }

        return true;
    }
}

uint8_t writeSSIDPW(const char *ssid, const char *pwd)
{
    ESP_LOGI(TAG, "writeSSIDPW");
    nvs_handle_t storage;
    esp_err_t err = nvs_open("store", NVS_READWRITE, &storage);
    if (err != ESP_OK)
    {
        ESP_LOGI(TAG, "nvs_open: %s\n", esp_err_to_name(err));
        return false;
    }
    else
    {
        // Write SSID
        if (ssid == NULL)
        {
            err = nvs_erase_key(storage, "ssid");
            if (err != ESP_OK)
            {
                ESP_LOGI(TAG, "nvs_erase_key(ssid): %s\n", esp_err_to_name(err));
                nvs_close(storage);
                return false;
            }
        }
        else
        {
            err = nvs_set_str(storage, "ssid", ssid);
            if (err != ESP_OK)
            {
                ESP_LOGI(TAG, "nvs_set_str(ssid): %s\n", esp_err_to_name(err));
                nvs_close(storage);
                return false;
            }
        }

        // write password
        if (pwd == NULL)
        {
            err = nvs_erase_key(storage, "pwd");
            if (err != ESP_OK)
            {
                ESP_LOGI(TAG, "nvs_erase_key(pwd): %s\n", esp_err_to_name(err));
                nvs_close(storage);
                return false;
            }
        }
        else
        {
            err = nvs_set_str(storage, "pwd", pwd);
            if (err != ESP_OK)
            {
                ESP_LOGI(TAG, "nvs_set_str(pwd): %s\n", esp_err_to_name(err));
                nvs_close(storage);
                return false;
            }
        }

        // commit
        err = nvs_commit(storage);
        if (err != ESP_OK)
        {
            ESP_LOGI(TAG, "nvs_commit: %s\n", esp_err_to_name(err));
            nvs_close(storage);
            return false;
        }

        nvs_close(storage);
        return true;
    }
}

uint8_t writeSensorName(const char *name)
{
    ESP_LOGI(TAG, "writeSensorName");
    nvs_handle_t storage;
    esp_err_t err = nvs_open("store", NVS_READWRITE, &storage);
    if (err != ESP_OK)
    {
        ESP_LOGI(TAG, "nvs_open: %s\n", esp_err_to_name(err));
        return false;
    }

    // Write name
    if (name == NULL)
    {
        err = nvs_erase_key(storage, SENSOR_NAME_KEY);
        if (err != ESP_OK)
        {
            ESP_LOGI(TAG, "nvs_erase_key(" SENSOR_NAME_KEY "): %s\n", esp_err_to_name(err));
            nvs_close(storage);
            return false;
        }
    }
    else
    {
        err = nvs_set_str(storage, SENSOR_NAME_KEY, name);
        if (err != ESP_OK)
        {
            ESP_LOGI(TAG, "nvs_set_str(" SENSOR_NAME_KEY "): %s\n", esp_err_to_name(err));
            nvs_close(storage);
            return false;
        }
    }
    return true;
}

uint8_t tryReadSensorName(char *name)
{
    ESP_LOGI(TAG, "tryReadSensorName");
    nvs_handle_t storage;
    esp_err_t err = nvs_open("store", NVS_READWRITE, &storage);
    if (err != ESP_OK)
    {
        ESP_LOGI(TAG, "nvs_open: %s\n", esp_err_to_name(err));
        return false;
    }

    // Read name
    size_t length = MAX_SENSOR_NAME;
    err = nvs_get_str(storage, SENSOR_NAME_KEY, name, &length);
    switch (err)
    {
    case ESP_OK:
        ESP_LOGI(TAG, "Sensor name was: %s\n", name);
        break;
    default:
        ESP_LOGI(TAG, "nvs_get_str(" SENSOR_NAME_KEY "): %s\n", esp_err_to_name(err));
        nvs_close(storage);
        return false;
    }

    if (length > MAX_SENSOR_NAME)
    {
        ESP_LOGI(TAG, "Stored sensor name too long");
        nvs_close(storage);
        return false;
    }

    return true;
}