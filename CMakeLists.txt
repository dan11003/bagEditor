cmake_minimum_required(VERSION 2.4.6)
project(bag_editor)
set (CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE release)

if("$ENV{ROS_DISTRO}" STREQUAL "indigo")
    add_definitions(-std=c++11)
endif()
find_package(catkin REQUIRED
    geometry_msgs
    cmake_modules
    eigen_conversions
    pcl_ros
    roscpp
    sensor_msgs
    std_msgs
    tf_conversions
    tf
    genmsg
    tf_conversions
    )

find_package(cmake_modules REQUIRED)
find_package(Eigen REQUIRED COMPONENTS)
catkin_package(
    #DEPENDS Eigen libpcl-all-dev gnuplot
    #   CATKIN_DEPENDS ndt_registration ndt_map roscpp laser_geometry sensor_msgs tf_conversions tf geometry_msgs cmake_modules eigen_conversions pcl_ros
    DEPENDS Eigen libpcl-all-dev
    CATKIN_DEPENDS  roscpp sensor_msgs tf_conversions tf geometry_msgs cmake_modules eigen_conversions pcl_ros std_msgs genmsg
    #LIBRARIES ${PROJECT_NAME}
    )
include_directories(include  ${Boost_INCLUDE_DIRS} ${catkin_INCLUDE_DIRS} )


add_executable(merge_bag src/merge_bag.cpp )
target_link_libraries(merge_bag  ${catkin_LIBRARIES}  ${Boost_LIBRARIES} )



