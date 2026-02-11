#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"
#include <vector>
#include <iostream>

std::vector<int> data_vec;

void topic_callback(const std_msgs::msg::Int32::SharedPtr msg)
{
    data_vec.push_back(msg->data);

    for (int i = 0; i < (int)data_vec.size(); i++) {
        int count = 0;
        for (int j = 0; j < (int)data_vec.size(); j++) {
            if (data_vec[j] == data_vec[i])
                count++;
        }
        std::cout << "Numero: " << data_vec[i] << " Ocurrencias: " << count << std::endl;
    }
}

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    auto node = rclcpp::Node::make_shared("count_occurrences");

    auto subscription =
        node->create_subscription<std_msgs::msg::Int32>("number", 10, topic_callback);

    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}

