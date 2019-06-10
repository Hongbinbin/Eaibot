find_package(catkin REQUIRED COMPONENTS
  rosconsole
  roscpp
  sensor_msgs
  actionlib
)

## Declare a cpp executable
add_executable(TrajMoveDemo TrajMoveDemo.cpp)
add_dependencies(TrajMoveDemo neo_msgs_gencpp)
target_link_libraries(TrajMoveDemo ${catkin_LIBRARIES})

install(TARGETS TrajMoveDemo DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})


## Declare a cpp executable demo01
add_executable(demo01 demo01.cpp)
add_dependencies(demo01 neo_msgs_gencpp)
target_link_libraries(demo01 ${catkin_LIBRARIES})

install(TARGETS demo01 DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})


## Declare a cpp executable demo02
add_executable(demo02 demo02.cpp)
add_dependencies(demo02 neo_msgs_gencpp)
target_link_libraries(demo02 ${catkin_LIBRARIES})

install(TARGETS demo02 DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})

## Declare a cpp executable demo03
add_executable(demo03 demo03.cpp)
add_dependencies(demo03 neo_msgs_gencpp)
target_link_libraries(demo03 ${catkin_LIBRARIES})

install(TARGETS demo03 DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})

## Declare a cpp executable demo04
add_executable(demo04 demo04.cpp)
add_dependencies(demo04 neo_msgs_gencpp)
target_link_libraries(demo04 ${catkin_LIBRARIES})

install(TARGETS demo04 DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})


## Declare a cpp executable demo05
add_executable(demo05 demo05.cpp)
add_dependencies(demo05 neo_msgs_gencpp)
target_link_libraries(demo05 ${catkin_LIBRARIES})

install(TARGETS demo05 DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})



## Declare a cpp executable mix
add_executable(mix mix.cpp)
add_dependencies(mix neo_msgs_gencpp)
target_link_libraries(mix ${catkin_LIBRARIES})

install(TARGETS mix DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})