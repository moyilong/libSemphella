ADD_EXECUTABLE(xCPU emx.cpp feature.cpp hard_funcation.cpp xCPU.cpp stdafx.cpp)
INSTALL (TARGETS xCPU DESTINATION bin)
INCLUDE_DIRECTORIES(..)
TARGET_LINK_LIBRARIES(xCPU -lSemphella)