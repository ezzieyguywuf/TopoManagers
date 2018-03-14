include(${CMAKE_HELPER_DIR}/functions.cmake)

add_subdirectory(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(NEEDED_LIBS
    ${OCC_LIBS}
    OccFaced
    OccSolidd
    OccShaped
    OccBoxd
    OccCylinderd
    PrimitiveSolidManager 
    OccSolidModifierd 
    OccModifiedSolidd
    OccModifiedSolidsd
    OccSolidMakerd )

AddTest("PrimitiveSolidManagerTester" ${NEEDED_LIBS})

#add_executable("SimpleTest" "SimpleTest.cpp")
#target_link_libraries("SimpleTest" ${NEEDED_LIBS})
