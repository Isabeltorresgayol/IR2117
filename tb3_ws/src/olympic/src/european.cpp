#include <chrono>
#include <cmath>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "turtlesim/srv/set_pen.hpp"
#include "turtlesim/srv/teleport_absolute.hpp"


using namespace std::chrono_literals;

class EuropeanNode : public rclcpp::Node
{
public:
  EuropeanNode() : Node("european")
  {
    this->declare_parameter("radius", 1.0);

    publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);

    pen_client_ = this->create_client<turtlesim::srv::SetPen>("/turtle1/set_pen");
    teleport_client_ = this->create_client<turtlesim::srv::TeleportAbsolute>("/turtle1/teleport_absolute");

    // Posición inicial
    move_without_drawing(6.5, 5.8, 0.0);
    set_pen(0, 255, 0, 5, 0);

    timer_ = this->create_wall_timer(
      500ms, std::bind(&EuropeanNode::timer_callback, this));
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

  //AÑADIDO PARA LA EUROPEAN FLAG V1
	void draw_european_flag_v1()
	{
	  int n = 12;
	  double center_x = 6.0;
	  double center_y = 5.5;
	  double big_radius = 2.5;   // radio del círculo donde van las estrellas
	  double small_radius = 0.3; // tamaño de cada estrella (círculo)

	  for (int i = 0; i < n; i++)
	  {
	    double angle = i * 2 * M_PI / n;

	    double x = center_x + big_radius * cos(angle);
	    double y = center_y + big_radius * sin(angle);

	    move_without_drawing(x, y, 0.0);

	    // amarillo
	    set_pen(255, 255, 0, 3, 0);

	    draw_circle(small_radius);
	  }
	}
  //MODIFICADO: se ejecuta solo una vez
  void timer_callback()
  {
    static bool done = false;

    if (!done)
    {
      draw_european_flag_v1();
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
  rclcpp::spin(std::make_shared<EuropeanNode>());
  rclcpp::shutdown();
  return 0;
}
