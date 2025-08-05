#include "unity.h"
#include "esp_log.h"
#include "esp_mac.h"
#include "nvs_flash.h"
#include "usb_mac.h" // The component to test
#include "bios_config_aq.h"

// Helper to clear the specific NVS key
static void clear_nvs_mac_key(void)
{
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open("bios_cfg", NVS_READWRITE, &my_handle);
    if (err == ESP_OK) {
        nvs_erase_key(my_handle, "usb_mac");
        nvs_commit(my_handle);
        nvs_close(my_handle);
    }
}

TEST_CASE("MAC address generation is deterministic and correct", "[usb_mac]")
{
    // -- Setup --
    // Ensure NVS is initialized for the test
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    
    // Clear any previously stored MAC to ensure we test the generation logic
    clear_nvs_mac_key();

    // -- Get base MAC for comparison --
    uint8_t base_mac[6];
    ESP_ERROR_CHECK(esp_efuse_mac_get_default(base_mac));

    // -- First call: Generate and store MAC --
    uint8_t mac1[6];
    get_usb_ncm_mac(mac1);

    // -- Verification 1: Correctness of generated MAC --
    TEST_ASSERT_EQUAL_HEX8( (base_mac[0] & 0xFC) | 0x02, mac1[0] ); // Check locally administered bit
    TEST_ASSERT_EQUAL_HEX8_ARRAY(&base_mac[1], &mac1[1], 5); // Check remaining bytes match base MAC

    // -- Second call: Fetch from NVS --
    uint8_t mac2[6];
    get_usb_ncm_mac(mac2);

    // -- Verification 2: Persistence --
    TEST_ASSERT_EQUAL_HEX8_ARRAY(mac1, mac2, 6);

    // -- Teardown --
    clear_nvs_mac_key();
    nvs_flash_deinit();
}
