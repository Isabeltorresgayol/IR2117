
#include <memory>
#include <thread>
#include "olympic_interfaces/action/rings.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "geometry_msgs/msg/twist.hpp" //añadido en el PASO 6

using Rings = olympic_interfaces::action::Rings;
using GoalHandleRings = rclcpp_action::ServerGoalHandle<Rings>;


//---------------------------------------------------------------------------

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
void execute(const std::shared_ptr<GoalHandleRings> goal_handle, rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher){
  //Se ha añadido: rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher PASO 6
  auto feedback = std::make_shared<Rings::Feedback>();
  auto result = std::make_shared<Rings::Result>();

  RCLCPP_INFO(
    rclcpp::get_logger("rings_action_server"),
    "Executing goal");
    geometry_msgs::msg::Twist msg;

   // velocidad → círculo
   msg.linear.x = 2.0;
   msg.angular.z = 2.0;

   // duración del círculo (~6 segundos)
   for (int i = 0; i < 100; i++)
   {
    publisher->publish(msg);

    feedback->drawing_ring = 0;
    feedback->ring_angle = i * 3.6; // aprox 360 grados
 
    goal_handle->publish_feedback(feedback);

    rclcpp::sleep_for(std::chrono::milliseconds(60));
   }
  

   result->rings_completed = 5;
   goal_handle->succeed(result);

   RCLCPP_INFO(
    rclcpp::get_logger("rings_action_server"),
    "Goal succeeded");


}

// Callback cuando se acepta un goal
void handle_accepted(
  const std::shared_ptr<GoalHandleRings> goal_handle, rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher)
{
  std::thread{execute, goal_handle, publisher}.detach();
} //Se ha añadido el publisher PASO 6



// main
int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);

  auto node = rclcpp::Node::make_shared("rings_action_server");

  auto publisher =
    node->create_publisher<geometry_msgs::msg::Twist>("turtle1/cmd_vel", 10);

  auto action_server =
    rclcpp_action::create_server<Rings>(
      node,
      "rings",
      handle_goal,
      handle_cancel,
      std::bind(&handle_accepted, std::placeholders::_1, publisher)
    );

  rclcpp::spin(node);
  rclcpp::shutdown();

  return 0;
}

