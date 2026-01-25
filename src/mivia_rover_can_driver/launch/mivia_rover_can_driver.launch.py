#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():

    namespace = LaunchConfiguration("namespace")
    use_sim_time = LaunchConfiguration("use_sim_time")
    base_frame_id = LaunchConfiguration("base_frame_id")

    report_timeout_ms = LaunchConfiguration("report_timeout_ms")
    report_loop_rate = LaunchConfiguration("report_loop_rate")

    command_timeout_ms = LaunchConfiguration("command_timeout_ms")
    command_loop_rate = LaunchConfiguration("command_loop_rate")

    return LaunchDescription([
        DeclareLaunchArgument(
            "namespace",
            default_value="",
            description="Namespace ROS 2 applicato ad entrambi i nodi."
        ),
        DeclareLaunchArgument(
            "use_sim_time",
            default_value="false",
            description="Usa /clock se true (propagato ad entrambi)."
        ),
        DeclareLaunchArgument(
            "base_frame_id",
            default_value="base_link",
            description="base_frame_id (propagato ad entrambi; realmente usato dal report_parser)."
        ),

        # --- report_parser ---
        DeclareLaunchArgument(
            "report_timeout_ms",
            default_value="1000",
            description="Timeout (ms) per report_parser."
        ),
        DeclareLaunchArgument(
            "report_loop_rate",
            default_value="50.0",
            description="Loop rate (Hz) per report_parser."
        ),

        # --- control_command ---
        DeclareLaunchArgument(
            "command_timeout_ms",
            default_value="1000",
            description="Timeout (ms) per control_command."
        ),
        DeclareLaunchArgument(
            "command_loop_rate",
            default_value="50.0",
            description="Loop rate (Hz) per control_command."
        ),

        Node(
            package="mivia_rover_can_driver",
            executable="mivia_rover_can_driver_report_parser_node",  # <-- sostituisci se necessario
            name="report_parser",
            namespace=namespace,
            output="screen",
            parameters=[{
                "base_frame_id": base_frame_id,
                "report_timeout_ms": report_timeout_ms,
                "loop_rate": report_loop_rate,
                "use_sim_time": use_sim_time,
            }],
        ),

        Node(
            package="mivia_rover_can_driver",
            executable="mivia_rover_can_driver_control_command_node",  # <-- sostituisci se necessario
            name="control_command",
            namespace=namespace,
            output="screen",
            parameters=[{
                "base_frame_id": base_frame_id,
                "command_timeout_ms": command_timeout_ms,
                "loop_rate": command_loop_rate,
                "use_sim_time": use_sim_time,
            }],
        ),
    ])
