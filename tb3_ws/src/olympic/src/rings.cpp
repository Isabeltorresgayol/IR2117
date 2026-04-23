#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

using namespace std::chrono_literals;

class RingsNode : public rclcpp::Node
{
public:
  RingsNode() : Node("rings")
  {
    // Declararamos el parámetro (en la V5.1 ya lo hemos hecho)
    this->declare_parameter("radius", 1.0);

    publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);

    // Timer para publicar periódicamente
    timer_ = this->create_wall_timer(
      500ms, std::bind(&RingsNode::timer_callback, this));
  }

private:
  void timer_callback()
  {
    double radius = this->get_parameter("radius").as_double();

    if (radius <= 0.0) {
      RCLCPP_WARN(this->get_logger(), "El radio debería ser > 0. Using 1.0");
      radius = 1.0;
    }

    geometry_msgs::msg::Twist msg;

    double linear_speed = 1.0;
    msg.linear.x = linear_speed;
    msg.angular.z = linear_speed / radius;

    publisher_->publish(msg);

    RCLCPP_INFO(this->get_logger(), "radius = %f", radius);
  }

  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<RingsNode>());
  rclcpp::shutdown();
  return 0;
}
