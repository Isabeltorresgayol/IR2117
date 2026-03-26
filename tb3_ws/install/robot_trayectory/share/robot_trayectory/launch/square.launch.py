from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='robot_trayectory',
            executable='square',
            remappings=[
                ('/cmd_vel', '/turtle1/cmd_vel')
            ],
            parameters=[
                {'linear_speed': 0.5},
                {'angular_speed': 1.2},
                {'square_length': 1.5}
            ]

        )
    ])
