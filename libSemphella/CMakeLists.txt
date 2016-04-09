PROJECT(Semphella)
ADD_LIBRARY(Semphella SHARED xtables.cpp data_recovery.cpp argment.cpp translate.cpp APD_LOADER.cpp crypt.cpp debug.cpp files.cpp kernel.cpp libSemphella.cpp license.cpp math.cpp string.cpp sum.cpp utils.cpp)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__LINUX__ -std=c++11 -fpermissive -D_LINUX -O3 -fopenmp")

INSTALL (TARGETS Semphella DESTINATION lib)
INSTALL (FILES apd.h data_recovery.h xtables.h translate.h argment.h config.h crypt.h debug.h files.h kernel.h linux.h main.h math.h string.h sum.h utils.h license.h DESTINATION include/libSemphella)

