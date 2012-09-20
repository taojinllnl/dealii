#
# Configuration for the umfpack and amd libraries:
#


SET(FEATURE_P4EST_DEPENDS
  DEAL_II_WITH_MPI
  )


MACRO(FEATURE_P4EST_FIND_EXTERNAL var)
  FIND_PACKAGE(P4EST)
  FIND_PACKAGE(SC) # TODO

  IF(P4EST_FOUND AND SC_FOUND)

    #
    # Check whether p4est supports mpi:
    #
    LIST(APPEND CMAKE_REQUIRED_INCLUDES ${P4EST_INCLUDE_DIR})
    CHECK_CXX_SOURCE_COMPILES(
      "
      #include <p4est_config.h>
      #ifndef P4EST_MPI
      #  error p4est compiled without mpi support
      invalid
      #endif
      int main() { return 0; }
      "
      P4EST_WITH_MPI)
    LIST(REMOVE_ITEM CMAKE_REQUIRED_INCLUDES ${P4EST_INCLUDE_DIR}/p4est_config.h)

    IF(NOT P4EST_WITH_MPI)
      MESSAGE(WARNING "\n"
        "Could not find a sufficient p4est installation: "
        "P4est has to be configured with MPI support enabled.\n\n"
        )
    ELSE()
      SET(${var} TRUE)
    ENDIF()

  ENDIF()
ENDMACRO()


MACRO(FEATURE_P4EST_CONFIGURE_EXTERNAL var)
  INCLUDE_DIRECTORIES(${P4EST_INCLUDE_DIR} ${SC_INCLUDE_DIR})

  IF (CMAKE_BUILD_TYPE MATCHES "Debug")
    IF(P4EST_DEBUG_FOUND AND SC_DEBUG_FOUND)
      LIST(APPEND deal_ii_external_libraries
        ${P4EST_DEBUG_LIBRARY} ${SC_DEBUG_LIBRARY}
        )
    ELSE()
      MESSAGE(WARNING "\n"
        "deal.II was configured with CMAKE_BUILD_TYPE=Debug but no debug p4est and\n"
        "sc libraries were found. The regular p4est and sc libraries will be used\n"
        "instead.\n\n"
        )
      LIST(APPEND deal_ii_external_libraries
        ${P4EST_LIBRARY} ${SC_LIBRARY}
        )
    ENDIF()
  ELSE()
    LIST(APPEND deal_ii_external_libraries
      ${P4EST_LIBRARY} ${SC_LIBRARY}
      )
  ENDIF()

  SET(DEAL_II_USE_P4EST TRUE)

  SET(${var} TRUE)
ENDMACRO()


SET(FEATURE_P4EST_CUSTOM_ERROR_MESSAGE TRUE)


MACRO(FEATURE_P4EST_ERROR_MESSAGE)
  MESSAGE(SEND_ERROR "\n"
    "Could not find the p4est and sc libraries!\n\n"
    "Please ensure that the libraries are installed on your computer.\n"
    "If the libraries are not at a default location, either provide some hints\n"
    "for the autodetection, or set the relevant variables by hand in ccmake.\n\n"
    )
ENDMACRO()


CONFIGURE_FEATURE(P4EST)

