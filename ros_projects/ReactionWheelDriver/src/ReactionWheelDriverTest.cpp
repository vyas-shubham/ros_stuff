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

        // Increase Velocity to high RPM
        // std::cout << "Increasing Velocity to 1000 RPM..." << std::endl;
        // int loop_wait_time = 2000;
        // for (int i=0;i<10;i++)
        // {
        //     // std::cout << i << std::endl;
        //     int cmd_rpm_ = (i+1)*100;
        //     std::cout << "Commanding RPM: " << cmd_rpm_ << std::endl;
        //     if (!motorController.sendVelocityCommandRPM(cmd_rpm_)) std::cout << 
        //                                         "Command Failed!!" << std::endl;
        //     std::this_thread::sleep_for(std::chrono::milliseconds(loop_wait_time));
        //     motorController.readCurrentVelocityRPM(vel);
        //     std::cout << "Current Velocity (rpm): " << vel << std::endl; 
        // }

        // Positive Negative Velocity Cycle
        // int cmd_rpm_ = 100;
        // std::cout << "Commanding RPM: " << cmd_rpm_ << std::endl;
        // if (!motorController.sendVelocityCommandRPM(cmd_rpm_)) std::cout << 
        //                                     "Command Failed!!" << std::endl;
        // std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        // motorController.readCurrentVelocityRPM(vel);
        // std::cout << "Current Velocity (rpm): " << vel << std::endl; 

        // cmd_rpm_ = -100;
        // std::cout << "Commanding RPM: " << cmd_rpm_ << std::endl;
        // if (!motorController.sendVelocityCommandRPM(cmd_rpm_)) std::cout << 
        //                                     "Command Failed!!" << std::endl;
        // std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        // motorController.readCurrentVelocityRPM(vel);
        // std::cout << "Current Velocity (rpm): " << vel << std::endl;

        // Clear Faults
        // std::cout << "Trying to Clear Faults... " << std::endl;
        // if (!motorController.faultReset()) std::cout << "Fault Rest Command Send Failed" << std::endl;
        // std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        // Torque Tests
        std::cout << "---------------------------------------------------------"<< std::endl;
        int tau_ = 5;
        int motor_status = 1;
        if (!motorController.readMotorStatus(motor_status)) std::cout << "Could not Read Motor Status!" << std::endl;
        std::cout << "Motor Status: " << motor_status << std::endl;
        if (!motorController.enableTorqueMode()) std::cout << "Could not Enable Torque Mode!" << std::endl;
        if (!motorController.sendTorqueCommand(0.5)) std::cout << "Could Not command torque." << std::endl;
        else std::cout <<"Commanded 0.5 Nm..." << std::endl;
        if (!motorController.readMotorStatus(motor_status)) std::cout << "Could not Read Motor Status!" << std::endl;
        std::cout << "Motor Status: " << motor_status << std::endl;
        std::cout << "---------------------------------------------------------"<< std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        motorController.readCurrentVelocityRPM(vel);
        motorController.currentTorqueDemand(tau_);
        if (!motorController.readMotorStatus(motor_status)) std::cout << "Could not Read Motor Status!" << std::endl;
        std::cout << "Motor Status: " << motor_status << std::endl;
        std::cout << "Current Velocity (rpm): " << vel << std::endl; 
        std::cout << "Current Demanded Torque: " << tau_ << std::endl; 
        std::cout << "---------------------------------------------------------"<< std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        motorController.readCurrentVelocityRPM(vel);
        motorController.currentTorqueDemand(tau_);
        if (!motorController.readMotorStatus(motor_status)) std::cout << "Could not Read Motor Status!" << std::endl;
        std::cout << "Motor Status: " << motor_status << std::endl;
        std::cout << "Current Velocity (rpm): " << vel << std::endl;
        std::cout << "Current Demanded Torque: " << tau_ << std::endl; 
        std::cout << "---------------------------------------------------------"<< std::endl;
        if (!motorController.sendTorqueCommand(0.0)) std::cout << "Could Not command torque." << std::endl;
        else std::cout <<"Commanded 0 Nm..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        if (!motorController.disableTorqueMode()) std::cout << "Could not disable Torque Mode!" << std::endl;

        // Go back to Zero RPM
        // std::cout << "Going back to Zero RPM..." << std::endl; 
        // if (!motorController.sendVelocityCommandRPM(0)) std::cout << 
        //                             "Command Failed!!" << std::endl;
        // std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        // motorController.readCurrentVelocityRPM(vel);
        // std::cout << "Current Velocity (rpm): " << vel << std::endl; 

        if (motorController.disableReactionWheel()) std::cout << "Reaction Wheel Disabled!" << std::endl;
        else std::cout << "Failed to Disable reaction wheel!!" << std::endl; 
    }
    else std::cout << "Failed to enable reaction wheel!!" << std::endl;
    
    return 0;
}