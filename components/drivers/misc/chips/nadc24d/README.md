# NADC24D ADC Driver (RT-Thread)

This directory contains an RT-Thread external ADC driver for NADC24D.

## Kconfig

- `RT_USING_NADC24D`
- `RT_NADC24D_SPI_DEV_NAME` (default: `spi2`)
- `RT_NADC24D_SPI_MAX_HZ`
- `RT_NADC24D_DRDY_PIN` (0 disables DRDY polling)
- `RT_NADC24D_DRDY_TIMEOUT_MS`
- `RT_NADC24D_VREF_MV`

## MSH Test

```
nadc24d_read [channel] [count] [period_ms]
```

Example:
```
nadc24d_read 0 10 1000
```
