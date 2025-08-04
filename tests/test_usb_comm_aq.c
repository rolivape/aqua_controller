/*
 * Unit Tests for usb_comm_aq Component
 *
 * These tests use Unity and CMock to validate the usb_comm_aq component's
 * initialization logic, especially its interaction with NVS for loading
 * USB descriptors.
 */

#include "unity.h"
#include "cmock.h"
#include "esp_err.h"

// Mocks for dependencies
#include "mock_nvs_flash.h"
#include "mock_config_aq.h"
#include "mock_tinyusb.h"
#include "mock_esp_netif.h"
#include "mock_esp_event.h"
#include "mock_esp_log.h"
#include "mock_esp_mac.h"

// Header for the component being tested
#include "usb_comm_aq.h"

// Test setup and teardown
void setUp(void) {
    // Set up any necessary pre-test conditions
    esp_log_write_fake.custom_fake = (esp_log_write_fake_custom_fake_t)printf;
}

void tearDown(void) {
    // Clean up after tests
}

// --- Test Cases ---

void test_usb_comm_aq_init_success_with_nvs_values(void) {
    // Arrange: Mock expectations for a successful initialization
    // where NVS values are found and used.

    // Mock NVS/config functions to return success
    config_aq_get_string_ExpectAndReturn("usb_manuf", "MyCompany", 64, ESP_OK);
    config_aq_get_string_IgnoreArg_out_value();
    config_aq_get_string_ReturnThruPtr_out_value("MyCompany");

    config_aq_get_string_ExpectAndReturn("usb_prod", "MyProduct", 64, ESP_OK);
    config_aq_get_string_IgnoreArg_out_value();
    config_aq_get_string_ReturnThruPtr_out_value("MyProduct");

    config_aq_get_string_ExpectAndReturn("usb_serial", "SN12345", 64, ESP_OK);
    config_aq_get_string_IgnoreArg_out_value();
    config_aq_get_string_ReturnThruPtr_out_value("SN12345");

    // Mock ESP-IDF API calls
    esp_read_mac_ExpectAndReturn(ESP_OK, ESP_MAC_ETH);
    esp_netif_init_ExpectAndReturn(ESP_OK);
    esp_event_loop_create_default_ExpectAndReturn(ESP_OK);
    esp_netif_new_ExpectAnyArgsAndReturn( (esp_netif_t) 0x1 ); // Return a dummy pointer
    tinyusb_driver_install_ExpectAnyArgsAndReturn(ESP_OK);
    esp_netif_attach_ExpectAnyArgsAndReturn(ESP_OK);
    esp_netif_get_io_driver_ExpectAnyArgsAndReturn(NULL);
    esp_netif_dhcpc_stop_ExpectAnyArgsAndReturn(ESP_OK);
    esp_netif_set_ip_info_ExpectAnyArgsAndReturn(ESP_OK);


    // Act
    esp_err_t result = usb_comm_aq_init("panel-01", "192.168.7.2", (void*)0x1);

    // Assert
    TEST_ASSERT_EQUAL(ESP_OK, result);
}

void test_usb_comm_aq_init_success_with_default_values(void) {
    // Arrange: Mock NVS to fail, forcing the use of default values.
    config_aq_get_string_ExpectAndReturn("usb_manuf", "DefaultCorp", 64, ESP_ERR_NVS_NOT_FOUND);
    config_aq_get_string_IgnoreArg_out_value();
    config_aq_get_string_ReturnThruPtr_out_value("DefaultCorp");

    config_aq_get_string_ExpectAndReturn("usb_prod", "AquaPanel-S3", 64, ESP_ERR_NVS_NOT_FOUND);
    config_aq_get_string_IgnoreArg_out_value();
    config_aq_get_string_ReturnThruPtr_out_value("AquaPanel-S3");

    config_aq_get_string_ExpectAndReturn("usb_serial", "00000001", 64, ESP_ERR_NVS_NOT_FOUND);
    config_aq_get_string_IgnoreArg_out_value();
    config_aq_get_string_ReturnThruPtr_out_value("00000001");

    // Mock other ESP-IDF API calls
    esp_read_mac_ExpectAndReturn(ESP_OK, ESP_MAC_ETH);
    esp_netif_init_ExpectAndReturn(ESP_OK);
    esp_event_loop_create_default_ExpectAndReturn(ESP_OK);
    esp_netif_new_ExpectAnyArgsAndReturn( (esp_netif_t) 0x1 );
    tinyusb_driver_install_ExpectAnyArgsAndReturn(ESP_OK);
    esp_netif_attach_ExpectAnyArgsAndReturn(ESP_OK);
    esp_netif_get_io_driver_ExpectAnyArgsAndReturn(NULL);
    esp_netif_dhcpc_stop_ExpectAnyArgsAndReturn(ESP_OK);
    esp_netif_set_ip_info_ExpectAnyArgsAndReturn(ESP_OK);

    // Act
    esp_err_t result = usb_comm_aq_init("panel-01", "192.168.7.2", (void*)0x1);

    // Assert
    TEST_ASSERT_EQUAL(ESP_OK, result);
}

void test_usb_comm_aq_init_fails_on_driver_install(void) {
    // Arrange: Mock tinyusb_driver_install to fail.
    config_aq_get_string_IgnoreAndReturn(ESP_OK); // Assume NVS is fine
    esp_read_mac_ExpectAndReturn(ESP_OK, ESP_MAC_ETH);
    esp_netif_init_ExpectAndReturn(ESP_OK);
    esp_event_loop_create_default_ExpectAndReturn(ESP_OK);
    esp_netif_new_ExpectAnyArgsAndReturn( (esp_netif_t) 0x1 );
    tinyusb_driver_install_ExpectAnyArgsAndReturn(ESP_FAIL); // The critical failure

    // Act
    esp_err_t result = usb_comm_aq_init("panel-01", "192.168.7.2", (void*)0x1);

    // Assert
    TEST_ASSERT_EQUAL(ESP_FAIL, result);
}

void test_usb_comm_aq_init_invalid_args(void) {
    // Act & Assert
    TEST_ASSERT_EQUAL(ESP_ERR_INVALID_ARG, usb_comm_aq_init(NULL, "192.168.7.2", (void*)0x1));
    TEST_ASSERT_EQUAL(ESP_ERR_INVALID_ARG, usb_comm_aq_init("panel-01", NULL, (void*)0x1));
    TEST_ASSERT_EQUAL(ESP_ERR_INVALID_ARG, usb_comm_aq_init("panel-01", "192.168.7.2", NULL));
}

// Main function to run tests
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_usb_comm_aq_init_success_with_nvs_values);
    RUN_TEST(test_usb_comm_aq_init_success_with_default_values);
    RUN_TEST(test_usb_comm_aq_init_fails_on_driver_install);
    RUN_TEST(test_usb_comm_aq_init_invalid_args);
    return UNITY_END();
}
