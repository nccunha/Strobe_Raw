########### install files ###############
FILE(GLOB Common_FILES ".h" ".txt")

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	FILE(GLOB Common_FILES "android-ifaddrs.h" "CMakeLists.txt" "pthread-barrier.h" "stdint-msvc2008.h" "tree.h" "uv.h" "uv-aix.h" "uv-bsd.h" "uv-darwin.h"	
				"uv-errno.h" "uv-linux.h"  "uv-os390.h" "uv-posix.h" "uv-sunos.h" "uv-threadpool.h" "uv-unix.h" "uv-version.h" "uv-win.h"	
				)
endif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")

install(FILES ${Common_FILES}
		DESTINATION ${SOURCE_INSTALL_DIR}/examples/raw_commands/Common/include
		COMPONENT Devel)




