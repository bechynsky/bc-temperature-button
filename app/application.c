#include <application.h>

// LED instance
bc_led_t led;

// Button instance
bc_button_t button;

// On board thermometer
// http://www.ti.com/product/TMP112#
bc_tmp112_t temp;

void button_event_handler(bc_button_t *self, bc_button_event_t event, void *event_param)
{
    if (event != BC_BUTTON_EVENT_PRESS)
    {
        return;
    }

    float value;
    
    bc_tmp112_measure(&temp);

    if (bc_tmp112_get_temperature_celsius(&temp, &value))
    {
        bc_radio_pub_temperature(0, &value);
    }
    
    bc_led_pulse(&led, 200);
}

void application_init(void)
{
    // Initialize radio
    bc_radio_init(BC_RADIO_MODE_NODE_SLEEPING);

    // Initialize LED
    bc_led_init(&led, BC_GPIO_LED, false, false);
    bc_led_set_mode(&led, BC_LED_MODE_ON);


    // Initialize button
    bc_button_init(&button, BC_GPIO_BUTTON, BC_GPIO_PULL_DOWN, false);
    bc_button_set_event_handler(&button, button_event_handler, NULL);

    // initialize TMP112 sensor
    bc_tmp112_init(&temp, BC_I2C_I2C0, 0x49);

    bc_radio_pairing_request("temperature-button", VERSION);

   
    bc_led_pulse(&led, 2000);
}
