ADD_EXECUTABLE(PT  algorthim.cpp entry.cpp stdafx.cpp utils.cpp vector.cpp)
INSTALL (TARGETS PT DESTINATION bin)
TARGET_LINK_LIBRARIES(PT -lSemphella)
INCLUDE_DIRECTORIES(..)
