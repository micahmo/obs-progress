# obs-progress/CMakeLists.txt

cmake_minimum_required(VERSION 3.5)
project(obs-progress)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(LibObs REQUIRED)
find_package(date CONFIG REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)


set(obs-progress_SOURCES
	src/obs-progress.cpp
	src/ProgressDockWidget.cpp
	src/ProgressContainerLayout.cpp
	src/Globals.cpp
	src/ProgressSlider.cpp
	src/SliderStyle.cpp
	)
	  
set(obs-progress_HEADERS
	src/ProgressDockWidget.h
	src/ProgressContainerLayout.h
	src/Globals.h
	src/ProgressSlider.h
	src/SliderStyle.h
	)

add_library(obs-progress MODULE
	${obs-progress_SOURCES}
	${obs-progress_HEADERS}
	)

include_directories(
	${OBS_LIBOBS_INCLUDE}
	${OBS_FRONTEND_INCLUDE})

target_link_libraries(obs-progress
    ${OBS_LIB_DIR}
	Qt6::Core
	Qt6::Widgets)

if(WIN32)
	if(NOT DEFINED OBS_FRONTEND_LIB)
		set(OBS_FRONTEND_LIB "OBS_FRONTEND_LIB-NOTFOUND" CACHE FILEPATH "OBS frontend library")
		message(FATAL_ERROR "Could not find OBS Frontend API's library !")
	endif()
	
	target_link_libraries(obs-progress
		"${OBS_FRONTEND_LIB}")

	# --- Release package helper ---
	# The "release" folder has a structure similar OBS' one on Windows
	set(RELEASE_DIR "${PROJECT_SOURCE_DIR}/release")

endif()