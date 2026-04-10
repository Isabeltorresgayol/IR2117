#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "example_interfaces/msg/bool.hpp"


using namespace std::chrono_literals;

//Variables
bool front_obstacle = false;
bool left_obstacle = false;
bool right_obstacle = false;

//Estados
enum State {
  FORWARD,
  TURN_LEFT,
  TURN_RIGHT,
  STOP
};
State current_state = FORWARD;

//Callbacks vacíos by the moment
void callback_front(const example_interfaces::msg::Bool::SharedPtr msg) {
front_obstacle = msg->data;
}

void callback_left(const example_interfaces::msg::Bool::SharedPtr msg) {
left_obstacle = msg->data;
}

void callback_right(const example_interfaces::msg::Bool::SharedPtr msg) {
right_obstacle = msg->data;
}

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  auto node = rclcpp::Node::make_shared("avoidance");
  auto publisher = node->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);
  
   // Subscribers
  auto subs_front = node->create_subscription<example_interfaces::msg::Bool>(
    "/front/obstacle", 10, callback_front);

  auto subs_left = node->create_subscription<example_interfaces::msg::Bool>(
    "/left/obstacle", 10, callback_left);

  auto subs_right = node->create_subscription<example_interfaces::msg::Bool>(
    "/right/obstacle", 10, callback_right);

  geometry_msgs::msg::Twist message;
  rclcpp::WallRate loop_rate(50ms);

  while (rclcpp::ok()) {
    rclcpp::spin_some(node);
    
    //TRANSICIONES (FSA)
    if (front_obstacle) {
      current_state = STOP;
    } else if (left_obstacle) {
      current_state = TURN_RIGHT;
    } else if (right_obstacle) {
      current_state = TURN_LEFT;
    } else {
      current_state = FORWARD;
    }

    // ACCIONES (FSA)
    switch (current_state) {
      case FORWARD:
        message.linear.x = 0.2;
        message.angular.z = 0.0;
        break;

      case TURN_LEFT:
        message.linear.x = 0.0;
        message.angular.z = 0.5;
        break;

      case TURN_RIGHT:
        message.linear.x = 0.0;
        message.angular.z = -0.5;
        break;

      case STOP:
        message.linear.x = 0.0;
        message.angular.z = 0.0;
        break;
    }

    publisher->publish(message);
    loop_rate.sleep();
  }
  
  rclcpp::shutdown();
  return 0;
}
