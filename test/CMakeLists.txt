# Include files
SET(HEADERS ../include/stdafx.h)
SOURCE_GROUP(include FILES ${HEADERS})

# Source files
SET(Unit_Test_Cases test.cpp)
SOURCE_GROUP(source FILES ${Unit_Test_Cases} ${Http_Server})

ADD_EXECUTABLE(Unittest ${HEADERS} ${Unit_Test_Cases})
TARGET_LINK_LIBRARIES(Unittest ${Boost_LIBRARIES})

ADD_SUBDIRECTORY(httpserver)