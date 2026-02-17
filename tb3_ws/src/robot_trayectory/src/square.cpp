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
    rclcpp::WallRate loop_rate(10ms);

    int i = 0;
    int n = 1000;   // numero de iteraciones para 1 metro

    while (rclcpp::ok() && i < n) {
        i++;

        // Velocidad lineal en x
        message.linear.x = 0.1;

        // Sin giro
        message.angular.z = 0.0;

        publisher->publish(message);

        rclcpp::spin_some(node);
        loop_rate.sleep();
    }

    rclcpp::shutdown();
    return 0;
}

