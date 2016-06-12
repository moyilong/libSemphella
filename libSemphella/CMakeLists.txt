ADD_LIBRARY(NetUtils SHARED  
	modules.cpp
	protoco.cpp
	service.cpp
	sppeder.cpp
	)

INSTALL (TARGETS NetUtils DESTINATION lib)
INSTALL (FILES 
	libNetUtils.h
	DESTINATION include/libNetUtils)

