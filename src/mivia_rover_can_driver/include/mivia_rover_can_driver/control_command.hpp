#ifndef PIX_HOOKE_DRIVER__CONTROL_COMMAND_HPP_
#define PIX_HOOKE_DRIVER__CONTROL_COMMAND_HPP_

#include <string>
#include <memory>
 
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/bool.hpp>
#include <can_msgs/msg/frame.hpp>

#include <mivia_rover_can_msgs/msg/reference.hpp>
#include <mivia_rover_can_driver/reference.hpp>

//Added for logging
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <fstream>
#include <array>
#include <atomic>
#include <thread>

struct LogEntry {
  uint64_t t_strat_ns;
  uint64_t t_end_ns;
}

std::array<std::LogEntry, 4096> log_buffer_;
std::atomic<std::sized_t> log_head_{0};
std::atomic<std::sized_t> log_tail_{0};
std::thread logging_thread_;
std::atomic<bool> logging_is_running_{false};
std::mutex log_mutex_;
std::conditin_variable log_cv_;
void logging_thread_entry_();

namespace mivia_rover_can_driver{
  namespace control_command{

  /**
   * @brief param structure of control command node
   * @param base_frame_id frame id of vehicle
   * @param loop_rate loop rate of publishers in hz
   * @param command_timeout_ms timeout threshold of control command msg from control converter in ms
   */
    struct Param
    {
      std::string base_frame_id;
      double loop_rate;
      int command_timeout_ms;
    };

    class ControlCommand : public rclcpp::Node{
    private:
      
      Param param_;
      rclcpp::Subscription<mivia_rover_can_msgs::msg::Reference>::SharedPtr reference_sub_;
      mivia_rover_can_msgs::msg::Reference::ConstSharedPtr reference_ptr_;
      
      Reference reference_entity_;
      rclcpp::Time reference_received_time_;

      rclcpp::Publisher<can_msgs::msg::Frame>::SharedPtr can_frame_pub_;
      can_msgs::msg::Frame::ConstSharedPtr reference_can_ptr_;

      rclcpp::TimerBase::SharedPtr timer_;

    public:
      ControlCommand();
    
      void callbackReference(const mivia_rover_can_msgs::msg::Reference::ConstSharedPtr & msg);
      void timerCallback();

    };
  } 
} 
#endif // PIX_HOOKE_DRIVER__CONTROL_COMMAND_HPP_