#ifndef __NADC24D_CMD_H__
#define __NADC24D_CMD_H__

#define START_BYTE                          0x49
#define STOP_BYTE                           0x19
#define CHIP_ID_NUM                         0x2D
#define FW_VERSION                          0x01

#define SETTING_FINISH                      0xAA
#define CHECKSUM_FAILED                     0xBB
#define COMMAND_FAILED                      0xCC
#define FREQ_NOT_50HZ_60HZ                  0xDD

#define BYTE_X1                             0x01
#define BYTE_X4                             0x04
#define BYTE_X8                             0x08
#define BYTE_X25                            0x19

#define READ_CHIP_ID                        0x11
#define READ_FW_VERSION                     0x21

#define WRITE_1_CALI_AC_VOL                 0x30
#define WRITE_1_CALI_AC_CUR                 0x31
#define START_AC_CALI_1_POI                 0x32
#define SAVE_AC_CALI_1_POI                  0x33

#define WRITE_2_CALI_AC_VOL                 0x34
#define WRITE_2_CALI_AC_CUR                 0x35
#define START_AC_CALI_2_POI                 0x36
#define SAVE_AC_CALI_2_POI                  0x37

#define WRITE_1_CALI_DC_VOL                 0x40
#define WRITE_1_CALI_DC_CUR                 0x41
#define START_DC_CALI_1_POI                 0x42
#define SAVE_DC_CALI_1_POI                  0x43

#define WRITE_2_CALI_DC_VOL                 0x44
#define WRITE_2_CALI_DC_CUR                 0x45
#define START_DC_CALI_2_POI                 0x46
#define SAVE_DC_CALI_2_POI                  0x47

#define READ_1_CALI_AC_VOL                  0x50
#define READ_1_CALI_AC_CUR                  0x51
#define READ_2_CALI_AC_VOL                  0x52
#define READ_2_CALI_AC_CUR                  0x53
#define READ_V_RMS_GAIN                     0x54
#define READ_I_RMS_GAIN                     0x55

#define READ_1_CALI_DC_VOL                  0x60
#define READ_1_CALI_DC_CUR                  0x61
#define READ_2_CALI_DC_VOL                  0x62
#define READ_2_CALI_DC_CUR                  0x63
#define READ_V_DC_GAIN                      0x64
#define READ_I_DC_GAIN                      0x65

#define START_MEASURING                     0x70
#define STOP_MEASURING                      0x71

#define HARMONIC_VRMS_40TH                  0x80
#define HARMONIC_IRMS_40TH                  0x90

#endif /* __NADC24D_CMD_H__ */
