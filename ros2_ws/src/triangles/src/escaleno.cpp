#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include <cmath>
#include <chrono>

using namespace std::chrono_literals;

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    auto node = rclcpp::Node::make_shared("scalene");
    
    // Parámetros
    node->declare_parameter("angular_speed", 0.5);
    node->declare_parameter("linear_speed", 1.0);
    node->declare_parameter("a_side_length", 1.5);
    node->declare_parameter("b_side_length", 2.0);
    node->declare_parameter("c_side_length", 2.5);

    double w = node->get_parameter("angular_speed").as_double();
    double v = node->get_parameter("linear_speed").as_double();
    double a = node->get_parameter("a_side_length").as_double();
    double b = node->get_parameter("b_side_length").as_double();
    double c = node->get_parameter("c_side_length").as_double();



    auto publisher = node->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);
    geometry_msgs::msg::Twist message;
    rclcpp::WallRate loop_rate(10ms);

    // Parar
    message.linear.x = 0.0;
    message.angular.z = 0.0;
    publisher->publish(message);

    rclcpp::shutdown();
    return 0;
}
