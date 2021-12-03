#include "reactionwheelnode.hpp"
#include <assert.h>

ReactionWheelController::ReactionWheelController(std::string motor_address=DEFAULT_MOTOR_ADDRESS) : motor_address_{motor_address} 
{
    // Prepare URL from IP Address for REST/HTTP Communication
    motor_address_.insert(0, "http://");
    motor_address_.insert(motor_address_.length(), "/od/");

    for (auto paramIter=motor_setup_params.begin(); paramIter=motor_setup_params.end(); paramIter++)
    {
        auto param_register_ = paramIter->first;
        auto param_value_ = paramIter->second;
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
            isMotorEnabled = true;
            return true;
        }
        else
        {
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
            isMotorEnabled = false;
            return true;
        }
        else return false;
    }
    
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
    RestClient::Response resp_  = RestClient::post(write_address, content_type, cmd_value);
    if(resp_.code==200) return true;
    else return false;
}

int main(int argc, char ** argv)
{
    (void) argc;
    (void) argv;
    std::cout << "Hello World" << std::endl;
    RestClient::Response r = RestClient::get("http://www.google.com");
	std::cout << r.code << std::endl;

    ReactionWheelController rw_controller;
    std::cout << rw_controller.motor_address_ << std::endl;

}