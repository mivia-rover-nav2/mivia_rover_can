#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    
    base_frame_id = LaunchConfiguration("base_frame_id")
    report_timeout_ms = LaunchConfiguration("report_timeout_ms")
    loop_rate = LaunchConfiguration("loop_rate")

    use_sim_time = LaunchConfiguration("use_sim_time")
    namespace = LaunchConfiguration("namespace")

    return LaunchDescription([
        DeclareLaunchArgument(
            "base_frame_id",
            default_value="base_link",
            description="Frame ID da inserire nell'header dei messaggi pubblicati dal report_parser."
        ),
        DeclareLaunchArgument(
            "report_timeout_ms",
            default_value="1000",
            description="Timeout (ms) oltre il quale il report_parser segnala report timeout."
        ),
        DeclareLaunchArgument(
            "loop_rate",
            default_value="50.0",
            description="Frequenza (Hz) del timer del report_parser."
        ),
        DeclareLaunchArgument(
            "use_sim_time",
            default_value="false",
            description="Usa /clock se true."
        ),
        DeclareLaunchArgument(
            "namespace",
            default_value="",
            description="Namespace ROS 2 per il nodo."
        ),

        # Node(
        #     package="mivia_rover_can_driver",
        #     executable="mivia_rover_can_driver_report_parser_node",  # <-- sostituisci se il tuo target ha un nome diverso
        #     name="report_parser",
        #     namespace=namespace,
        #     output="screen",
        #     parameters=[{
        #         "base_frame_id": base_frame_id,
        #         "report_timeout_ms": report_timeout_ms,
        #         "loop_rate": loop_rate,
        #         "use_sim_time": use_sim_time,
        #     }],
        #     remappings=[("input/can_rx", "from_can_bus")]
        # ),
    ])
