#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

using namespace std::chrono_literals;

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  auto node = rclcpp::Node::make_shared("rings");

  // Declaramos el radio
  node->declare_parameter("radius", 1.0);

  // Obtenemos el valor del parámetro
  double radius = node->get_parameter("radius").as_double();

  auto publisher = node->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);
  geometry_msgs::msg::Twist message;
  rclcpp::WallRate loop_rate(500ms);

  double linear_speed = 1.0;

  while (rclcpp::ok()) {
    message.linear.x = linear_speed;
    message.angular.z = linear_speed / radius;  // usamos el radio

    publisher->publish(message);
    rclcpp::spin_some(node);
    loop_rate.sleep();
  }

  rclcpp::shutdown();
  return 0;
}
