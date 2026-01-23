#ifndef PIX_HOOKE_DRIVER__REPORT_PARSER_HPP_
#define PIX_HOOKE_DRIVER__REPORT_PARSER_HPP_

#include <string>
#include <memory>

#include <rclcpp/rclcpp.hpp>

#include <std_msgs/msg/bool.hpp>
#include <std_msgs/msg/header.hpp>

#include <can_msgs/msg/frame.hpp>


// include- msgs header file
// Example: #include "pix_driver_msgs/BrakeCommand.h"
// #include pix_driver_msgs/protocols["name"].h
#include <mivia_rover_can_msgs/msg/encoder_rpms.hpp>
#include <mivia_rover_can_msgs/msg/imu_gyro_xy.hpp>
#include <mivia_rover_can_msgs/msg/imu_accel_xy.hpp>
#include <mivia_rover_can_msgs/msg/imu_z.hpp>



// include- Parse header file
// Example: #include "brake_command_101.hpp"
// #include protocols["name"].cpp
#include <mivia_rover_can_driver/encoder_rpms.hpp>
#include <mivia_rover_can_driver/imu_gyro_xy.hpp>
#include <mivia_rover_can_driver/imu_accel_xy.hpp>
#include <mivia_rover_can_driver/imu_z.hpp>


namespace mivia_rover_can_driver
{
namespace report_parser
{

/**
 * @brief param structure of report parser node
 * @param base_frame_id frame id of vehicle
 * @param loop_rate loop rate of publishers in hz
 * @param report_timeout_ms timeout threshold of report can Frame msg from canbus driver
 */
struct Param
{
  std::string base_frame_id;
  double loop_rate;
  int report_timeout_ms;
};

class ReportParser : public rclcpp::Node
{
private:
  // parameters of node
  Param param_;

  // is publish subscrber
  rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr is_publish_sub_;
  bool is_publish_;

  // subscribers from socketcan interface
  rclcpp::Subscription<can_msgs::msg::Frame>::SharedPtr can_frame_sub_;

  // publishers
  /** example
  rclcpp::Publisher<V2aBrakeStaFb>::SharedPtr brake_sta_fb_pub_;
  **/
  rclcpp::Publisher<mivia_rover_can_msgs::msg::EncoderRpms>::SharedPtr encoder_rpms_pub_;
rclcpp::Publisher<mivia_rover_can_msgs::msg::ImuGyroXy>::SharedPtr imu_gyro_xy_pub_;
rclcpp::Publisher<mivia_rover_can_msgs::msg::ImuAccelXy>::SharedPtr imu_accel_xy_pub_;
rclcpp::Publisher<mivia_rover_can_msgs::msg::ImuZ>::SharedPtr imu_z_pub_;


  // publish msgs
  /** example
  V2aBrakeStaFb::ConstSharedPtr brake_sta_fb_ptr_;
  **/
  mivia_rover_can_msgs::msg::EncoderRpms::ConstSharedPtr encoder_rpms_ptr_;
mivia_rover_can_msgs::msg::ImuGyroXy::ConstSharedPtr imu_gyro_xy_ptr_;
mivia_rover_can_msgs::msg::ImuAccelXy::ConstSharedPtr imu_accel_xy_ptr_;
mivia_rover_can_msgs::msg::ImuZ::ConstSharedPtr imu_z_ptr_;


  // can frame entities
  /** example
  V2adrivestafb530  v2a_drivestafb_530_entity_;
  **/
  EncoderRpms encoder_rpms_entity_;
ImuGyroXy imu_gyro_xy_entity_;
ImuAccelXy imu_accel_xy_entity_;
ImuZ imu_z_entity_;


  // msg reveived time
  /** example
  rclcpp::Time brake_sta_fb_received_time_;
  **/
  rclcpp::Time encoder_rpms_received_time_;
rclcpp::Time imu_gyro_xy_received_time_;
rclcpp::Time imu_accel_xy_received_time_;
rclcpp::Time imu_z_received_time_;


  // timer
  rclcpp::TimerBase::SharedPtr timer_;

public:
  ReportParser();
  // callback
  /**
   * @brief callback function of can Frame msgs, to store the data to member variable
   * 
   * @param msg 
   */
  void callbackCan(const can_msgs::msg::Frame::ConstSharedPtr & msg);
  /**
   * @brief callback function of Bool msg, to store the data to member variable, decide publish report msgs or not
   * 
   * @param msg 
   */
  void callbackIsPublish(const std_msgs::msg::Bool::ConstSharedPtr & msg);
  /**
   * @brief parser can frames, convert can frames to pix_driver_msgs
   * 
   */
  void timerCallback();
};
} // report_parser
} // mivia_rover_can_driver
#endif // PIX_HOOKE_DRIVER__REPORT_PARSER_HPP_