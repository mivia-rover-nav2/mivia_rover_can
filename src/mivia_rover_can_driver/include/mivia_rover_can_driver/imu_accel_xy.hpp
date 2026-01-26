#pragma once

#include <mivia_rover_can_driver/Byte.hpp>
#include <iostream>

class ImuAccelXy {
public:
    static const uint32_t ID = 0xD;

    ImuAccelXy();
    void Parse();
    void update_bytes(uint8_t bytes_data[8]);
    
    int accel_x_;
    int accel_y_;
    

private:
    uint8_t bytes[8];
    
  // config detail: {'bit': 0, 'is_signed_var': True, 'len': 32, 'name': 'accel_x', 'offset': 0.0, 'order': 'intel', 'physical_range': '[0|0]', 'physical_unit': 'm/s^2', 'precision': 1.0, 'type': 'int'}
  int accelx();

  // config detail: {'bit': 32, 'is_signed_var': True, 'len': 32, 'name': 'accel_y', 'offset': 0.0, 'order': 'intel', 'physical_range': '[0|0]', 'physical_unit': 'm/s^2', 'precision': 1.0, 'type': 'int'}
  int accely();
};



