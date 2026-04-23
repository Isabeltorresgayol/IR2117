#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "turtlesim/srv/set_pen.hpp"

using namespace std::chrono_literals;

class RingsNode : public rclcpp::Node
{
public:
  RingsNode() : Node("rings")
  {
    // Parámetro del radio
    this->declare_parameter("radius", 1.0);

    // Publisher de velocidad
    publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);

    // Cliente del servicio set_pen
    client_ = this->create_client<turtlesim::srv::SetPen>("/turtle1/set_pen");

    // Llamamos al servicio una vez al inicio
    set_pen();

    // Timer
    timer_ = this->create_wall_timer(
      500ms, std::bind(&RingsNode::timer_callback, this));
  }

private:
  void set_pen()
  {
    // Esperar a que el servicio esté disponible
    while (!client_->wait_for_service(1s)) {
      if (!rclcpp::ok()) {
        RCLCPP_ERROR(this->get_logger(), "Interrumpido esperando el servicio");
        return;
      }
      RCLCPP_INFO(this->get_logger(), "Esperando servicio /turtle1/set_pen...");
    }

    auto request = std::make_shared<turtlesim::srv::SetPen::Request>();

    // Configuración del lápiz
    request->r = 255;   // rojo
    request->g = 0;
    request->b = 0;
    request->width = 5;
    request->off = 0;   // 0 = dibuja, 1 = no dibuja

    client_->async_send_request(request);
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

    RCLCPP_INFO(this->get_logger(), "radius = %f", radius);
  }

  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
  rclcpp::Client<turtlesim::srv::SetPen>::SharedPtr client_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<RingsNode>());
  rclcpp::shutdown();
  return 0;
}
