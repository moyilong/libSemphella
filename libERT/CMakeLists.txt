ADD_LIBRARY(ERT SHARED 
	algorthim.cpp
	basic.fhand.cpp
	basic.mod.cpp
	fhandle.cpp
	libERT.cpp
	performance_test.cpp
	stdafx.cpp
	aes.mod.cpp
	external_library.cpp
	)
INSTALL(TARGETS ERT DESTINATION lib)
INSTALL(FILES 
	dll.h
	libERT.h
	headproto.h
	ext_io.h
	DESTINATION include/libERT
	)
	

INCLUDE_DIRECTORIES(..)
TARGET_LINK_LIBRARIES(ERT -lSemphella)
