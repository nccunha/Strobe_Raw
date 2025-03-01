########### install files ###############
FILE(GLOB Common_FILES "libuv.so" "CMakeLists.txt")

install(FILES ${Common_FILES}
		DESTINATION ${SOURCE_INSTALL_DIR}/examples/raw_commands/Common/lib/linux/x86
		COMPONENT Devel)


if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "i686")
        
        install(FILES ${Common_FILES}
	        DESTINATION /usr/local/lib
		COMPONENT Devel)
    endif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "i686")
endif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")