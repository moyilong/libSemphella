ADD_LIBRARY(Skovia SHARED libSkovia.cpp
	SkoviaAutoInit.cpp
	socket.cpp
	utils.cpp
	)

TARGET_LINK_LIBRARIES(Skovia -lSemphella)
INSTALL(TARGETS Skovia DESTINATION lib)
INSTALL(FILES 
	libSkovia.h
	socket.h
	DESTINATION include/libSkovia
)