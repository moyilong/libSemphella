ADD_EXECUTABLE(cryptutil2 
	cryptutil2.cpp
	license_create.cpp
	)
INSTALL (TARGETS cryptutil2 DESTINATION bin)
TARGET_LINK_LIBRARIES(cryptutil2 -lSemphella -lERT)
INCLUDE_DIRECTORIES(..)