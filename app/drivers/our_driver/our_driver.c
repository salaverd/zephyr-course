#include "our_driver.h"

#include "zephyr/device.h"
#include "zephyr/devicetree.h"
#include "zephyr/logging/log.h"

#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>

#define DT_DRV_COMPAT our_driver

LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF);

struct our_driver_config
{
    struct gpio_dt_spec led;
};

struct our_driver_data
{
    bool led_state;
    uint32_t blink_period;
};

/* Custom extension API */
int our_driver_set_blink_period(const struct device *dev, uint32_t period)
{
    struct our_driver_data *data = dev->data;

    data->blink_period = period;

    LOG_INF("blink_period changed to %d", period);

    return 0;
}

static int sample_fetch_my_impl(const struct device *dev,
                                enum sensor_channel chan)
{
    const struct our_driver_config *cfg = dev->config;
    struct our_driver_data *data = dev->data;

    gpio_pin_set_dt(&cfg->led, 1);

    data->led_state = true;

    LOG_INF("Led ON, channel %d", chan);

    return 0;
}

static int channel_get_my_impl(const struct device *dev,
                               enum sensor_channel chan,
                               struct sensor_value *val)
{
    const struct our_driver_config *cfg = dev->config;
    struct our_driver_data *data = dev->data;

    gpio_pin_set_dt(&cfg->led, 0);

    data->led_state = false;

    LOG_INF("Led OFF, channel %d", chan);
    LOG_INF("blink_period = %d", data->blink_period);

    return 0;
}

static DEVICE_API(sensor, api_iomico_lecture) = {
    .sample_fetch = sample_fetch_my_impl,
    .channel_get = channel_get_my_impl,
};

static int init(const struct device *dev)
{
    const struct our_driver_config *cfg = dev->config;
    struct our_driver_data *data = dev->data;

    if (!gpio_is_ready_dt(&cfg->led))
    {
        LOG_ERR("GPIO not ready");
        return -ENODEV;
    }

    gpio_pin_configure_dt(
        &cfg->led,
        GPIO_OUTPUT_INACTIVE);

    data->led_state = false;
    data->blink_period = 1000;

    LOG_INF("Device Initialized");

    return 0;
}

#define CREATE_CONFIG(inst)                                 \
    static const struct our_driver_config config_##inst = { \
        .led = GPIO_DT_SPEC_INST_GET(inst, led_gpios),      \
    };

#define CREATE_DATA(inst) \
    static struct our_driver_data data_##inst;

#define DEV_INST(inst)     \
    CREATE_CONFIG(inst)    \
    CREATE_DATA(inst)      \
    DEVICE_DT_INST_DEFINE( \
        inst,              \
        init,              \
        NULL,              \
        &data_##inst,      \
        &config_##inst,    \
        POST_KERNEL,       \
        80,                \
        &api_iomico_lecture);

DT_INST_FOREACH_STATUS_OKAY(DEV_INST);