#include <memory>
#include <thread>
#include <vector>
#include <cmath>

#include "olympic_interfaces/action/rings.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "turtlesim/srv/set_pen.hpp"
#include "turtlesim/srv/teleport_absolute.hpp"

using Rings = olympic_interfaces::action::Rings;
using GoalHandleRings = rclcpp_action::ServerGoalHandle<Rings>;

struct RingConfig {
    double x;
    double y;
    int r, g, b;
};


// Superiores separadas por 2.1m (centro en 5.5)
// Inferiores más juntas (centradas en los huecos)
static const std::vector<RingConfig> rings_data = {
    {3.4, 7.0, 0, 129, 200},   // Azul
    {5.5, 7.0, 0, 0, 0},       // Negro
    {7.6, 7.0, 238, 51, 78},   // Rojo
    {4.45, 5.8, 252, 177, 49}, // Amarillo
    {6.55, 5.8, 0, 166, 81}    // Verde
};

rclcpp::Client<turtlesim::srv::SetPen>::SharedPtr pen_client;
rclcpp::Client<turtlesim::srv::TeleportAbsolute>::SharedPtr teleport_client;

void set_pen_state(bool off, int r=0, int g=0, int b=0) {
    auto req = std::make_shared<turtlesim::srv::SetPen::Request>();
    req->off = off ? 1 : 0;
    req->r = r; req->g = g; req->b = b;
    req->width = 6; // Un poco más grueso para que resalte en blanco
    pen_client->async_send_request(req);
}

void move_to(double x, double y) {
    auto req = std::make_shared<turtlesim::srv::TeleportAbsolute::Request>();
    req->x = x;
    req->y = y;
    req->theta = 0;
    teleport_client->async_send_request(req);
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
}

void draw_circle(rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pub, 
                 float radius, std::shared_ptr<GoalHandleRings> goal_handle, int current_ring) {
    auto feedback = std::make_shared<Rings::Feedback>();
    geometry_msgs::msg::Twist msg;
    
    msg.linear.x = 2.0; 
    msg.angular.z = msg.linear.x / radius; 

    const double total_time = (2.0 * M_PI * radius) / msg.linear.x;
    const int steps = 60;
    const double dt = total_time / steps;

    for (int i = 0; i <= steps; i++) {
        if (goal_handle->is_canceling()) return;
        pub->publish(msg);
        
        feedback->drawing_ring = current_ring;
        feedback->ring_angle = (360.0 / steps) * i;
        goal_handle->publish_feedback(feedback);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(dt * 1000)));
    }
    pub->publish(geometry_msgs::msg::Twist()); 
}

void execute(const std::shared_ptr<GoalHandleRings> goal_handle, 
             rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pub) {
    auto result = std::make_shared<Rings::Result>();
    float radius = goal_handle->get_goal()->radius;

    int completed = 0;
    for (const auto &ring : rings_data) {
        if (goal_handle->is_canceling()) {
            result->rings_completed = completed;
            goal_handle->canceled(result);
            return;
        }

        set_pen_state(true); 
        move_to(ring.x, ring.y - radius); 
        set_pen_state(false, ring.r, ring.g, ring.b); 

        draw_circle(pub, radius, goal_handle, completed + 1);
        completed++;
    }

    result->rings_completed = completed;
    goal_handle->succeed(result);
}

rclcpp_action::GoalResponse handle_goal(const rclcpp_action::GoalUUID &, std::shared_ptr<const Rings::Goal>) {
    return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
}

rclcpp_action::CancelResponse handle_cancel(const std::shared_ptr<GoalHandleRings>) {
    return rclcpp_action::CancelResponse::ACCEPT;
}

void handle_accepted(const std::shared_ptr<GoalHandleRings> goal_handle, 
                    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pub) {
    std::thread{execute, goal_handle, pub}.detach();
}

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    auto node = rclcpp::Node::make_shared("rings_action_server");

    auto pub = node->create_publisher<geometry_msgs::msg::Twist>("turtle1/cmd_vel", 10);
    pen_client = node->create_client<turtlesim::srv::SetPen>("turtle1/set_pen");
    teleport_client = node->create_client<turtlesim::srv::TeleportAbsolute>("turtle1/teleport_absolute");

    auto server = rclcpp_action::create_server<Rings>(
        node, "rings",
        handle_goal, handle_cancel,
        std::bind(&handle_accepted, std::placeholders::_1, pub)
    );

    RCLCPP_INFO(node->get_logger(), "Servidor olímpico (Compacto) listo.");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
