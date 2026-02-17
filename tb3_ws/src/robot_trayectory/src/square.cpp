#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

using namespace std::chrono_literals; //Preguntar a Fede (duda en la libreta de apuntes)

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    // Nodo llamado "square"
    auto node = rclcpp::Node::make_shared("square");

    // Publisher en /cmd_vel con mensaje Twist 
    auto publisher =
        node->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);

    geometry_msgs::msg::Twist message;

    rclcpp::WallRate loop_rate(500ms);

    while (rclcpp::ok()) {

        // Velocidad lineal en x (las pongo primero así, a lo mejor la svoy cambiando)
        message.linear.x = 0.5;

        // Velocidad angular en z (giro, lo pongo como primera pruebaaa)
        message.angular.z = 0.5;

        publisher->publish(message);

        rclcpp::spin_some(node);
        loop_rate.sleep();
    }

    rclcpp::shutdown();
    return 0;
}
