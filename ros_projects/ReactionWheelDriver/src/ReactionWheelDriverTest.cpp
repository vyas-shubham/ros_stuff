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
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    if (motorController.enableReactionWheel())
    {
        std::cout << "Reaction Wheel Enabled!" << std::endl;
        int vel=5;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        motorController.readCurrentVelocityRPM(vel);
        std::cout << "Current Velocity (rpm): " << vel << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        std::cout << "Increasing Velocity to 500 RPM..." << std::endl;
        int loop_wait_time = 2000;
        for (int i=0;i<10;i++)
        {
            // std::cout << i << std::endl;
            int cmd_rpm_ = (i+1)*50;
            std::cout << "Commanding RPM: " << cmd_rpm_ << std::endl;
            if (!motorController.sendVelocityCommandRPM(cmd_rpm_)) std::cout << 
                                                "Command Failed!!" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(loop_wait_time));
            motorController.readCurrentVelocityRPM(vel);
            std::cout << "Current Velocity (rpm): " << vel << std::endl; 
        }
        // Go back to Zero RPM
        std::cout << "Going back to Zero RPM..." << std::endl; 
        if (!motorController.sendVelocityCommandRPM(0)) std::cout << 
                                    "Command Failed!!" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        motorController.readCurrentVelocityRPM(vel);
        std::cout << "Current Velocity (rpm): " << vel << std::endl; 
        // std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        // Clear Faults
        // std::cout << "Trying to Clear Faults... " << std::endl;
        // if (!motorController.faultReset()) std::cout << "Fault Rest Command Send Failed" << std::endl;
        // std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        if (motorController.disableReactionWheel()) std::cout << "Reaction Wheel Disabled!" << std::endl;
        else std::cout << "Failed to Disable reaction wheel!!" << std::endl; 
    }
    else std::cout << "Failed to enable reaction wheel!!" << std::endl;
    
    return 0;



}