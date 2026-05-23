#ifndef __NADC24D_H__
#define __NADC24D_H__

/* No heavy headers here; driver exposes a simple init function. */

#ifdef __cplusplus
extern "C" {
#endif

int rt_hw_nadc24d_init(void);

/*
 * This header exposes the RT-Thread chip driver initialization function.
 * Vendor-specific helpers and calibration routines were intentionally
 * removed and should be implemented in board-specific code if required.
 */

#ifdef __cplusplus
}
#endif

#endif /* __NADC24D_H__ */
