#pragma once

#include <mivia_rover_can_driver/Byte.hpp>
#include <iostream>

class ImuGyroXy {
public:
    static const uint32_t ID = 0xC;
    ImuGyroXy();
    void Parse();
    void update_bytes(uint8_t bytes_data[8]);
    // singal
    int gyro_x_;
    int gyro_y_;
    

private:
    uint8_t bytes[8];
    
  // config detail: {'bit': 0, 'is_signed_var': True, 'len': 32, 'name': 'gyro_x', 'offset': 0.0, 'order': 'intel', 'physical_range': '[0|0]', 'physical_unit': 'rad/s', 'precision': 1.0, 'type': 'int'}
  int gyrox();

  // config detail: {'bit': 32, 'is_signed_var': True, 'len': 32, 'name': 'gyro_y', 'offset': 0.0, 'order': 'intel', 'physical_range': '[0|0]', 'physical_unit': 'rad/s', 'precision': 1.0, 'type': 'int'}
  int gyroy();
};



