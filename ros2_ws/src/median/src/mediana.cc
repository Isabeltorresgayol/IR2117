#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"
#include "std_msgs/msg/float64.hpp"
#include <vector>

std::vector<int> data_vec;
std::shared_ptr<rclcpp::Publisher<std_msgs::msg::Float64>> publisher;

void topic_callback(const std_msgs::msg::Int32::SharedPtr msg)
{
    data_vec.push_back(msg->data);

    for (int i = 0; i < (int)data_vec.size(); i++) {
        for (int j = 0; j < (int)data_vec.size() - 1; j++) {
            if (data_vec[j] > data_vec[j + 1]) {
                int tmp = data_vec[j];
                data_vec[j] = data_vec[j + 1];
                data_vec[j + 1] = tmp;
            }
        }
    }

    int n = data_vec.size();
    double median;

    if (n % 2 == 1)
        median = data_vec[n / 2];
    else
        median = (data_vec[n / 2 - 1] + data_vec[n / 2]) / 2.0;

    std_msgs::msg::Float64 out_msg;
    out_msg.data = median;
    publisher->publish(out_msg);
}

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    auto node = rclcpp::Node::make_shared("median");

    auto subscription =
        node->create_subscription<std_msgs::msg::Int32>("number", 10, topic_callback);

    publisher = node->create_publisher<std_msgs::msg::Float64>("median", 10);

    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}


