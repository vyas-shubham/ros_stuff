#include "include/ReactionWheelDriver.hpp"

ReactionWheelController::ReactionWheelController(std::string motor_address=DEFAULT_MOTOR_ADDRESS) :
                                            motor_address_{motor_address}, isMotorEnabled{false},
                                            max_velocity_rpm{1000}
{
    // Prepare URL from IP Address for REST/HTTP Communication
    motor_address_.insert(0, "http://");
    motor_address_.insert(motor_address_.length(), "/od/");

    for (auto paramIter=motor_setup_params.cbegin(); paramIter!=motor_setup_params.cend(); paramIter++)
    {
        std::string param_register_ = paramIter->first;
        std::string param_value_ = paramIter->second;
        assert (writeMotorRegister(param_register_, param_value_) && "Set Motor Param Failed!!");
    }
}

ReactionWheelController::~ReactionWheelController()
{
}

bool ReactionWheelController::enableReactionWheel()
{
    if (isMotorEnabled)
    {
        std::cout << "Motor Is already Enabled!";
        return true;
    }
    else
    {
        if (writeMotorRegister(motor_command_register_, enable_motor_cmd_))
        {
            std::cout << "Motor Enabled." << std::endl;
            isMotorEnabled = true;
            return true;
        }
        else
        {
            std::cout << "Unable to enable motor!" << std::endl;
            isMotorEnabled = false;
            return false;
        }
    }

}

bool ReactionWheelController::disableReactionWheel()
{
    if (!isMotorEnabled)
    {
        std::cout << "Motor Is already Disabled!";
        return true;
    }
    else
    {
        if (writeMotorRegister(motor_command_register_, disable_motor_cmd_))
        {
            std::cout << "Motor Disabled." << std::endl;
            isMotorEnabled = false;
            return true;
        }
        else 
        {
            std::cout << "Unable to Disable Motor!" << std::endl;
            return false;
        }
    }
    
}

bool ReactionWheelController::faultReset()
{
    if(writeMotorRegister(motor_command_register_, fault_reset_cmd_)) return true;
    else return false;
}

bool ReactionWheelController::readCurrentVelocityRPM(int &read_vel)
{
    std::string raw_vel_value_;
    std::stringstream temp_sstring_;
    int temp_vel_;
    bool vel_negative_polarity_ = false;
    std::string raw_vel_polarity_;
    if (readMotorRegister(vel_polarity_register_, raw_vel_polarity_))
    {
        int  vel_polarity_ = hexStringToInt(raw_vel_polarity_);
        // If 8 bit polarity is either 01000000 or 11000000 the velocity is negative.
        // https://en.nanotec.com/products/manual/N5_CAN_EN/object_dictionary%252Fod_motion_0x607E.html
        if (vel_polarity_==64 || vel_polarity_==192) vel_negative_polarity_ = true;
        else vel_negative_polarity_ = false;
    }
    else
    {
        std::cout << "Velocity Polarity Read Failed!" << std::endl;
        return false;
    }

    if(readMotorRegister(motor_vel_read_register_, raw_vel_value_))
    {
        int abs_vel_ = hexStringToInt(raw_vel_value_);
        if (vel_negative_polarity_) read_vel = -abs_vel_;
        else read_vel = abs_vel_;
        return true;
    }
    else 
    {
        std::cout << "Velocity Read Failed!" << std::endl;
        return false;
    }

}

bool ReactionWheelController::readCurrentVelocity(float &read_vel)
{
    int temp_vel_;
    if(readCurrentVelocityRPM(temp_vel_))
    {
        read_vel = temp_vel_*0.10472;
        return true;
    }
    else return false;
}

bool ReactionWheelController::sendVelocityCommandRPM(int cmd_vel)
{
    if (isMotorEnabled)
    {
        if(!torqueModeEnabled)
        {
            // Check for limits
            if (cmd_vel > max_velocity_rpm)
            {
                cmd_vel = max_velocity_rpm;
                std::cout << "Command exceeded RPM limit. Commanding Max RPM: " << max_velocity_rpm << std::endl;
            }
            else if (cmd_vel < -max_velocity_rpm)
            {
                cmd_vel = -max_velocity_rpm;
                std::cout << "Command exceeded RPM limit. Commanding Min RPM: " << -max_velocity_rpm << std::endl;
            }

            if (cmd_vel < 0)
            {
                cmd_vel = -cmd_vel;
                if (!writeMotorRegister(vel_polarity_register_, vel_negative_direction_cmd_)) 
                {
                    std::cout << "Unable to change to Negative Polarity." << std::endl;
                    return false;
                }
            }
            else 
            {
                if (!writeMotorRegister(vel_polarity_register_, vel_positive_direction_cmd_)) 
                {
                    std::cout << "Unable to change to Positive Polarity." << std::endl;
                    return false;
                }
            }

            std::string cmd_vel_string_ = toHexString(cmd_vel, 32);

            // Testing Code: Print Hex Command Instead of sending.
            // std::cout << "Writing to register: " << motor_vel_cmd_register_ << std::endl;
            // std::cout << "Writing Value: " << cmd_vel_string_ << std::endl;
            // return true;

            // Send Commands.
            if (writeMotorRegister(motor_vel_cmd_register_, cmd_vel_string_)) return true;
            else return false;
        }
        else 
        {
            std::cout << "Torque Mode enabled. Cannot command velocity" << std::endl;
            return false;
        }
    }
    else 
    {
        std::cout << "Motor Not enabled. Cannot command velocity" << std::endl;
        return false;
    }
}

bool ReactionWheelController::sendVelocityCommand(float cmd_vel)
{
    // Convert to RPM as Motor Works with that Unit
    int cmd_vel_rpm = (int)(cmd_vel*9.5493);
    if (sendVelocityCommandRPM(cmd_vel_rpm)) return true;
    else return false;

}

bool ReactionWheelController::readMotorRegister(std::string readRegister, std::string &readValue)
{
    std::string read_address = motor_address_ + readRegister;
    RestClient::Response resp_ = RestClient::get(read_address);
    if (resp_.code==200)
    {
        readValue = resp_.body;
        return true; 
    }
    else return false;
}

bool ReactionWheelController::writeMotorRegister(std::string writeRegister, std::string writeValue)
{   
    std::string write_address = motor_address_ + writeRegister;
    // Add Double Quotes before/after value for communication specification
    std::string cmd_value_ = writeValue;
    cmd_value_ = "\"" + cmd_value_ + "\"";
    std::string content_type = "application/x-www-form-urlencoded";
    RestClient::Response resp_  = RestClient::post(write_address, content_type, cmd_value_);
    if(resp_.code==200) return true;
    // else return false;
    else
    {
        std::cout << "Response Code: " << resp_.code << std::endl;
        return false;
    }
}

std::string ReactionWheelController::toHexString(int i, int bits)
{
    std::stringstream stream;
    stream << std::setfill ('0') << std::setw(bits/4)
            << std::uppercase << std::hex << i;
    return stream.str();
}

int ReactionWheelController::hexStringToInt(std::string hexString)
{
    std::stringstream temp_sstring_;
    int temp_vel_;
    // Remove Double Quotes from received string
    if (hexString.at(0) == '"')
    {
        hexString.erase(0,1);
        hexString.erase(hexString.size()-1);
    }
    // Convert string to int
    temp_sstring_ << std::hex << hexString;
    temp_sstring_ >> temp_vel_;
    return static_cast<int>(temp_vel_);
}

bool ReactionWheelController::enableTorqueMode()
{
    if (isMotorEnabled)
    {
        if (torqueModeEnabled)
        {
            std::cout << "Torque Mode Is already Enabled!";
            return true;
        }
        else
        {
            if (writeMotorRegister(motor_mode_register_, enable_torque_cmd_))
            {
                std::cout << "Torque Mode Enabled." << std::endl;
                // std::string motor_mode_;
                // readMotorRegister(motor_mode_register_, motor_mode_);
                // std::cout << "Motor Mode: " <<  motor_mode_ << std::endl;
                torqueModeEnabled = true;
                return true;
            }
            else
            {
                std::cout << "Unable to enable Torque Mode!" << std::endl;
                torqueModeEnabled = false;
                return false;
            }
        }
    }
    else
    {
        std::cout << "Enable Motor First!" << std::endl;
        return false;
    }

}

bool ReactionWheelController::disableTorqueMode()
{
    if (isMotorEnabled)
    {
        if (!torqueModeEnabled)
        {
            std::cout << "Torque Mode Is already Disabled!";
            return true;
        }
        else
        {
            if (writeMotorRegister(motor_mode_register_, disable_torque_cmd_))
            {
                std::cout << "Torque Mode Disabled." << std::endl;
                torqueModeEnabled = false;
                return true;
            }
            else 
            {
                std::cout << "Unable to Disable Torque Mode!" << std::endl;
                return false;
            }
        }
    }
    else
    {
        std::cout << "Enable Motor First!" << std::endl;
        return false;
    }
    
}

bool ReactionWheelController::sendTorqueCommand(float cmd_torque)
{
    if (isMotorEnabled)
    {
        if (torqueModeEnabled)
        {
            // Check for limits
            if (cmd_torque > max_torque)
            {
            cmd_torque = max_torque;
            std::cout << "Command exceeded torque limit. Commanding Max RPM: " << max_torque << std::endl;
            }
            else if (cmd_torque < -max_torque)
            {
            cmd_torque = -max_torque;
            std::cout << "Command exceeded torque limit. Commanding Min RPM: " << -max_torque << std::endl;
            }

            // Find Percentage of Max Torque Commanded as that is the motor torque unit
            float torque_cmd_percentage_ = (cmd_torque / max_torque) * 100; 
            
            // Motor Cmd is "tenths of a percent of the rated torque"
            // This object is calculated as thousandths of the torque, e.g., the value "500" means
            // "50%" of the rated torque; "1100" is equivalent to 110%. The rated torque corresponds
            // to the rated current in object 203Bh:01.
            int cmd_torque_to_send_ = (int)(torque_cmd_percentage_ * 10);
            std::cout << "Commanded Torque in tenth percent: " << cmd_torque_to_send_ << std::endl;
            
            // Testing Code: Print Hex Command Instead of sending.
            // std::cout << "Writing to register: " << motor_torque_cmd_register_ << std::endl;
            // std::cout << "Writing Value: " << toHexString(cmd_torque_to_send_, 16) << std::endl;
            // return true;

            // Send Commands.
            if (writeMotorRegister(motor_torque_cmd_register_, toHexString(cmd_torque_to_send_, 16)))
            {
                std::string raw_register_val;
                readMotorRegister(motor_torque_cmd_register_, raw_register_val);
                std::cout << "Torque Command Sent: " <<  raw_register_val << std::endl;
                return true;
            }
            else return false;
        }
        else
        {
            std::cout << "Torque Mode Not Enabled." << std::endl;
            return false;
        }
    }
    else
    {
        std::cout << "Motor Not Enabled." << std::endl;
        return false;
    }
}

bool ReactionWheelController::currentTorqueDemand(int &tau_)
{
    std::string raw_register_val_;
    if(!readMotorRegister("6074/00", raw_register_val_)) 
    {
        std::cout << "Reading Torque Demand Failed!" << std::endl;
        return false;
    }
    else
    {   
        tau_ = hexStringToInt(raw_register_val_);
        std::cout << "Raw Demanded Toque: " << raw_register_val_ << std::endl;
        return true;
    }
}

bool ReactionWheelController::readMotorStatus(int &status)
{
    std::string raw_register_val_;
    if(!readMotorRegister("6041/00", raw_register_val_)) 
    {
        std::cout << "Reading Torque Demand Failed!" << std::endl;
        return false;
    }
    else
    {   
        status = hexStringToInt(raw_register_val_);
        std::cout << "Raw Motor Status: " << raw_register_val_ << std::endl;
        return true;
    }

}