ADD_LIBRARY(Semphella SHARED  
	argment.cpp
	translate.cpp
	APD_LOADER.cpp
	crypt.cpp
	debug.cpp
	files.cpp
	kernel.cpp
	libSemphella.cpp
	license.cpp
	math.cpp
	string.cpp
	sum.cpp
	AES.cpp
	date_recovery_plus.cpp
	time.cpp
	emmx.cpp
	db.cpp
	utils.cpp)

INSTALL (TARGETS Semphella DESTINATION lib)
INSTALL (FILES apd.h
	translate.h
	argment.h
	config.h
	crypt.h
	AES.h
	db.h
	debug.h
	files.h
	kernel.h
	linux.h
	time.h
	main.h
	math.h
	string.h
	sum.h
	utils.h
	date_recovery_plus.h
	license.h 
	sysapi_modcall.h
	emmx.h
	DESTINATION include/libSemphella)

