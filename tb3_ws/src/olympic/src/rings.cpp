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
    // Parámetro del radio
    this->declare_parameter("radius", 1.0);

    // Publisher de velocidad
    publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);

    // Clientes de servicios
    pen_client_ = this->create_client<turtlesim::srv::SetPen>("/turtle1/set_pen");
    teleport_client_ = this->create_client<turtlesim::srv::TeleportAbsolute>("/turtle1/teleport_absolute");

    // Secuencia inicial
    move_without_drawing(5.0, 5.0, 0.0); // mover al centro
    set_pen(0, 0, 255, 5, 0); // azul

    // Timer
    timer_ = this->create_wall_timer(
      500ms, std::bind(&RingsNode::timer_callback, this));
  }

private:

  //Servicio set_pen
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

  //Teleport SIN dibujar
  void move_without_drawing(double x, double y, double theta)
  {
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

    // Encender lápiz otra vez (azul)
    set_pen(0, 0, 255, 5, 0);
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
