#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"

using namespace std::chrono_literals;

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    auto node = rclcpp::Node::make_shared("wandering");

    auto publisher =
        node->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);
        
    // Subscriber a /scan (callback vacío de momento)
    auto subscription = node->create_subscription<sensor_msgs::msg::LaserScan>(
        "/scan", 10,
        [](const sensor_msgs::msg::LaserScan::SharedPtr msg) {
            auto n = msg->ranges.size();
		if (n >= 360) {
                
                std::vector<float> front_back;
                front_back.insert(front_back.end(), msg->ranges.begin(), msg->ranges.begin() + 10);
                front_back.insert(front_back.end(), msg->ranges.begin() + 350, msg->ranges.begin() + 360);

                // Calcular mínimo
                float min_range = *std::min_element(front_back.begin(), front_back.end());

                // Mostrar en pantalla
                std::cout << "Mínimo distancia (0..9 + 350..359): " << min_range << std::endl;
                std::cout << "------------------------" << std::endl;
            }
        }
    );

    geometry_msgs::msg::Twist message;

    message.linear.x = 0.0;
    message.angular.z = 0.0;

    rclcpp::WallRate loop_rate(10ms);

    while (rclcpp::ok()) {
        publisher->publish(message);

        rclcpp::spin_some(node);

        loop_rate.sleep();
    }

    rclcpp::shutdown();
    return 0;
}

