CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

PROJECT(camera_calibration)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	# set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c++20 -Ofast -lstdc++fs")
	# set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -Ofast -lstdc++fs")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c++20 -lstdc++fs")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -lstdc++fs")

	# # set(CMAKE_VERBOSE_MAKEFILE ON)
endif()

# ############# Required Libraries
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake_modules")

find_package(OpenCV REQUIRED)

# ############# Default target to build
if(NOT CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

MESSAGE("Compiled in ${CMAKE_BUILD_TYPE}")

SET(CMAKE_EXPORT_COMPILE_COMMANDS 1)
MESSAGE("Export compile commands: ${CMAKE_EXPORT_COMPILE_COMMANDS}")

message("CMake C Flags pre: ${CMAKE_C_FLAGS}")
message("CMake CXX Flags pre: ${CMAKE_CXX_FLAGS}")

# ############# Output to the "bin" and "lib" folders
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
	STRING(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
	SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/bin)
	SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/lib)
endforeach(OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)

# ####### Includes for everybody ############
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/src")
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})

# ####### Install directories ############
if(NOT WIN32)
	SET(INSTALL_INCLUDE_DIR /usr/local/include/vic/camera_calibration)
	SET(INSTALL_LIB_DIR /usr/local/lib64)
	SET(INSTALL_BIN_DIR /usr/local/bin)
else()
	SET(INSTALL_BIN_DIR d:/devel/3rdparty/bin)
endif()

# For generating folders in IDE by same directory structure
file(GLOB_RECURSE _source_list *.cpp* *.h* *.hpp*)

foreach(_source IN ITEMS ${_source_list})
	get_filename_component(_source_path "${_source}" PATH)
	string(REPLACE "${CMAKE_SOURCE_DIR}" "" _group_path "${_source_path}")
	string(REPLACE "/" "\\" _group_path "${_group_path}")
	source_group("${_group_path}" FILES "${_source}")
endforeach()

# #### @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ #####
# #### @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ #####
# #### @@@@@ LIBRARIES @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ #####
# #### @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ #####
# #### @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ #####

# ############################################################
# #### CAMERA CALIBRATION LIBRARY ############################
# ############################################################
ADD_LIBRARY(camera_calibration # SHARED
	src/camera_calibration/chessboard_pattern.hpp
	src/camera_calibration/chessboard_pattern.cpp
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set_target_properties(camera_calibration PROPERTIES LINKER_LANGUAGE CXX)
	target_compile_options(camera_calibration PRIVATE -Wno-maybe-uninitialized -Wno-deprecated-copy -Werror -Wno-deprecated -Wall -Wextra)
endif()

TARGET_LINK_LIBRARIES(camera_calibration
	${OpenCV_LIBRARIES}
)
file(GLOB_RECURSE header_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/src/camera_calibration/ *.hpp)

foreach(file ${header_files})
	get_filename_component(dir ${file} DIRECTORY)
	install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/camera_calibration/${file} DESTINATION ${INSTALL_INCLUDE_DIR}/${dir})
endforeach()

# #### @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ #####
# #### @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ #####
# #### @@@@@ APPS @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ #####
# #### @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ #####
# #### @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ #####

# ############################################################
# #### EXE CALIBRATE CAMERA ##################################
# ############################################################
ADD_EXECUTABLE(calibrate_camera
	src/exe_calibrate_camera/main_calibrate_camera.cpp
	src/exe_calibrate_camera/main_calibrate_camera.hpp
	src/exe_calibrate_camera/calibrate_camera_error_handler.hpp
	src/exe_calibrate_camera/calibrate_camera_error_handler.cpp
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set_target_properties(calibrate_camera PROPERTIES LINKER_LANGUAGE CXX)
	target_compile_options(calibrate_camera PRIVATE -Wno-deprecated-enum-enum-conversion -Wno-deprecated-copy -Werror -Wall -Wextra)
endif()

TARGET_LINK_LIBRARIES(calibrate_camera
	${OpenCV_LIBRARIES}
	camera_calibration
)

install(TARGETS calibrate_camera DESTINATION ${INSTALL_BIN_DIR})

# ############################################################
# #### EXE FIND CHESSBOARD CORNERS ###########################
# ############################################################
ADD_EXECUTABLE(find_chessboard_corners
	src/exe_find_chessboard_corners/main_find_chessboard_corners.cpp
	src/exe_find_chessboard_corners/main_find_chessboard_corners.hpp
	src/exe_find_chessboard_corners/find_chessboard_corners_error_handler.hpp
	src/exe_find_chessboard_corners/find_chessboard_corners_error_handler.cpp
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set_target_properties(find_chessboard_corners PROPERTIES LINKER_LANGUAGE CXX)
	target_compile_options(find_chessboard_corners PRIVATE -Wno-deprecated-enum-enum-conversion -Wno-deprecated-copy -Werror -Wall -Wextra)
endif()

TARGET_LINK_LIBRARIES(find_chessboard_corners
	${OpenCV_LIBRARIES}
	camera_calibration
)

install(TARGETS find_chessboard_corners DESTINATION ${INSTALL_BIN_DIR})

# ############################################################
# #### SANDBOX ###############################################
# ############################################################
ADD_EXECUTABLE(sandbox
	src/exe_sandbox/main_sandbox.cpp
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set_target_properties(sandbox PROPERTIES LINKER_LANGUAGE CXX)
	target_compile_options(sandbox PRIVATE -Wno-deprecated-enum-enum-conversion -Wno-deprecated-copy -Werror -Wall -Wextra)
endif()

TARGET_LINK_LIBRARIES(sandbox
	${OpenCV_LIBRARIES}
	camera_calibration
)

install(TARGETS sandbox DESTINATION ${INSTALL_BIN_DIR})
