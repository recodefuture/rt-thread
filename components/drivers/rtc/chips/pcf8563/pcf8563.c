#include <rtconfig.h>

#if defined(RT_USING_RTC_PCF8563)

#include <rtdevice.h>
#include <rtthread.h>
#include <drivers/dev_rtc.h>
#include <time.h>

#ifndef RT_RTC_PCF8563_I2C_BUS_NAME
#define RT_RTC_PCF8563_I2C_BUS_NAME "i2c0"
#endif

#ifndef RT_RTC_PCF8563_I2C_ADDR
#define RT_RTC_PCF8563_I2C_ADDR 0x51
#endif

#ifndef RT_RTC_PCF8563_DEV_NAME
#define RT_RTC_PCF8563_DEV_NAME "rtc"
#endif

#ifndef RT_RTC_PCF8563_I2C_BUS_SPEED
#define RT_RTC_PCF8563_I2C_BUS_SPEED 100000
#endif

#define PCF8563_REG_SEC   0x02
#define PCF8563_REG_MIN   0x03
#define PCF8563_REG_HOUR  0x04
#define PCF8563_REG_DAY   0x05
#define PCF8563_REG_WDAY  0x06
#define PCF8563_REG_MONTH 0x07
#define PCF8563_REG_YEAR  0x08

#define PCF8563_SEC_VL_MASK   0x80
#define PCF8563_SEC_MASK      0x7F
#define PCF8563_MIN_MASK      0x7F
#define PCF8563_HOUR_MASK     0x3F
#define PCF8563_DAY_MASK      0x3F
#define PCF8563_WDAY_MASK     0x07
#define PCF8563_MONTH_MASK    0x1F
#define PCF8563_CENTURY_MASK  0x80

#define PCF8563_YEAR_MIN 2000
#define PCF8563_YEAR_MAX 2099

struct pcf8563_device
{
    struct rt_i2c_bus_device *bus;
    rt_uint8_t addr;
};

static struct pcf8563_device g_pcf8563;
static rt_rtc_dev_t g_pcf8563_rtc;

static rt_uint8_t pcf8563_bin2bcd(rt_uint8_t val)
{
    return (rt_uint8_t)(((val / 10U) << 4) | (val % 10U));
}

static rt_uint8_t pcf8563_bcd2bin(rt_uint8_t val)
{
    return (rt_uint8_t)(((val >> 4) * 10U) + (val & 0x0FU));
}

static rt_err_t pcf8563_read_regs(rt_uint8_t reg, rt_uint8_t *buf, rt_size_t len)
{
    struct rt_i2c_msg msgs[2];

    msgs[0].addr = g_pcf8563.addr;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf = &reg;
    msgs[0].len = 1;

    msgs[1].addr = g_pcf8563.addr;
    msgs[1].flags = RT_I2C_RD;
    msgs[1].buf = buf;
    msgs[1].len = len;

    if (rt_i2c_transfer(g_pcf8563.bus, msgs, 2) != 2)
    {
        return -RT_ERROR;
    }

    return RT_EOK;
}

static rt_err_t pcf8563_write_regs(rt_uint8_t reg, const rt_uint8_t *buf, rt_size_t len)
{
    rt_uint8_t tx_buf[1 + 7];
    struct rt_i2c_msg msg;

    if (len > 7U)
    {
        return -RT_EINVAL;
    }

    tx_buf[0] = reg;
    rt_memcpy(&tx_buf[1], buf, len);

    msg.addr = g_pcf8563.addr;
    msg.flags = RT_I2C_WR;
    msg.buf = tx_buf;
    msg.len = len + 1U;

    if (rt_i2c_transfer(g_pcf8563.bus, &msg, 1) != 1)
    {
        return -RT_ERROR;
    }

    return RT_EOK;
}

rt_err_t pcf8563_clear_vl_flag(void)
{
    rt_uint8_t sec_reg = 0;

    if (pcf8563_read_regs(PCF8563_REG_SEC, &sec_reg, 1) != RT_EOK)
    {
        return -RT_ERROR;
    }

    sec_reg &= (rt_uint8_t)(~PCF8563_SEC_VL_MASK);

    return pcf8563_write_regs(PCF8563_REG_SEC, &sec_reg, 1);
}

static rt_err_t pcf8563_get_time(time_t *time)
{
    rt_uint8_t buf[7];
    struct tm tm_out;
    rt_uint8_t month_reg;
    int year_base;

    if (pcf8563_read_regs(PCF8563_REG_SEC, buf, sizeof(buf)) != RT_EOK)
    {
        return -RT_ERROR;
    }

    if (buf[0] & PCF8563_SEC_VL_MASK)
    {
        pcf8563_clear_vl_flag();
    }

    month_reg = buf[5];
    year_base = (month_reg & PCF8563_CENTURY_MASK) ? 1900 : 2000;

    tm_out.tm_sec = pcf8563_bcd2bin(buf[0] & PCF8563_SEC_MASK);
    tm_out.tm_min = pcf8563_bcd2bin(buf[1] & PCF8563_MIN_MASK);
    tm_out.tm_hour = pcf8563_bcd2bin(buf[2] & PCF8563_HOUR_MASK);
    tm_out.tm_mday = pcf8563_bcd2bin(buf[3] & PCF8563_DAY_MASK);
    tm_out.tm_wday = pcf8563_bcd2bin(buf[4] & PCF8563_WDAY_MASK);
    tm_out.tm_mon = pcf8563_bcd2bin(month_reg & PCF8563_MONTH_MASK) - 1;
    tm_out.tm_year = (year_base + pcf8563_bcd2bin(buf[6])) - 1900;

    *time = mktime(&tm_out);
    return RT_EOK;
}

static rt_err_t pcf8563_set_time(const time_t *time)
{
    struct tm tm_in;
    rt_uint8_t buf[7];
    int year;

    if (localtime_r(time, &tm_in) == RT_NULL)
    {
        return -RT_ERROR;
    }

    year = tm_in.tm_year + 1900;
    if (year < PCF8563_YEAR_MIN || year > PCF8563_YEAR_MAX)
    {
        return -RT_EINVAL;
    }

    buf[0] = pcf8563_bin2bcd((rt_uint8_t)tm_in.tm_sec) & PCF8563_SEC_MASK;
    buf[1] = pcf8563_bin2bcd((rt_uint8_t)tm_in.tm_min) & PCF8563_MIN_MASK;
    buf[2] = pcf8563_bin2bcd((rt_uint8_t)tm_in.tm_hour) & PCF8563_HOUR_MASK;
    buf[3] = pcf8563_bin2bcd((rt_uint8_t)tm_in.tm_mday) & PCF8563_DAY_MASK;
    buf[4] = pcf8563_bin2bcd((rt_uint8_t)tm_in.tm_wday) & PCF8563_WDAY_MASK;
    buf[5] = pcf8563_bin2bcd((rt_uint8_t)(tm_in.tm_mon + 1)) & PCF8563_MONTH_MASK;
    buf[6] = pcf8563_bin2bcd((rt_uint8_t)(year - 2000));

    return pcf8563_write_regs(PCF8563_REG_SEC, buf, sizeof(buf));
}

static rt_err_t pcf8563_get_secs(time_t *sec)
{
    if (sec == RT_NULL)
    {
        return -RT_EINVAL;
    }

    return pcf8563_get_time(sec);
}

static rt_err_t pcf8563_set_secs(time_t *sec)
{
    if (sec == RT_NULL)
    {
        return -RT_EINVAL;
    }

    return pcf8563_set_time((const time_t *)sec);
}

static const struct rt_rtc_ops pcf8563_rtc_ops =
{
    .init = RT_NULL,
    .get_secs = pcf8563_get_secs,
    .set_secs = pcf8563_set_secs,
    .get_timeval = RT_NULL,
    .set_timeval = RT_NULL,
    .get_alarm = RT_NULL,
    .set_alarm = RT_NULL,
};

int rt_hw_rtc_pcf8563_init(void)
{
    rt_err_t ret;
    rt_uint32_t bus_speed = RT_RTC_PCF8563_I2C_BUS_SPEED;

    g_pcf8563.bus = (struct rt_i2c_bus_device *)rt_device_find(RT_RTC_PCF8563_I2C_BUS_NAME);
    if (g_pcf8563.bus == RT_NULL)
    {
        return -RT_ERROR;
    }

    g_pcf8563.addr = (rt_uint8_t)RT_RTC_PCF8563_I2C_ADDR;

    rt_device_open((rt_device_t)g_pcf8563.bus, RT_DEVICE_FLAG_RDWR);
    rt_device_control((rt_device_t)g_pcf8563.bus, RT_I2C_DEV_CTRL_CLK, &bus_speed);

    g_pcf8563_rtc.ops = &pcf8563_rtc_ops;
    ret = rt_hw_rtc_register(&g_pcf8563_rtc, RT_RTC_PCF8563_DEV_NAME, RT_DEVICE_FLAG_RDWR, RT_NULL);
    if (ret != RT_EOK)
    {
        return ret;
    }

    return RT_EOK;
}
INIT_DEVICE_EXPORT(rt_hw_rtc_pcf8563_init);

#ifdef RT_USING_FINSH
static void pcf8563_time_set_cmd(int argc, char *argv[])
{
    rt_device_t rtc_dev = RT_NULL;
    struct tm tm_in = {0};
    time_t t;

    if (argc != 7)
    {
        rt_kprintf("Usage: pcf8563_set_time YYYY MM DD hh mm ss\n");
        return;
    }

    tm_in.tm_year = atoi(argv[1]) - 1900;
    tm_in.tm_mon = atoi(argv[2]) - 1;
    tm_in.tm_mday = atoi(argv[3]);
    tm_in.tm_hour = atoi(argv[4]);
    tm_in.tm_min = atoi(argv[5]);
    tm_in.tm_sec = atoi(argv[6]);

    if (tm_in.tm_year < (PCF8563_YEAR_MIN - 1900) || tm_in.tm_year > (PCF8563_YEAR_MAX - 1900))
    {
        rt_kprintf("pcf8563: year out of range\n");
        return;
    }

    t = mktime(&tm_in);

    rtc_dev = rt_device_find(RT_RTC_PCF8563_DEV_NAME);
    if (rtc_dev == RT_NULL)
    {
        rt_kprintf("pcf8563: device '%s' not found\n", RT_RTC_PCF8563_DEV_NAME);
        return;
    }

    if (rt_device_control(rtc_dev, RT_DEVICE_CTRL_RTC_SET_TIME, &t) != RT_EOK)
    {
        rt_kprintf("pcf8563: set time failed\n");
        return;
    }

    rt_kprintf("pcf8563: set time ok\n");
}
MSH_CMD_EXPORT(pcf8563_time_set_cmd, pcf8563_time_set YYYY MM DD hh mm ss);

static void pcf8563_time_test(int argc, char *argv[])
{
    rt_tick_t delay = RT_TICK_PER_SECOND;
    rt_uint32_t count = 10;
    rt_device_t rtc_dev = RT_NULL;

    if (argc == 2)
    {
        int ms = atoi(argv[1]);
        if (ms > 0)
        {
            delay = rt_tick_from_millisecond((rt_int32_t)ms);
        }
    }
    else if (argc >= 3)
    {
        int ms = atoi(argv[1]);
        int times = atoi(argv[2]);

        if (ms > 0)
        {
            delay = rt_tick_from_millisecond((rt_int32_t)ms);
        }

        if (times > 0)
        {
            count = (rt_uint32_t)times;
        }
    }

    rtc_dev = rt_device_find(RT_RTC_PCF8563_DEV_NAME);
    if (rtc_dev == RT_NULL)
    {
        rt_kprintf("pcf8563: device '%s' not found\n", RT_RTC_PCF8563_DEV_NAME);
        return;
    }

    while (count--)
    {
        time_t now = 0;
        if (rt_device_control(rtc_dev, RT_DEVICE_CTRL_RTC_GET_TIME, &now) == RT_EOK)
        {
            rt_kprintf("%s", ctime(&now));
        }
        else
        {
            rt_kprintf("pcf8563: read time failed\n");
        }
        rt_thread_mdelay(delay);
    }
}
MSH_CMD_EXPORT(pcf8563_time_test, pcf8563_time_test [period_ms] [count]);
#endif

#endif /* RT_USING_RTC_PCF8563 */
