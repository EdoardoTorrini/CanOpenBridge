#include "can_open.hpp"

#ifndef CAN_NODE_PPM_HPP
#define CAN_NODE_PPM_HPP

namespace CanOpenPPM
{
    typedef struct parametersPPM
    {
        float fFollowinfErrorWindow;
        float fVelocity;
        float fAcceleration;
        float fDeceleration;
        float fQuickStopDeceleration;
        float fMotionProfileType;
    };


    class CanNodePPM : public CanOpenBridge::CanOpen
    {
        private:

            std::vector<int> m_vObjToRead;
            int m_nState;
            parametersPPM m_param;

            void polling();

            float m_fTargetReached;

        protected:

            std::thread* m_tPollingUpload;
            bool m_bStop;

        public:

            // impostare il costruttore e distruttore protected - finiti i test
            CanNodePPM::CanNodePPM(
                std::string sInterface, 
                int nNodeID, std::vector<int> vObjToRead, 
                parametersPPM param={0.0, 0.0, 0.0, 0.0, 0.0, 0.0}
            );

            virtual ~CanNodePPM() {};

            float getStatuswordTargetReached() { return this->m_fTargetReached; }
            void stop() {this->m_nState=  CanOpenBridge::CLOSE;}
    };
}

#endif