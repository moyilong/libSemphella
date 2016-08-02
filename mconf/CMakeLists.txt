ADD_EXECUTABLE(mconf apply.cpp CC.cpp compile_config.cpp get_config.cpp mconf.cpp menu_process.cpp prompt.cpp stdafx.cpp)
INSTALL (TARGETS mconf DESTINATION bin)
TARGET_LINK_LIBRARIES(mconf -lSemphella)
INCLUDE_DIRECTORIES(..)
