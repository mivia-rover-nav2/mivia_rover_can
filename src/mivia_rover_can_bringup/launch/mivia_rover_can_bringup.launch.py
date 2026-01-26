#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.substitutions import LaunchConfiguration

from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_xml.launch_description_sources import XMLLaunchDescriptionSource

from launch_ros.substitutions import FindPackageShare
from launch.substitutions import PathJoinSubstitution


def generate_launch_description():
    # =========================
    # Argomenti "globali" bringup
    # =========================
    namespace = LaunchConfiguration("namespace")
    use_sim_time = LaunchConfiguration("use_sim_time")
    base_frame_id = LaunchConfiguration("base_frame_id")

    # socketcan
    interface = LaunchConfiguration("interface")
    receiver_interval_sec = LaunchConfiguration("receiver_interval_sec")
    sender_timeout_sec = LaunchConfiguration("sender_timeout_sec")
    enable_can_fd = LaunchConfiguration("enable_can_fd")

    # can_driver
    report_timeout_ms = LaunchConfiguration("report_timeout_ms")
    report_loop_rate = LaunchConfiguration("report_loop_rate")
    command_timeout_ms = LaunchConfiguration("command_timeout_ms")
    command_loop_rate = LaunchConfiguration("command_loop_rate")

    # Topics (coerenti con i remapping del suo can_driver.launch.py)
    from_can_bus_topic = LaunchConfiguration("from_can_bus_topic")
    to_can_bus_topic = LaunchConfiguration("to_can_bus_topic")

    # =========================
    # Percorsi ai launch esterni
    # =========================
    can_driver_launch = PathJoinSubstitution([
        FindPackageShare("mivia_rover_can_driver"),
        "launch",
        "mivia_rover_can_driver.launch.py",
    ])

    socketcan_xml_launch = PathJoinSubstitution([
        FindPackageShare("ros2_socketcan"),
        "launch",
        "socket_can_bridge.launch.xml",
    ])

    # =========================
    # Include launch: socketcan (XML)
    # =========================
    include_socketcan = IncludeLaunchDescription(
        XMLLaunchDescriptionSource(socketcan_xml_launch),
        launch_arguments={
            "interface": interface,
            "receiver_interval_sec": receiver_interval_sec,
            "sender_timeout_sec": sender_timeout_sec,
            "enable_can_fd": enable_can_fd,
            "from_can_bus_topic": from_can_bus_topic,
            "to_can_bus_topic": to_can_bus_topic,
        }.items(),
    )

    # =========================
    # Include launch: can_driver (Python)
    # =========================
    include_can_driver = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(can_driver_launch),
        launch_arguments={
            "namespace": namespace,
            "use_sim_time": use_sim_time,
            "base_frame_id": base_frame_id,
            "report_timeout_ms": report_timeout_ms,
            "report_loop_rate": report_loop_rate,
            "command_timeout_ms": command_timeout_ms,
            "command_loop_rate": command_loop_rate,
        }.items(),
    )

    # =========================
    # LaunchDescription finale
    # =========================
    return LaunchDescription([
        # --- bringup args ---
        DeclareLaunchArgument("namespace", default_value="", description="Namespace ROS 2 (per il can_driver)."),
        DeclareLaunchArgument("use_sim_time", default_value="false", description="Usa /clock se true."),
        DeclareLaunchArgument("base_frame_id", default_value="base_link", description="Base frame per il can_driver."),

        # --- socketcan args ---
        DeclareLaunchArgument("interface", default_value="can0", description="Interfaccia SocketCAN (es. can0)."),
        DeclareLaunchArgument("receiver_interval_sec", default_value="0.01", description="Polling receiver (s)."),
        DeclareLaunchArgument("sender_timeout_sec", default_value="0.01", description="Timeout sender (s)."),
        DeclareLaunchArgument("enable_can_fd", default_value="false", description="Abilita CAN-FD."),

        # --- topics: devono combaciare con i remapping in mivia_rover_can_driver.launch.py ---
        DeclareLaunchArgument("from_can_bus_topic", default_value="from_can_bus", description="Topic RX dal bus."),
        DeclareLaunchArgument("to_can_bus_topic", default_value="to_can_bus", description="Topic TX verso il bus."),

        # --- can_driver args ---
        DeclareLaunchArgument("report_timeout_ms", default_value="1000", description="Timeout report_parser (ms)."),
        DeclareLaunchArgument("report_loop_rate", default_value="50.0", description="Loop rate report_parser (Hz)."),
        DeclareLaunchArgument("command_timeout_ms", default_value="1000", description="Timeout control_command (ms)."),
        DeclareLaunchArgument("command_loop_rate", default_value="50.0", description="Loop rate control_command (Hz)."),

        # --- inclusioni ---
        include_socketcan,
        include_can_driver,
    ])
