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

    //Starting the logging_thread
    log_head_.store(0);
    log_tail_.store(0);
    logging_is_running_.store(true);

    logging_thread_ = std::thread(&ControlCommand::logging_thread_entry_, this);
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

            auto now_steady_clock = std::chrono::steady_clock::now();
            uint64_t t_end_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(now_steady_clock.time_since_epoch()).count();

            uint64_t t_start_ns = reference_can_ptr_->header.stamp.sec * 1000000000ULL + reference_can_ptr_->header.stamp.nanosec;

            //Saving in the buffer
            size_t head = log_head_.load(std::memory_order_relaxed);
            size_t next_head = (head + 1) % 4096;
            if (next_head != log_tail_.load(std::memory_order_acquire)) {
              log_buffer_[head].t_start_ns = t_start_ns;
              log_buffer_[head].t_end_ns = t_end_ns;
              log_head_.store(next_head, std::memory_order_release);
              log_cv_.notify_one();
            }
        }

        
    }

    void ControlCommand::logging_thread_entry_() {
      std::ofstream log_file("/tmp/log_with_dds.csv", std::ios::out | std::ios::trunc);
      if (log_file.is_open()) log_file << "t_start_ns,t_end_ns\n";

      while(logging_is_running_.load()) {
        size_t tail = log_tail_.load(std::memory_order_relaxed);

        std::unique_lock<std::mutex> lock(log_mutex_);
        log_cv_.wait(lock, [this, &tail] {
          return (tail != log_head_.load(std::memory_order_acquire)) || !logging_is_running_.load();
        });
        if(!logging_is_running_.load() && (tail == log_head_.load(std::memory_order_acquire))) break;

        size_t head = log_head_.load(std::memory_order_acquire);

        while(tail != head) {
          log_file << log_buffer_[tail].t_start_ns << "," << log_buffer_[tail].t_end_ns << "\n";
          tail = (tail + 1) % 4096;
        }
        log_tail_.store(tail, std::memory_order_release);
      }
      if (log_file.is_open()) log_file.close();
    }

  } 
} 
