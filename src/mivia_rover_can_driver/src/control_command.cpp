#include <mivia_rover_can_driver/control_command.hpp>

namespace mivia_rover_can_driver
{
namespace control_command
{
ControlCommand::ControlCommand() : Node("control_command")
{
  // ros params
  param_.base_frame_id = declare_parameter("base_frame_id", "base_link");
  param_.command_timeout_ms = declare_parameter("command_timeout_ms", 1000);
  param_.loop_rate = declare_parameter("loop_rate", 50.0);

  // initialize msg received time, make reservation of data size
  // example brake_command_received_time_ = this->now();
  reference_received_time_ = this->now();

  is_engage_ = true;

  using std::placeholders::_1;

  /* subscriber */
  {
    // from rover driver autoware interface
    /**
    a2v_brake_ctrl_sub_ = create_subscription<A2vBrakeCtrl>(
      "/pix_hooke/a2v_brakectrl_131", 1, std::bind(&ControlCommand::callbackBrakeCtrl, this, _1));
    **/
    reference_sub_ = create_subscription<mivia_rover_can_msgs::msg::Reference>("/pix_rover/reference", 1, std::bind(&ControlCommand::callbackReference, this, _1));

    // engage
    engage_ctrl_sub_ = create_subscription<std_msgs::msg::Bool>(
      "input/engage", 1, std::bind(&ControlCommand::callbackEngage, this, _1));
  }
  /* publisher */
  {
    // to socketcan drivier
    can_frame_pub_ = create_publisher<can_msgs::msg::Frame>("output/can_tx", rclcpp::QoS{1});
  }
  {
    // timer
    timer_ = rclcpp::create_timer(
      this, get_clock(), rclcpp::Rate(param_.loop_rate).period(),
      std::bind(&ControlCommand::timerCallback, this));
  }
}

// calback functions
/** example
void ControlCommand::callbackBrakeCtrl(const A2vBrakeCtrl::ConstSharedPtr & msg)
{
  brake_command_received_time_ = this->now();
  brake_ctrl_ptr_ = msg;
  a2v_brakectrl_131_entity_.Reset();
  a2v_brakectrl_131_entity_.UpdateData(
    msg->acu_chassis_brake_en, msg->acu_chassis_aeb_ctrl, msg->acu_chassis_brake_pdl_target,
    msg->acu_chassis_epb_ctrl, msg->acu_brake_life_sig, msg->acu_check_sum_131);
  can_msgs::msg::Frame brake_ctrl_can_msg;
  brake_ctrl_can_msg.header.stamp = msg->header.stamp;
  brake_ctrl_can_msg.dlc = 8;
  brake_ctrl_can_msg.id = a2v_brakectrl_131_entity_.ID;
  brake_ctrl_can_msg.is_extended = false;
  uint8_t *signal_bits;
  signal_bits = a2v_brakectrl_131_entity_.get_data();
  for (int i = 0; i < 8; i++)
  {
    brake_ctrl_can_msg.data[i] = *signal_bits;
    signal_bits += 1;
  }
  brake_ctrl_can_ptr_ = std::make_shared<can_msgs::msg::Frame>(brake_ctrl_can_msg);
}
**/


    void ControlCommand::callbackReference(const mivia_rover_can_msgs::msg::Reference::ConstSharedPtr & msg)
    {
        reference_received_time_ = this->now();
        reference_ptr_ = msg;
        reference_entity_.Reset();
        reference_entity_.UpdateData(msg->front_left, msg->rear_left, msg->front_right, msg->rear_right);
        can_msgs::msg::Frame reference_can_msg;
        reference_can_msg.header.stamp = msg->header.stamp;
        reference_can_msg.dlc = 8;
        reference_can_msg.id = reference_entity_.ID;
        reference_can_msg.is_extended = false;
        uint8_t *signal_bits;
        signal_bits = reference_entity_.get_data();
        for (int i = 0; i < 8; i++)
        {
            reference_can_msg.data[i] = *signal_bits;
            signal_bits += 1;
        }
        reference_can_ptr_ = std::make_shared<can_msgs::msg::Frame>(reference_can_msg);
    }

    

void ControlCommand::callbackEngage(const std_msgs::msg::Bool::ConstSharedPtr & msg)
{
  is_engage_ = msg->data;
}

void ControlCommand::timerCallback()
{
  if (!is_engage_) return;
  const rclcpp::Time current_time = this->now();

  // publishing msg
  /** example
  // brake control command 
  const double brake_command_delta_time_ms =
    (current_time - brake_command_received_time_).seconds() * 1000.0;
  if (brake_command_delta_time_ms > param_.command_timeout_ms || brake_ctrl_can_ptr_==nullptr) {
    RCLCPP_ERROR_THROTTLE(
      get_logger(), *this->get_clock(), std::chrono::milliseconds(5000).count(),
      "brake command timeout = %f ms.", brake_command_delta_time_ms);
  } else {
    can_frame_pub_->publish(*brake_ctrl_can_ptr_);
  }
  **/
  

    // reference
    const double reference_delta_time_ms =
        (current_time - reference_received_time_).seconds() * 1000.0;
    if (reference_delta_time_ms > param_.command_timeout_ms || reference_can_ptr_==nullptr) {
        RCLCPP_ERROR_THROTTLE(
        get_logger(), *this->get_clock(), std::chrono::milliseconds(5000).count(),
        "reference timeout = %f ms.", reference_delta_time_ms);
    } else {
        can_frame_pub_->publish(*reference_can_ptr_);
    }

    
}

} // namespace control_command
} // namespace mivia_rover_can_driver
