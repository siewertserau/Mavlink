project (uavionix_test)

cmake_minimum_required(VERSION 2.8)

#include_directories(protocol)
#include_directories(protocol/uAvionix_in)

add_library(mavlink_parse_adsb mavlink_parse_adsb.cxx)
add_library(type_conversions type_conversions.cxx)
add_executable(uavionix_test main.cxx)
target_link_libraries(uavionix_test mavlink_parse_adsb type_conversions)
