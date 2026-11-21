
// ==========================================================================
//
// DCARCommand.cpp  v1.00   10/19/2025 (RES)
//
// DCAR Communications Implementation Class
//
// Copyright (C) 2025, Pinnacle Technologies
//
// ==========================================================================

#include "DCARApp.h"
#include "DCARCommand.h"

#define T15_UNDERFLOW       32767
#define T15_UNDERFLOW_DSP   "----"

// ===========================================================================
// DCARCommand Construction/Destruction
// ===========================================================================

DCARCommand::DCARCommand()
{
    m_open = false;

    // Set the DCAR physical unit address
    m_uaddr = DCAR_UNIT_ADDRESS;

    // Setup the IP address
    m_addr.AnyAddress();
    m_addr.Service(DCAR_PORT_LAN);

    m_sock = nullptr;
}

DCARCommand::~DCARCommand()
{
    Destroy();
}


int DCARCommand::Initialize()
{
    if (m_sock != nullptr)
        return -1;

    if (m_open)
        return -1;

    // Setup the IP address
    m_addr.AnyAddress();
    m_addr.Service(DCAR_PORT_LAN);

    //create and bind to the address above
    m_sock = new wxDatagramSocket(m_addr);

    //enable event handling
    //m_sock->SetEventHandler(*this,SOCKET_ID);

    //Notify us about incomming data
    //m_dsock->SetNotify(wxSOCKET_INPUT_FLAG);

    //enable event handling
    //m_dsock->Notify(true);

    m_open = true;

    return 0;
}

int DCARCommand::Destroy()
{
    int rc = 0;

    if (m_sock != nullptr)
    {
        delete m_sock;
        m_sock = nullptr;
        rc = 1;
    }

    m_open = false;

    return rc;
}


// ===========================================================================
// DCARCommand Attributes
// ===========================================================================

void DCARCommand::SetAddress(wxString address, u_short portnum)
{
    //in my case my local IP, could have used 127.0.0.1 as well
    m_addr.Hostname(address);

    //the port to send it to
    m_addr.Service(portnum);
}

void DCARCommand::SetUnitAddress(uint16_t addr)
{
    m_uaddr = addr;
}

// ===========================================================================
// Message Send/Receive Helpers
// ===========================================================================

#if 0
uint16_t DCARCommand::CRC16Update(uint16_t crc, uint8_t d)
{
    static const uint16_t table[256] = {
         0x0000, 0x1189, 0x2312, 0x329B, 0x4624, 0x57AD, 0x6536, 0x74BF,
         0x8C48, 0x9DC1, 0xAF5A, 0xBED3, 0xCA6C, 0xDBE5, 0xE97E, 0xF8F7,
         0x1081, 0x0108, 0x3393, 0x221A, 0x56A5, 0x472C, 0x75B7, 0x643E,
         0x9CC9, 0x8D40, 0xBFDB, 0xAE52, 0xDAED, 0xCB64, 0xF9FF, 0xE876,
         0x2102, 0x308B, 0x0210, 0x1399, 0x6726, 0x76AF, 0x4434, 0x55BD,
         0xAD4A, 0xBCC3, 0x8E58, 0x9FD1, 0xEB6E, 0xFAE7, 0xC87C, 0xD9F5,
         0x3183, 0x200A, 0x1291, 0x0318, 0x77A7, 0x662E, 0x54B5, 0x453C,
         0xBDCB, 0xAC42, 0x9ED9, 0x8F50, 0xFBEF, 0xEA66, 0xD8FD, 0xC974,
         0x4204, 0x538D, 0x6116, 0x709F, 0x0420, 0x15A9, 0x2732, 0x36BB,
         0xCE4C, 0xDFC5, 0xED5E, 0xFCD7, 0x8868, 0x99E1, 0xAB7A, 0xBAF3,
         0x5285, 0x430C, 0x7197, 0x601E, 0x14A1, 0x0528, 0x37B3, 0x263A,
         0xDECD, 0xCF44, 0xFDDF, 0xEC56, 0x98E9, 0x8960, 0xBBFB, 0xAA72,
         0x6306, 0x728F, 0x4014, 0x519D, 0x2522, 0x34AB, 0x0630, 0x17B9,
         0xEF4E, 0xFEC7, 0xCC5C, 0xDDD5, 0xA96A, 0xB8E3, 0x8A78, 0x9BF1,
         0x7387, 0x620E, 0x5095, 0x411C, 0x35A3, 0x242A, 0x16B1, 0x0738,
         0xFFCF, 0xEE46, 0xDCDD, 0xCD54, 0xB9EB, 0xA862, 0x9AF9, 0x8B70,
         0x8408, 0x9581, 0xA71A, 0xB693, 0xC22C, 0xD3A5, 0xE13E, 0xF0B7,
         0x0840, 0x19C9, 0x2B52, 0x3ADB, 0x4E64, 0x5FED, 0x6D76, 0x7CFF,
         0x9489, 0x8500, 0xB79B, 0xA612, 0xD2AD, 0xC324, 0xF1BF, 0xE036,
         0x18C1, 0x0948, 0x3BD3, 0x2A5A, 0x5EE5, 0x4F6C, 0x7DF7, 0x6C7E,
         0xA50A, 0xB483, 0x8618, 0x9791, 0xE32E, 0xF2A7, 0xC03C, 0xD1B5,
         0x2942, 0x38CB, 0x0A50, 0x1BD9, 0x6F66, 0x7EEF, 0x4C74, 0x5DFD,
         0xB58B, 0xA402, 0x9699, 0x8710, 0xF3AF, 0xE226, 0xD0BD, 0xC134,
         0x39C3, 0x284A, 0x1AD1, 0x0B58, 0x7FE7, 0x6E6E, 0x5CF5, 0x4D7C,
         0xC60C, 0xD785, 0xE51E, 0xF497, 0x8028, 0x91A1, 0xA33A, 0xB2B3,
         0x4A44, 0x5BCD, 0x6956, 0x78DF, 0x0C60, 0x1DE9, 0x2F72, 0x3EFB,
         0xD68D, 0xC704, 0xF59F, 0xE416, 0x90A9, 0x8120, 0xB3BB, 0xA232,
         0x5AC5, 0x4B4C, 0x79D7, 0x685E, 0x1CE1, 0x0D68, 0x3FF3, 0x2E7A,
         0xE70E, 0xF687, 0xC41C, 0xD595, 0xA12A, 0xB0A3, 0x8238, 0x93B1,
         0x6B46, 0x7ACF, 0x4854, 0x59DD, 0x2D62, 0x3CEB, 0x0E70, 0x1FF9,
         0xF78F, 0xE606, 0xD49D, 0xC514, 0xB1AB, 0xA022, 0x92B9, 0x8330,
         0x7BC7, 0x6A4E, 0x58D5, 0x495C, 0x3DE3, 0x2C6A, 0x1EF1, 0x0F78
    };

    return table[d ^ (uint8_t)(crc >> (16 - 8))] ^ (crc << 8);
}
#endif // 0


uint16_t DCARCommand::CRC16Update(uint16_t crc, uint8_t d)
{
    int32_t bit;

    for (bit = 1; bit <= 8; bit++)
    {
        if (crc & 0x8000)
        {                                       /* a 1 will shift off          */
            crc = (uint16_t)(crc << 1);

            if (d & 0x80)
                crc |= 1;                       /* message bit shifts in       */

            crc ^= (uint16_t)DCAR_CCITT_POLY;
        }
        else
        {
            crc = (uint16_t)(crc << 1);

            if (d & 0x80)
                crc |= 1;
        }

        d = (uint8_t)(d << 1);
    }

    return crc;
}

int DCARCommand::DoFrameTx(void* txtbuf, uint16_t txtlen)
{
    int rc = DCAR_ERR_SUCCESS;
    uint8_t  b;
    uint16_t i;
    uint16_t crc;

    uint8_t* textbuf = (uint8_t*)txtbuf;
    uint16_t textlen = txtlen;

    /* Send the preamble MSB for the frame start */
    b = DCAR_PREAMBLE_MSB;
    m_sock->SendTo(m_addr, &b, 1);

    /* Send the preamble LSB for the frame start */
    b = DCAR_PREAMBLE_LSB;
    m_sock->SendTo(m_addr, &b, 1);

    /* CRC starts here, sum in the seed byte first */
    crc = CRC16Update(DCAR_CRC_SEED, DCAR_PHANTOM_BYTE);

    /* Send message data */

    for (i=0; i < textlen; i++)
    {
        b = *textbuf++;
        crc = CRC16Update(crc, b);
        m_sock->SendTo(m_addr, &b, 1);
    }

    /* Send the CRC MSB */
    b = (uint8_t)(crc >> 8);
    m_sock->SendTo(m_addr, &b, 1);

    /* Send the CRC LSB */
    b = (uint8_t)(crc & 0xFF);
    m_sock->SendTo(m_addr, &b, 1);

    return rc;
}

/*
 * Receive a DCAR message of specified length
 */

int DCARCommand::DoFrameRx(void* txtbuf, uint16_t txtlen)
{
    int i;
    int rc = DCAR_ERR_SUCCESS;
    uint8_t b;
    uint16_t lsb;
    uint16_t msb;
    uint16_t rxcrc;
    uint16_t crc = 0;

    uint8_t *textbuf = (uint8_t*)txtbuf;
    uint16_t textlen = txtlen;

    /* First, try to synchronize to 0x79 SOF byte */

    i = 0;

    do {

        /* Read the preamble MSB for the frame start */
        if (m_sock->RecvFrom(m_addr, &b, 1).Error())
            return DCAR_ERR_TIMEOUT;

        /* Garbage flood, no preamble?? */
        if (i++ > 128)
            return DCAR_ERR_RX_OVERFLOW;

    } while (b != DCAR_PREAMBLE_MSB);

    /* We found the first 0x89 sequence, next byte
     * has to be 0xFC for a valid SOF sequence.
     */

    if (m_sock->RecvFrom(m_addr, &b, 1).Error())
        return DCAR_ERR_TIMEOUT;

    if (b != DCAR_PREAMBLE_LSB)
        return DCAR_ERR_SYNC;

    /* We got a full preamble word */

    /* CRC starts here, sum in the seed byte first */
    crc = CRC16Update(crc, DCAR_PHANTOM_BYTE);

    /* Read the message length less two bytes for CRC */
    textlen = textlen - 2;

    for (i=0; i < textlen; i++)
    {
        if (m_sock->RecvFrom(m_addr, &b, 1).Error())
            return DCAR_ERR_SHORT_FRAME;

        /* update the CRC */
        crc = CRC16Update(crc, b);

        /* If we overflow, continue reading the packet
         * data, but don't store the data into the buffer.
         */
        if (i >= textlen)
        {
            rc = DCAR_ERR_RX_OVERFLOW;
            continue;
        }

        if (textbuf)
            *textbuf++ = b;
    }

    /* Read the packet CRC MSB */
    if (m_sock->RecvFrom(m_addr, &b, 1).Error())
        return DCAR_ERR_SHORT_FRAME;

    msb = (uint16_t)b & 0xFF;

    /* Read the packet CRC LSB */
    if (m_sock->RecvFrom(m_addr, &b, 1).Error())
        return DCAR_ERR_TIMEOUT;

    if (b != DCAR_PREAMBLE_LSB)
        rc = DCAR_ERR_CRC;

    lsb = (uint16_t)b & 0xFF;

    /* Build and validate the CRC */
    rxcrc = (uint16_t)((msb << 8) | lsb) & 0xFFFF;

    /* Validate the CRC values match */
    if (rxcrc != crc)
        rc = DCAR_ERR_CRC;

    return rc;
}

/*
 * Send a DCAR Command Message
 */

int DCARCommand::DoCommand(uint8_t command)
{
    int rc;
    DCAR_COMMAND message;
    DCAR_CONFIRM reply;

    message.preamble = DCAR_PREAMBLE;
    message.type     = DCAR_TYPE_COMMAND;
    message.address  = m_uaddr;
    message.command  = command;

    /* Send the DCAR command request */

	rc = DoFrameTx(&message, sizeof(DCAR_COMMAND));

    if (rc != DCAR_ERR_SUCCESS)
        return rc;

    /* Attempt to read the DCAR reply */

    memset(&reply, 0, sizeof(reply));

 	rc = DoFrameRx(&reply, sizeof(DCAR_CONFIRM));

	return rc;
}

/*------------------------------------------------------------------------------
 * dBm2rms -- dBm*10 to mVrms (50 ohm), returned as an integer string
 *   mVrms = 1000 * sqrt(0.05 * 10^(dBm/10))
 *----------------------------------------------------------------------------*/

char* DCARCommand::dBm2rms(int16_t dBm_x_10, char *out, size_t n)
{
    float dbm, mv_rms;

    if (dBm_x_10 == T15_UNDERFLOW)
    {
        snprintf(out, n, "%s", T15_UNDERFLOW_DSP);
        return out;
    }

    dbm = (float)dBm_x_10;

    dbm = dbm / 10;

    mv_rms = (float)(1000 * sqrt(0.05 * pow(10, (dbm / 10))));

    snprintf(out, n, "%ld", lround((double)mv_rms));

    return out;
}

// ===========================================================================
// DCAR Command Implementation Methods
// ===========================================================================

/*
 * Ping a DCAR
 */

int DCARCommand::Ping()
{
    return DoCommand(DCAR_CMD_PING);
}

/*
 * Set DCAR to Receive Mode
 */

int DCARCommand::SetModeReceive()
{
    return DoCommand(DCAR_CMD_RXMODE);
}

/*
 * Set DCAR to Transmit Mode
 */

int DCARCommand::SetModeTransmit()
{
    return DoCommand(DCAR_CMD_TXMODE);
}

/*
 * Set DCAR to Safe Mode
 */

int DCARCommand::SetModeSafe()
{
    return DoCommand(DCAR_CMD_SAFEMODE);
}

/*
 * Reset DCAR DC Null Offsets
 */

int DCARCommand::SetDCNullOffset()
{
    return DoCommand(DCAR_CMD_NULLOFFSET);
}

/*
 * Reset DCAR Alarm State
 */

int DCARCommand::SetAlarmReset()
{
    return DoCommand(DCAR_CMD_ALARMRESET);
}

/*
 * Set DCAR Alarm to Silence
 */

int DCARCommand::SetAlarmSilence()
{
    return DoCommand(DCAR_CMD_ALARMSILENCE);
}

// ===========================================================================
// DCAR Read/Write All Parameters Implementation Methods
// ===========================================================================

/*
 * Read all DCAR Settings Data
 */

int DCARCommand::GetAllStatus(DCAR_READ_ALL* data)
{
    int rc = DCAR_ERR_SUCCESS;
#if 0
    uint16_t crc;
    DCAR_COMMAND message;

    message.preamble = DCAR_PREAMBLE;
    message.type     = DCAR_TYPE_READ_ALL;
    message.address  = GetAddress();
    message.command  = 0;

    /* Send the DCAR command request */

	rc = DoFrameTx(handle, &message, sizeof(DCAR_COMMAND));

    if (rc != DCAR_ERR_SUCCESS)
        return rc;

    /* Attempt to read the DCAR reply */

    memset(data, 0, sizeof(DCAR_READ_ALL));

 	rc = DoFrameRx(handle, data, sizeof(DCAR_READ_ALL));
#endif
	return rc;
}

/*
 * Write all DCAR Settings Data
 */

int DCARCommand::SetAllStatus(DCAR_SEND_ALL* data)
{
    int rc = DCAR_ERR_SUCCESS;
#if 0
    uint16_t crc;
    DCAR_COMMAND message;

    message.preamble = DCAR_PREAMBLE;
    message.type     = DCAR_TYPE_SEND_ALL;
    message.address  = GetAddress();
    message.command  = 0;

    /* Send the DCAR command request */

	rc = DoFrameTx(handle, &message, sizeof(DCAR_COMMAND));

    if (rc != DCAR_ERR_SUCCESS)
        return rc;

    /* Attempt to read the DCAR reply */

    memset(status, 0, sizeof(DCAR_READ_ALL));

 	rc = DoFrameRx(handle, parms, sizeof(DCAR_READ_ALL));
#endif
	return rc;
}

// End-Of-File
