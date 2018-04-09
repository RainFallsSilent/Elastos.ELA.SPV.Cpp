# REQUIREMENTS: this script needs CMake 2.8 or higher. Downloads are available at http://www.cmake.org

# How to build Elastos.ELA.SPV.Cpp using CMake under Unix (Linux, Darwin etc.)?
# 1. Copy this file, as-is, to the top-level Elastos.ELA.SPV.Cpp folder
# 2. Make a build directory(Elastos.ELA.SPV.Cpp.build) somewhere outside the Elastos.ELA.SPV.Cpp source tree
# 3. cd Elastos.ELA.SPV.Cpp.build; cmake "relative/path/to/Elastos.ELA.SPV.Cpp"
# 4. make
# 5. make install (or sudo make install)

# How to build Elastos.ELA.SPV.Cpp using CMake under Windows (Linux, Darwin etc.)?
# 1. Copy this file, as-is, to the top-level folder
# 2. Launch the cmake-gui, and select the top-level folder
# 3. You can choose whatever you want as an install directory
# 4. Click 'Configure'
# 5. Choose your generator
# 6. When configure is achived, you can change the variable with a red background
# 7. Click 'Generate'
# 8. From your IDE, open the generated makefile and run the compilation.

cmake_minimum_required( VERSION 3.0 )

project(SPVCLIENT VERSION 0.1.0.0)

set(TARGET_NAME cli-spv)

message(STATUS "${TARGET_NAME} v${PROJECT_VERSION}")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define helper macro option_with_default
macro( option_with_default OPTION_NAME OPTION_STRING OPTION_DEFAULT )
	if( NOT DEFINED ${OPTION_NAME} )
		set( ${OPTION_NAME} ${OPTION_DEFAULT} )
	endif( NOT DEFINED ${OPTION_NAME} )
	option( ${OPTION_NAME} "${OPTION_STRING}" ${${OPTION_NAME}} )
endmacro( option_with_default OPTION_NAME OPTION_STRING OPTION_DEFAULT )

############################
# Check 32/64 bit platform #
############################
if(${CMAKE_SIZEOF_VOID_P} MATCHES "8") # It is 64bit, otherwise 32 bit systems match 4
	set(BIT 64)
else (${CMAKE_SIZEOF_VOID_P} MATCHES "8")
	set(BIT 32)
endif(${CMAKE_SIZEOF_VOID_P} MATCHES "8")

message(STATUS "Build ${BIT}bit")

if(NOT CMAKE_CONFIGURATION_TYPES)
	if(CMAKE_BUILD_TYPE STREQUAL "")
		set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the build type." FORCE)
		set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Release" "Debug" "RelWithDebInfo" "MinSizeRel")
	endif(CMAKE_BUILD_TYPE STREQUAL "")
endif(NOT CMAKE_CONFIGURATION_TYPES)

if(CMAKE_BUILD_TOOL STREQUAL "nmake")
	set(NMAKE TRUE)
endif(CMAKE_BUILD_TOOL STREQUAL "nmake")

if(MSVC)
	option_with_default( SPV_INSTALL_PDB_FILES "Install PDB files (debug informations)" ON )
endif(MSVC)

if(WIN32)
	option_with_default( SPV_AUTOINSTALL_DEPENDENT_LIBS "Autoinstall dependent DLLs" ON )
	mark_as_advanced(SPV_AUTOINSTALL_DEPENDENT_LIBS)
endif()

if( NOT DEFINED SPV_INSTALL_PREFIX )
	if( WIN32 )
		set(SPV_INSTALL_PREFIX "c:/${TARGET_NAME}-${SPV_VERSION}")
	else( WIN32 )
		set(SPV_INSTALL_PREFIX "/usr/local/")
	endif( WIN32 )
endif( NOT DEFINED SPV_INSTALL_PREFIX )

set( SPV_INSTALL_PREFIX ${SPV_INSTALL_PREFIX} CACHE
	PATH "Install prefix prepended to target to create install location" )
set( CMAKE_INSTALL_PREFIX "${SPV_INSTALL_PREFIX}" CACHE INTERNAL "Prefix jprepended to install directories if target destination is not absolute, immutable" FORCE )

option_with_default( SPV_BUILD_SHARED_LIB "Build shared SPV libs" ON )

if( SPV_BUILD_SHARED_LIB )
	set( SPV_LIBRARY_TYPE SHARED )
	set( SPV_BUILD_STATIC_LIB FALSE )
else( SPV_BUILD_SHARED_LIB )
	set( SPV_LIBRARY_TYPE STATIC )
	set( SPV_BUILD_STATIC_LIB TRUE )
	if ( WIN32 )
		add_definitions("-DHAVE_NO_DLL")
	endif( WIN32 )
endif( SPV_BUILD_SHARED_LIB )


set_directory_properties(PROPERTIES COMPILE_DEFINITIONS_RELEASE NDEBUG)
set_directory_properties(PROPERTIES COMPILE_DEFINITIONS_MINSIZEREL NDEBUG)
set_directory_properties(PROPERTIES COMPILE_DEFINITIONS_RELWITHDEBINFO NDEBUG)

add_subdirectory(SDK)
add_subdirectory(Core)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/App APPLICATION_SOURCE_FILES)
add_executable(${TARGET_NAME} ${APPLICATION_SOURCE_FILES})

target_link_libraries(${TARGET_NAME} sqlite3)
target_link_libraries(${TARGET_NAME} spvsdk)
target_link_libraries(${TARGET_NAME} spvcore)
target_link_libraries(${TARGET_NAME} pthread)

target_include_directories(${TARGET_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/SDK>)
target_include_directories(${TARGET_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Core>)
target_include_directories(${TARGET_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include/Catch2/single_include>)
target_include_directories(${TARGET_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include/nlohmann/single_include>)
target_include_directories(${TARGET_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include/spdlog/include>)
