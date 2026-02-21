#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

using namespace std::chrono_literals;

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    
    auto node = rclcpp::Node::make_shared("square");
    
    //Declaración de los parámetros en ROS, VERSIÓN 1
    node->declare_parameter("linear_speed", 0.1);
    node->declare_parameter("angular_speed", 0.5);

    double linear_speed = node->get_parameter("linear_speed").as_double();
    double angular_speed = node->get_parameter("angular_speed").as_double();

    auto publisher =
        node->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);

    geometry_msgs::msg::Twist message;

    rclcpp::WallRate loop_rate(10ms);

    
    for(int j = 0; j < 4; j++)
    {
        int i = 0;
        int n = 1000;  

        
        while (rclcpp::ok() && (i < n)) {
            i++;

            message.linear.x = linear_speed;
            message.angular.z = 0.0;

            publisher->publish(message);
            rclcpp::spin_some(node);
            loop_rate.sleep();
        }

        
        i = 0;
        n = 314;   

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

