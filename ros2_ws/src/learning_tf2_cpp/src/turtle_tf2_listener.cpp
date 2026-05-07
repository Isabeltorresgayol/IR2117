#include <chrono>
#include <memory>
#include <string>

#include "geometry_msgs/msg/twist.hpp"
#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/transform_listener.h"
#include "tf2_ros/buffer.h"

using namespace std::chrono_literals;

class TurtleFollower : public rclcpp::Node {
public:
  TurtleFollower() : Node("turtle_tf2_frame_listener") {
    // Declarar parámetros
    target_frame_ = this->declare_parameter<std::string>("target_frame", "turtle1");
    turtle_name_ = this->declare_parameter<std::string>("turtlename", "turtle2");

    // Inicializar TF
    tf_buffer_ = std::make_unique<tf2_ros::Buffer>(this->get_clock());
    tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);

    // Publicador
    publisher_ = this->create_publisher<geometry_msgs::msg::Twist>(
      turtle_name_ + "/cmd_vel", 1);

    // Timer: ejecuta la lógica cada 100ms
    timer_ = this->create_wall_timer(100ms, std::bind(&TurtleFollower::on_timer, this));
  }

private:
  void on_timer() {
    geometry_msgs::msg::TransformStamped t;

    try {
      t = tf_buffer_->lookupTransform(turtle_name_, target_frame_, tf2::TimePointZero);
    } catch (const tf2::TransformException & ex) {
      return; // Esperar a que el frame esté disponible
    }

    geometry_msgs::msg::Twist msg;
    msg.angular.z = 1.0 * atan2(t.transform.translation.y, t.transform.translation.x);
    msg.linear.x = 0.5 * sqrt(pow(t.transform.translation.x, 2) + pow(t.transform.translation.y, 2));

    publisher_->publish(msg);
  }

  std::string target_frame_;
  std::string turtle_name_;
  std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
  std::unique_ptr<tf2_ros::Buffer> tf_buffer_;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<TurtleFollower>());
  rclcpp::shutdown();
  return 0;
}
