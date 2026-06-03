#include <stdlib.h>

#include <zephyr/device.h>
#include <zephyr/shell/shell.h>

#include "our_driver.h"

static const struct device *sensor_dev = DEVICE_DT_GET(DT_NODELABEL(our_driver0));

static int cmd_set(const struct shell *sh, size_t argc, char **argv)
{
    ARG_UNUSED(argc);

    long value = strtol(argv[1], NULL, 10);

    if (value < 100 || value > 5000)
    {
        shell_error(sh, "Value must be between 100 and 5000 ms");
        return -EINVAL;
    }

    int ret = our_driver_set_blink_period(sensor_dev, (uint32_t)value);

    if (ret)
    {
        shell_error(sh, "Failed to set value (%d)", ret);
        return ret;
    }

    shell_print(sh, "Blink period set to %ld ms", value);

    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(
    sensor_cmds,

    SHELL_CMD_ARG(set, NULL, "Set blink period", cmd_set, 2, 0),

    SHELL_SUBCMD_SET_END);

SHELL_CMD_REGISTER(sensor, &sensor_cmds, "Sensor commands", NULL);