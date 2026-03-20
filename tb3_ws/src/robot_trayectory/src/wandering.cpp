#include <chrono>
#include <iostream>
#include <algorithm> 
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"

using namespace std::chrono_literals;

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    auto node = rclcpp::Node::make_shared("wandering");

    auto publisher =
        node->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);

    
    float min_left = std::numeric_limits<float>::infinity();
    float min_right = std::numeric_limits<float>::infinity();

    // Subscriber a /scan
    auto subscription = node->create_subscription<sensor_msgs::msg::LaserScan>(
        "/scan", 10,
        [&](const sensor_msgs::msg::LaserScan::SharedPtr msg) {

            auto n = msg->ranges.size();
            if (n >= 360) {

                
                min_left = *std::min_element(
                    msg->ranges.begin(),
                    msg->ranges.begin() + 10
                );

                min_right = *std::min_element(
                    msg->ranges.begin() + 350,
                    msg->ranges.begin() + 360
                );

                
                std::cout << "Min izquierda: " << min_left << std::endl;
                std::cout << "Min derecha: " << min_right << std::endl;
                std::cout << "------------------------" << std::endl;
            }
        }
    );

    geometry_msgs::msg::Twist message;

    message.linear.x = 0.0;
    message.angular.z = 0.0;

    rclcpp::WallRate loop_rate(10ms);

    while (rclcpp::ok()) {

       
        if (min_left >= 1.0 && min_right >= 1.0) {
            // avanzar si no hay obstáculos
            message.linear.x = 0.2;
            message.angular.z = 0.0;
        } else {
            
            message.linear.x = 0.0;

            if (min_left > min_right) {
                message.angular.z = 0.5;  
            } else {
                message.angular.z = -0.5;  
            }
        }

        publisher->publish(message);
        rclcpp::spin_some(node);
        loop_rate.sleep();
    }

    rclcpp::shutdown();
    return 0;
}

