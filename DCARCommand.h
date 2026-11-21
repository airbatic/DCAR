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
    void SetAddress(wxString address, u_short portnum=DCAR_PORT_LAN);
    void SetUnitAddress(uint16_t addr=DCAR_UNIT_ADDRESS);
    bool IsOpen() { return m_open; }

// Operations
public:
    int Initialize();
    int Destroy();
    // commands to a DCAR
    int Ping();
    int SetModeReceive();
    int SetModeTransmit();
    int SetModeSafe();
    int SetDCNullOffset();
    int SetAlarmReset();
    int SetAlarmSilence();
    int GetAllStatus(DCAR_READ_ALL* data);
    int SetAllStatus(DCAR_SEND_ALL* data);

    // Helpers
    char* dBm2rms(int16_t dBm_x_10, char *out, size_t n);

// Implementation
private:
    int DoFrameTx(void* txtbuf, uint16_t txtlen);
    int DoFrameRx(void* txtbuf, uint16_t txtlen);
    int DoCommand(uint8_t command);

    uint16_t CRC16Update(uint16_t crc, uint8_t d);

// Overrides
protected:

// Implementation
public:
    bool                m_open;     // port open flag
    uint16_t            m_uaddr;    // DCAR unit address
    wxIPV4address       m_addr;     // IP4 address
    wxDatagramSocket*   m_sock;     // datagram socket handle

	virtual ~DCARCommand();
};

// End-Of-File
