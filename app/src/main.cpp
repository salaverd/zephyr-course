#include "zephyr/device.h"
#include "zephyr/devicetree.h"
#include "zephyr/drivers/sensor.h"
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
// #define SLEEP_TIME_MS 1000

#include "our_driver.h"

/* The devicetree node identifier for the "led0" alias. */
#define LED_NODE DT_ALIAS(app_led)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

namespace {
    void test(){
        const struct device* driver = DEVICE_DT_GET(DT_NODELABEL(our_driver0));
        struct sensor_value val ;
        auto ret = sensor_sample_fetch(driver);
        k_sleep(K_MSEC(1000));
        ret = sensor_channel_get(driver , SENSOR_CHAN_AMBIENT_TEMP, &val) ;
        LOG_INF("Channel ret %d" , ret) ;
    }
}


int main(void)
{
    const struct device *driver =
        DEVICE_DT_GET(DT_NODELABEL(our_driver0));

    if (!device_is_ready(driver))
    {
        LOG_ERR("Driver not ready");
        return 0;
    }

    our_driver_set_blink_period(driver, 500);

    while (1)
    {
        test();
        k_msleep(1000);
    }

    return 0;
}
