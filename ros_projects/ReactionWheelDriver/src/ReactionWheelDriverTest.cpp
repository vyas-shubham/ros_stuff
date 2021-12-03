#include <iostream>
#include <chrono>
#include <thread>

#include "include/ReactionWheelDriver.hpp"


int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    std::cout << "Hello World" << std::endl;

    // std::string DEFAULT_MOTOR_ADDRESS = "192.168.1.55";
    std::cout << ReactionWheelController::DEFAULT_MOTOR_ADDRESS << std::endl;
    ReactionWheelController motorController(ReactionWheelController::DEFAULT_MOTOR_ADDRESS);

    std::cout << "Successful Initialization!" << std::endl;

    if (motorController.enableReactionWheel())
    {
        std::cout << "Reaction Wheel Enabled!" << std::endl;
        int vel=5;
        motorController.readCurrentVelocityRPM(vel);
        std::cout << "Current Velocity (rpm): " << vel << std::endl;
        
        int cmd_rpm = 10;
        std::cout << "Writing " << cmd_rpm << " rpm to motor." <<std::endl;
        if(!motorController.sendVelocityCommandRPM(cmd_rpm)) std::cout << "Write failed!!";
        std::cout << "Sleeping for 4 seconds.... " << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(4000));
        motorController.readCurrentVelocityRPM(vel);
        std::cout << "Current Velocity (rpm): " << vel << std::endl;

        cmd_rpm = 0;
        std::cout << "Writing " << cmd_rpm << " rpm to motor." <<std::endl;
        if(!motorController.sendVelocityCommandRPM(cmd_rpm)) std::cout << "Write failed!!";
        std::cout << "Sleeping for 4 seconds.... " << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(4000));
        motorController.readCurrentVelocityRPM(vel);
        std::cout << "Current Velocity (rpm): " << vel << std::endl;
        
        if (motorController.disableReactionWheel()) std::cout << "Reaction Wheel Disabled!" << std::endl;
        else std::cout << "Failed to Disable reaction wheel!!" << std::endl; 
    }
    else std::cout << "Failed to enable reaction wheel!!" << std::endl;
    
    return 0;

}