# Require of cmake version
CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

FIND_PACKAGE(Boost)

# Project name
PROJECT (CxxWeb)

IF(NOT Boost_FOUND)
	# Force not advance
	MARK_AS_ADVANCED(CLEAR Boost_LIBRARY_DIR Boost_INCLUDE_DIR)
	SET(Boost_INCLUDE_DIR	""	CACHE PATH "You need to specify the absolute boost dir")
	SET(Boost_LIBRARY_DIR	""	CACHE PATH "You need to specify the absolute boost dir")
	MESSAGE("Could not detect boost library automatically!")
	MESSAGE("You need to specify the absolute boost dir")
ENDIF()

IF(Boost_FOUND)
	INCLUDE_DIRECTORIES("${PROJECT_ROOT_PATH}/include/")

	# Include boost dirs
	INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
	LINK_DIRECTORIES(${Boost_LIBRARY_DIR})

	#ADD_SUBDIRECTORY(src)
	ADD_SUBDIRECTORY(test)
ENDIF()
