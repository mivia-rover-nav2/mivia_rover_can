#include <mivia_rover_can_driver/encoder_rpms.hpp>


EncoderRpms::EncoderRpms() {}

void EncoderRpms::update_bytes(uint8_t bytes_data[8])
{
  for(uint i=0;i<8;i++)
  {
    bytes[i] = bytes_data[i];
  }
}

void EncoderRpms::Parse() {
  front_left_ = frontleft();
  rear_left_ = rearleft();
  front_right_ = frontright();
  rear_right_ = rearright();
}


// config detail: {'bit': 0, 'is_signed_var': True, 'len': 16, 'name': 'front_left', 'offset': 0.0, 'order': 'intel', 'physical_range': '[-180|180]', 'physical_unit': 'rpm', 'precision': 1.0, 'type': 'int'}
int EncoderRpms::frontleft() {
  Byte t0(*(bytes + 1));
  int32_t x = t0.get_byte(0, 8);

  Byte t1(*(bytes + 0));
  int32_t t = t1.get_byte(0, 8);
  x <<= 8;
  x |= t;

  x <<= 16;
  x >>= 16;

  int ret = x;
  return ret;
}

// config detail: {'bit': 16, 'is_signed_var': True, 'len': 16, 'name': 'rear_left', 'offset': 0.0, 'order': 'intel', 'physical_range': '[-180|180]', 'physical_unit': 'rpm', 'precision': 1.0, 'type': 'int'}
int EncoderRpms::rearleft() {
  Byte t0(*(bytes + 3));
  int32_t x = t0.get_byte(0, 8);

  Byte t1(*(bytes + 2));
  int32_t t = t1.get_byte(0, 8);
  x <<= 8;
  x |= t;

  x <<= 16;
  x >>= 16;

  int ret = x;
  return ret;
}

// config detail: {'bit': 32, 'is_signed_var': True, 'len': 16, 'name': 'front_right', 'offset': 0.0, 'order': 'intel', 'physical_range': '[-180|180]', 'physical_unit': 'rpm', 'precision': 1.0, 'type': 'int'}
int EncoderRpms::frontright() {
  Byte t0(*(bytes + 5));
  int32_t x = t0.get_byte(0, 8);

  Byte t1(*(bytes + 4));
  int32_t t = t1.get_byte(0, 8);
  x <<= 8;
  x |= t;

  x <<= 16;
  x >>= 16;

  int ret = x;
  return ret;
}

// config detail: {'bit': 48, 'is_signed_var': True, 'len': 16, 'name': 'rear_right', 'offset': 0.0, 'order': 'intel', 'physical_range': '[-180|180]', 'physical_unit': 'rpm', 'precision': 1.0, 'type': 'int'}
int EncoderRpms::rearright() {
  Byte t0(*(bytes + 7));
  int32_t x = t0.get_byte(0, 8);

  Byte t1(*(bytes + 6));
  int32_t t = t1.get_byte(0, 8);
  x <<= 8;
  x |= t;

  x <<= 16;
  x >>= 16;

  int ret = x;
  return ret;
}

