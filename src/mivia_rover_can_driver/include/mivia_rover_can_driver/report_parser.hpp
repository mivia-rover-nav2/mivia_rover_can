#ifndef PIX_HOOKE_DRIVER__REPORT_PARSER_HPP_
#define PIX_HOOKE_DRIVER__REPORT_PARSER_HPP_

#include <string>
#include <memory>

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/bool.hpp>
#include <std_msgs/msg/header.hpp>
#include <can_msgs/msg/frame.hpp>

#include <mivia_rover_can_msgs/msg/encoder_rpms.hpp>
#include <mivia_rover_can_msgs/msg/imu_gyro_xy.hpp>
#include <mivia_rover_can_msgs/msg/imu_accel_xy.hpp>
#include <mivia_rover_can_msgs/msg/imu_z.hpp>

#include <mivia_rover_can_driver/encoder_rpms.hpp>
#include <mivia_rover_can_driver/imu_gyro_xy.hpp>
#include <mivia_rover_can_driver/imu_accel_xy.hpp>
#include <mivia_rover_can_driver/imu_z.hpp>


namespace mivia_rover_can_driver{
  namespace report_parser{

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

    class ReportParser : public rclcpp::Node{

    private:
      
      Param param_;

      rclcpp::Subscription<can_msgs::msg::Frame>::SharedPtr can_frame_sub_;

      rclcpp::Publisher<mivia_rover_can_msgs::msg::EncoderRpms>::SharedPtr encoder_rpms_pub_;
      rclcpp::Publisher<mivia_rover_can_msgs::msg::ImuGyroXy>::SharedPtr imu_gyro_xy_pub_;
      rclcpp::Publisher<mivia_rover_can_msgs::msg::ImuAccelXy>::SharedPtr imu_accel_xy_pub_;
      rclcpp::Publisher<mivia_rover_can_msgs::msg::ImuZ>::SharedPtr imu_z_pub_;

      mivia_rover_can_msgs::msg::EncoderRpms::ConstSharedPtr encoder_rpms_ptr_;
      mivia_rover_can_msgs::msg::ImuGyroXy::ConstSharedPtr imu_gyro_xy_ptr_;
      mivia_rover_can_msgs::msg::ImuAccelXy::ConstSharedPtr imu_accel_xy_ptr_;
      mivia_rover_can_msgs::msg::ImuZ::ConstSharedPtr imu_z_ptr_;

      EncoderRpms encoder_rpms_entity_;
      ImuGyroXy imu_gyro_xy_entity_;
      ImuAccelXy imu_accel_xy_entity_;
      ImuZ imu_z_entity_;

      rclcpp::Time encoder_rpms_received_time_;
      rclcpp::Time imu_gyro_xy_received_time_;
      rclcpp::Time imu_accel_xy_received_time_;
      rclcpp::Time imu_z_received_time_;

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
  } 
} 
#endif // PIX_HOOKE_DRIVER__REPORT_PARSER_HPP_