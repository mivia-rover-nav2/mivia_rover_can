#pragma once
#include <mivia_rover_can_driver/Byte.hpp>

class Reference {
public:
	static  int32_t ID;

	Reference();

  	void UpdateData(int front_left, int rear_left, int front_right, int rear_right);

  	void Reset();
  
  	uint8_t *get_data();


private:
	
  // config detail: {'bit': 0, 'is_signed_var': True, 'len': 16, 'name': 'front_left', 'offset': 0.0, 'order': 'intel', 'physical_range': '[0|0]', 'physical_unit': 'rpm', 'precision': 1.0, 'type': 'int'}
  void set_p_front_left(int front_left);

  // config detail: {'bit': 16, 'is_signed_var': True, 'len': 16, 'name': 'rear_left', 'offset': 0.0, 'order': 'intel', 'physical_range': '[0|0]', 'physical_unit': 'rpm', 'precision': 1.0, 'type': 'int'}
  void set_p_rear_left(int rear_left);

  // config detail: {'bit': 32, 'is_signed_var': True, 'len': 16, 'name': 'front_right', 'offset': 0.0, 'order': 'intel', 'physical_range': '[0|0]', 'physical_unit': 'rpm', 'precision': 1.0, 'type': 'int'}
  void set_p_front_right(int front_right);

  // config detail: {'bit': 48, 'is_signed_var': True, 'len': 16, 'name': 'rear_right', 'offset': 0.0, 'order': 'intel', 'physical_range': '[0|0]', 'physical_unit': 'rpm', 'precision': 1.0, 'type': 'int'}
  void set_p_rear_right(int rear_right);

private:
	uint8_t data[8];
};



