#include <iostream>
#include <string>
#include <chrono>

#include "ReactionWheelDriver.hpp"

using namespace std;

using namespace std::chrono;

// Get time stamp in microseconds.
uint64_t get_time_in_microseconds()
{
    uint64_t us = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::
                  now().time_since_epoch()).count();
    return us; 
}



int main(int argc, char **argv)
{
	std::cout << "Hello World" << std::endl;
	std::cout << "X = " << x << std::endl;
	RestClient::Response r = RestClient::get("http://www.google.com");
	std::cout << r.code << std::endl;

	std::string motor_address = "192.168.1.155";
	motor_address.insert(0, "http://");
	motor_address.insert(motor_address.length(), "/od/");

	// std::string cmd_register = "3210/01";
	// std::string cmd_value = "000003E8";
	std::string cmd_register = "2030/00";
	std::string cmd_value = "00000004";
	// std::cout << "Cmd Value: " << cmd_value << std::endl;
	cmd_value = "\"" + cmd_value+"\"";
	// std::cout << "Cmd Value: " << cmd_value << std::endl;
	std::string cmd_string = motor_address + cmd_register;
	std::string content_type = "application/x-www-form-urlencoded";
	
	// std::cout << "Cmd String: " << cmd_string << std::endl;
	// std::cout << "Cmd Value: " << cmd_value << std::endl;

	int numCommands = 1;
	RestClient::Response resp;
	auto startT = get_time_in_microseconds();
	for (int i=0; i<numCommands; i++)
	{
		//  resp = RestClient::get("http://192.168.1.155/od/2005/00");
		 resp = RestClient::get("http://192.168.1.155/od/606C/00");
		// resp = RestClient::get(cmd_string);
		// resp  = RestClient::post(cmd_string, content_type, cmd_value);
	}
	auto endT = get_time_in_microseconds();

	auto dt = endT - startT;
	 if(resp.body.at(0) == '"'){
          resp.body.erase(0,1);
          resp.body.erase(resp.body.size() -1);
        }
	std::cout << "Motor Resp Status Code: " << resp.code << std::endl;
	std::cout << "Motor Resp Body: " << resp.body << std::endl;
	
	std::cout << "Time taken for " << numCommands << " commands: " << dt << " micro-seconds." << std::endl;
	std::cout << "Average Time taken for one command: " << ((float)dt/(float)numCommands)/(float)1000 << " milli-seconds." << std::endl;

	return 0;
	
}
