#include <mivia_rover_can_driver/imu_z.hpp>


ImuZ::ImuZ() {}

void ImuZ::update_bytes(uint8_t bytes_data[8])
{
  for(uint i=0;i<8;i++)
  {
    bytes[i] = bytes_data[i];
  }
}

void ImuZ::Parse() {
  gyro_z_ = gyroz();
  accel_z_ = accelz();
}


// config detail: {'bit': 0, 'is_signed_var': True, 'len': 32, 'name': 'gyro_z', 'offset': 0.0, 'order': 'intel', 'physical_range': '[0|0]', 'physical_unit': 'rad/s', 'precision': 1.0, 'type': 'int'}
int ImuZ::gyroz() {
  Byte t0(*(bytes + 3));
  int32_t x = t0.get_byte(0, 8);

  Byte t1(*(bytes + 2));
  int32_t t = t1.get_byte(0, 8);
  x <<= 8;
  x |= t;

  Byte t2(*(bytes + 1));
  t = t2.get_byte(0, 8);
  x <<= 8;
  x |= t;

  Byte t3(*(bytes + 0));
  t = t3.get_byte(0, 8);
  x <<= 8;
  x |= t;

  x <<= 0;
  x >>= 0;

  int ret = x;
  return ret;
}

// config detail: {'bit': 32, 'is_signed_var': True, 'len': 32, 'name': 'accel_z', 'offset': 0.0, 'order': 'intel', 'physical_range': '[0|0]', 'physical_unit': 'm/s^2', 'precision': 1.0, 'type': 'int'}
int ImuZ::accelz() {
  Byte t0(*(bytes + 7));
  int32_t x = t0.get_byte(0, 8);

  Byte t1(*(bytes + 6));
  int32_t t = t1.get_byte(0, 8);
  x <<= 8;
  x |= t;

  Byte t2(*(bytes + 5));
  t = t2.get_byte(0, 8);
  x <<= 8;
  x |= t;

  Byte t3(*(bytes + 4));
  t = t3.get_byte(0, 8);
  x <<= 8;
  x |= t;

  x <<= 0;
  x >>= 0;

  int ret = x;
  return ret;
}

