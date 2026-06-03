#include <zephyr/shell/shell.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

#include "our_driver.h"

static const struct device *sensor_dev = DEVICE_DT_GET(DT_NODELABEL(our_driver0));

static int cmd_info(const struct shell *sh, size_t argc, char **argv)
{
    shell_print(sh, "Device: %s", sensor_dev->name);

    shell_print(sh, "Ready: %s", device_is_ready(sensor_dev) ? "yes" : "no");

    return 0;
}

static int cmd_fetch(const struct shell *sh, size_t argc, char **argv)
{
    int ret = sensor_sample_fetch(sensor_dev);

    if (ret)
    {
        shell_error(sh, "sensor_sample_fetch failed (%d)", ret);
        return ret;
    }

    shell_print(sh, "Sample fetched");

    return 0;
}

static int cmd_read(const struct shell *sh, size_t argc, char **argv)
{
    struct sensor_value val;

    int ret = sensor_channel_get( sensor_dev, SENSOR_CHAN_AMBIENT_TEMP, &val);

    if (ret)
    {
        shell_error(sh, "sensor_channel_get failed (%d)", ret);
        return ret;
    }

    shell_print(sh, "Value: %d.%06d", val.val1, val.val2);

    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(
    sensor_cmds,

    SHELL_CMD(fetch, NULL, "Call sensor_sample_fetch()", cmd_fetch),

    SHELL_CMD(read, NULL, "Call sensor_channel_get()", cmd_read),

    SHELL_CMD(info, NULL, "Device information", cmd_info),

    SHELL_SUBCMD_SET_END);

SHELL_CMD_REGISTER(sensor, &sensor_cmds, "Sensor commands", NULL);