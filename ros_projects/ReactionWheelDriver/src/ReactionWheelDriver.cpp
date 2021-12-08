#include "include/ReactionWheelDriver.hpp"

ReactionWheelController::ReactionWheelController(std::string motor_address=DEFAULT_MOTOR_ADDRESS) :
                                            motor_address_{motor_address}, isMotorEnabled{false},
                                            max_velocity_rpm{500.0}
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

bool ReactionWheelController::readCurrentVelocityRPM(int &read_vel)
{
    std::string raw_vel_value_;
    std::stringstream temp_sstring_;
    int temp_vel_;
    if(readMotorRegister(motor_vel_read_register_, raw_vel_value_))
    {
        // Remove Double Quotes from received string
        if (raw_vel_value_.at(0) == '"')
        {
            raw_vel_value_.erase(0,1);
            raw_vel_value_.erase(raw_vel_value_.size()-1);
        }
        // Convert string to int
        temp_sstring_ << std::hex << raw_vel_value_;
        temp_sstring_ >> temp_vel_;
        read_vel = static_cast<int>(temp_vel_);
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

        std::string cmd_vel_string_ = toHexString32Bit(cmd_vel);

        // Testing Code: Print Hex Command Instead of sending.
        std::cout << "Writing to register: " << motor_vel_cmd_register_ << std::endl;
        std::cout << "Writing Value: " << cmd_vel_string_ << std::endl;
        return true;

        // Send Commands.
        // if (writeMotorRegister(motor_vel_cmd_register_, cmd_vel_string_)) return true;
        // else return false;
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
    else return false;
}

std::string ReactionWheelController::toHexString32Bit(int i )
{
  std::stringstream stream;
  stream << std::setfill ('0') << std::setw(8)
         << std::uppercase << std::hex << i;
  return stream.str();
}

std::string ReactionWheelController::toHexString16Bit(int i )
{
  std::stringstream stream;
  stream << std::setfill ('0') << std::setw(4)
         << std::uppercase << std::hex << i;
  return stream.str();
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
            if (writeMotorRegister(torque_mode_regster_, enable_torque_cmd_))
            {
                std::cout << "Torque Mode Enabled." << std::endl;
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
            if (writeMotorRegister(torque_mode_regster_, disable_torque_cmd_))
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
            std::cout << "Command exceeded RPM limit. Commanding Max RPM: " << max_torque << std::endl;
            }
            else if (cmd_torque < -max_torque)
            {
            cmd_torque = -max_torque;
            std::cout << "Command exceeded RPM limit. Commanding Min RPM: " << -max_torque << std::endl;
            }

            // Find Percentage of Max Torque Commanded as that is the motor torque unit
            float torque_cmd_percentage_ = (cmd_torque / max_torque) * 100; 
            
            // Motor Cmd is "tenths of a percent of the rated torque"
            // This object is calculated as thousandths of the torque, e.g., the value "500" means
            // "50%" of the rated torque; "1100" is equivalent to 110%. The rated torque corresponds
            // to the rated current in object 203Bh:01.
            float cmd_torque_to_send_ = torque_cmd_percentage_ * 10;
            
            // Testing Code: Print Hex Command Instead of sending.
            std::cout << "Writing to register: " << motor_torque_cmd_register_ << std::endl;
            std::cout << "Writing Value: " << toHexString16Bit(cmd_torque_to_send_) << std::endl;
            return true;

            // Send Commands.
            // if (writeMotorRegister(motor_torque_cmd_register_, toHexString16Bit(cmd_torque_to_send_))) return true;
            // else return false;
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