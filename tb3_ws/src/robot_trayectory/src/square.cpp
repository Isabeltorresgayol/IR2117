#include <chrono>
#include <cmath>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

using namespace std::chrono_literals;

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    
    auto node = rclcpp::Node::make_shared("square");
    
    
    node->declare_parameter("linear_speed", 0.1);
    node->declare_parameter("angular_speed", 0.5);
    node->declare_parameter("square_length", 1.0);

    double linear_speed = node->get_parameter("linear_speed").as_double();
    double angular_speed = node->get_parameter("angular_speed").as_double();
    double square_length = node->get_parameter("square_length").as_double();
    
    double distance = square_length;        // meters
    //VERSION 3 with 4
    double angle = M_PI / 2.0;    // 90 degrees
    double loop_period = 0.01;    // 10ms

    int linear_iterations = distance / (loop_period * linear_speed);
    int angular_iterations = angle / (loop_period * angular_speed);

    auto publisher =
        node->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);

    geometry_msgs::msg::Twist message;

    rclcpp::WallRate loop_rate(10ms);

    
    for(int j = 0; j < 4; j++)
    {
        int i = 0;
        int n = linear_iterations;  

        
        while (rclcpp::ok() && (i < n)) {
            i++;

            message.linear.x = linear_speed;
            message.angular.z = 0.0;

            publisher->publish(message);
            rclcpp::spin_some(node);
            loop_rate.sleep();
        }

        
        i = 0;
        n = angular_iterations;   

        while (rclcpp::ok() && (i < n)) {
            i++;

            message.linear.x = 0.0;
            message.angular.z = angular_speed;

            publisher->publish(message);
            rclcpp::spin_some(node);
            loop_rate.sleep();
        }
    }

    
    message.linear.x = 0.0;
    message.angular.z = 0.0;
    publisher->publish(message);

    rclcpp::shutdown();
    return 0;
}

