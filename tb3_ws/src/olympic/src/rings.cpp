#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "turtlesim/srv/set_pen.hpp"
#include "turtlesim/srv/teleport_absolute.hpp"

using namespace std::chrono_literals;

class RingsNode : public rclcpp::Node
{
public:
  RingsNode() : Node("rings")
  {
    this->declare_parameter("radius", 1.0);

    publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);

    pen_client_ = this->create_client<turtlesim::srv::SetPen>("/turtle1/set_pen");
    teleport_client_ = this->create_client<turtlesim::srv::TeleportAbsolute>("/turtle1/teleport_absolute");

    // Posición inicial del aro
    move_without_drawing(7.5, 7.0, 0.0);

    // Color negro
    set_pen(255, 0, 0, 5, 0);

    timer_ = this->create_wall_timer(
      500ms, std::bind(&RingsNode::timer_callback, this));
  }

private:

  void set_pen(int r, int g, int b, int width, int off)
  {
    while (!pen_client_->wait_for_service(1s)) {
      if (!rclcpp::ok()) return;
    }

    auto request = std::make_shared<turtlesim::srv::SetPen::Request>();
    request->r = r;
    request->g = g;
    request->b = b;
    request->width = width;
    request->off = off;

    pen_client_->async_send_request(request);
  }

  void move_without_drawing(double x, double y, double theta)
  {
    // PARAR movimiento antes de teletransportar
    geometry_msgs::msg::Twist stop;
    stop.linear.x = 0.0;
    stop.angular.z = 0.0;
    publisher_->publish(stop);

    // Apagar lápiz
    set_pen(0, 0, 0, 1, 1);

    while (!teleport_client_->wait_for_service(1s)) {
      if (!rclcpp::ok()) return;
    }

    auto request = std::make_shared<turtlesim::srv::TeleportAbsolute::Request>();
    request->x = x;
    request->y = y;
    request->theta = theta;

    teleport_client_->async_send_request(request);

    // Encender lápiz
    set_pen(255, 0, 0, 5, 0);
  }

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
  }

  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
  rclcpp::Client<turtlesim::srv::SetPen>::SharedPtr pen_client_;
  rclcpp::Client<turtlesim::srv::TeleportAbsolute>::SharedPtr teleport_client_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<RingsNode>());
  rclcpp::shutdown();
  return 0;
}
