#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include <cmath>

using namespace std::chrono_literals;

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    auto node = rclcpp::Node::make_shared("isosceles");

    // Parámetros
    node->declare_parameter("angular_speed", 0.5);
    node->declare_parameter("linear_speed", 1.0);
    node->declare_parameter("base_length", 1.0);
    node->declare_parameter("lateral_length", 2.0);

    double v = node->get_parameter("linear_speed").as_double();
    double w = node->get_parameter("angular_speed").as_double();
    double base = node->get_parameter("base_length").as_double();
    double lado = node->get_parameter("lateral_length").as_double();

    auto pub = node->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);
    geometry_msgs::msg::Twist vel;
    rclcpp::WallRate loop_rate(10ms);

    // Cálculos de ángulos básicos
    // Usamos el teorema del coseno para el ángulo de la punta (gamma)
    double cos_gamma = (2 * lado * lado - base * base) / (2 * lado * lado);
    double angulo_punta = std::acos(cos_gamma);
    // El ángulo de la base es lo que queda para llegar a 180 (PI)
    double angulo_base = (M_PI - angulo_punta) / 2.0;

    // Giros externos que debe hacer la tortuga
    double giro_base = M_PI - angulo_base;
    double giro_punta = M_PI - angulo_punta;

    //LADO 1: LA BASE
    int steps = base / (v * 0.01);
    for(int i=0; i<steps && rclcpp::ok(); i++){
        vel.linear.x = v; vel.angular.z = 0.0;
        pub.get()->publish(vel);
        loop_rate.sleep();
    }
    // Giro 1
    steps = giro_base / (w * 0.01);
    for(int i=0; i<steps && rclcpp::ok(); i++){
        vel.linear.x = 0.0; vel.angular.z = w;
        pub.get()->publish(vel);
        loop_rate.sleep();
    }

    //LADO 2: LATERAL
    steps = lado / (v * 0.01);
    for(int i=0; i<steps && rclcpp::ok(); i++){
        vel.linear.x = v; vel.angular.z = 0.0;
        pub.get()->publish(vel);
        loop_rate.sleep();
    }
    // Giro 2 (La punta)
    steps = giro_punta / (w * 0.01);
    for(int i=0; i<steps && rclcpp::ok(); i++){
        vel.linear.x = 0.0; vel.angular.z = w;
        pub.get()->publish(vel);
        loop_rate.sleep();
    }

    //LADO 3: LATERAL
    steps = lado / (v * 0.01);
    for(int i=0; i<steps && rclcpp::ok(); i++){
        vel.linear.x = v; vel.angular.z = 0.0;
        pub.get()->publish(vel);
        loop_rate.sleep();
    }

    // Parar
    vel.linear.x = 0.0; vel.angular.z = 0.0;
    pub.get()->publish(vel);

    rclcpp::shutdown();
    return 0;
}
