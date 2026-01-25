#pragma once

#include <mivia_rover_can_driver/Byte.hpp>
#include <iostream>

class ImuZ {
public:
    static const uint32_t ID = 0xE;

    ImuZ();
    void Parse();
    void update_bytes(uint8_t bytes_data[8]);
    
    int gyro_z_;
    int accel_z_;
    

private:
    uint8_t bytes[8];
    
  // config detail: {'bit': 0, 'is_signed_var': True, 'len': 32, 'name': 'gyro_z', 'offset': 0.0, 'order': 'intel', 'physical_range': '[0|0]', 'physical_unit': 'rad/s', 'precision': 1.0, 'type': 'int'}
  int gyroz();

  // config detail: {'bit': 32, 'is_signed_var': True, 'len': 32, 'name': 'accel_z', 'offset': 0.0, 'order': 'intel', 'physical_range': '[0|0]', 'physical_unit': 'm/s^2', 'precision': 1.0, 'type': 'int'}
  int accelz();
};



