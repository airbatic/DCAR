// ==========================================================================
//
// DCARMessages.h   v1.00   10/19/2025 (RES)
//
// DCAR Communications Protocol Message Structures
//
// Copyright (C) 2025, Pinnacle Technologies
//
// ==========================================================================

#pragma once                /* only includes this header once */
#pragma pack(push, 8)       /* pack structures on 8-bit alignment */

#include <cstdint>

// Default IP port number
#define DCAR_PORT_LAN           27182
#define DCAR_UNIT_ADDRESS       263

// ==========================================================================
// DCAR MESSAGE TYPES
// ==========================================================================

/* Type 12: Send a command to the DCAR (Controller to DCAR) */
#define DCAR_TYPE_COMMAND       12
/* Type 13: Message confirmation and error reporting (DCAR to Controller) */
#define DCAR_TYPE_CONFIRM       13
/* Type 14: Send all DCAR parameters to the DCAR (Controller to DCAR) */
#define DCAR_TYPE_SEND_ALL      14
/* Type 15: Read all DCAR parameters from the DCAR (DCAR to Controller) */
#define DCAR_TYPE_READ_ALL      15

// ==========================================================================
// DCAR COMMAND MESSAGE STRUCTURE (TYPE 12)
// ==========================================================================

typedef struct _DCAR_COMMAND {
    uint16_t    preamble;                   /* preamble value = 0x89FC  */
    uint8_t     type;                       /* the message type (0x0C)  */
    uint16_t    address;                    /* destination address      */
    uint8_t     command;                    /* the message command      */
} DCAR_COMMAND;

/* Command Function to Execute (DCAR_COMMAND.command) */
#define DCAR_CMD_PING           0x00        /* Ping the DCAR      (T13) */
#define DCAR_CMD_RXMODE         0x01        /* Receive Mode       (T13) */
#define DCAR_CMD_TXMODE         0x02        /* Transmit Mode      (T13) */
#define DCAR_CMD_SAFEMODE       0x03        /* Safe Mode          (T13) */
#define DCAR_CMD_NULLOFFSET     0x04        /* Null DC Offset     (T13) */
#define DCAR_CMD_ALARMRESET     0x05        /* Alarm Reset        (T13) */
#define DCAR_CMD_ALARMSILENCE   0x06        /* Alarm Silence      (T13) */
#define DCAR_CMD_STATUS         0x07        /* Get Status         (T15) */

// ==========================================================================
// DCAR CONFIRM MESSAGE STRUCTURE (TYPE 13)
// ==========================================================================

typedef struct _DCAR_CONFIRM {
    uint16_t    preamble;                   /* preamble value = 0x89FC  */
    uint8_t     type;                       /* the message type (0x0D)  */
    uint16_t    address;                    /* source address           */
    uint8_t     response;                   /* the returned response    */
} DCAR_CONFIRM;

/* Response Status Codes (DCAR_CONFIRM.response) */
#define DCAR_RESPONSE_RECEIVED  0x00        /* Message Received         */
#define DCAR_RESPONSE_RANGE     0x01        /* Message Received         */
#define DCAR_RESPONSE_UNKNOWN   0x02        /* Unknown Cmd or Request   */

// ==========================================================================
// DCAR SEND ALL MESSAGE STRUCTURE (TYPE 14)
// ==========================================================================

typedef struct _DCAR_SEND_ALL {
    uint16_t    preamble;                   /* preamble value = 0x89FC  */
    uint8_t     type;                       /* the message type (0x0E)  */
    uint16_t    address;                    /* destination address      */
    uint8_t     protocol_flags;             /*                          */
    /* Channel-1 */
    int8_t      att_rx_ch1;                 /* Attn dB, -10 to +70 dB   */
    int8_t      att_tx_ch1;                 /* Attn dB, -10 to +70 dB   */
    uint8_t     lpf_cutoff_ch1;             /* 0-73: 5-72 MHz, 0-4 & 73 */
    uint8_t     freq_band_ch1;              /* Frequency Band – CH1     */
    /* Channel-2 */
    int8_t      att_rx_ch2;                 /* Attn dB, -10 to +70 dB   */
    int8_t      att_tx_ch2;                 /* Attn dB, -10 to +70 dB   */
    uint8_t     lpf_cutoff_ch2;             /* 0-73: 5-72 MHz, 0-4 & 73 */
    uint8_t     freq_band_ch2;              /* Frequency Band – CH1     */
    /* Global settings */
    uint8_t     mode;                       /* RX=0, TX=1, Safe = 2,    */
    uint8_t     bitvalues;                  /* individual bit values    */
    uint8_t     reserved1;                  /* reservered byte-1        */
} DCAR_SEND_ALL;

// ==========================================================================
// DCAR READ ALL MESSAGE STRUCTURE (TYPE 15)
// ==========================================================================

typedef struct _DCAR_READ_ALL {
    uint16_t    preamble;                   /* preamble value = 0x89FC  */
    uint8_t     type;                       /* the message type (0x0E)  */
    uint16_t    address;                    /* destination address      */
    uint8_t     mode;                       /* RX=0, TX=1, Safe=2       */
    uint8_t     last_set_by;                /*                          */
    uint16_t    alarm_red;                  /* Alarm (RED) states       */
    uint16_t    alarm_yellow;               /* Alarm (YEL) states       */
    /* Channel-1 Status */
    int8_t      att_rx_ch1;                 /* Attn dB, -10 to +70 dB   */
    int8_t      att_tx_ch1;                 /* Attn dB, -10 to +70 dB   */
    uint8_t     lpf_cutoff_ch1;             /* 0-73: 5-72 MHz, 0-4 & 73 */
    uint8_t     freq_band_ch1;              /* freqy band-ch1 (1-10)    */
    int16_t     rf_input_power_ch1;         /* RF In Power-CH1 in dBm   */
    int16_t     lo_input_power_ch1;         /* LO In Power-CH1 in dBm   */
    int16_t     I_output_power_ch1;         /* I Output Power-CH1 dBm   */
    int16_t     Q_output_power_ch1;         /* Q Output Power-CH1 dBm   */
    int16_t     I_output_dc_offset_ch1;     /* I Output DC Offset-CH1   */
    int16_t     Q_output_dc_offset_ch1;     /* Q Output DC Offset-CH1   */
    int8_t      temp_ch1;                   /* temp-CH1 1 Receiver      */
    uint8_t     bit_values_ch1;             /* Bit Values-CH1           */
    uint8_t     receiver_ver_ch1;           /* CH-1 firmware version    */
    uint16_t    serial_num_ch1;             /* CH-1 receiver serial#    */
    /* Channel-2 Status */
    int8_t      att_rx_ch2;                 /* Attn dB, -10 to +70 dB   */
    int8_t      att_tx_ch2;                 /* Attn dB, -10 to +70 dB   */
    uint8_t     lpf_cutoff_ch2;             /* 0-73: 5-72 MHz, 0-4 & 73 */
    uint8_t     freq_band_ch2;              /* freqy band-ch1 (1-10)    */
    int16_t     rf_input_power_ch2;         /* RF In Power-CH1 in dBm   */
    int16_t     lo_input_power_ch2;         /* LO In Power-CH1 in dBm   */
    int16_t     I_output_power_ch2;         /* I Output Power-CH1 dBm   */
    int16_t     Q_output_power_ch2;         /* Q Output Power-CH1 dBm   */
    int16_t     I_output_dc_offset_ch2;     /* I Output DC Offset-CH2   */
    int16_t     Q_output_dc_offset_ch2;     /* Q Output DC Offset-CH2   */
    int8_t      temp_ch2;                   /* temp-CH1 2 Receiver      */
    uint8_t     bit_values_ch2;             /* Bit Values-CH1 2         */
    uint8_t     receiver_ver_ch2;           /* CH-2 firmware version    */
    uint16_t    serial_num_ch2;             /* CH-2 receiver serial#    */
    /* General Status & Info */
    uint16_t    power_pos_12V;              /* Power Supply +12V        */
    uint16_t    power_neg_12V;              /* Power Supply -12V        */
    int8_t      power_supply_temp;          /* PSU temp 0.5°C/Count     */
    uint8_t     bit_values;                 /* Individual Bit Values    */
    uint8_t     version;                    /* Panel Firmware Version   */
    uint8_t     serial_num;                 /* DCAR Serial Number       */
} DCAR_READ_ALL;

/* DCAR Current Mode (DCAR_READ_ALL.mode) */
#define DCAR_MODE_RX            0
#define DCAR_MODE_TX            1
#define DCAR_MODE_SAFE          2

/* DCAR Last Mode Set by (DCAR_READ_ALL.last_set_by) */
#define DCAR_SETBY_PANEL        0
#define DCAR_SETBY_REMOTE       1
#define DCAR_SETBY_AUX          2
#define DCAR_SETBY_ALARM        3

/* DCAR RED Alarm Bit Flags (DCAR_READ_ALL.alarm_red) */
#define DCAR_B_RED_CH1_OVL      (1 << 0)    /* Ch-1 Overload */
#define DCAR_B_RED_CH2_OVL      (1 << 1)    /* Ch-1 Overload */
#define DCAR_B_RED_CH1_LO_LVL   (1 << 2)    /* Ch-1 LO Level */
#define DCAR_B_RED_CH2_LO_LVL   (1 << 3)    /* Ch-2 LO Level */
#define DCAR_B_RED_CH1_RX_FAIL  (1 << 4)    /* Ch-1 Receiver Failure */
#define DCAR_B_RED_CH2_RX_FAIL  (1 << 5)    /* Ch-2 Receiver Failure */
#define DCAR_B_RED_POS_PWR_ERR  (1 << 6)    /* Pos Supply Failure */
#define DCAR_B_RED_NEG_PWR_ERR  (1 << 7)    /* Neg Supply Failure */
#define DCAR_B_RED_OVER_TEMP    (1 << 8)    /* Over-Temperature */

/* DCAR YEL Alarm Bit Flags (DCAR_READ_ALL.alarm_yel) */
#define DCAR_B_YEL_CH1_OVL      (1 << 0)    /* Ch-1 Overload */
#define DCAR_B_YEL_CH2_OVL      (1 << 1)    /* Ch-2 Overload */
#define DCAR_B_YEL_CH1_LO_LVL   (1 << 2)    /* Ch-1 LO Level */
#define DCAR_B_YEL_CH2_LO_LVL   (1 << 3)    /* Ch-2 LO Level */
#define DCAR_B_YEL_CH1_RX_FAIL  (1 << 4)    /* Ch-1 Receiver Failure */
#define DCAR_B_YEL_CH2_RX_FAIL  (1 << 5)    /* Ch-2 Receiver Failure */
#define DCAR_B_YEL_POS_PWR_ERR  (1 << 6)    /* Pos Supply Failure */
#define DCAR_B_YEL_NEG_PWR_ERR  (1 << 7)    /* Neg Supply Failure */
#define DCAR_B_YEL_OVER_TEMP    (1 << 8)    /* Over-Temperature */

/* DCAR Low-Pass Filter Cutoff (DCAR_READ_ALL.lpf_cutoff_ch1) */
/* DCAR Low-Pass Filter Cutoff (DCAR_READ_ALL.lpf_cutoff_ch2) */
#define DCAR_LPF_156_25_KHZ     0           /* 156.25 KHz */
#define DCAR_LPF_312_5_KHZ      1           /* 312.5 KHz */
#define DCAR_LPF_625_KHZ        2           /* 625 KHz */
#define DCAR_LPF_1_25_MHZ       3           /* 1.25 MHz */
#define DCAR_LPF_2_5_MHZ        4           /* 2.5 MHz */
#define DCAR_LPF_RANGE_MIN_MHZ  5           /* 5-72 Cutoff in MHz */
#define DCAR_LPF_RANGE_MAX_MHZ  72          /* 5-72 Cutoff in MHz */
#define DCAR_LPF_BYPASS         73          /* LPF Bypassed (~100 MHz) */

/* DCAR Operating Frequency Band (DCAR_READ_ALL.freq_band_ch1) */
/* DCAR Operating Frequency Band (DCAR_READ_ALL.freq_band_ch2) */
#define DCAR_BAND_100_150       0           /* Band 100-150 MHz */
#define DCAR_BAND_150_250       1           /* Band 150-250 MHz */
#define DCAR_BAND_250_350       3           /* Band 250-350 MHz */
#define DCAR_BAND_350_450       4           /* Band 350-450 MHz */
#define DCAR_BAND_450_550       5           /* Band 450-550 MHz */
#define DCAR_BAND_550_650       6           /* Band 550-650 MHz */
#define DCAR_BAND_650_750       7           /* Band 650-750 MHz */
#define DCAR_BAND_750_850       8           /* Band 750-850 MHz */
#define DCAR_BAND_850_950       9           /* Band 850-950 MHz */

/* DCAR Channel Bit Flags (DCAR_READ_ALL.bit_values_ch1) */
/* DCAR Channel Bit Flags (DCAR_READ_ALL.bit_values_ch2) */
#define DCAR_B_AC_COUPLE        (1 << 0)    /* bit-0 set for AC */

/* DCAR Audio Alarm Sounding (DCAR_READ_ALL.bit_values) */
#define DCAR_B_ALARM            (1 << 0)    /* alarm sounding */

// ==========================================================================
// Some Useful Constants and Macros
// ==========================================================================

#define DCAR_PHANTOM_BYTE       0x80        /* for CRC calculation */

#define DCAR_CRC_SEED           0xFFFF

#define DCAR_CCITT_POLY         0x1021

#define DCAR_PREAMBLE           0x89FC      /* message packet preamble */

#define DCAR_PREAMBLE_MSB       ((uint8_t)(DCAR_PREAMBLE >> 8))
#define DCAR_PREAMBLE_LSB       ((uint8_t)(DCAR_PREAMBLE & 0xFF))

/* Min/Max RX attenuation dBm
 * The receiver’s relative attenuation (the amount the gain is reduced
 * from its maximum value) while in receive mode. Each count is 1 dB.
 * The range is 0 dB to 70 dB for cutoffs greater than or equal to 5 MHz.
 * The range is -10 dB to 70 dB for cutoffs less than 5 MHz.
 * Negative values correspond to gain.
 */
#define DCAR_ATT_CUTOFF_GTE_5MHZ_MIN_F  (0.0f)      /* cutoff >= 5 MHz */
#define DCAR_ATT_CUTOFF_GTE_5MHZ_MAX_F  (+70.0f)

#define DCAR_ATT_CUTOFF_LT_5MHZ_MIN_F   (-10.0f)    /* cutoff < 5 MHz  */
#define DCAR_ATT_CUTOFF_LT_5MHZ_MAX_F   (+70.0f)

/* Low power error indication (unsigned) */
#define DCAR_POWER_LOW                  0x7FFF

/* Power scaled as 0.1 dBm per step (float) */
#define DCAR_POWER_DBM_F(p)             (((float)p * 0.1f))

/* Temperature, 0.5°C/count (float) */
#define DCAR_TEMP_C_F(p)                (((float)p * 0.5f))

/* DC Offset 0.1 mV per count (float) */
#define DCAR_VOFFSET_F(v)               (((float)v * 0.1f))

/* Supply Voltage 0.1 mV per count (float) */
#define DCAR_VSUPPLY_F(v)               (((float)v * 0.1f))

/*
 * restore previous structure packing
 */
#pragma pack(pop)

/* End-Of-File */
