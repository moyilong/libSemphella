set (SOURCE_FILES argment.cpp
	translate.cpp
	APD_LOADER.cpp
	crypt.cpp
	debug.cpp
	files.cpp
	kernel.cpp
	libSemphella.cpp
	math.cpp
	estring.cpp
	string.cpp
	AES.cpp
	time.cpp
	secureLoad.cpp
	emmx.cpp
	stl.cpp
	hal.cpp
	linux_serial.cpp
	sysapi_modcall.cpp
	utils.cpp)


ADD_LIBRARY(Semphella SHARED  ${SOURCE_FILES})
ADD_EXECUTABLE(libSemphella ${SOURCE_FILES})

INSTALL (TARGETS Semphella DESTINATION lib)
INSTALL (TARGETS libSemphella DESTINATION bin)
INSTALL (FILES apd.h
	translate.h
	argment.h
	config.h
	crypt.h
	stl.h
	hal.h
	AES.h
	debug.h
	secload.h
	files.h
	kernel.h
	VER.h
	linux.h
	time.h
	net.h
	math.h
	error.h
	string.h
	utils.h
	sysapi_modcall.h
	emmx.h
	libSemphella.h
	DESTINATION include/libSemphella)

TARGET_LINK_LIBRARIES(Semphella -ldl)
TARGET_LINK_LIBRARIES(libSemphella -ldl)
