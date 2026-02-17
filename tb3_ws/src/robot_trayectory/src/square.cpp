#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    // Nombre del nodo cambiado a "square"
    auto node = rclcpp::Node::make_shared("square");

    auto publisher =
        node->create_publisher<std_msgs::msg::String>("topic", 10);

    std_msgs::msg::String message;
    int publish_count = 0;

    rclcpp::WallRate loop_rate(500ms);

    while (rclcpp::ok()) {
        message.data = "Hello, world! " + std::to_string(publish_count++);
        publisher->publish(message);

        rclcpp::spin_some(node);
        loop_rate.sleep();
    }

    rclcpp::shutdown();
    return 0;
}

