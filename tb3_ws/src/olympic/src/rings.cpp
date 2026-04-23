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

    // Posición inicial
    move_without_drawing(6.5, 5.8, 0.0);
    set_pen(0, 255, 0, 5, 0);

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
    publisher_->publish(geometry_msgs::msg::Twist());
    rclcpp::sleep_for(150ms);

    set_pen(0, 0, 0, 1, 1);

    while (!teleport_client_->wait_for_service(1s)) {
      if (!rclcpp::ok()) return;
    }

    auto request = std::make_shared<turtlesim::srv::TeleportAbsolute::Request>();
    request->x = x;
    request->y = y;
    request->theta = theta;

    teleport_client_->async_send_request(request);

    set_pen(0, 255, 0, 5, 0);
  }

  //AÑADIDO: dibujar círculo completo
  void draw_circle(double radius)
  {
    geometry_msgs::msg::Twist msg;
    msg.linear.x = 1.0;
    msg.angular.z = 1.0 / radius;

    double time = 2 * M_PI * radius;

    auto start = this->now();
    rclcpp::Rate rate(30);

    while ((this->now() - start).seconds() < time) {
      publisher_->publish(msg);
      rate.sleep();
    }

    publisher_->publish(geometry_msgs::msg::Twist());
  }

  //AÑADIDO: los 5 aros con for
  void draw_olympic_rings()
  {
    double x[5] = {5.0, 7.0, 9.0, 6.0, 8.0};
    double y[5] = {6.0, 6.0, 6.0, 5.0, 5.0};

    int red[5]   = {0, 0, 255, 255, 0};
    int green[5] = {0, 0, 0, 255, 255};
    int blue[5]  = {255, 0, 0, 0, 0};

    double radius = 1.0;

    for (int r = 0; r < 5; r++)
    {
      move_without_drawing(x[r], y[r], 0.0);
      set_pen(red[r], green[r], blue[r], 5, 0);
      draw_circle(radius);
    }
  }

  //MODIFICADO: se ejecuta solo una vez
  void timer_callback()
  {
    static bool done = false;

    if (!done)
    {
      draw_olympic_rings();
      done = true;

      timer_->cancel();  // evita que se repita
    }
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

