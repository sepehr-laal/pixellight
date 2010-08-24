# - Find PCRE
# Find the native PCRE includes and library
#
#  PCRE_INCLUDE_DIR - where to find pcre.h, etc.
#  PCRE_LIBRARIES   - List of libraries when using libpcre.
#  PCRE_FOUND       - True if pcre found.

IF (PCRE_INCLUDE_DIR)
  # Already in cache, be silent
  SET(PCRE_FIND_QUIETLY TRUE)
ENDIF (PCRE_INCLUDE_DIR)

FIND_PATH(PCRE_INCLUDE_DIR pcre.h)

if(UNIX)
  SET(PCRE_NAMES libpcre.a libpcreposix.a)
else()
  SET(PCRE_NAMES pcre libpcre)
endif()
FIND_LIBRARY(PCRE_LIBRARY NAMES ${PCRE_NAMES} )

# handle the QUIETLY and REQUIRED arguments and set PCRE_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PCRE DEFAULT_MSG PCRE_LIBRARY PCRE_INCLUDE_DIR)

IF(PCRE_FOUND)
  SET( PCRE_LIBRARIES ${PCRE_LIBRARY} CACHE INTERNAL "")
ELSE(PCRE_FOUND)
  SET( PCRE_LIBRARIES CACHE INTERNAL "")
ENDIF(PCRE_FOUND)

MARK_AS_ADVANCED( PCRE_LIBRARY PCRE_INCLUDE_DIR )
