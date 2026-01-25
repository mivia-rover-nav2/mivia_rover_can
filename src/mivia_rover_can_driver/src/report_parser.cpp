#include <mivia_rover_can_driver/report_parser.hpp>

namespace mivia_rover_can_driver{
  namespace report_parser{
    
  ReportParser::ReportParser() : Node("report_parser"){
    
    param_.base_frame_id = declare_parameter("base_frame_id", "base_link");
    param_.report_timeout_ms = declare_parameter("report_timeout_ms", 1000);
    param_.loop_rate = declare_parameter("loop_rate", 50.0);

    encoder_rpms_received_time_ = this->now();
    imu_gyro_xy_received_time_ = this->now();
    imu_accel_xy_received_time_ = this->now();
    imu_z_received_time_ = this->now();

    using std::placeholders::_1;

    {
      can_frame_sub_ = create_subscription<can_msgs::msg::Frame>(
        "input/can_rx", 1, std::bind(&ReportParser::callbackCan, this, _1)); 
    }

    {
      encoder_rpms_pub_ = create_publisher<mivia_rover_can_msgs::msg::EncoderRpms>("/mivia_rover/encoder_rpms", rclcpp::QoS{1});
      imu_gyro_xy_pub_ = create_publisher<mivia_rover_can_msgs::msg::ImuGyroXy>("/mivia_rover/imu_gyro_xy", rclcpp::QoS{1});
      imu_accel_xy_pub_ = create_publisher<mivia_rover_can_msgs::msg::ImuAccelXy>("/mivia_rover/imu_accel_xy", rclcpp::QoS{1});
      imu_z_pub_ = create_publisher<mivia_rover_can_msgs::msg::ImuZ>("/mivia_rover/imu_z", rclcpp::QoS{1});
    }

    {
      timer_ = rclcpp::create_timer(
        this, get_clock(), rclcpp::Rate(param_.loop_rate).period(),
        std::bind(&ReportParser::timerCallback, this));
    }
  }


  
  void ReportParser::callbackCan(const can_msgs::msg::Frame::ConstSharedPtr & msg){

    std_msgs::msg::Header header;
    header.frame_id = param_.base_frame_id;
    header.stamp = msg->header.stamp;

    mivia_rover_can_msgs::msg::EncoderRpms encoder_rpms_msg;
    mivia_rover_can_msgs::msg::ImuGyroXy imu_gyro_xy_msg;
    mivia_rover_can_msgs::msg::ImuAccelXy imu_accel_xy_msg;
    mivia_rover_can_msgs::msg::ImuZ imu_z_msg;


    uint8_t byte_temp[8];
    switch (msg->id){
      
      case EncoderRpms::ID:
      encoder_rpms_received_time_ = this->now();
      
      for(uint i=0;i<8;i++){
      byte_temp[i] = msg->data[i];
      }

      encoder_rpms_entity_.update_bytes(byte_temp);
      encoder_rpms_entity_.Parse();

      encoder_rpms_msg.header = header;
      encoder_rpms_msg.front_left = encoder_rpms_entity_.front_left_;
      encoder_rpms_msg.rear_left = encoder_rpms_entity_.rear_left_;
      encoder_rpms_msg.front_right = encoder_rpms_entity_.front_right_;
      encoder_rpms_msg.rear_right = encoder_rpms_entity_.rear_right_;

      encoder_rpms_ptr_ = std::make_shared<mivia_rover_can_msgs::msg::EncoderRpms>(encoder_rpms_msg);
      break;
      

      case ImuGyroXy::ID:
      imu_gyro_xy_received_time_ = this->now();
      
      for(uint i=0;i<8;i++){
      byte_temp[i] = msg->data[i];
      }

      imu_gyro_xy_entity_.update_bytes(byte_temp);
      imu_gyro_xy_entity_.Parse();

      imu_gyro_xy_msg.header = header;
      imu_gyro_xy_msg.gyro_x = imu_gyro_xy_entity_.gyro_x_;
      imu_gyro_xy_msg.gyro_y = imu_gyro_xy_entity_.gyro_y_;

      imu_gyro_xy_ptr_ = std::make_shared<mivia_rover_can_msgs::msg::ImuGyroXy>(imu_gyro_xy_msg);
      break;
      

      case ImuAccelXy::ID:
      imu_accel_xy_received_time_ = this->now();
      
      for(uint i=0;i<8;i++){
      byte_temp[i] = msg->data[i];
      }

      imu_accel_xy_entity_.update_bytes(byte_temp);
      imu_accel_xy_entity_.Parse();

      imu_accel_xy_msg.header = header;
      imu_accel_xy_msg.accel_x = imu_accel_xy_entity_.accel_x_;
      imu_accel_xy_msg.accel_y = imu_accel_xy_entity_.accel_y_;

      imu_accel_xy_ptr_ = std::make_shared<mivia_rover_can_msgs::msg::ImuAccelXy>(imu_accel_xy_msg);
      break;
      

      case ImuZ::ID:
      imu_z_received_time_ = this->now();
      
      for(uint i=0;i<8;i++){
      byte_temp[i] = msg->data[i];
      }
      imu_z_entity_.update_bytes(byte_temp);
      imu_z_entity_.Parse();

      imu_z_msg.header = header;
      imu_z_msg.gyro_z = imu_z_entity_.gyro_z_;
      imu_z_msg.accel_z = imu_z_entity_.accel_z_;

      imu_z_ptr_ = std::make_shared<mivia_rover_can_msgs::msg::ImuZ>(imu_z_msg);
      break;
      

    default:
      break;
    }
  }

  void ReportParser::timerCallback(){
    

    const rclcpp::Time current_time = this->now();
    
    const double encoder_rpms_report_delta_time_ms =
    (current_time - encoder_rpms_received_time_).seconds() * 1000.0;
    if(encoder_rpms_report_delta_time_ms>param_.report_timeout_ms || encoder_rpms_ptr_==nullptr)
    {
        RCLCPP_ERROR_THROTTLE(
        get_logger(), *this->get_clock(), std::chrono::milliseconds(5000).count(),
        "encoder_rpms report timeout = %f ms.", encoder_rpms_report_delta_time_ms);
    }else{
        encoder_rpms_pub_->publish(*encoder_rpms_ptr_);
    }
    
    const double imu_gyro_xy_report_delta_time_ms =
    (current_time - imu_gyro_xy_received_time_).seconds() * 1000.0;
    if(imu_gyro_xy_report_delta_time_ms>param_.report_timeout_ms || imu_gyro_xy_ptr_==nullptr)
    {
        RCLCPP_ERROR_THROTTLE(
        get_logger(), *this->get_clock(), std::chrono::milliseconds(5000).count(),
        "imu_gyro_xy report timeout = %f ms.", imu_gyro_xy_report_delta_time_ms);
    }else{
        imu_gyro_xy_pub_->publish(*imu_gyro_xy_ptr_);
    }
    
    const double imu_accel_xy_report_delta_time_ms =
    (current_time - imu_accel_xy_received_time_).seconds() * 1000.0;
    if(imu_accel_xy_report_delta_time_ms>param_.report_timeout_ms || imu_accel_xy_ptr_==nullptr)
    {
        RCLCPP_ERROR_THROTTLE(
        get_logger(), *this->get_clock(), std::chrono::milliseconds(5000).count(),
        "imu_accel_xy report timeout = %f ms.", imu_accel_xy_report_delta_time_ms);
    }else{
        imu_accel_xy_pub_->publish(*imu_accel_xy_ptr_);
    }
    
    const double imu_z_report_delta_time_ms =
    (current_time - imu_z_received_time_).seconds() * 1000.0;
    if(imu_z_report_delta_time_ms>param_.report_timeout_ms || imu_z_ptr_==nullptr)
    {
        RCLCPP_ERROR_THROTTLE(
        get_logger(), *this->get_clock(), std::chrono::milliseconds(5000).count(),
        "imu_z report timeout = %f ms.", imu_z_report_delta_time_ms);
    }else{
        imu_z_pub_->publish(*imu_z_ptr_);
    }
      
    
  }

  } 
} 
