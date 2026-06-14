#include "rtthread.h"
#include <rtconfig.h>

#if defined(RT_USING_SRAM)

#include "drv_ebi.h"
#include "cy62157e_45zs.h"

#ifndef RT_CY62157_USE_CUSTOM_TIMING
#define RT_CY62157_USE_CUSTOM_TIMING 1
#endif

#ifndef RT_CY62157_MCLKDIV
#define RT_CY62157_MCLKDIV EBI_MCLKDIV_1
#endif

#ifndef RT_CY62157_TCTL
#define RT_CY62157_TCTL 0x00050104
#endif

#ifndef RT_CY62157_TEST_STRIDE
#define RT_CY62157_TEST_STRIDE 0x40
#endif

#ifndef CY62157_BANK
#define CY62157_BANK EBI_BANK0
#endif

#ifndef CY62157_BASE_ADDR
#define CY62157_BASE_ADDR EBI_BANK0_BASE_ADDR
#endif

#ifndef CY62157_SIZE_BYTES
#define CY62157_SIZE_BYTES EBI_MAX_SIZE
#endif

static rt_bool_t g_cy62157_ready = RT_FALSE;

static volatile rt_uint8_t *cy62157_mem(void)
{
    return (volatile rt_uint8_t *)CY62157_BASE_ADDR;
}

static void cy62157_config_ebi(void)
{
    nu_ebi_init(CY62157_BANK,
                EBI_BUSWIDTH_8BIT,
                EBI_TIMING_VERYFAST,
                EBI_OPMODE_NORMAL,
                EBI_CS_ACTIVE_LOW);

#if defined(RT_CY62157_USE_CUSTOM_TIMING)
    EBI_SetBusTiming(CY62157_BANK, RT_CY62157_TCTL, RT_CY62157_MCLKDIV);
#endif
}

static int cy62157_quick_test(void)
{
    volatile rt_uint8_t *mem = cy62157_mem();
    rt_uint32_t i;
    
    const char *test_str = "chabaonuanxue!!!";
    rt_uint32_t str_len = 16; 

    if (!g_cy62157_ready)
        return -RT_ERROR;

    for (i = 0; i < CY62157_SIZE_BYTES; i++)
    {
        rt_uint8_t value = (rt_uint8_t)test_str[i % str_len];
        
        mem[i] = value;
        
        if (mem[i] != value)
            return -RT_ERROR;
    }
    rt_kprintf("SRAM 快速测试通过！\n");
    return RT_EOK;
}

static int cy62157_full_test(void)
{
    volatile rt_uint8_t *mem = cy62157_mem();
    rt_uint32_t i;

    if (!g_cy62157_ready)
        return -RT_ERROR;

    for (i = 0; i < CY62157_SIZE_BYTES; i++)
        mem[i] = (rt_uint8_t)(i & 0xFF);

    for (i = 0; i < CY62157_SIZE_BYTES; i++)
    {
        if (mem[i] != (rt_uint8_t)(i & 0xFF))
            return -RT_ERROR;
    }

    rt_kprintf("SRAM 递增数整片测试通过！\n");
    return RT_EOK;
}

int rt_hw_cy62157_init(void)
{
    rt_pin_mode(rt_pin_get("PD.7"), PIN_MODE_OUTPUT);
    rt_pin_write(rt_pin_get("PD.7"), PIN_LOW);

    cy62157_config_ebi();
    g_cy62157_ready = RT_TRUE;
    return RT_EOK;
}
INIT_DEVICE_EXPORT(rt_hw_cy62157_init);

static void cy62157_test(int argc, char **argv)
{
    int ret;

    if (argc > 1 && !rt_strcmp(argv[1], "full"))
        ret = cy62157_full_test();
    else
        ret = cy62157_quick_test();

    rt_kprintf("cy62157 test: %s\n", (ret == RT_EOK) ? "ok" : "fail");
}
MSH_CMD_EXPORT(cy62157_test, test CY62157 SRAM);

static int cy62157_dynamic_random_test(void)
{
    volatile rt_uint8_t *mem = (volatile rt_uint8_t *)cy62157_mem();
    rt_uint32_t i;
    rt_uint32_t total_bytes = 1 * 1024 * 1024; // 1MB

    if (!g_cy62157_ready)
        return -RT_ERROR;

    // 核心改动：获取当前系统运行的 Tick 数（每次执行这个值都不可能相同）
    // 把它作为这次测试的“动态魔数”
    rt_uint8_t dynamic_seed = (rt_uint8_t)(rt_tick_get() & 0xFF);
    if (dynamic_seed == 0) dynamic_seed = 0xA5; // 避免种子为0导致扰动变弱

    rt_kprintf("SRAM 动态随机数写入中... (种子: 0x%02X)\n", dynamic_seed);
    for (i = 0; i < total_bytes; i++)
    {
        // 将 dynamic_seed 混入算法中，让每一个地址的输出随种子整体改变
        rt_uint8_t random_data = (rt_uint8_t)((((i ^ 0x5A) + dynamic_seed) * 17) & 0xFF);
        mem[i] = random_data;
    }

    rt_kprintf("SRAM 动态随机数校验中...\n");
    for (i = 0; i < total_bytes; i++)
    {
        // 校验时使用同一颗动态种子进行反向计算
        rt_uint8_t expected = (rt_uint8_t)((((i ^ 0x5A) + dynamic_seed) * 17) & 0xFF);
        rt_uint8_t actual_got = mem[i]; // 锁死单次采样

        if (actual_got != expected)
        {
            rt_kprintf("动态随机数错误! 地址: 0x%08X, 期望: 0x%02X, 读到: 0x%02X\n", 
                       &mem[i], expected, actual_got);
            return -RT_ERROR;
        }
    }

    rt_kprintf("SRAM 8位动态随机数测试完成\n");
    return RT_EOK;
}
MSH_CMD_EXPORT(cy62157_dynamic_random_test, test CY62157 SRAM DynamicRandom);
#endif /* RT_USING_SRAM */