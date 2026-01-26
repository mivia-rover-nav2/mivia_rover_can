#include <mivia_rover_can_driver/reference.hpp>

int32_t Reference::ID = 0xA;

// public
Reference::Reference() { Reset(); }

void Reference::UpdateData(int front_left, int rear_left, int front_right, int rear_right) {
  set_p_front_left(front_left);
  set_p_rear_left(rear_left);
  set_p_front_right(front_right);
  set_p_rear_right(rear_right);
}

void Reference::Reset() {
  // TODO(All) :  you should check this manually
  for(uint8_t i=0;i<8;i++)
  {
    data[i] = 0;
  }
}

uint8_t * Reference::get_data()
{
  return data;
}



// config detail: {'bit': 0, 'is_signed_var': True, 'len': 16, 'name': 'front_left', 'offset': 0.0, 'order': 'intel', 'physical_range': '[0|0]', 'physical_unit': 'rpm', 'precision': 1.0, 'type': 'int'}
void Reference::set_p_front_left(int front_left) {
  // front_left = ProtocolData::BoundedValue(0, 0, front_left);
  int x = front_left;
  uint8_t a = 0;
  uint8_t t = 0;

  t = x & 0xFF;
  Byte to_set0(a);
  to_set0.set_value(t, 0, 8);
  data[0] += to_set0.return_byte_t();
  x >>= 8;

  t = x & 0xFF;
  Byte to_set1(a);
  to_set1.set_value(t, 0, 8);
  data[1] += to_set1.return_byte_t();
}

// config detail: {'bit': 16, 'is_signed_var': True, 'len': 16, 'name': 'rear_left', 'offset': 0.0, 'order': 'intel', 'physical_range': '[0|0]', 'physical_unit': 'rpm', 'precision': 1.0, 'type': 'int'}
void Reference::set_p_rear_left(int rear_left) {
  // rear_left = ProtocolData::BoundedValue(0, 0, rear_left);
  int x = rear_left;
  uint8_t a = 0;
  uint8_t t = 0;

  t = x & 0xFF;
  Byte to_set0(a);
  to_set0.set_value(t, 0, 8);
  data[2] += to_set0.return_byte_t();
  x >>= 8;

  t = x & 0xFF;
  Byte to_set1(a);
  to_set1.set_value(t, 0, 8);
  data[3] += to_set1.return_byte_t();
}

// config detail: {'bit': 32, 'is_signed_var': True, 'len': 16, 'name': 'front_right', 'offset': 0.0, 'order': 'intel', 'physical_range': '[0|0]', 'physical_unit': 'rpm', 'precision': 1.0, 'type': 'int'}
void Reference::set_p_front_right(int front_right) {
  // front_right = ProtocolData::BoundedValue(0, 0, front_right);
  int x = front_right;
  uint8_t a = 0;
  uint8_t t = 0;

  t = x & 0xFF;
  Byte to_set0(a);
  to_set0.set_value(t, 0, 8);
  data[4] += to_set0.return_byte_t();
  x >>= 8;

  t = x & 0xFF;
  Byte to_set1(a);
  to_set1.set_value(t, 0, 8);
  data[5] += to_set1.return_byte_t();
}

// config detail: {'bit': 48, 'is_signed_var': True, 'len': 16, 'name': 'rear_right', 'offset': 0.0, 'order': 'intel', 'physical_range': '[0|0]', 'physical_unit': 'rpm', 'precision': 1.0, 'type': 'int'}
void Reference::set_p_rear_right(int rear_right) {
  // rear_right = ProtocolData::BoundedValue(0, 0, rear_right);
  int x = rear_right;
  uint8_t a = 0;
  uint8_t t = 0;

  t = x & 0xFF;
  Byte to_set0(a);
  to_set0.set_value(t, 0, 8);
  data[6] += to_set0.return_byte_t();
  x >>= 8;

  t = x & 0xFF;
  Byte to_set1(a);
  to_set1.set_value(t, 0, 8);
  data[7] += to_set1.return_byte_t();
}


