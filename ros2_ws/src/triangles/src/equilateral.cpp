#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

using namespace std::chrono_literals;

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    
    auto node = rclcpp::Node::make_shared("equilateral");
    
    //añadimos los parámetros
    node->declare_parameter("angular_speed", 0.5);
    node->declare_parameter("linear_speed", 1.0);
    node->declare_parameter("side_length", 2.0);

    double angular_speed=node->get_parameter("angular_speed").as_double();
    double linear_speed=node->get_parameter("linear_speed").as_double();
    double side_length=node->get_parameter("side_length").as_double();
    
    double distance = side_length;
    double loop_period = 0.01;
    int linear_iterations = distance / (loop_period * linear_speed);

    auto publisher =
        node->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);

    geometry_msgs::msg::Twist message;

    rclcpp::WallRate loop_rate(10ms);
    int i = 0;
    
    while (rclcpp::ok() && i < linear_iterations) {
	    i++;

	    message.linear.x = linear_speed;
	    message.angular.z = 0.0;

	    publisher->publish(message);
	    rclcpp::spin_some(node);
	    loop_rate.sleep();
	}



    rclcpp::shutdown();
    return 0;
}
