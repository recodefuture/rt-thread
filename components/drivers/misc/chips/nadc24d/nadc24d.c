#include "rttypes.h"
#include <rtconfig.h>

#if defined(RT_USING_NADC24D)

#include <rtdevice.h>
#include <rtthread.h>
#include <drivers/adc.h>
#include <drivers/dev_pin.h>

#include <stdlib.h>

#include "nadc24d_reg.h"
#include "nadc24d_cmd.h"

#ifndef RT_NADC24D_SPI_DEV_NAME
#define RT_NADC24D_SPI_DEV_NAME "spi2"
#endif

#ifndef RT_NADC24D_SPI_MAX_HZ
#define RT_NADC24D_SPI_MAX_HZ 1000000
#endif

#ifndef RT_NADC24D_SPI_DEVICE_NAME
#define RT_NADC24D_SPI_DEVICE_NAME "nadc24d_spi"
#endif

#ifndef RT_NADC24D_DRDY_PIN
#define RT_NADC24D_DRDY_PIN 0
#endif

#ifndef RT_NADC24D_DRDY_TIMEOUT_MS
#define RT_NADC24D_DRDY_TIMEOUT_MS 200
#endif

#ifndef RT_NADC24D_VREF_MV
#define RT_NADC24D_VREF_MV 2400
#endif

#define NADC24D_ADC_RESOLUTION 24

#define NADC24D_EXPECTED_ID 0xDA

struct nadc24d_device
{
    struct rt_spi_device *spi;
    struct rt_adc_device adc;
};

static struct nadc24d_device g_nadc24d;
static struct rt_spi_device g_nadc24d_spi_dev;

static rt_err_t nadc24d_spi_transfer(struct rt_spi_device *spi,
                                    const rt_uint8_t *tx,
                                    rt_uint8_t *rx,
                                    rt_size_t len,
                                    rt_bool_t cs_take,
                                    rt_bool_t cs_release)
{
    rt_ssize_t transferred;

    RT_UNUSED(cs_take);
    RT_UNUSED(cs_release);

    transferred = rt_spi_transfer(spi, tx, rx, len);
    return (transferred == (rt_ssize_t)len) ? RT_EOK : -RT_ERROR;
}

static rt_err_t nadc24d_send_cmd(rt_uint8_t cmd)
{
    return nadc24d_spi_transfer(g_nadc24d.spi, &cmd, RT_NULL, 1, RT_TRUE, RT_TRUE);
}

static rt_err_t nadc24d_write_reg(rt_uint8_t reg, rt_uint8_t value)
{
    rt_uint8_t tx[4] = {
        (rt_uint8_t)((reg & 0x1F) | ADC_WRITE_REG_ADDR_CMD), 
        0x00,                                               
        value,                                              
        0x00,                                               
    };

    return nadc24d_spi_transfer(g_nadc24d.spi, tx, RT_NULL, sizeof(tx), RT_TRUE, RT_TRUE);
}

static rt_err_t nadc24d_read_reg(rt_uint8_t reg, rt_uint8_t *value)
{
    rt_uint8_t tx[4] = {
        (rt_uint8_t)((reg & 0x1F) | ADC_READ_REG_ADDR_CMD),  
        0x00,                                              
        0x00,                                               
        0x00,                                              
    };
    rt_uint8_t rx[4] = {0};

    if (value == RT_NULL)
    {
        return -RT_EINVAL;
    }

    if (nadc24d_spi_transfer(g_nadc24d.spi, tx, rx, sizeof(tx), RT_TRUE, RT_TRUE) != RT_EOK)
    {
        return -RT_ERROR;
    }

    *value = rx[2]; 
    return RT_EOK;
}

static rt_err_t nadc24d_spi_read_24(rt_uint32_t *value, rt_bool_t cs_take)
{
    rt_uint8_t rx[3] = {0};
    rt_uint32_t raw;

    if (nadc24d_spi_transfer(g_nadc24d.spi, RT_NULL, rx, sizeof(rx), cs_take, RT_TRUE) != RT_EOK)
    {
        return -RT_ERROR;
    }

    raw = ((rt_uint32_t)rx[0] << 16) | ((rt_uint32_t)rx[1] << 8) | rx[2];

    if (raw & 0x800000)
    {
        raw |= 0xFF000000;
    }

    if (value)
    {
        *value = raw;
    }

    return RT_EOK;
}

static rt_err_t nadc24d_wait_drdy(void)
{
#if RT_NADC24D_DRDY_PIN > 0
    rt_tick_t timeout = rt_tick_from_millisecond(RT_NADC24D_DRDY_TIMEOUT_MS);
    rt_tick_t start = rt_tick_get();

    while (rt_pin_read(RT_NADC24D_DRDY_PIN) == 1)
    {
        if (rt_tick_get() - start > timeout)
        {
            return -RT_ETIMEOUT;
        }
        rt_thread_mdelay(1);
    }
#else
    rt_thread_mdelay(10);
#endif

    return RT_EOK;
}

static rt_err_t nadc24d_select_channel(rt_int8_t channel)
{
    rt_uint8_t reg = 0;
    rt_uint8_t val = 0;

    if (nadc24d_read_reg(REG_ADDR_PGA_CTRL1, &val) != RT_EOK)
    {
        return -RT_ERROR;
    }

    val &= (rt_uint8_t)(~(PGA_CTRL1_SEL_PGA_INP_Msk | PGA_CTRL1_SEL_PGA_INN_Msk));

    if (channel == 0)
    {
        reg = (rt_uint8_t)(PGA_CTRL1_SEL_PGA_INN_CH_0 | PGA_CTRL1_SEL_PGA_INP_CH_1);
    }
    else if (channel == 1)
    {
        reg = (rt_uint8_t)(PGA_CTRL1_SEL_PGA_INN_CH_2 | PGA_CTRL1_SEL_PGA_INP_CH_3);
    }
    else
    {
        return -RT_EINVAL;
    }

    val |= reg;

    return nadc24d_write_reg(REG_ADDR_PGA_CTRL1, val);
}

static rt_err_t nadc24d_read_raw(rt_uint32_t *value)
{
    rt_uint32_t raw = 0;

    if (value == RT_NULL)
    {
        return -RT_EINVAL;
    }

    /* Start conversion */
    if (nadc24d_send_cmd(ADC_START_CONVERSION_CMD) != RT_EOK)
        return -RT_ERROR;

    if (nadc24d_wait_drdy() != RT_EOK)
        return -RT_ETIMEOUT;

    /* Read 24-bit value */
    if (nadc24d_spi_read_24(&raw, RT_TRUE) != RT_EOK)
        return -RT_ERROR;

    *value = raw;

    /* Stop conversion */
    nadc24d_send_cmd(ADC_STOP_CONVERSION_CMD);

    return RT_EOK;
}

static rt_err_t nadc24d_adc_enabled(struct rt_adc_device *device, rt_int8_t channel, rt_bool_t enabled)
{
    RT_UNUSED(device);
    RT_UNUSED(channel);

    if (enabled)
    {
        return RT_EOK;
    }

    return RT_EOK;
}

static rt_err_t nadc24d_adc_convert(struct rt_adc_device *device, rt_int8_t channel, rt_uint32_t *value)
{
    RT_UNUSED(device);

    if (nadc24d_select_channel(channel) != RT_EOK)
    {
        return -RT_EINVAL;
    }

    return nadc24d_read_raw(value);
}

static rt_uint8_t nadc24d_adc_get_resolution(struct rt_adc_device *device)
{
    RT_UNUSED(device);
    return NADC24D_ADC_RESOLUTION;
}

static rt_int16_t nadc24d_adc_get_vref(struct rt_adc_device *device)
{
    RT_UNUSED(device);
    return RT_NADC24D_VREF_MV;
}

static const struct rt_adc_ops nadc24d_adc_ops =
{
    .enabled = nadc24d_adc_enabled,
    .convert = nadc24d_adc_convert,
    .get_resolution = nadc24d_adc_get_resolution,
    .get_vref = nadc24d_adc_get_vref,
};

static rt_err_t nadc24d_chip_reset(void)
{
    rt_uint8_t id = 0;

    rt_thread_mdelay(5);

    if (nadc24d_write_reg(REG_ADDR_PWD_CTRL2, 0x00) != RT_EOK)
    {
        return -RT_ERROR;
    }

    if (nadc24d_read_reg(REG_ADDR_ADC_VER_ID, &id) != RT_EOK)
    {
        return -RT_ERROR;
    }

    if (id != NADC24D_EXPECTED_ID)
    {
        return -RT_ERROR;
    }

    if (nadc24d_send_cmd(ADC_RESET_CMD) != RT_EOK)
    {
        return -RT_ERROR;
    }

    nadc24d_write_reg(REG_ADDR_PWD_CTRL2, 0x00);
    nadc24d_write_reg(REG_ADDR_CHOP_CTRL, 0x00);

    return RT_EOK;
}

int rt_hw_nadc24d_init(void)
{
    struct rt_spi_configuration cfg;
    rt_device_t dev;

    rt_pin_mode(rt_pin_get("PD.9"), PIN_MODE_OUTPUT);
    rt_pin_write(rt_pin_get("PD.9"), PIN_HIGH);

    if (rt_spi_bus_attach_device_cspin(&g_nadc24d_spi_dev, RT_NADC24D_SPI_DEV_NAME, "spi2", (rt_base_t)rt_pin_get("PE.11"),RT_NULL) != RT_EOK)
        return -RT_ERROR;

    // if (rt_spi_bus_attach_device(&g_nadc24d_spi_dev, RT_NADC24D_SPI_DEV_NAME, "spi2", RT_NULL) != RT_EOK)
    //     return -RT_ERROR;

    g_nadc24d.spi = (struct rt_spi_device *)rt_device_find(RT_NADC24D_SPI_DEV_NAME);
    if (g_nadc24d.spi == RT_NULL)
    {
        return -RT_ERROR;
    }

    cfg.data_width = 8;
    cfg.mode = RT_SPI_MASTER | RT_SPI_MODE_1 | RT_SPI_MSB;
    cfg.max_hz = RT_NADC24D_SPI_MAX_HZ;

    if (rt_spi_configure(g_nadc24d.spi, &cfg) != RT_EOK)
    {
        return -RT_ERROR;
    }

#if RT_NADC24D_DRDY_PIN > 0
    rt_pin_mode(RT_NADC24D_DRDY_PIN, PIN_MODE_INPUT_PULLUP);
#endif

    if (nadc24d_chip_reset() != RT_EOK)
    {
        return -RT_ERROR;
    }

    return rt_hw_adc_register(&g_nadc24d.adc, "nadc24d", &nadc24d_adc_ops, RT_NULL);
}
INIT_DEVICE_EXPORT(rt_hw_nadc24d_init);

#ifdef RT_USING_FINSH
static void nadc24d_read(int argc, char *argv[])
{
    rt_adc_device_t adc = RT_NULL;
    rt_int8_t channel = 0;
    rt_uint32_t count = 10;
    rt_tick_t delay = RT_TICK_PER_SECOND;

    if (argc >= 2)
    {
        channel = (rt_int8_t)atoi(argv[1]);
    }

    if (argc >= 3)
    {
        count = (rt_uint32_t)atoi(argv[2]);
    }

    if (argc >= 4)
    {
        delay = rt_tick_from_millisecond((rt_int32_t)atoi(argv[3]));
    }

    adc = (rt_adc_device_t)rt_device_find("nadc24d");
    if (adc == RT_NULL)
    {
        rt_kprintf("nadc24d: device not found\n");
        return;
    }

    while (count--)
    {
        rt_uint32_t value = rt_adc_read(adc, channel);
        rt_kprintf("nadc24d ch%d: %d\n", channel, (rt_int32_t)value);
        rt_thread_mdelay(delay);
    }
}
MSH_CMD_EXPORT(nadc24d_read, nadc24d_read [channel] [count] [period_ms]);
#endif

#endif /* RT_USING_NADC24D */
