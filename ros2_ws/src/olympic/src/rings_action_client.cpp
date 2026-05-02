#include <memory>
#include <chrono>

#include "olympic_interfaces/action/rings.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"

using Rings = olympic_interfaces::action::Rings;
using GoalHandleRings = rclcpp_action::ClientGoalHandle<Rings>;
using namespace std::chrono_literals;

rclcpp::Node::SharedPtr g_node = nullptr;

void feedback_callback(
  GoalHandleRings::SharedPtr,
  const std::shared_ptr<const Rings::Feedback> feedback)
{
  RCLCPP_INFO(rclcpp::get_logger("rings_action_client"),
    "Ring: %d | Angle: %.2f",
    feedback->drawing_ring,
    feedback->ring_angle);
}

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);

  g_node = rclcpp::Node::make_shared("rings_action_client");

  auto action_client =
    rclcpp_action::create_client<Rings>(g_node, "rings");

  if (!action_client->wait_for_action_server(20s)) {
    RCLCPP_ERROR(g_node->get_logger(), "Server not available");
    return 1;
  }

  Rings::Goal goal_msg;
  goal_msg.radius = 1.0;

  rclcpp_action::Client<Rings>::SendGoalOptions options;
  options.feedback_callback = feedback_callback;

  auto future = action_client->async_send_goal(goal_msg, options);

  if (rclcpp::spin_until_future_complete(g_node, future)
      != rclcpp::FutureReturnCode::SUCCESS)
  {
    return 1;
  }

  auto goal_handle = future.get();

  auto result_future = action_client->async_get_result(goal_handle);

  rclcpp::spin_until_future_complete(g_node, result_future);

  auto result = result_future.get().result;

  RCLCPP_INFO(g_node->get_logger(),
    "Rings completed: %d",
    result->rings_completed);

  rclcpp::shutdown();
  return 0;
}
