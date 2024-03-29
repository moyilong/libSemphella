LINK_DIRECTORIES(${PROJECT_BINARY_DIR}/lib)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lrt -ldl -D__LINUX__  -I${CMAKE_SOURCE_DIR} -Wno-error=unused-result  -std=c++11 -fpermissive -lm -D_LINUX -O2 -fopenmp -Wunused-variable -Wformat -ggdb -pipe -D__DEFAULT_DEBUG_STAT=false")

# Basic Libraries
ADD_SUBDIRECTORY(libSemphella)
ADD_SUBDIRECTORY(libERT)


# Application
ADD_SUBDIRECTORY(mpshell)
ADD_SUBDIRECTORY(cryptutil2)
ADD_SUBDIRECTORY(APD_UTILS)
ADD_SUBDIRECTORY(stlcheck)
ADD_SUBDIRECTORY(mpshell2)
ADD_SUBDIRECTORY(libMps)
ADD_SUBDIRECTORY(serial)

ADD_DEPENDENCIES(mpshell Semphella)
ADD_DEPENDENCIES(mpshell2 Semphella Mps)
ADD_DEPENDENCIES(Mps Semphella)
ADD_DEPENDENCIES(ERT Semphella)
ADD_DEPENDENCIES(cryptutil2 Semphella ERT)
ADD_DEPENDENCIES(APD_UTILS Semphella)
ADD_DEPENDENCIES(stlcheck Semphella)
ADD_DEPENDENCIES(serial Semphella)
