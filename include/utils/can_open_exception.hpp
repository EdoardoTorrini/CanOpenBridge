#pragma once
#ifndef CAN_OPEN_EXCEPTION_HPP
#define CAN_OPEN_EXCEPTION_HPP

#include <string>
#include <exception>

namespace CanOpenExceptionBase {

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
}

#endif