#include "reactionwheelnode.hpp"

class ReactionWheelController {
    public:
        ReactionWheelController(std::string motor_address=DEFAULT_MOTOR_ADDRESS) : motor_address_{motor_address} 
        {
            // Prepare URL from IP Address for REST/HTTP Communication
            motor_address_.insert(0, "http://");
            motor_address_.insert(motor_address_.length(), "/od/");
        }
        std::string motor_address_;

        bool sendVelocityCommand(int cmdVel); 
    private:
        int readCurrentVelocity();
        void velocityPublisherCallback();
};

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