ADD_EXECUTABLE(qrcreate entry.cpp)
TARGET_LINK_LIBRARIES(qrcreate -lQr -lSemphella)
INSTALL(TARGETS qrcreate DESTINATION bin)