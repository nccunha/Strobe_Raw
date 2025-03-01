########### install files ###############
FILE(GLOB Common_FILES "libuv.so" "CMakeLists.txt")

install(FILES ${Common_FILES}
		DESTINATION ${SOURCE_INSTALL_DIR}/examples/raw_commands/Common/lib/linux/x64
		COMPONENT Devel)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
        
	install(FILES ${Common_FILES}
	        DESTINATION /usr/local/lib
		COMPONENT Devel)
    endif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
endif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")