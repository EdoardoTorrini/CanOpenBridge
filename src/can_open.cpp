#include "../include/can_open.hpp"

CanOpen::CanOpen(char* sInterface, int nNodeID)
{
    this->m_socket = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (this->m_socket < 0)
        throw CanOpenException(SOCKET_ERR, "Error on socket define");

    strcpy(this->m_ifr.ifr_name, sInterface);
    ioctl(this->m_socket, SIOCGIFINDEX, &this->m_ifr);

    memset(&this->m_addr, 0, sizeof(this->m_addr));
    this->m_addr.can_family = AF_CAN;
    this->m_addr.can_ifindex = this->m_ifr.ifr_ifindex;

    if (bind(this->m_socket, (struct sockaddr *)&this->m_addr, sizeof(this->m_addr)) < 0)
        throw CanOpenException(BINDING_ERR, "Error on socket association");

    this->m_nNodeID = nNodeID;
    this->init();
}

// ------------------------------------- PRIVATE METHODS --------------------------------------------------

void CanOpen::init()
{
    int nRet = 0;
    can_frame frame;
    
    frame.can_id = 0x0;
    frame.can_dlc = 2;
    frame.data[0] = 0x1;
    frame.data[1] = 0x1;
    
    if (write(this->m_socket, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame))
        throw CanOpenException(WRITE_ON_SCK_ERR, "Error on Init Communication");
}

int CanOpen::write_data(struct can_open_frame co_frame)
{
    /**
     * @param co_frame: is the CanOpenFrame struct to send on the CanBus Socket
     * 
     * @return nRet: is grather equal 0 if it success, else less than 0
    */

    int nRet = 0;
    can_frame frame = this->get_cframe_coframe(co_frame);

    if (write(this->m_socket, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame))
        nRet = WRITE_ON_SCK_ERR;

    return nRet;
}

can_open_frame CanOpen::read_data()
{
    /**
     * @return co_frame: is a CanOpenFrame struct that represent the first packet received with len > 0
    */

    struct can_frame cfd;
    can_open_frame co_frame;
    int nBytes = 0;

    while (nBytes <= 0)
        nBytes = read(this->m_socket, &cfd, sizeof(struct can_frame));

    co_frame = this->get_coframe_cframe(cfd);

    return co_frame;
}

template <typename T>
can_open_frame CanOpen::get_frame_from_data(int nMsgType, uint16_t nIndex, uint8_t nSubIndex, T value)
{
    /**
     * @param nMsgType: Type of the Msg to Write in the "Header"
     * @param nIndex: Index of the CanOpenFrame
     * @param nSubIndex: SubIndex of the CanOpenFrame
     * @param value: is evaluate if nMsgType equal to DOWNLOAD_REQ
     * 
     * @return co_frame: is the CanOpenFrame that resul
    */

    can_open_frame co_frame;

    // In the First Byte of the Payload: 
    // n is equal to the number of byte discarded starting from seventh byte
    int nLen = CAN_MAX_PLEN - sizeof(T);

    // Configuration of the first Header Byte
    this->get_first_byte(&co_frame.can_byte[0], nMsgType, nLen);

    // Set Index of the CanOpenFrame
    co_frame.can_index = nIndex;

    // Set SubIndex of the CanOpenFrame
    co_frame.can_subIndex = nSubIndex;

    // Set Payload of the CanOpenFrame
    memcpy(co_frame.payload, &value, sizeof(T));

    return co_frame;
}

can_open_frame CanOpen::get_frame_from_data(int nMsgType, uint16_t nIndex, uint8_t nSubIndex)
{
    can_open_frame co_frame;

    // Configuration of the first Header Byte
    this->get_first_byte(&co_frame.can_byte[0], nMsgType);

    // Set Index of the CanOpenFrame
    co_frame.can_index = nIndex;

    // Set SubIndex of the CanOpenFrame
    co_frame.can_subIndex = nSubIndex;

    return co_frame;
}

void CanOpen::get_first_byte(bool* can_byte, int nTypeMsg, int nPayloadLen)
{
    /**
     * @param nTypeMsg: it could by DOWNLOAD_REQ or UPLOAD
     * @param nPyloadLen: is the len of the Payload
     * 
     * @return bRet[CAN_OPEN_BYTE_0]: is the first byte of the CanBus Msg
    */

    for (int i = 2; i >= 0; i--)
        can_byte[i] = (uint8_t)nTypeMsg & (1 << i - 1);
    can_byte[3] = false;

    switch (nTypeMsg)
    {
        case DOWNLOAD_REQ:

            for (int i = 5; i >= 4; i--)
                can_byte[i] = (uint8_t)nPayloadLen & (1 << CAN_MAX_DLEN - i - 3);
        
            can_byte[6] = 1;
            can_byte[7] = 1;

            break;

        case UPLOAD:

            for (int i = 4; i < CAN_OPEN_BYTE_0; i++)
                can_byte[i] = nPayloadLen & (1 << i);
        
            break;

        default:
            return;
    }
}

// ------------------------------------- PROTECTED METHODS --------------------------------------------------

template <typename T>
can_open_frame CanOpen::download(uint16_t nIndex, uint8_t nSubIndex, T value, int nTimeOut)
{
    /**
     * @def: Write a value on the right Register, defined using Index:SubIndex
     * 
     * @param nIndex: is the Index of the Motor Map to set the new value
     * @param nSubIndex: is the SubIndex of the Index
     * @param value: is the Value to write on a certain Index:SubIndex
     * @param nTimeOut: is the number of try where read data from the CanBus to attend if the CanOpenFrame have a different Index, by default is 5 times
     * 
     * @return: return a CanOpenStruct with the Response of the Server
    */

    can_open_frame co_frame;
    can_open_frame co_frame_ret;
    
    // CANOpen protocol accept Payload of a maximum 4 byte long
    if ((this->instanceof<double>(value)) || (this->instanceof<long long int>(value)))
        throw CanOpenException(MAX_LEN_EXCEEDED, "Maximum Lenght of Payload Exceeded");

    // Construct the can_open_frame using nMsgType, nIndex, nSubIndex, value
    co_frame = this->get_frame_from_data<T>(DOWNLOAD_REQ, nIndex, nSubIndex, value);

    // Applied the NodeID with the standard CanID
    co_frame.can_id = CAN_OPEN_BASE_D_ID + this->m_nNodeID;

    // Send of the data on the CanBus
    if (this->write_data(co_frame) < 0)
        throw CanOpenException(WRITE_ON_SCK_ERR, "Error on Write Msg -> DOWNLOAD");
    
    // Read the CanBus socket until the Index is equal to the Index send
    // Using a Default TimeOut: I use to avoid infinite loop
    for (int i = 0; i < nTimeOut; i++)
    {
        co_frame_ret = this->read_data();
        if ((co_frame_ret.can_index == nIndex) && (co_frame_ret.can_subIndex == nSubIndex))
            break;
    }

    if (co_frame_ret.can_index != nIndex)
        throw CanOpenException(FAILED_RECV_RESP, "Failed to Read the Response from CanBus -> DOWNLOAD");

    return co_frame_ret;
}

can_open_frame CanOpen::upload(uint16_t nIndex, uint8_t nSubIndex, int nTimeOut)
{
    /**
     * @def: Read a value on the right Register, defined using Index and SubIndex
     * 
     * @param nIndex: is the Index of the Motor Map to read the current value
     * @param nSubIndex: is the SubIndex of the Index
     * @param nTimeOut: is the number of try where data is read from the CanBus to attend if the CanOpenFrame have a different Index, by default is 5 times
     * 
     * @return co_frame: return a CanOpenStruct with the Response of the Server
    */

    can_open_frame co_frame;
    can_open_frame co_frame_ret;

    // Construct the can_open_frame using nMsgType, nIndex, nSubIndex
    co_frame = this->get_frame_from_data(UPLOAD, nIndex, nSubIndex);

    // Applied the NodeID with the standard CanID
    co_frame.can_id = CAN_OPEN_BASE_U_ID + this->m_nNodeID;

    // Send of the data on the CanBus
    if (this->write_data(co_frame) < 0)
        throw CanOpenException(WRITE_ON_SCK_ERR, "Error on Write Msg -> UPLOAD");
    
    // Read the CanBus socket until the Index is equal to the Index send
    // Using a Default TimeOut: I use to avoid infinite loop
    for (int i = 0; i < nTimeOut; i++)
    {
        co_frame_ret = this->read_data();
        if ((co_frame_ret.can_index == nIndex) && (co_frame_ret.can_subIndex == nSubIndex))
            break;
    }

    if (co_frame_ret.can_index != nIndex)
        throw CanOpenException(FAILED_RECV_RESP, "Failed to Read the Response from CanBus -> UPLOAD");

    return co_frame_ret;
}

// ------------------------------------- PUBLIC METHODS --------------------------------------------------

can_open_frame CanOpen::get_coframe_cframe(struct can_frame frame)
{
    can_open_frame co_frame;

    co_frame.can_id = frame.can_id;
    for (int i = 0; i < CAN_OPEN_BYTE_0; i++)
        co_frame.can_byte[i] = (frame.data[BYTE_O] >> i) & 1;
    co_frame.can_index = frame.data[INDEX_HIGH_BYTE] << 8 | frame.data[INDEX_LOW_BYTE];
    co_frame.can_subIndex = frame.data[SUB_INDEX];

    int j = 8;
    for (int i = PAYLOAD_0; i < TOTAL_PAYLOAD; i++)
    {
        co_frame.payload[i] = frame.data[j];
        j --;
    }

    return co_frame;
}

can_frame CanOpen::get_cframe_coframe(struct can_open_frame co_frame)
{
    can_frame frame;
    int j = CAN_MAX_DLEN - 1;

    frame.can_id = co_frame.can_id;
    frame.can_dlc = CAN_MAX_DLEN;
    memset(frame.data, '\0', CAN_MAX_DLEN);

    for (int i = 0; i < CAN_OPEN_BYTE_0; i++)
    {
        if (co_frame.can_byte[i])
            frame.data[BYTE_O] |= 1 << (CAN_OPEN_BYTE_0 - i - 1);
    }

    frame.data[INDEX_LOW_BYTE] = (uint8_t)co_frame.can_index;
    frame.data[INDEX_HIGH_BYTE] = (uint8_t)(co_frame.can_index >> 8);
    frame.data[SUB_INDEX] = co_frame.can_subIndex;
    for (int i = CAN_MAX_PLEN - 1; i >= 0; i--)
    {
        frame.data[j] = co_frame.payload[i];
        j --;
    }

    return frame;
}