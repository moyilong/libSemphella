ADD_LIBRARY(Qr SHARED 
	bitstream.c
	mask.c
	mmask.c
	mqrspec.c
	qrapi.cpp
	qrencode.c
	qrinput.c
	qrspec.c
	rscode.c
	splite.c
	)

INSTALL(TARGETS Qr DESTINATION lib)
INSTALL(FILES libQr.h DESTINATION include/libQr)