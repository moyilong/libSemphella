set (SOURCE_FILES argment.cpp
	translate.cpp
	APD_LOADER.cpp
	crypt.cpp
	debug.cpp
	sendmail.cpp
	files.cpp
	compress.cpp
	kernel.cpp
	libSemphella.cpp
	license.cpp
	math.cpp
	string.cpp
	AES.cpp
	time.cpp
	emmx.cpp
	stl.cpp
	bmp.cpp
	logd.cpp
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
	AES.h
	debug.h
	files.h
	kernel.h
	VER.h
	linux.h
	time.h
	logd.h
	main.h
	compress.h
	net.h
	math.h
	string.h
	utils.h
	license.h 
	sysapi_modcall.h
	emmx.h
	libSemphella.h
	BMP.h
	DESTINATION include/libSemphella)

