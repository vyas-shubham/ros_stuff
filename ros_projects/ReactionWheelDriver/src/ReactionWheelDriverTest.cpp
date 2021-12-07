#include <iostream>
#include <chrono>
#include <thread>

#include "include/ReactionWheelDriver.hpp"


int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    std::cout << ReactionWheelController::DEFAULT_MOTOR_ADDRESS << std::endl;
    ReactionWheelController motorController(ReactionWheelController::DEFAULT_MOTOR_ADDRESS);

    std::cout << "Successful Initialization!" << std::endl;

    if (motorController.enableReactionWheel())
    {
        std::cout << "Reaction Wheel Enabled!" << std::endl;
        int vel=5;
        motorController.readCurrentVelocityRPM(vel);
        std::cout << "Current Velocity (rpm): " << vel << std::endl;
        
        // int cmd_rpm = 50;
        // // std::cout << "Sleeping for 5 seconds.... " << std::endl;
        // // std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        // // motorController.readCurrentVelocityRPM(vel);
        // // std::cout << "Current Velocity (rpm): " << vel << std::endl;
        // std::string temp_string_input;
        // std::cout << "Send 20 Commands to Motor...." << std::endl;
        // int counter = 0;
        // while (counter < 20)
        // {
        //     std::getline(std::cin, temp_string_input);
        //     if (temp_string_input == "")
        //     {
        //         motorController.readCurrentVelocityRPM(vel);
        //         std::cout << "Current Velocity (rpm): " << vel << std::endl;
        //     }
        //     else
        //     {
        //         cmd_rpm = std::stoi(temp_string_input);
        //         if(!motorController.sendVelocityCommandRPM(cmd_rpm)) std::cout << "Write failed!!" << std::endl;
        //         else std::cout << "Written " << cmd_rpm << " rpm." << std::endl;
        //     }
        //     counter++;
        // }
        
        if (motorController.disableReactionWheel()) std::cout << "Reaction Wheel Disabled!" << std::endl;
        else std::cout << "Failed to Disable reaction wheel!!" << std::endl; 
    }
    else std::cout << "Failed to enable reaction wheel!!" << std::endl;
    
    return 0;

}