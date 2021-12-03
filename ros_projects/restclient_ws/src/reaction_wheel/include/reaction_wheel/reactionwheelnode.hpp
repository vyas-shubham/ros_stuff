#ifndef REACTION_WHEEL_HPP
#define REACTION_WHEEL_HPP

#include <map>
#include <string>
#include <iostream>
#include <chrono>


#include "rclcpp/rclcpp.hpp"
#include "restclient-cpp/restclient.h"


class ReactionWheelController {
    public:
        ReactionWheelController(std::string motor_address);
        std::string motor_address_;
        bool sendVelocityCommand(int cmdVel); 
        bool enableReactionWheel();
		bool disableReactionWheel();
        const std::string DEFAULT_MOTOR_ADDRESS = "192.168.1.55";
		float max_velocity_rad = 500;  	// 500 rpm max velocity
		// To be made private later. here for testing.
		bool readMotorRegister(std::string readRegister, std::string &readValue);
		bool writeMotorRegister(std::string writeRegister, std::string writeValue);
    private:
		bool isMotorEnabled;
        int readCurrentVelocity();
        void velocityPublisherCallback();
		


	// For understanding Command Dictionary and the set values 
	// see: https://en.nanotec.com/products/manual/N5_CAN_EN/object_dictionary%252Fobject_dir_intro.html
	// All Values are hex strings
	std::map<std::string, std::string> motor_setup_params = 
	{
		{"3210/01", "000003E8"}, // POS P (4bytes): 1000
		{"3210/02", "00000000"}, // POS I (4bytes): 0 
		{"3210/03", "000493E0"}, // VEL P (4bytes): 300000
		{"3210/04", "0000000A"}, // VEL I (4bytes): 10
		{"3210/05", "00000BB8"}, // Flux Current Loop P (4bytes): 3000
		{"3210/06", "0000000A"}, // Flux Current Loop I (4bytes): 10
		{"3210/07", "00000BB8"}, // Torque Loop P (4bytes): 3000
		{"3210/08", "0000000A"}, // Torque Loop I (4bytes): 10

		{"3202/00", "00000041"}, // Motor type (4bytes): BLDC
		{"2030/00", "00000004"}, // Motor Pole Pair (4bytes): 4
		{"608F/01", "00001000"}, // Encoder Increments for user defined inputs (4bytes): 4096 
		{"2052/00", "00001000"}, // Physical Encoder Increments: 4096

		{"2031/00", "00002EE0"}, // Current max mA (4bytes): 12000 
		{"203B/01", "00002EE0"}, // Current nom mA (4bytes): 12000
		{"203B/02", "00000064"}, // Max current duration (4bytes): 100

		{"6072/00", "03E8"},     // Max Torque (2bytes): 1000
		{"6087/00", "000003E8"}, // Torque Slope (4bytes): 1000
		{"60FF/00", "00000000"}, // Set Initial Velocity in User Defined Units (default: rpm) (4bytes): 0
		{"6060/00", "03"},       // Operation Mode (1byte int): 3 (Profile Velocity Mode) 
		// Control Words for Motor State Machine Control (2bytes)
		{"6040/00", "0006"},     // Enable Voltage but don't switch on. Disable Quick Stop. Disable Operation

		// Main Enable/Disable Using Separate Functions Instead of in Motor Setup
		// {"6040/00", "0007"},     // Enable Voltage and Switch on. Disable Quick Stop. Disable Operation
		// {"6040/00", "000F"}      // Enable Voltage, Operation and Switch on. Disable Quick Stop.
	};

	const std::string motor_command_register_ = "6040/00";
	const std::string enable_motor_cmd_ = "000F";
	const std::string disable_motor_cmd_ = "0007";
	const std::string motor_vel_cmd_register_ = "60FF/00";
	const std::string motor_vel_read_register_ = "606C/00";


	std::string toHexString(int n)
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

};

#endif // REACTION_WHEEL_HPP