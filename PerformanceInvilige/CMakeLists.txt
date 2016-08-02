ADD_EXECUTABLE(PerformanceInvilige PerformanceInvilige.cpp)
INSTALL(TARGETS PerformanceInvilige DESTINATION bin)
TARGET_LINK_LIBRARIES(PerformanceInvilige -lTAHITI -lSemphella)
INCLUDE_DIRECTORIES(..)
