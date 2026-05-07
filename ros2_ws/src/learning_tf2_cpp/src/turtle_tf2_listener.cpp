#include <chrono>
#include <string>
#include <cmath>

#include "geometry_msgs/msg/transform_stamped.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "rclcpp/rclcpp.hpp"
#include "tf2/exceptions.h"
#include "tf2_ros/transform_listener.h"
#include "tf2_ros/buffer.h"

using namespace std::chrono_literals;

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  auto node = rclcpp::Node::make_shared("turtle_tf2_frame_listener");

  // Parámetros dinámicos
  auto target_frame = node->declare_parameter<std::string>("target_frame", "turtle1");
  auto turtle_name = node->declare_parameter<std::string>("turtlename", "turtle2");

  auto publisher = node->create_publisher<geometry_msgs::msg::Twist>(
    turtle_name + "/cmd_vel", 1);

  auto tf_buffer = std::make_unique<tf2_ros::Buffer>(node->get_clock());
  auto tf_listener = std::make_shared<tf2_ros::TransformListener>(*tf_buffer);

  // Corregido: 100ms (10Hz) para evitar error de tipos
  rclcpp::WallRate loop_rate(100ms);

  geometry_msgs::msg::TransformStamped t;
  geometry_msgs::msg::Twist msg;

  while (rclcpp::ok()) {
    try {
      t = tf_buffer->lookupTransform(
        turtle_name, target_frame,
        tf2::TimePointZero);

      msg.angular.z = 1.0 * atan2(t.transform.translation.y, t.transform.translation.x);
      msg.linear.x = 0.5 * sqrt(pow(t.transform.translation.x, 2) + pow(t.transform.translation.y, 2));
      
      publisher->publish(msg);
    } catch (const tf2::TransformException & ex) {
      // Mensaje silencioso para no llenar la terminal mientras aparecen las tortugas
    }

    rclcpp::spin_some(node);
    loop_rate.sleep();
  }

  rclcpp::shutdown();
  return 0;
}
