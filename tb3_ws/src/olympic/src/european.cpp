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
    publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);

    pen_client_ = this->create_client<turtlesim::srv::SetPen>("/turtle1/set_pen");
    teleport_client_ = this->create_client<turtlesim::srv::TeleportAbsolute>("/turtle1/teleport_absolute");
    
    //POSICIÓN INICIAL V4
    move_without_drawing(6.0, 5.5, 0.0);

    timer_ = this->create_wall_timer(
      500ms, std::bind(&EuropeanNode::timer_callback, this));
  }

private:

  void set_pen(int r, int g, int b, int width, int off){
  while (!pen_client_->wait_for_service(1s)) {}

    auto request = std::make_shared<turtlesim::srv::SetPen::Request>();
    request->r = r;
    request->g = g;
    request->b = b;
    request->width = width;
    request->off = off;

    pen_client_->async_send_request(request);
  }
  
  //Mover sin dibujar
  void move_without_drawing(double x, double y, double theta){
	  publisher_->publish(geometry_msgs::msg::Twist());
	    rclcpp::sleep_for(150ms);

	    set_pen(0, 0, 0, 1, 1);

	    while (!teleport_client_->wait_for_service(1s)) {}

	    auto request = std::make_shared<turtlesim::srv::TeleportAbsolute::Request>();
	    request->x = x;
	    request->y = y;
	    request->theta = theta;

	    teleport_client_->async_send_request(request);
	    
	    //amarillo para las estrellas
	    set_pen(255, 255, 0, 5, 0);
  }
  
  //EN LA V5 QUITAMOS EL draw_circle, y lo sustituimos por esta función que sí dibuja estrellas reales
  void draw_star(double x, double y, double size)
  {
  set_pen(255, 215, 0, 3, 0); // amarillo UE

	  for (int i = 0; i < 5; i++)
	  {
	    double angle1 = i * 2 * M_PI / 5;
	    double angle2 = (i + 2) * 2 * M_PI / 5;

	    double x1 = x + size * cos(angle1);
	    double y1 = y + size * sin(angle1);

	    double x2 = x + size * 0.4 * cos(angle2);
	    double y2 = y + size * 0.4 * sin(angle2);

	    move_without_drawing(x1, y1, 0.0);

	    geometry_msgs::msg::Twist msg;
	    msg.linear.x = 1.0;
	    publisher_->publish(msg);

	    rclcpp::sleep_for(50ms);

	    move_without_drawing(x2, y2, 0.0);
	  }

  publisher_->publish(geometry_msgs::msg::Twist());
  }
  //AÑADIDO PARA LA EUROPEAN FLAG : ESTRELLITAS EN EL CÍRCULO
	void draw_european_flag_v5()
	{
	  int n = 12;
	  double center_x = 6.0;
	  double center_y = 5.5;
	  double radius = 2.0;
	  
	  //ESTRELLAS UNA POR UNA
	  for (int i = 0; i < n; i++)
	  {
	    double angle = i * 2 * M_PI / n;

	    double x = center_x + radius * cos(angle);
	    double y = center_y + radius * sin(angle);
	    
	    rclcpp::sleep_for(200ms);
	    draw_star(x, y, 0.4);

	    move_without_drawing(x, y, angle + M_PI/2);
	  }
	}
  //callback
  void timer_callback()
  {
    static bool done = false;

    if (!done)
    {
      draw_european_flag_v5();
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
