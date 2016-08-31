ADD_EXECUTABLE(SkoviaSpeed Skovia.cpp)
TARGET_LINK_LIBRARIES(SkoviaSpeed -lSkovia -lSemphella)
INSTALL(TARGETS SkoviaSpeed DESTINATION bin)