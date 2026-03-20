#include <chrono>
#include <iostream>
#include <algorithm> // std::min_element
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

    //V5: variable para guardar la mínima distancia detectada
    float min_distance = std::numeric_limits<float>::infinity();

    // Subscriber a /scan
    auto subscription = node->create_subscription<sensor_msgs::msg::LaserScan>(
        "/scan", 10,
        [&](const sensor_msgs::msg::LaserScan::SharedPtr msg) { 
            auto n = msg->ranges.size();
            if (n >= 360) {
                std::vector<float> front_back;
                front_back.insert(front_back.end(), msg->ranges.begin(), msg->ranges.begin() + 10);
                front_back.insert(front_back.end(), msg->ranges.begin() + 350, msg->ranges.begin() + 360);

                
                min_distance = *std::min_element(front_back.begin(), front_back.end());

                
                std::cout << "Mínimo distancia (0..9 + 350..359): " << min_distance << std::endl; 
                std::cout << "------------------------" << std::endl;
            }
        }
    );

    geometry_msgs::msg::Twist message;

    message.linear.x = 0.0;
    message.angular.z = 0.0;

    rclcpp::WallRate loop_rate(10ms);

    while (rclcpp::ok()) {
        
        if (min_distance >= 1.0) {       //si hay espacio, avanza
            message.linear.x = 0.2;      //velocidad hacia delante
            message.angular.z = 0.0;
        } else {                         //si obstáculo a <1m, para
            message.linear.x = 0.0;
            message.angular.z = 0.0;
        }

        publisher->publish(message);
        rclcpp::spin_some(node);
        loop_rate.sleep();
    }

    rclcpp::shutdown();
    return 0;
}

