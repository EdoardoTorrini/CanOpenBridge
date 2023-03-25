#pragma once
#ifndef CAN_OPEN_HPP
#define CAN_OPEN_HPP

#include <thread>

// can bus include
#include <linux/can.h>
#include <linux/can/raw.h>

// socket include
#include <sys/socket.h>
#include "config_motor.hpp"
#include "can_open_exception.hpp"

#include <string.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <vector>

namespace CanOpenBridge {

    #define CAN_MAX_PLEN        4
    #define CAN_OPEN_BYTE_0     8
    #define CAN_OPEN_BASE_D_ID  1536
    #define CAN_OPEN_BASE_U_ID  1408

    enum CanOpenMsgType
    {
        DOWNLOAD_REQ = 2,
        UPLOAD = 4,
        DOWNLOAD_REP = 6,
        ERROR = 8 
    };

    typedef struct can_open_frame {

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
            can_open_frame get_frame_from_data(int nMsgType, uint16_t nIndex, uint8_t nSubIndex, T value)
            {
                can_open_frame co_frame;

                // In the First Byte of the Payload: 
                // n is equal to the number of byte discarded starting from seventh byte
                int nLen = CAN_MAX_PLEN - sizeof(T);

                co_frame = this->get_frame_from_data(nMsgType, nIndex, nSubIndex, nLen);

                // Set Payload of the CanOpenFrame
                memcpy(co_frame.payload, &value, sizeof(T));

                return co_frame;
            }
            can_open_frame get_frame_from_data(int nMsgType, uint16_t nIndex, uint8_t nSubIndex, int nLen=0);
            void get_first_byte(bool* can_byte, int nTypeMsg, int nPayloadLen=0);

            template<typename Base, typename T>
            inline bool instanceof(const T) { return std::is_base_of<Base, T>::value; }

        protected: 
            CanOpen(std::string sInterface, int nNodeID);
            
            template <typename T>
            can_open_frame download(uint16_t nIndex, uint8_t nSubIndex, T value, int nTimeOut=5)
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
                    throw CanOpenExceptionBase::CanOpenException(
                        CanOpenExceptionBase::MAX_LEN_EXCEEDED, 
                        "Maximum Lenght of Payload Exceeded"
                    );

                // Construct the can_open_frame using nMsgType, nIndex, nSubIndex, value
                co_frame = this->get_frame_from_data<T>(DOWNLOAD_REQ, nIndex, nSubIndex, value);

                // Applied the NodeID with the standard CanID
                co_frame.can_id = CAN_OPEN_BASE_D_ID + this->m_nNodeID;

                // Send of the data on the CanBus
                if (this->write_data(co_frame) < 0)
                    throw CanOpenExceptionBase::CanOpenException(
                        CanOpenExceptionBase::WRITE_ON_SCK_ERR, 
                        "Error on Write Msg -> DOWNLOAD"
                    );
                
                // Read the CanBus socket until the Index is equal to the Index send
                // Using a Default TimeOut: I use to avoid infinite loop
                for (int i = 0; i < nTimeOut; i++)
                {
                    co_frame_ret = this->read_data();
                    if ((co_frame_ret.can_index == nIndex) && (co_frame_ret.can_subIndex == nSubIndex))
                        break;
                }

                if (co_frame_ret.can_index != nIndex)
                    throw CanOpenExceptionBase::CanOpenException(
                        CanOpenExceptionBase::FAILED_RECV_RESP, 
                        "Failed to Read the Response from CanBus -> DOWNLOAD"
                    );

                return co_frame_ret;
            }
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

    enum CanNodeStatus
    {
        OPERATION_MODE = 0,
        PARAMETERS,
        ENABLE_DEVICE,
        LISTENER,
        ERROR_STATE,
        RESET,
        CLOSE
    };  
}

#endif