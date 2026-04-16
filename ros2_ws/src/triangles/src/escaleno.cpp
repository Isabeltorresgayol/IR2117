#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include <cmath>
#include <chrono>

using namespace std::chrono_literals;

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    auto node = rclcpp::Node::make_shared("scalene");
    
    node->declare_parameter("angular_speed", 0.5);
    node->declare_parameter("linear_speed", 1.0);
    node->declare_parameter("a_side_length", 1.5);
    node->declare_parameter("b_side_length", 2.0);
    node->declare_parameter("c_side_length", 2.5);

    double vl = node->get_parameter("linear_speed").as_double();
    double va = node->get_parameter("angular_speed").as_double();
    double la = node->get_parameter("a_side_length").as_double();
    double lb = node->get_parameter("b_side_length").as_double();
    double lc = node->get_parameter("c_side_length").as_double();

    double A = std::acos((lb*lb + lc*lc - la*la) / (2.0*lb*lc));
    double B = std::acos((la*la + lc*lc - lb*lb) / (2.0*la*lc));
    double C = M_PI - A - B;

    auto pub = node->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);
    geometry_msgs::msg::Twist msg;
    rclcpp::WallRate loop_rate(10ms);

    // LADO A
    int n1 = la / (0.01 * vl);
    for (int i = 0; i < n1 && rclcpp::ok(); i++) {
        msg.linear.x = vl; msg.angular.z = 0.0;
        pub->publish(msg);
        loop_rate.sleep();
    }
    int g1 = (M_PI - C) / (0.01 * va);
    for (int i = 0; i < g1 && rclcpp::ok(); i++) {
        msg.linear.x = 0.0; msg.angular.z = va;
        pub->publish(msg);
        loop_rate.sleep();
    }

    // LADO B
    int n2 = lb / (0.01 * vl);
    for (int i = 0; i < n2 && rclcpp::ok(); i++) {
        msg.linear.x = vl; msg.angular.z = 0.0;
        pub->publish(msg);
        loop_rate.sleep();
    }
    int g2 = (M_PI - A) / (0.01 * va);
    for (int i = 0; i < g2 && rclcpp::ok(); i++) {
        msg.linear.x = 0.0; msg.angular.z = va;
        pub->publish(msg);
        loop_rate.sleep();
    }

    // LADO C
    int n3 = lc / (0.01 * vl);
    for (int i = 0; i < n3 && rclcpp::ok(); i++) {
        msg.linear.x = vl; msg.angular.z = 0.0;
        pub->publish(msg);
        loop_rate.sleep();
    }

    msg.linear.x = 0.0;
    msg.angular.z = 0.0;
    pub->publish(msg);

    rclcpp::shutdown();
    return 0;
}
