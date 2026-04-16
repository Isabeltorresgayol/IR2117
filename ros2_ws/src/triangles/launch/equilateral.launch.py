from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([

        Node(
            package='turtlesim',
            executable='turtlesim_node'
        ),

        Node(
            package='triangles',
            executable='equilateral',
            parameters=[
                {'angular_speed': 2.0},
                {'linear_speed': 1.0},
                {'side_length': 2.5}
            ]
        )
    ])
