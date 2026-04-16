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
            executable='isosceles',
            parameters=[
                {'angular_speed': 0.5},
                {'linear_speed': 2.0},
                {'base_length': 1.5},
                {'lateral_length': 2.5}
            ]
        )
    ])
