#include "can_node_cst.hpp"

namespace CanOpenCST
{
    CanNodeCST::CanNodeCST(std::string sInterface, int nNodeID, std::vector<int> vObjToRead, parametersCST param) 
    : CanOpenBridge::CanOpen(sInterface, nNodeID)
    {
        memcpy(&this->m_param, &param, sizeof(parametersCST));

        this->m_bStop = false;
        this->m_nState = CanOpenBridge::OPERATION_MODE;
        this->m_tPollingUpload = new std::thread(&CanNodeCST::polling, this);
    }

    void CanNodeCST::polling()
    {
        while(!this->m_bStop)
        {
            CanOpenBridge::can_open_frame co_frame;

            switch (this->m_nState)
            {
                case CanOpenBridge::OPERATION_MODE:

                    co_frame = this->download(0x6060, 0x00, 0x0A);
                    if(co_frame.can_byte[7])
                    {
                        this->m_nState = CanOpenBridge::ERROR_STATE;
                        break;
                    }
                    
                    this->m_nState = CanOpenBridge::PARAMETERS;
                    break;

                case CanOpenBridge::PARAMETERS:
                
                    if(this->m_param.fMaxMotorSpeed != 0)
                    {
                        co_frame = this->download(0x6080, 0x00, this->m_param.fMaxMotorSpeed);
                        if(co_frame.can_byte[7])
                        {
                            this->m_nState = CanOpenBridge::ERROR_STATE;
                            break;
                        }   
                    }

                    if(this->m_param.fMaxGearInputSpeed != 0)
                    {
                        co_frame = this->download(0x3003, 0x03, this->m_param.fMaxGearInputSpeed);
                        if(co_frame.can_byte[7])
                        {
                            this->m_nState = CanOpenBridge::ERROR_STATE;
                            break;
                        }   
                    }

                    if(this->m_param.fProfileDecelarion != 0)
                    {
                        co_frame = this->download(0x6084, 0x00, this->m_param.fProfileDecelarion);
                        if(co_frame.can_byte[7])
                        {
                            this->m_nState = CanOpenBridge::ERROR_STATE;
                            break;
                        }   
                    }
                    
                    if(this->m_param.fQuickStopDeceleration != 0)
                    {
                        co_frame = this->download(0x6085, 0x00, this->m_param.fQuickStopDeceleration);
                        if(co_frame.can_byte[7])
                        {
                            this->m_nState = CanOpenBridge::ERROR_STATE;
                            break;
                        }   
                    }
                    this->m_nState = CanOpenBridge::ENABLE_DEVICE;
                    break;

                case CanOpenBridge::ENABLE_DEVICE:

                    co_frame = this->download(0x6040, 0x00, 0x0600);
                    if(co_frame.can_byte[7])
                    {
                        this->m_nState = CanOpenBridge::ERROR_STATE;
                        break;
                    }

                    co_frame = this->download(0x6040, 0x00, 0x0F00);
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
                            case CanOpenHeaderUtils::CST_READ_STATUSWORD:
                                this->m_fStatusWord = *(float*)&co_frame.payload;
                                break;
                        
                            case CanOpenHeaderUtils::CST_READ_POSITION:
                                this->m_fPosition = *(float*)&co_frame.payload;
                                break;
                            
                            case CanOpenHeaderUtils::CST_READ_VELOCITY_ACTUAL_VALUE_AVERAGED:
                                this->m_fVelocityActualValueAvaraged = *(float*)&co_frame.payload;
                                break;
                            
                            case CanOpenHeaderUtils::CST_READ_TORQUE_ACTUAL_VALUE:
                                this->m_fTorqueActualValue = *(float*)&co_frame.payload;
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
                
                    co_frame = this->download(0x6040, 0x00, 0x000B);
                    if(co_frame.can_byte[7])
                    {
                        this->m_nState = CanOpenBridge::ERROR_STATE;
                        break;
                    }
                    co_frame = this->download(0x6040, 0x00, 0x0000);
                    if(co_frame.can_byte[7])
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
}
