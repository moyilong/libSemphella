ADD_EXECUTABLE(mpshell2  
	main.cpp
	)

INCLUDE_DIRECTORIES(..)
INSTALL(TARGETS mpshell2 DESTINATION bin)
TARGET_LINK_LIBRARIES(mpshell2 -lSemphella -lMps)