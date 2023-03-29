#include "can_node_ppm.hpp"

using namespace CanOpenExceptionBase;

namespace CanOpenPPM
{
    CanNodePPM::CanNodePPM(std::string sInterface, int nNodeID, std::vector<int> vObjToRead, parametersPPM param) 
    : CanOpenBridge::CanOpen(sInterface, nNodeID)
    {
        memcpy(&this->m_param, &param, sizeof(parametersPPM));
        this->m_vObjToRead = vObjToRead;

        this->m_bStop = false;
        this->m_nState = CanOpenBridge::OPERATION_MODE;
        this->m_tPollingUpload = new std::thread(&CanNodePPM::polling, this);
    }

    void CanNodePPM::polling()
    {
        CanOpenBridge::can_open_frame co_frame;
        while(!this->m_bStop)
        {
            switch (this->m_nState)
            {
                case CanOpenBridge::OPERATION_MODE:

                    co_frame = this->download(0x6060, 0x00, (uint8_t)0x01);
                    if (co_frame.can_byte[7])
                    {
                        this->m_nState = CanOpenBridge::ERROR_STATE;
                        break;
                    }

                    this->m_nState = CanOpenBridge::PARAMETERS;
                    break;

                case CanOpenBridge::PARAMETERS:

                    if (this->m_param.fFollowinfErrorWindow != 0)
                    {
                        co_frame = this->download(0x6065, 0x00, this->m_param.fFollowinfErrorWindow);
                        if (co_frame.can_byte[7])
                        {
                            this->m_nState=CanOpenBridge::ERROR_STATE;
                            break;
                        }   
                    }

                    if (this->m_param.fVelocity != 0)
                    {
                        co_frame = this->download(0x6081, 0x00, this->m_param.fVelocity);
                        if (co_frame.can_byte[7])
                        {
                            this->m_nState = CanOpenBridge::ERROR_STATE;
                            break;
                        }   
                    }

                    if (this->m_param.fAcceleration != 0)
                    {
                        co_frame = this->download(0x6083, 0x00, this->m_param.fAcceleration);
                        if(co_frame.can_byte[7])
                        {
                            this->m_nState = CanOpenBridge::ERROR_STATE;
                            break;
                        }   
                    }

                    if (this->m_param.fDeceleration != 0)
                    {
                        co_frame = this->download(0x6084, 0x00, this->m_param.fDeceleration);
                        if(co_frame.can_byte[7])
                        {
                            this->m_nState = CanOpenBridge::ERROR_STATE;
                            break;
                        }   
                    }

                    if (this->m_param.fQuickStopDeceleration != 0)
                    {
                        co_frame = this->download(0x6085, 0x00, this->m_param.fQuickStopDeceleration);
                        if(co_frame.can_byte[7])
                        {
                            this->m_nState = CanOpenBridge::ERROR_STATE;
                            break;
                        }   
                    }
                    
                    if (this->m_param.fMotionProfileType != 0)
                    {
                        co_frame = this->download(0x6086, 0x00, this->m_param.fMotionProfileType);
                        if(co_frame.can_byte[7])
                        {
                            this->m_nState = CanOpenBridge::ERROR_STATE;
                            break;
                        }   
                    }
                    
                    this->m_nState = CanOpenBridge::ENABLE_DEVICE;
                    break;

                case CanOpenBridge::ENABLE_DEVICE:

                    co_frame = this->download(0x6040, 0x00, (uint16_t)0x0006);
                    if (co_frame.can_byte[7])
                    {
                        this->m_nState = CanOpenBridge::ERROR_STATE;
                        break;
                    }

                    co_frame = this->download(0x6040, 0x00, (uint16_t)0x000F);
                    if(co_frame.can_byte[7])
                    {
                        this->m_nState = CanOpenBridge::ERROR_STATE;
                        break;
                    } 

                    this->m_nState = CanOpenBridge::LISTENER;
                    break;

                case CanOpenBridge::LISTENER:

                    for (int &nMsgType: this->m_vObjToRead)
                    {
                        CanOpenHeaderUtils::HeaderMapping co_header(nMsgType);
                        co_frame = this->upload(co_header.GetIndex(), co_header.GetSubindex()); 
                        if (co_frame.can_byte[7])
                        {
                            this->m_nState = CanOpenBridge::ERROR_STATE;
                            break;
                        }

                        switch (nMsgType)
                        {
                            case CanOpenHeaderUtils::PPM_READ_STATUSWORD_TARGET_REACHED:
                                this->m_fTargetReached = *(float*)&co_frame.payload;
                                break;

                            default:
                                break;
                        }

                    }

                    break;
                    
                case CanOpenBridge::ERROR_STATE:

                    // TODO: gestione dell'errore, chiedere numero di errori e tipologia
                    this->m_nState = CanOpenBridge::RESET;
                    break;
                    
                case CanOpenBridge::RESET:

                    co_frame = this->download(0x6040, 0x00, 0x8000);
                    if(co_frame.can_byte[7])
                    {
                        this->m_nState = CanOpenBridge::ERROR_STATE;
                        break;
                    } 

                    this->m_nState = CanOpenBridge::LISTENER;
                    break;

                case CanOpenBridge::CLOSE:

                    co_frame = this->download(0x6040, 0x00, (uint16_t)0x000B);
                    if (co_frame.can_byte[7])
                    {
                        this->m_nState = CanOpenBridge::ERROR_STATE;
                        break;
                    }

                    co_frame = this->download(0x6040, 0x00, (uint16_t)0x0000);
                    if (co_frame.can_byte[7])
                    {
                        this->m_nState = CanOpenBridge::ERROR_STATE;
                        break;
                    }

                    this->m_bStop = true;
                    break;

                default:
                    this->m_nState = CanOpenBridge::ERROR_STATE;
                    break;
            }
        }
    }

    void CanNodePPM::join()
    {
        this->m_tPollingUpload->join();
    }
}
