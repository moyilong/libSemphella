ADD_LIBRARY(cp2_extlibtest SHARED 
	cp2_extlibtest.cpp
	)

INCLUDE_DIRECTORIES(..)
INSTALL(TARGETS cp2_extlibtest DESTINATION lib/libSemphellaUtils)