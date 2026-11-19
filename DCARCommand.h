// ==========================================================================
//
// DCARCommand.h   v1.00   10/19/2025 (RES)
//
// DCAR Communications Implementation Class
//
// Copyright (C) 2025, Pinnacle Technologies
//
// ==========================================================================

#pragma once

#include "DCARMessage.h"

#define DCAR_ERR_SUCCESS         0
#define DCAR_ERR_TIMEOUT         1           /* comm port timeout     */
#define DCAR_ERR_SYNC            2           /* SOF frame sync error  */
#define DCAR_ERR_SHORT_FRAME     3           /* short rx-frame error  */
#define DCAR_ERR_RX_OVERFLOW     4           /* rx buffer overflow    */
#define DCAR_ERR_FRAME_LEN       5           /* bad rx-frame length   */
#define DCAR_ERR_TEXT_LEN        6           /* bad rx-text length    */
#define DCAR_ERR_CONFIRM         7           /* bad response message  */
#define DCAR_ERR_CRC             8           /* rx-frame checksum bad */

// ==========================================================================
// DCAR Command I/O Class
// ==========================================================================

class DCARCommand
{
// Construction
public:
	DCARCommand();

// Attributes
public:
    uint16_t GetAddress(void);
    void SetAddress(uint16_t address);

// Operations
public:
    int Ping(int handle);
    int SetModeReceive(int handle);
    int SetModeTransmit(int handle);
    int SetModeSafe(int handle);
    int SetDCNullOffset(int handle);
    int SetAlarmReset(int handle);
    int SetAlarmSilence(int handle);
    int GetAllStatus(int handle, DCAR_READ_ALL* data);
    int SetAllStatus(int handle, DCAR_SEND_ALL* data);

// Implementation
private:
    int DoFrameTx(int handle, void* txtbuf, uint16_t txtlen);
    int DoFrameRx(int handle, void* txtbuf, uint16_t txtlen);
    int DoCommand(int handle, uint8_t command);

    uint16_t CRC16Update(uint16_t crc, uint8_t d);

// Overrides
protected:

// Implementation
public:
    uint16_t    m_address;      /* 16-bit DCAR address */

	virtual ~DCARCommand();
};

// End-Of-File
