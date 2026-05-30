#ifndef OUR_DRIVER_H
#define OUR_DRIVER_H

#include <zephyr/device.h>

#ifdef __cplusplus
extern "C" {
#endif

int our_driver_set_blink_period(const struct device *dev, uint32_t period);

#ifdef __cplusplus
}
#endif

#endif