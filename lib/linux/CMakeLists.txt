CMAKE_MINIMUM_REQUIRED(VERSION 2.8.9)

add_subdirectory(x64)
add_subdirectory(x86)

########### install files ###############
FILE(GLOB txt_FILES "*.txt")

install(FILES ${txt_FILES}
		DESTINATION ${SOURCE_INSTALL_DIR}/examples/raw_commands/Common/lib/linux)

