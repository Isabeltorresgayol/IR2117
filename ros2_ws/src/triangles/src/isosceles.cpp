#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

using namespace std::chrono_literals;

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    
    auto node = rclcpp::Node::make_shared("equilateral");
    
    //añadimos los parámetros
    node->declare_parameter("angular_speed", 0.5);
    node->declare_parameter("linear_speed", 1.0);
    node->declare_parameter("side_length", 1.0);
    node->declare_parameter("lateral_length", 2.0);

    double angular_speed=node->get_parameter("angular_speed").as_double();
    double linear_speed=node->get_parameter("linear_speed").as_double();
    double side_length=node->get_parameter("side_length").as_double();
    double lateral_length= node->get_parameter("lateral_length").as_double();
    
    // Iteraciones para cada lado
    int base_iterations = base_length / (loop_period * linear_speed);
    int lateral_iterations = lateral_length / (loop_period * linear_speed);

    // Ángulos
    double angle_base = M_PI / 3.0;        // 60°
    double angle_lateral = 2 * M_PI / 3.0; // 120°

    int angle_base_iter = angle_base / (loop_period * angular_speed);
    int angle_lateral_iter = angle_lateral / (loop_period * angular_speed);
    
    auto publisher =
        node->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);

    geometry_msgs::msg::Twist message;

    rclcpp::WallRate loop_rate(10ms);
    
    //Triángulo completo: 
    for (int j = 0; j < 3; j++)
    {
        //LADO
        int i = 0;
        int n = linear_iterations;

        while (rclcpp::ok() && i < n) {
            i++;

            message.linear.x = linear_speed;
            message.angular.z = 0.0;

            publisher->publish(message);
            rclcpp::spin_some(node);
            loop_rate.sleep();
        }

        //GIRO
        i = 0;
        n = angular_iterations;

        while (rclcpp::ok() && i < n) {
            i++;

            message.linear.x = 0.0;
            message.angular.z = angular_speed;

            publisher->publish(message);
            rclcpp::spin_some(node);
            loop_rate.sleep();
        }
    }

    //parar al final
    message.linear.x = 0.0;
    message.angular.z = 0.0;
    publisher->publish(message);

    rclcpp::shutdown();
    return 0;
}
