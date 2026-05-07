import os
from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import ExecuteProcess, TimerAction

def generate_launch_description():
    ld = LaunchDescription()

    # 1. Nodo de Turtlesim
    ld.add_action(Node(
        package='turtlesim',
        executable='turtlesim_node',
        name='sim'
    ))

    # 2. Spawner de tortugas (2 a 5) con un pequeño retraso para que el simulador cargue
    for i in range(2, 6):
        spawn_cmd = f'ros2 service call /spawn turtlesim/srv/Spawn "{{x: {float(i)}, y: {float(i)}, theta: 0.0, name: \'turtle{i}\' }}"'
        ld.add_action(TimerAction(
            period=float(i-1), # Escalonamos el spawn
            actions=[ExecuteProcess(cmd=[spawn_cmd], shell=True)]
        ))

    # 3. Nodos de TF para cada tortuga
    for i in range(1, 6):
        # Cada tortuga necesita emitir su posición
        ld.add_action(Node(
            package='learning_tf2_cpp',
            executable='turtle_tf2_broadcaster',
            name=f'broadcaster{i}',
            parameters=[{'turtlename': f'turtle{i}'}]
        ))

        # Las tortugas 2 a 5 siguen a la anterior
        if i > 1:
            ld.add_action(Node(
                package='learning_tf2_cpp',
                executable='turtle_tf2_listener',
                name=f'listener{i}',
                parameters=[
                    {'turtlename': f'turtle{i}'},
                    {'target_frame': f'turtle{i-1}'}
                ]
            ))

    return ld
