
#include <memory>
#include <chrono>

#include "olympic_interfaces/action/rings.hpp" //cambiado
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"

using Rings = olympic_interfaces::action::Rings; //cambiado
using GoalHandleRings = rclcpp_action::ClientGoalHandle<Rings>; //cambiado
using namespace std::chrono_literals;

rclcpp::Node::SharedPtr g_node = nullptr;

// Callback de feedback
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

  g_node = rclcpp::Node::make_shared("rings_action_client"); //cambiado

  auto action_client =
    rclcpp_action::create_client<Rings>(
      g_node,
      "rings");

  // Esperar servidor
  if (!action_client->wait_for_action_server(20s)) {
    RCLCPP_ERROR(g_node->get_logger(),
      "Action server not available after waiting");
    return 1;
  }

  // Crear goal
  auto goal_msg = Rings::Goal(); //cambiado
  goal_msg.radius = 1.0;//cambiado

  RCLCPP_INFO(g_node->get_logger(), "Sending goal");

  rclcpp_action::Client<Rings>::SendGoalOptions send_goal_options; //cambiado
  send_goal_options.feedback_callback = feedback_callback;

  auto goal_handle_future =
    action_client->async_send_goal(goal_msg, send_goal_options);

  auto return_code =
    rclcpp::spin_until_future_complete(g_node, goal_handle_future);

  if (return_code != rclcpp::FutureReturnCode::SUCCESS) {
    RCLCPP_ERROR(g_node->get_logger(),
      "send goal call failed :(");
    return 1;
  }

  auto goal_handle = goal_handle_future.get();

  if (!goal_handle) {
    RCLCPP_ERROR(g_node->get_logger(),
      "Goal was rejected by server");
    return 1;
  }

  // Pedir resultado
  auto result_future =
    action_client->async_get_result(goal_handle);

  RCLCPP_INFO(g_node->get_logger(), "Waiting for result");

  return_code =
    rclcpp::spin_until_future_complete(g_node, result_future);

  if (return_code != rclcpp::FutureReturnCode::SUCCESS) {
    RCLCPP_ERROR(g_node->get_logger(),
      "get result call failed :(");
    return 1;
  }

  auto wrapped_result = result_future.get();
  auto result = wrapped_result.result;

  RCLCPP_INFO(
  g_node->get_logger(),
  "Rings completed: %d",
  result->rings_completed);

  
 
  action_client.reset();
  g_node.reset();
  rclcpp::shutdown();

  return 0;
}
