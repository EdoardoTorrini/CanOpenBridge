#pragma once
#ifndef CAN_NODE_CST_HPP
#define CAN_NODE_CST_HPP

#include "can_open.hpp"

namespace CanOpenCST
{
    
    typedef struct parametersCST
    {
        float fMaxMotorSpeed;
        float fMaxGearInputSpeed;
        float fProfileDecelarion;
        float fQuickStopDeceleration;
    };

    class CanNodeCST : public CanOpenBridge::CanOpen
    {
        private:
            std::vector<int> m_vObjToRead;
            int m_nState;
            parametersCST m_param;
            
            void polling();

            float m_fStatusWord;
            float m_fPosition;
            float m_fVelocityActualValueAvaraged;
            float m_fTorqueActualValue;

        protected:
            std::thread* m_tPollingUpload;
            bool m_bStop;

        public:
            // impostare il costruttore e distruttore protected - finiti i test
            CanNodeCST(
                std::string sInterface, 
                int nNodeID, 
                std::vector<int> vObjToRead,
                parametersCST param={0.0, 0.0, 0.0, 0.0}
            );

            float getStatusWord() { return this->m_fStatusWord; }
            float getPosition() { return this->m_fPosition; }
            float getTorqueActualValue() { return this->m_fTorqueActualValue; }
            float getVelocityActualValueAvaraged() { return this->m_fVelocityActualValueAvaraged; }
            void stop() { this->m_nState = CanOpenBridge::CLOSE; }
            
            virtual ~CanNodeCST() {};
    };
}
#endif