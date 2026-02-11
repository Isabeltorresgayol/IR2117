#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"
#include <vector>
#include <iostream>

std::vector<int> data_vec;

void topic_callback(const std_msgs::msg::Int32::SharedPtr msg)
{
    
    data_vec.push_back(msg->data);

    
    std::cout << "Vector: ";
    for (int x : data_vec) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    auto node = rclcpp::Node::make_shared("vector");

    auto subscription =
        node->create_subscription<std_msgs::msg::Int32>("number", 10, topic_callback);

    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}


