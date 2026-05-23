#ifndef __NADC24D_REG_H__
#define __NADC24D_REG_H__

#include <rtdef.h>

/* ADC Commands */
#define ADC_NOP_CMD                             0x00
#define ADC_WAKE_UP_CMD                         0x02
#define ADC_POWER_DOWN_CMD                      0x04
#define ADC_RESET_CMD                           0x06
#define ADC_START_CONVERSION_CMD                0x08
#define ADC_STOP_CONVERSION_CMD                 0x0A

#define ADC_SYSOC_CMD                           0x10
#define ADC_TEMPSENSORP1C_CMD                   0x14

#define ADC_READ_DATA_CMD                       0x20

#define ADC_READ_REG_ADDR_CMD                   0x40
#define ADC_READ_REG_LEN_CMD                    0x1F

#define ADC_WRITE_REG_ADDR_CMD                  0x60
#define ADC_WRITE_REG_LEN_CMD                   0x1F

#define ADC_READ_COEF_ADDR_CMD                  0x9F
#define ADC_READ_COEF_LEN_CMD                   0x02

#define ADC_WRITE_COEF_ADDR_CMD                 0xBF
#define ADC_WRITE_COEF_LEN_CMD                  0x02

/* Bit Field Definition */
#define PWD_CTRL1_PWD_ADC_INT_REF_Pos          (2)
#define PWD_CTRL1_PWD_ADC_INT_REF_Msk          (1 << PWD_CTRL1_PWD_ADC_INT_REF_Pos)
#define PWD_CTRL1_PWD_ADC_INT_REF_POWER_UP     (0 << PWD_CTRL1_PWD_ADC_INT_REF_Pos)
#define PWD_CTRL1_PWD_ADC_INT_REF_POWER_DOWN   (1 << PWD_CTRL1_PWD_ADC_INT_REF_Pos)

#define PWD_CTRL1_PWD_OSC_Pos           (5)
#define PWD_CTRL1_PWD_OSC_Msk           (1 << PWD_CTRL1_PWD_OSC_Pos)
#define PWD_CTRL1_PWD_OSC_POWER_UP      (0 << PWD_CTRL1_PWD_OSC_Pos)
#define PWD_CTRL1_PWD_OSC_POWER_DOWN    (1 << PWD_CTRL1_PWD_OSC_Pos)

#define PWD_CTRL1_PWD_DAC_Pos           (6)
#define PWD_CTRL1_PWD_DAC_Msk           (1 << PWD_CTRL1_PWD_DAC_Pos)
#define PWD_CTRL1_PWD_DAC_POWER_UP      (0 << PWD_CTRL1_PWD_DAC_Pos)
#define PWD_CTRL1_PWD_DAC_POWER_DOWN    (1 << PWD_CTRL1_PWD_DAC_Pos)

#define PWD_CTRL1_PWD_DACBUF_Pos            (7)
#define PWD_CTRL1_PWD_DACBUF_Msk            (1 << PWD_CTRL1_PWD_DACBUF_Pos)
#define PWD_CTRL1_PWD_DACBUF_POWER_UP       (0 << PWD_CTRL1_PWD_DACBUF_Pos)
#define PWD_CTRL1_PWD_DACBUF_POWER_DOWN     (1 << PWD_CTRL1_PWD_DACBUF_Pos)

#define PWD_CTRL2_PWD_MOD_REFP_Pos          (0)
#define PWD_CTRL2_PWD_MOD_REFP_Msk          (1 << PWD_CTRL2_PWD_MOD_REFP_Pos)
#define PWD_CTRL2_PWD_MOD_REFP_POWER_UP     (0 << PWD_CTRL2_PWD_MOD_REFP_Pos)
#define PWD_CTRL2_PWD_MOD_REFP_POWER_DOWN   (1 << PWD_CTRL2_PWD_MOD_REFP_Pos)

#define PWD_CTRL2_PWD_PGA_BUFF_Pos          (1)
#define PWD_CTRL2_PWD_PGA_BUFF_Msk          (1 << PWD_CTRL2_PWD_PGA_BUFF_Pos)
#define PWD_CTRL2_PWD_PGA_BUFF_POWER_UP     (0 << PWD_CTRL2_PWD_PGA_BUFF_Pos)
#define PWD_CTRL2_PWD_PGA_BUFF_POWER_DOWN   (1 << PWD_CTRL2_PWD_PGA_BUFF_Pos)

#define PWD_CTRL2_PWD_CHIP_Pos          (7)
#define PWD_CTRL2_PWD_CHIP_Msk          (1 << PWD_CTRL2_PWD_CHIP_Pos)
#define PWD_CTRL2_PWD_CHIP_POWER_UP     (0 << PWD_CTRL2_PWD_CHIP_Pos)
#define PWD_CTRL2_PWD_CHIP_POWER_DOWN   (1 << PWD_CTRL2_PWD_CHIP_Pos)

#define SET_PGA_BIAS1_CURRENT_Pos               (0)
#define SET_PGA_BIAS1_CURRENT_Msk               (0x3)
#define SET_PGA_BIAS1_CURRENT_0_5X          (0 << SET_PGA_BIAS1_CURRENT_Pos)
#define SET_PGA_BIAS1_CURRENT_1_0X          (1 << SET_PGA_BIAS1_CURRENT_Pos)
#define SET_PGA_BIAS1_CURRENT_2_0X          (2 << SET_PGA_BIAS1_CURRENT_Pos)
#define SET_PGA_BIAS1_CURRENT_3_0X          (3 << SET_PGA_BIAS1_CURRENT_Pos)

#define SET_ADC_MODULATOR_OPAMP_2_BIAS_Pos          (0)
#define SET_ADC_MODULATOR_OPAMP_2_BIAS_Msk          (0x3)
#define SET_ADC_MODULATOR_OPAMP_2_BIAS_0_5X         (0 << SET_ADC_MODULATOR_OPAMP_2_BIAS_Pos)
#define SET_ADC_MODULATOR_OPAMP_2_BIAS_1_0X         (1 << SET_ADC_MODULATOR_OPAMP_2_BIAS_Pos)
#define SET_ADC_MODULATOR_OPAMP_2_BIAS_2_0X         (2 << SET_ADC_MODULATOR_OPAMP_2_BIAS_Pos)
#define SET_ADC_MODULATOR_OPAMP_2_BIAS_3_0X         (3 << SET_ADC_MODULATOR_OPAMP_2_BIAS_Pos)

#define SET_ADC_MODULATOR_OPAMP_1_BIAS_Pos          (2)
#define SET_ADC_MODULATOR_OPAMP_1_BIAS_Msk          (0xC)
#define SET_ADC_MODULATOR_OPAMP_1_BIAS_0_5X         (0 << SET_ADC_MODULATOR_OPAMP_1_BIAS_Pos)
#define SET_ADC_MODULATOR_OPAMP_1_BIAS_1_0X         (1 << SET_ADC_MODULATOR_OPAMP_1_BIAS_Pos)
#define SET_ADC_MODULATOR_OPAMP_1_BIAS_2_0X         (2 << SET_ADC_MODULATOR_OPAMP_1_BIAS_Pos)
#define SET_ADC_MODULATOR_OPAMP_1_BIAS_3_0X         (3 << SET_ADC_MODULATOR_OPAMP_1_BIAS_Pos)

#define SET_ADC_MODULATOR_WHOLE_BIAS_Pos            (4)
#define SET_ADC_MODULATOR_WHOLE_BIAS_Msk            (0x30)
#define SET_ADC_MODULATOR_WHOLE_BIAS_0_5X           (0 << SET_ADC_MODULATOR_WHOLE_BIAS_Pos)
#define SET_ADC_MODULATOR_WHOLE_BIAS_1_0X           (1 << SET_ADC_MODULATOR_WHOLE_BIAS_Pos)
#define SET_ADC_MODULATOR_WHOLE_BIAS_2_0X           (2 << SET_ADC_MODULATOR_WHOLE_BIAS_Pos)
#define SET_ADC_MODULATOR_WHOLE_BIAS_3_0X           (3 << SET_ADC_MODULATOR_WHOLE_BIAS_Pos)

#define SET_ADC_BUFFER_BIAS_Pos             (6)
#define SET_ADC_BUFFER_BIAS_Msk             (0xC0)
#define SET_ADC_BUFFER_BIAS_0_5X            (0 << SET_ADC_BUFFER_BIAS_Pos)
#define SET_ADC_BUFFER_BIAS_1_0X            (1 << SET_ADC_BUFFER_BIAS_Pos)
#define SET_ADC_BUFFER_BIAS_2_0X            (2 << SET_ADC_BUFFER_BIAS_Pos)
#define SET_ADC_BUFFER_BIAS_3_0X            (3 << SET_ADC_BUFFER_BIAS_Pos)

#define DF1_CTRL_OSR_SEL_Pos            (0)
#define DF1_CTRL_OSR_SEL_Msk            (0xF << DF1_CTRL_OSR_SEL_Pos)
#define DF1_CTRL_OSR_SEL_64             (0x9 << DF1_CTRL_OSR_SEL_Pos)

#define DF1_CTRL_BYPASS_FIR_Pos         (4)
#define DF1_CTRL_BYPASS_FIR_Msk         (1 << DF1_CTRL_BYPASS_FIR_Pos)
#define DF1_CTRL_BYPASS_FIR_DISABLE     (0 << DF1_CTRL_BYPASS_FIR_Pos)
#define DF1_CTRL_BYPASS_FIR_ENABLE      (1 << DF1_CTRL_BYPASS_FIR_Pos)

#define DF1_CTRL_DF_GAIN_Pos            (6)
#define DF1_CTRL_DF_GAIN_Msk            (0x3 << DF1_CTRL_DF_GAIN_Pos)
#define DF1_CTRL_DF_GAIN_1x             (0 << DF1_CTRL_DF_GAIN_Pos)

#define ADC_CTRL_ADC_CLK_SET_Pos        (2)
#define ADC_CTRL_ADC_CLK_SET_Msk        (0x3 << ADC_CTRL_ADC_CLK_SET_Pos)
#define ADC_CTRL_ADC_CLK_SET_1024KHz    (0x3 << ADC_CTRL_ADC_CLK_SET_Pos)

#define ADC_CTRL_SETLSEL_Pos            (5)
#define ADC_CTRL_SETLSEL_Msk            (0x7 << ADC_CTRL_SETLSEL_Pos)
#define ADC_CTRL_SETLSEL_SETTLE_0       (0x0 << ADC_CTRL_SETLSEL_Pos)

#define OSC_CTRL_SEL_INT_OSC_Pos        (7)
#define OSC_CTRL_SEL_INT_OSC_Msk        (1 << OSC_CTRL_SEL_INT_OSC_Pos)
#define OSC_CTRL_SEL_INT_OSC_Int        (1 << OSC_CTRL_SEL_INT_OSC_Pos)

#define OSC_CTRL_OSC_BIAS_Pos           (2)
#define OSC_CTRL_OSC_BIAS_Msk           (0x3 << OSC_CTRL_OSC_BIAS_Pos)
#define OSC_CTRL_OSC_BIAS_1_0X          (1 << OSC_CTRL_OSC_BIAS_Pos)

#define OSC_CTRL_SEL_INT_OSC_F_Pos      (4)
#define OSC_CTRL_SEL_INT_OSC_F_Msk      (0x3 << OSC_CTRL_SEL_INT_OSC_F_Pos)
#define OSC_CTRL_SEL_INT_OSC_F_49152KHz (3 << OSC_CTRL_SEL_INT_OSC_F_Pos)

#define VREF_CTRL_SEL_24V_INT_REF_Pos   (0)
#define VREF_CTRL_SEL_24V_INT_REF_Msk   (1 << VREF_CTRL_SEL_24V_INT_REF_Pos)
#define VREF_CTRL_SEL_24V_INT_REF_2_4V  (1 << VREF_CTRL_SEL_24V_INT_REF_Pos)

#define VREF_CTRL_SEL_INT_REF_DRV_Pos       (6)
#define VREF_CTRL_SEL_INT_REF_DRV_Msk       (0x3 << VREF_CTRL_SEL_INT_REF_DRV_Pos)
#define VREF_CTRL_SEL_INT_REF_HIGHEST       (3 << VREF_CTRL_SEL_INT_REF_DRV_Pos)

#define PGA_CTRL1_SEL_PGA_INP_Pos       (2)
#define PGA_CTRL1_SEL_PGA_INP_Msk       (0x7 << PGA_CTRL1_SEL_PGA_INP_Pos)
#define PGA_CTRL1_SEL_PGA_INN_Pos       (5)
#define PGA_CTRL1_SEL_PGA_INN_Msk       (0x7 << PGA_CTRL1_SEL_PGA_INN_Pos)

#define PGA_CTRL1_SEL_PGA_INP_CH_1      (0x0 << PGA_CTRL1_SEL_PGA_INP_Pos)
#define PGA_CTRL1_SEL_PGA_INP_CH_3      (0x1 << PGA_CTRL1_SEL_PGA_INP_Pos)
#define PGA_CTRL1_SEL_PGA_INP_VCM       (0x7 << PGA_CTRL1_SEL_PGA_INP_Pos)

#define PGA_CTRL1_SEL_PGA_INN_CH_0      (0x0 << PGA_CTRL1_SEL_PGA_INN_Pos)
#define PGA_CTRL1_SEL_PGA_INN_CH_2      (0x1 << PGA_CTRL1_SEL_PGA_INN_Pos)
#define PGA_CTRL1_SEL_PGA_INN_VCM       (0x7 << PGA_CTRL1_SEL_PGA_INN_Pos)

#define PGA_CTRL2_PGA_BUF_GAIN_Msk      (1 << 4)
#define PGA_CTRL2_PGA_BUF_GAIN_1x       (0 << 4)

#define PGA_CTRL2_BUF_BYPASS_Msk        (1 << 5)
#define PGA_CTRL2_BUF_BYPASS_DISABLE    (0 << 5)

#define PGA_CTRL2_BUF_AUTO_EN_Msk       (1 << 6)
#define PGA_CTRL2_BUF_AUTO_DISABLE      (0 << 6)

#define PGA_CTRL2_PGA_GAIN_Msk          (0x7 << 1)
#define PGA_CTRL2_PGA_GAIN_4x           (0x2 << 1)

#define PGA_CTRL2_OFFSETDIS_Msk         (1 << 7)
#define PGA_CTRL2_OFFSETDIS_ENABLE      (0 << 7)

/* ADC register */
#define REG_ADDR_PWD_CTRL1                      0x00
#define REG_ADDR_PWD_CTRL2                      0x01
#define REG_ADDR_DF1_CTRL                       0x02
#define REG_ADDR_DF2_CTRL                       0x03
#define REG_ADDR_ADC_CTRL                       0x04
#define REG_ADDR_BIAS1_CTRL                     0x05
#define REG_ADDR_BIAS2_CTRL                     0x06
#define REG_ADDR_CHOP_CTRL                      0x07
#define REG_ADDR_BG_CTRL1                       0x08
#define REG_ADDR_OSC_CTRL1                      0x09
#define REG_ADDR_OSC_CTRL2                      0x0A
#define REG_ADDR_REF_CTRL                       0x0B
#define REG_ADDR_DAC_DATA1                      0x0C
#define REG_ADDR_DAC_DATA2                      0x0D
#define REG_ADDR_DAC_CTRL                       0x0E
#define REG_ADDR_PGA_CTRL1                      0x0F
#define REG_ADDR_PGA_CTRL2                      0x10
#define REG_ADDR_ADC_OFFSET_CAL1                0x11
#define REG_ADDR_ADC_OFFSET_CAL2                0x12
#define REG_ADDR_ADC_OFFSET_CAL3                0x13
#define REG_ADDR_ADC_TEMP_SENSOR_CAL1           0x14
#define REG_ADDR_ADC_TEMP_SENSOR_CAL2           0x15
#define REG_ADDR_ADC_TEMP_SENSOR_CAL3           0x16
#define REG_ADDR_ADC_STATUS                     0x17
#define REG_ADDR_ADC_OTP_DOUT0                  0x18
#define REG_ADDR_ADC_OTP_DOUT1                  0x19
#define REG_ADDR_ADC_OTP_DOUT2                  0x1A
#define REG_ADDR_ADC_OTP_DOUT3                  0x1B
#define REG_ADDR_ADC_OTP_CTRL                   0x1C
#define REG_ADDR_ADC_OTP_DIN                    0x1D
#define REG_ADDR_MISC_CTRL                      0x1E
#define REG_ADDR_ADC_VER_ID                     0x1F

/* OTP commands */
#define WRITE_OTP1_DOUT0_CMD                    0x10
#define WRITE_OTP1_DOUT1_CMD                    0x11
#define WRITE_OTP1_DOUT2_CMD                    0x12
#define WRITE_OTP1_DOUT3_CMD                    0x13
#define READ_OTP1_DOUT_REG_CMD                  0x20

#define WRITE_OTP2_DOUT0_CMD                    0x50
#define WRITE_OTP2_DOUT1_CMD                    0x51
#define WRITE_OTP2_DOUT2_CMD                    0x52
#define WRITE_OTP2_DOUT3_CMD                    0x53
#define READ_OTP2_DOUT_REG_CMD                  0x60

#define CLEAR_OTP1_DOUT0_WRITE_CMD              0x00
#define CLEAR_OTP1_DOUT1_WRITE_CMD              0x01
#define CLEAR_OTP1_DOUT2_WRITE_CMD              0x02
#define CLEAR_OTP1_DOUT3_WRITE_CMD              0x03
#define CLEAR_OTP1_DOUT_READ_CMD                0x00

#define CLEAR_OTP2_DOUT0_WRITE_CMD              0x40
#define CLEAR_OTP2_DOUT1_WRITE_CMD              0x41
#define CLEAR_OTP2_DOUT2_WRITE_CMD              0x42
#define CLEAR_OTP2_DOUT3_WRITE_CMD              0x43
#define CLEAR_OTP2_DOUT_READ_CMD                0x40

#endif /* __NADC24D_REG_H__ */
