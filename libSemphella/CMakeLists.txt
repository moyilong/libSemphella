PROJECT(Semphella)
ADD_LIBRARY(Semphella SHARED debug.cpp  APD_LOADER.cpp bios.cpp crypt.cpp files.cpp kernel.cpp libSemphella.cpp math.cpp string.cpp sum.cpp utils.cpp)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__LINUX__ -std=c++11 -fpermissive -D_LINUX -O3 -fopenmp")

INSTALL (TARGETS Semphella DESTINATION lib)
INSTALL (FILES apd.h config.h crypt.h debug.h files.h kernel.h libbios.h linux.h main.h math.h string.h sum.h utils.h libSemphella.so DESTINATION include/libSemphella)

