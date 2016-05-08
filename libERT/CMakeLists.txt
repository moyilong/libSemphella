PROJECT(Semphella)
ADD_LIBRARY(ERT SHARED 
	algorthim.cpp
	basic.fhand.cpp
	basic.mod.cpp
	fhandle.cpp
	frecovery.fhand.cpp
	libERT.cpp
	performance_test.cpp
	stdafx.cpp
	utils.cpp
	)
INSTALL(TARGETS ERT DESTINATION lib)
INSTALL(FILES 
	dll.h
	libERT.h
	headproto.h
	DESTINATION include/libERT
	)
	

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__LINUX__  -Wno-error=unused-result -std=c++11 -fpermissive -D_LINUX -O3 -fopenmp ")
INCLUDE_DIRECTORIES(..)
TARGET_LINK_LIBRARIES(ERT -lSemphella)