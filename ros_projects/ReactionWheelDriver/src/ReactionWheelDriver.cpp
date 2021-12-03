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

    std::string cmd_vel_string_ = toHexString(cmd_vel);

    // Testing Code
    // std::cout << "Writing to register: " << motor_vel_cmd_register_ << std::endl;
    // std::cout << "Writing Value: " << cmd_vel_string_ << std::endl;
    // return true;

    // Send Commands.
    if (writeMotorRegister(motor_vel_cmd_register_, cmd_vel_string_)) return true;
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

std::string ReactionWheelController::toHexString(int n)
{
    std::string s;
        bool negsign = false;
    int result[8]={0,0,0,0,0,0,0,0};
    int remain[8]={0,0,0,0,0,0,0,0};
    if(n < 0 ){
        negsign = true;
        n= -n;
    }
    for(int i = 8 ; i > 0 ; i--){
        remain[i-1] = n % 16;
        n /= 16;
    }
    if(negsign){
        int sub[8]={15,15,15,15,15,15,15,16};
        for(int i = 7 ; i >= 0 ; i--)
            result[i] = sub[i] - remain[i];
    }
    else
        for(int i = 7 ; i >= 0 ; i--)
            result[i] = remain[i];

    for(int j = 0 ; j < 8 ; j++){
        if(result[j] < 10)
            s = s+char(result[j] + 48);//result[j]+ s;
        else
            if(result[j] == 10)
            s = s+ "A";
        else
            if(result[j] == 11)
                s = s+ "B";
        else
            if(result[j] == 12)
            s = s+ "C";
        else
            if(result[j] == 13)
                s = s+"D" ;
        else
            if(result[j] == 14)
                s = s+ "E" ;
        else
            if(result[j] == 15)
                s = s+"F";
    }
    return s;
}//To Hex