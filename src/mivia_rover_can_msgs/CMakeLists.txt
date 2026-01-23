cmake_minimum_required(VERSION 3.5)
project(mivia_rover_can_msgs)

find_package(ament_cmake_auto REQUIRED)
ament_auto_find_build_dependencies()

set(msg_files
	"msg/Reference.msg"
	"msg/EncoderRpms.msg"
	"msg/ImuGyroXy.msg"
	"msg/ImuAccelXy.msg"
	"msg/ImuZ.msg"

)

set(msg_dependencies
	std_msgs
  builtin_interfaces)

rosidl_generate_interfaces(${PROJECT_NAME}
  ${msg_files}
  DEPENDENCIES ${msg_dependencies}
  ADD_LINTER_TESTS)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_auto_package()