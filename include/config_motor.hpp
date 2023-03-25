#include <iostream>
#include <map>

namespace CanOpenHeaderUtils 
{
    typedef struct CanOpenHeader{
        uint16_t nIndex;   
        uint8_t nSubIndex;
    };
    
    enum CanOpenConfigType {

        MOTOR_ENABLE = 0,
        RESET,

        /* Profile Positon Mode */
        PPM_MODES_OF_OPERATION = 10,
        PPM_FOLLOWING_ERROR_WINDOW,
        PPM_PROFILE_ACCELERATION,
        PPM_PROFILE_DECELERATION,
        PPM_QUICK_STOP_DECELERATION,
        PPM_MOTION_PROFILE_TYPE,
        PPM_CONTROLWORD_SHUTDOWN,
        PPM_CONTROLWORD_SWITCH_ON_ENABLE,
        PPM_TARGET_POSITION,
        PPM_ABSOLUTE_POSITION,
        PPM_ABSOLUTE_POSITION_START_IMMEDIATELY,
        PPM_RELATIVE_POSITION_START_IMMEDIATELY,
        PPM_RELATIVE_POSITION,
        PPM_CONTROLWORD_NEW_POSITION,
        PPM_READ_STATUSWORD_TARGET_REACHED,

        /* Profile Velocity Mode */
        PVM_MODES_OF_OPERATION = 100,
        PVM_PROFILE_ACCELERATION,
        PVM_PROFILE_DECELERATION,
        PVM_QUICK_STOP_DECELERATION,
        PVM_MOTION_PROFILE_TYPE,
        PVM_CONTROLWORD_SHUTDOWN,
        PVM_CONTROLWORD_SWITCH_ON_ENABLE,
        PVM_TARGET_POSITION,
        PVM_CONTROLWORD,
        
        
        /* Homing Mode */
        HMM_MODES_OF_OPERATION = 200,
        HMM_FOLLOWING_ERROR_WINDOW,
        HMM_HOME_OFFSET_MOVE_DISTANCE,
        HMM_QUICK_STOP_DECELERATION,
        HMM_SPEED_FOR_SWITCH_SEARCH,
        HMM_SPEED_FOR_ZERO_SEARCH,
        HMM_HOMING_ACCELERATION,
        HMM_HOME_POSITION,
        HMM_HOMING_METHOD,
        HMM_CONTROLWORD_SHUTDOWN,
        HMM_CONTROLWORD_SWITCH_ON_ENABLE,
        HMM_CONTROLWORD_START_HOMING,

        /* Cyclic Synchronous Torque */
        CST_MODES_OF_OPERATION = 300,
        CST_MAX_MOTOR_SPEED,
        CST_MAX_GEAR_INPUT_SPEED,
        CST_PROFILE_DECELERATION,
        CST_QUICK_STOP_DECELERATION,
        CST_CONTROLWORD_SHUTDOWN,
        CST_CONTROLWORD_SWITCH_ON_ENABLE,
        CST_TORQUE_OFFSET,
        CST_SET_TARGE_TORQUE,
        CST_STOP_TARGE_TORQUE,
        CST_CONTROLWORD_QUICK_STOP,
        CST_CONTROLWORD_CLEAR_FAULT,
        CST_READ_STATUSWORD,
        CST_READ_POSITION,
        CST_READ_VELOCITY_ACTUAL_VALUE_AVERAGED,
        CST_READ_TORQUE_ACTUAL_VALUE,
        CST_SAVE_ALL_PARAMETERS,
        CST_RESTORE_ALL_DEFAULT_PARAMETERS,

        /* Exception */
        ERR_MOTION_ERROR = 400,
        ERR_RESERVED,
        ERR_DEVICE_PROFILE_SPECIFIC,
        ERR_COMMUNICATION_ERROR,
        ERR_TEMEPRATURE_ERROR,
        ERR_VOLTAGE_ERROR,
        ERR_CURRENT_ERROR,
        ERR_GENERIC_ERROR,

        NUMBER_OF_ERRORS = 500
    };

    std::map<int, CanOpenHeader> ConfigMotor
    {
        {MOTOR_ENABLE, {0x0001, 0x00}},
        {RESET, {0x6040, 0x00}},

        /* Profile Positon Mode */
        {PPM_MODES_OF_OPERATION, {0x6060, 0x00}},
        {PPM_FOLLOWING_ERROR_WINDOW, {0x6065, 0x00}},
        {PPM_PROFILE_ACCELERATION, {0x6083, 0x00}},
        {PPM_PROFILE_DECELERATION, {0x6084, 0x00}},
        {PPM_QUICK_STOP_DECELERATION, {0x6085, 0x00}},
        {PPM_MOTION_PROFILE_TYPE, {0x6086, 0x00}},
        {PPM_CONTROLWORD_SHUTDOWN, {0x6040, 0x00}},
        {PPM_CONTROLWORD_SWITCH_ON_ENABLE, {0x6040, 0x00}},
        {PPM_TARGET_POSITION, {0x607A, 0x00}},
        {PPM_ABSOLUTE_POSITION, {0x6040, 0x00}},
        {PPM_ABSOLUTE_POSITION_START_IMMEDIATELY, {0x6040, 0x00}},
        {PPM_RELATIVE_POSITION_START_IMMEDIATELY, {0x6040, 0x00}},
        {PPM_RELATIVE_POSITION, {0x6040, 0x00}},
        {PPM_CONTROLWORD_NEW_POSITION, {0x6040, 0x00}},
        {PPM_READ_STATUSWORD_TARGET_REACHED, {0x6041, 0x00}},

        /* Profile Velocity Mode */
        {PVM_MODES_OF_OPERATION, {0x6060, 0x00}},
        {PVM_PROFILE_ACCELERATION, {0x6083, 0x00}},
        {PVM_PROFILE_DECELERATION, {0x6084, 0x00}},
        {PVM_QUICK_STOP_DECELERATION, {0x6085, 0x00}},
        {PVM_MOTION_PROFILE_TYPE, {0x6086, 0x00}},
        {PVM_CONTROLWORD_SHUTDOWN, {0x6040, 0x00}},
        {PVM_CONTROLWORD_SWITCH_ON_ENABLE, {0x6040, 0x00}},
        {PVM_TARGET_POSITION, {0x60FF, 0x00}},
        {PVM_CONTROLWORD, {0x6040, 0x00}},
        
        
        /* Homing Mode */
        {HMM_MODES_OF_OPERATION, {0x6060, 0x00}},
        {HMM_FOLLOWING_ERROR_WINDOW, {0x6065, 0x00}},
        {HMM_HOME_OFFSET_MOVE_DISTANCE, {0x30B1, 0x00}},
        {HMM_QUICK_STOP_DECELERATION, {0x6085, 0x00}},
        {HMM_SPEED_FOR_SWITCH_SEARCH, {0x6099, 0x01}},
        {HMM_SPEED_FOR_ZERO_SEARCH, {0x6099, 0x02}},
        {HMM_HOMING_ACCELERATION, {0x609A, 0x00}},
        {HMM_HOME_POSITION, {0x30B0, 0x00}},
        {HMM_HOMING_METHOD, {0x6098, 0x00}},
        {HMM_CONTROLWORD_SHUTDOWN, {0x6040, 0x00}},
        {HMM_CONTROLWORD_SWITCH_ON_ENABLE, {0x6040, 0x00}},
        {HMM_CONTROLWORD_START_HOMING, {0x6040, 0x00}},

        /* Cyclic Synchronous Torque */
        {CST_MODES_OF_OPERATION, {0x6060, 0x00}},
        {CST_MAX_MOTOR_SPEED, {0x6080, 0x00}},
        {CST_MAX_GEAR_INPUT_SPEED, {0x3003, 0x03}},
        {CST_PROFILE_DECELERATION, {0x6084, 0x00}}, 
        {CST_QUICK_STOP_DECELERATION, {0x6085, 0x00}}, 
        {CST_CONTROLWORD_SHUTDOWN, {0x6040, 0x00}},
        {CST_CONTROLWORD_SWITCH_ON_ENABLE, {0x6040, 0x00}},
        {CST_TORQUE_OFFSET, {0x60B2, 0x00}},
        {CST_SET_TARGE_TORQUE, {0x6071, 0x00}},
        {CST_STOP_TARGE_TORQUE, {0x6071, 0x00}},
        {CST_CONTROLWORD_QUICK_STOP, {0x6040, 0x00}},
        {CST_CONTROLWORD_CLEAR_FAULT, {0x6040, 0x00}},
        {CST_READ_STATUSWORD, {0x6041, 0x00}},
        {CST_READ_POSITION, {0x6064, 0x00}},
        {CST_READ_VELOCITY_ACTUAL_VALUE_AVERAGED, {0x30D3, 0x01}},
        {CST_READ_TORQUE_ACTUAL_VALUE, {0x6077, 0x00}},
        {CST_SAVE_ALL_PARAMETERS, {0x1010, 0x01}},
        {CST_RESTORE_ALL_DEFAULT_PARAMETERS, {0x1011, 0x01}},

        /* Exception */
        {ERR_MOTION_ERROR, {0x1001, 0x00}},
        {ERR_RESERVED, {0x1001, 0x00}},
        {ERR_DEVICE_PROFILE_SPECIFIC, {0x1001, 0x00}},
        {ERR_COMMUNICATION_ERROR, {0x1001, 0x00}},
        {ERR_TEMEPRATURE_ERROR, {0x1001, 0x00}},
        {ERR_VOLTAGE_ERROR, {0x1001, 0x00}},
        {ERR_CURRENT_ERROR, {0x1001, 0x00}},
        {ERR_GENERIC_ERROR, {0x1001, 0x00}},

        {NUMBER_OF_ERRORS, {0x1003, 0x00}},
    };

    class HeaderMapping {

        private:
            uint16_t m_nIndex;   
            uint8_t m_nSubIndex;

        public:

            HeaderMapping(int nType) 
            {
                this->m_nIndex = ConfigMotor.at(nType).nIndex;
                this->m_nSubIndex = ConfigMotor.at(nType).nSubIndex;
            }

            uint16_t GetIndex() { return this->m_nIndex; }
            uint8_t GetSubindex() { return this->m_nSubIndex; }

    };
}