#include <mivia_rover_can_driver/control_command.hpp>

namespace mivia_rover_can_driver{
  namespace control_command{

  ControlCommand::ControlCommand() : Node("control_command"){
    
    param_.base_frame_id = declare_parameter("base_frame_id", "base_link");
    param_.command_timeout_ms = declare_parameter("command_timeout_ms", 1000);
    param_.loop_rate = declare_parameter("loop_rate", 50.0);

    reference_received_time_ = this->now();

    using std::placeholders::_1;
  
    {
      reference_sub_ = create_subscription<mivia_rover_can_msgs::msg::Reference>("/mivia_rover/reference", 1, std::bind(&ControlCommand::callbackReference, this, _1));

    }
    
    {
      can_frame_pub_ = create_publisher<can_msgs::msg::Frame>("output/can_tx", rclcpp::QoS{1});
    }

    {
      timer_ = rclcpp::create_timer(
        this, get_clock(), rclcpp::Rate(param_.loop_rate).period(),
        std::bind(&ControlCommand::timerCallback, this));
    }
  }



    void ControlCommand::callbackReference(const mivia_rover_can_msgs::msg::Reference::ConstSharedPtr & msg){
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

      

    void ControlCommand::timerCallback(){
      
      const rclcpp::Time current_time = this->now();

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

  } 
} 
