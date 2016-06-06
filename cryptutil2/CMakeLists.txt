ADD_LIBRARY(cryptutil2 SHARED 
	cryptutil2.cpp
	license_create.cpp
	)

INCLUDE_DIRECTORIES(..)
INSTALL(TARGETS cryptutil2 DESTINATION bin)