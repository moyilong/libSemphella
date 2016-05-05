ADD_LIBRARY(TAHITI SHARED ExtenisionLIB/configure.cpp ExtenisionLIB/translate.cpp client.cpp event.cpp hertbeat.cpp modules.cpp permission.cpp protocoV1.cpp protocoV2.cpp server.cpp serverutils.cpp TAHITI.cpp)
INSTALL(FILES modules.h stdafx.h permission.h event.h build.sh feature_define.h hertbeat.h config.h multi_platform.h inline.h ExtenisionLIB/configure.h ExtenisionLIB/translate.h kernel.h net_base.h serverutil.h DESTINATION include/TAHITI)
INSTALL(TARGETS TAHITI DESTINATION lib)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__LINUX__ -std=c++11 -fpermissive -D_LINUX -O3 -fopenmp")

INSTALL(FILES ExtenisionLIB/configure.h ExtenisionLIB/translate.h DESTINATION include/TAHITI/ExtenisionLIB)
TARGET_LINK_LIBRARIES(TAHITI -lSemphella)