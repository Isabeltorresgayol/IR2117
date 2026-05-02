
#include <memory>
#include <thread>
#include "olympic_interfaces/action/rings.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"

using Rings = olympic_interfaces::action::Rings;
using GoalHandleRings = rclcpp_action::ServerGoalHandle<Rings>;

// Callback para nuevos goals
rclcpp_action::GoalResponse handle_goal(
  const rclcpp_action::GoalUUID & uuid,
  std::shared_ptr<const Rings::Goal> goal)
{
  RCLCPP_INFO(rclcpp::get_logger("rings_action_server"),
    "Received radius: %f", goal->radius);
  return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
}

// Callback para cancelación
rclcpp_action::CancelResponse handle_cancel(
  const std::shared_ptr<GoalHandleRings> goal_handle)
{
  RCLCPP_INFO(rclcpp::get_logger("rings_action_server"),
    "Got request to cancel goal");

  return rclcpp_action::CancelResponse::ACCEPT;
}

// Declaración adelantada
void execute(const std::shared_ptr<GoalHandleRings> goal_handle){

  auto feedback = std::make_shared<Rings::Feedback>();
  auto result = std::make_shared<Rings::Result>();

  RCLCPP_INFO(
    rclcpp::get_logger("rings_action_server"),
    "Executing goal");

  for (int i = 0; i < 5; i++)
  {
    feedback->drawing_ring = i;
    feedback->ring_angle = i * 10.0;

    goal_handle->publish_feedback(feedback);

    rclcpp::sleep_for(std::chrono::milliseconds(500));
  }

  result->rings_completed = 5;
  goal_handle->succeed(result);

  RCLCPP_INFO(
    rclcpp::get_logger("rings_action_server"),
    "Goal succeeded");


}

// Callback cuando se acepta un goal
void handle_accepted(
  const std::shared_ptr<GoalHandleRings> goal_handle)
{
  std::thread{execute, goal_handle}.detach();
}



// main
int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);

  auto node = rclcpp::Node::make_shared("rings_action_server");

  auto action_server =
    rclcpp_action::create_server<Rings>(
      node,
      "rings",
      handle_goal,
      handle_cancel,
      handle_accepted);

  rclcpp::spin(node);
  rclcpp::shutdown();

  return 0;
}
