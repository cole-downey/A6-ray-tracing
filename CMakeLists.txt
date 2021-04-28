CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# Name of the project
PROJECT(A6)

# To run A6.sln in command line: MSBuild A6.sln -p:Configuration=Release

# Use glob to get the list of all source files.
FILE(GLOB_RECURSE SOURCES "src/*.cpp")

# We don't really need to include header and resource files to build, but it's
# nice to have them show up in IDEs.
FILE(GLOB_RECURSE HEADERS "src/*.h")
FILE(GLOB_RECURSE GLSL "resources/*.glsl")

# Set the executable.
ADD_EXECUTABLE(${CMAKE_PROJECT_NAME} ${SOURCES} ${HEADERS} ${GLSL})

# Get the GLM environment variable. Since GLM is a header-only library, we
# just need to add it to the include directory.
SET(GLM_INCLUDE_DIR "$ENV{GLM_INCLUDE_DIR}")
IF(NOT GLM_INCLUDE_DIR)
	# The environment variable was not set
	SET(ERR_MSG "Please point the environment variable GLM_INCLUDE_DIR to the root directory of your GLM installation.")
	IF(WIN32)
		# On Windows, try the default location
		MESSAGE(STATUS "Looking for GLM in ${DEF_DIR_GLM}")
		IF(IS_DIRECTORY ${DEF_DIR_GLM})
			MESSAGE(STATUS "Found!")
			SET(GLM_INCLUDE_DIR ${DEF_DIR_GLM})
		ELSE()
			MESSAGE(FATAL_ERROR ${ERR_MSG})
		ENDIF()
	ELSE()
		MESSAGE(FATAL_ERROR ${ERR_MSG})
	ENDIF()
ENDIF()
INCLUDE_DIRECTORIES(${GLM_INCLUDE_DIR})

# OS specific options and libraries
IF(WIN32)
	# c++11 is enabled by default.
	# -Wall produces way too many warnings.
	# -pedantic is not supported.
	# Disable warning 4996.
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4996")
	TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} opengl32.lib)
	SET_PROPERTY(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME})
ELSE()
	# Enable all pedantic warnings.
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pedantic")
	IF(APPLE)
		# Add required frameworks for GLFW.
		TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} "-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo")
	ELSE()
		#Link the Linux OpenGL library
		TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} "GL")
	ENDIF()
ENDIF()
