#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"
#include <vector>

std::vector<int> data_vec;
std::shared_ptr<rclcpp::Publisher<std_msgs::msg::Int32>> publisher;

void topic_callback(const std_msgs::msg::Int32::SharedPtr msg)
{
    data_vec.push_back(msg->data);

    int max_count = 0;
    int mode = data_vec[0];

    for (int i = 0; i < (int)data_vec.size(); i++) {
        int count = 0;
        for (int j = 0; j < (int)data_vec.size(); j++) {
            if (data_vec[j] == data_vec[i])
                count++;
        }
        if (count > max_count) {
            max_count = count;
            mode = data_vec[i];
        }
    }

    std_msgs::msg::Int32 out_msg;
    out_msg.data = mode;
    publisher->publish(out_msg);
}

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    auto node = rclcpp::Node::make_shared("mode_single");

    auto subscription =
        node->create_subscription<std_msgs::msg::Int32>("number", 10, topic_callback);

    publisher = node->create_publisher<std_msgs::msg::Int32>("mode", 10);

    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}

