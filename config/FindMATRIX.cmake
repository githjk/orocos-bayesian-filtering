# Locate MATRIX LIB install directory

# This module defines
# MATRIX_INSTALL where to find include, lib, bin, etc.
# MATRIX_FOUND, is set to true

MESSAGE("Searching for matrix lib ${MATRIX_LIB}")

SET(__MATRIXWRAPPER_NEWMAT__ OFF)
SET(__MATRIXWRAPPER_LTI__ OFF)


IF (MATRIX_LIB STREQUAL "newmat")
  SET(NEWMAT_FOUND NEWMAT_FOUND-NOTFOUND)
  MARK_AS_ADVANCED(NEWMAT_FOUND)
  FIND_FILE(NEWMAT_FOUND include.h ${MATRIX_INSTALL}/include/newmat/ )
  IF ( NEWMAT_FOUND )
    MESSAGE("-- Looking for Newmat in ${MATRIX_INSTALL}/include/newmat/ - found")
    SET( MATRIX_INCLUDE "${MATRIX_INSTALL}/include/")
    SET( MATRIX_LIBS "-L${MATRIX_INSTALL}/lib -lnewmat") 
    MESSAGE( "-- Newmat includes ${MATRIX_INCLUDE}")
    MESSAGE( "-- Newmat libs     ${MATRIX_LIBS}")
    SET(__MATRIXWRAPPER_NEWMAT__ ON)	
  ELSE ( NEWMAT_FOUND )
    MESSAGE( FATAL_ERROR "Looking for Newmat in ${MATRIX_INSTALL}/include/newmat/ - not found")
  ENDIF ( NEWMAT_FOUND )
ELSE (MATRIX_LIB STREQUAL "newmat")


IF (MATRIX_LIB STREQUAL "lti")
  SET(LTI_FOUND LTI_FOUND-NOTFOUND)
  MARK_AS_ADVANCED(LTI_FOUND)
  FIND_FILE(LTI_FOUND ltiMatrix.h ${MATRIX_INSTALL}/include/ltilib/ )
  IF ( LTI_FOUND )
    MESSAGE("-- Looking for Lti in ${MATRIX_INSTALL}/include/ltilib/ - found")
    SET( MATRIX_INCLUDE "${MATRIX_INSTALL}/include/")
    SET( MATRIX_LIBS "-L${MATRIX_INSTALL}/lib/ltilib -lltir") 
    MESSAGE( "-- Lti includes ${MATRIX_INCLUDE}")
    MESSAGE( "-- Lti libs     ${MATRIX_LIBS}")
    SET(__MATRIXWRAPPER_LTI__ ON)	
  ELSE ( LTI_FOUND )
    MESSAGE(FATAL ERROR "Looking for Lti in ${MATRIX_INSTALL}/include/ltilib/ - not found")
  ENDIF ( LTI_FOUND )
ELSE (MATRIX_LIB STREQUAL "lti")


#IF (MATRIX_LIB STREQUAL "boost")
#  SET(BOOST_FOUND BOOST_FOUND-NOTFOUND)
#  MARK_AS_ADVANCED(BOOST_FOUND)
#  FIND_FILE(BOOST_FOUND matrix.hpp ${MATRIX_INSTALL}/include/boost/numeric/ublas/ )
#  IF ( BOOST_FOUND )
#    MESSAGE("-- Looking for Boost in ${MATRIX_INSTALL}/include/boost/numeric/ublas/ - found")
#    SET( MATRIX_INCLUDE "${MATRIX_INSTALL}/include/")
#    SET( MATRIX_LIBS "") 
#    MESSAGE( "-- Boost includes ${MATRIX_INCLUDE}")
#    MESSAGE( "-- Boost libs     ${MATRIX_LIBS}")
#    SET(__MATRIXWRAPPER_BOOST__ ON)	
#  ELSE ( BOOST_FOUND )
#    MESSAGE(FATAL ERROR "Looking for Boost in ${MATRIX_INSTALL}/include/boost/numeric/ublas/ - not found")
#  ENDIF ( BOOST_FOUND )
#ELSE (MATRIX_LIB STREQUAL "boost")


MESSAGE( FATAL_ERROR "No valid matrix lib specified. Please choose lti or newmat")

#ENDIF (MATRIX_LIB STREQUAL "boost")
ENDIF (MATRIX_LIB STREQUAL "lti")
ENDIF (MATRIX_LIB STREQUAL "newmat")