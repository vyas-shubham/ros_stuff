#include <iostream>

#include "ReactionWheelDrive.hpp"


int main(int argc, char **argv)
{
	std::cout << "Hello World" << std::endl;
	std::cout << "X = " << x << std::endl;
	std::cout << "Motor Speed = " << motor_speed << std::endl;
	RestClient::Response r = RestClient::get("http://www.google.com");
	std::cout << r.code << std::endl;
}
