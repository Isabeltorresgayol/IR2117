#include <sstream>
#include <string>
#include <unordered_map>
//Añadido
#include <chrono>

#include "geometry_msgs/msg/transform_stamped.hpp"
#include "rclcpp/rclcpp.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_ros/transform_broadcaster.h"
#include "turtlesim/msg/pose.hpp"
//Añadido:
#include "turtlesim/srv/set_pen.hpp"

std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster;

std::string turtlename;

std::shared_ptr<rclcpp::Node> node;

struct RGB {
    int r, g, b;
};

RGB getColorValues(const std::string& colorName) {
    static const std::unordered_map<std::string, RGB> colorMap = {
        {"red",    {255, 0, 0}},
        {"orange", {255, 127, 0}},
        {"yellow", {255, 255, 0}},
        {"green",  {0, 255, 0}},
        {"blue",   {0, 0, 255}},
        {"indigo", {75, 0, 130}},
        {"violet", {148, 0, 211}}
    };
    auto it = colorMap.find(colorName);
    if (it != colorMap.end()) {
        return it->second;
    }    
    return {0, 0, 0}; 
}


void handle_turtle_pose(
  const std::shared_ptr<turtlesim::msg::Pose> msg)
{
  geometry_msgs::msg::TransformStamped t;

  t.header.stamp = node->get_clock()->now();

  t.header.frame_id = "world";
  t.child_frame_id = turtlename.c_str();

  t.transform.translation.x = msg->x;
  t.transform.translation.y = msg->y;
  t.transform.translation.z = 0.0;

  tf2::Quaternion q;
  q.setRPY(0, 0, msg->theta);

  t.transform.rotation.x = q.x();
  t.transform.rotation.y = q.y();
  t.transform.rotation.z = q.z();
  t.transform.rotation.w = q.w();

  tf_broadcaster->sendTransform(t);
}

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  node = rclcpp::Node::make_shared(
    "turtle_tf2_frame_publisher");

  turtlename = node->declare_parameter<std::string>(
    "turtlename",
    "turtle");
    //Añadido
  std::string color;

  if (turtlename == "turtle1") color = "red";
  else if (turtlename == "turtle2") color = "orange";
  else if (turtlename == "turtle3") color = "yellow";
  else if (turtlename == "turtle4") color = "green";
  else if (turtlename == "turtle5") color = "blue";
  else if (turtlename == "turtle6") color = "indigo";
  else if (turtlename == "turtle7") color = "violet";

  RGB rgb = getColorValues(color);

  auto pen_client =
    node->create_client<turtlesim::srv::SetPen>(
      "/" + turtlename + "/set_pen");

  while (!pen_client->wait_for_service(std::chrono::seconds(1))) {
    if (!rclcpp::ok()) {
      return 0;
    }
  }

  auto request =
    std::make_shared<turtlesim::srv::SetPen::Request>();

  request->r = rgb.r;
  request->g = rgb.g;
  request->b = rgb.b;
  request->width = 3;
  request->off = 0;

  pen_client->async_send_request(request);

  tf_broadcaster =
    std::make_unique<tf2_ros::TransformBroadcaster>(
      *node);

  std::string topic_name =
    "/" + turtlename + "/pose";

  auto subscription =
    node->create_subscription<turtlesim::msg::Pose>(
      topic_name,
      10,
      handle_turtle_pose);

  rclcpp::spin(node);

  rclcpp::shutdown();

  return 0;
}
