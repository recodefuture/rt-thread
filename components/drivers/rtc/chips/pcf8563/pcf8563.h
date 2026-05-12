#ifndef __PCF8563_H__
#define __PCF8563_H__

#include <rtdef.h>

#ifdef __cplusplus
extern "C" {
#endif

int rt_hw_rtc_pcf8563_init(void);
rt_err_t pcf8563_clear_vl_flag(void);

#ifdef __cplusplus
}
#endif

#endif /* __PCF8563_H__ */
