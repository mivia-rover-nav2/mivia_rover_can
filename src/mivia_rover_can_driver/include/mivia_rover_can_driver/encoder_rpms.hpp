#pragma once

#include <mivia_rover_can_driver/Byte.hpp>
#include <iostream>

class EncoderRpms {
public:
    static const uint32_t ID = 0xB;
    EncoderRpms();
    void Parse();
    void update_bytes(uint8_t bytes_data[8]);
    // singal
    int front_left_;
    int rear_left_;
    int front_right_;
    int rear_right_;
    

private:
    uint8_t bytes[8];
    
  // config detail: {'bit': 0, 'is_signed_var': True, 'len': 16, 'name': 'front_left', 'offset': 0.0, 'order': 'intel', 'physical_range': '[-180|180]', 'physical_unit': 'rpm', 'precision': 1.0, 'type': 'int'}
  int frontleft();

  // config detail: {'bit': 16, 'is_signed_var': True, 'len': 16, 'name': 'rear_left', 'offset': 0.0, 'order': 'intel', 'physical_range': '[-180|180]', 'physical_unit': 'rpm', 'precision': 1.0, 'type': 'int'}
  int rearleft();

  // config detail: {'bit': 32, 'is_signed_var': True, 'len': 16, 'name': 'front_right', 'offset': 0.0, 'order': 'intel', 'physical_range': '[-180|180]', 'physical_unit': 'rpm', 'precision': 1.0, 'type': 'int'}
  int frontright();

  // config detail: {'bit': 48, 'is_signed_var': True, 'len': 16, 'name': 'rear_right', 'offset': 0.0, 'order': 'intel', 'physical_range': '[-180|180]', 'physical_unit': 'rpm', 'precision': 1.0, 'type': 'int'}
  int rearright();
};



