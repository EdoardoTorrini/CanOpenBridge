#include <thread>

// can bus include
#include <linux/can.h>
#include <linux/can/raw.h>

// socket include
#include <sys/socket.h>

#include <string.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>

#ifndef CAN_OPEN_HPP
#define CAN_OPEN_HPP

#define CAN_MAX_PLEN        4
#define CAN_OPEN_BYTE_0     8
#define CAN_OPEN_BASE_D_ID  1536
#define CAN_OPEN_BASE_U_ID  1408

typedef struct can_open_frame{

    uint16_t can_id;
    bool can_byte[CAN_OPEN_BYTE_0];
    uint16_t can_index;
    uint8_t can_subIndex;
    uint8_t payload[CAN_MAX_PLEN];

};

class CanOpen
{
    private:
        int m_socket;
        struct ifreq m_ifr;
        struct sockaddr_can m_addr;

        int m_nNodeID;
        void init();
        
        can_open_frame read_data();
        int write_data(struct can_open_frame frame);

        template <typename T>
        can_open_frame get_frame_from_data(int nMsgType, uint16_t nIndex, uint8_t nSubIndex, T value);
        can_open_frame get_frame_from_data(int nMsgType, uint16_t nIndex, uint8_t nSubIndex);
        void get_first_byte(bool* can_byte, int nTypeMsg, int nPayloadLen=0);

        template<typename Base, typename T>
        inline bool instanceof(const T) { return std::is_base_of<Base, T>::value; }

    protected: 
        CanOpen(char* sInterface, int nNodeID);
        
        template <typename T>
        can_open_frame download(uint16_t nIndex, uint8_t nSubIndex, T value, int nTimeOut=5);
        can_open_frame upload(uint16_t nIndex, uint8_t nSubIndex, int nTimeOut=5);

        virtual ~CanOpen() {};

    public:
        can_open_frame get_coframe_cframe(struct can_frame frame);
        can_frame get_cframe_coframe(struct can_open_frame frame);
};

enum CanOpenPayloadByte 
{
    BYTE_O = 0,
    INDEX_LOW_BYTE,
    INDEX_HIGH_BYTE, 
    SUB_INDEX,
    PAYLOAD_0,
    PAYLOAD_1,
    PAYLOAD_2,
    PAYLOAD_3,

    TOTAL_PAYLOAD
};

enum CanOpenMsgType
{
    DOWNLOAD_REQ = 2,
    UPLOAD = 4,
    DOWNLOAD_REP = 6,
    ERROR = 8 
};

class CanOpenException : public std::exception
{
    private:
        int m_nStatusCode;
        std::string m_sMsg;

    public:
        CanOpenException(int nStatusCode, std::string sMsg)
        {
            this->m_nStatusCode = nStatusCode;
            this->m_sMsg = sMsg;
        }

        int getStatusCode() { return m_nStatusCode; }
        std::string getMsg() { return m_sMsg; }

};

enum CanOpenTypeError
{
    SOCKET_ERR = -1,
    BINDING_ERR = -2,

    WRITE_ON_SCK_ERR = -3,
    RECIVE_CAN_ERR = -4,
    MAX_LEN_EXCEEDED = -5,
    FAILED_RECV_RESP = -6
};

#endif