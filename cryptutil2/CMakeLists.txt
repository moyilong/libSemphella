ADD_EXECUTABLE(cryptutil2 
	cryptutil2.cpp
	license_create.cpp
	)
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -D__LINUX__ -Wnarrowing -std=c++11 -fpermissive -D_LINUX -O3 -fopenmp")
INSTALL (TARGETS cryptutil2 DESTINATION bin)
TARGET_LINK_LIBRARIES(cryptutil2 -lSemphella -lERT)
INCLUDE_DIRECTORIES(..)