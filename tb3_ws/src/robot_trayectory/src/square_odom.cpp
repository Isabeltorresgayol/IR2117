#include <chrono>
#include <cmath>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "nav_msgs/msg/odometry.hpp"

using namespace std::chrono_literals;

// Variables globales
double x = 0.0;
double y = 0.0;
double theta = 0.0;   // V4

// Callback odom
void odom_callback(const nav_msgs::msg::Odometry::SharedPtr msg)
{
    // Posición
    x = msg->pose.pose.position.x;
    y = msg->pose.pose.position.y;

    // Quaternion
    double qx = msg->pose.pose.orientation.x;
    double qy = msg->pose.pose.orientation.y;
    double qz = msg->pose.pose.orientation.z;
    double qw = msg->pose.pose.orientation.w;

    // Convertir a yaw (theta)
    theta = atan2(2.0 * (qw * qz + qx * qy),
                  1.0 - 2.0 * (qy * qy + qz * qz));
}

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    
    auto node = rclcpp::Node::make_shared("square_odom");
    
    node->declare_parameter("linear_speed", 0.1);
    node->declare_parameter("angular_speed", 0.5);
    node->declare_parameter("square_length", 1.0);

    double linear_speed = node->get_parameter("linear_speed").as_double();
    double angular_speed = node->get_parameter("angular_speed").as_double();
    double square_length = node->get_parameter("square_length").as_double();
    
    double distance = square_length;
    double angle = M_PI / 2.0;
    double loop_period = 0.01;

    int linear_iterations = distance / (loop_period * linear_speed);
    int angular_iterations = angle / (loop_period * angular_speed);

    auto publisher =
        node->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);

    auto sub = node->create_subscription<nav_msgs::msg::Odometry>(
        "/odom", 10, odom_callback);

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

            // Mostrar variables
            RCLCPP_INFO(node->get_logger(),
                        "x: %f y: %f theta: %f", x, y, theta);

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

            RCLCPP_INFO(node->get_logger(),
                        "x: %f y: %f theta: %f", x, y, theta);

            loop_rate.sleep();
        }
    }

    message.linear.x = 0.0;
    message.angular.z = 0.0;
    publisher->publish(message);

    rclcpp::shutdown();
    return 0;
}
