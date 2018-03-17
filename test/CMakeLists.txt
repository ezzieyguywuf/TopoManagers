include(${CMAKE_HELPER_DIR}/functions.cmake)

add_subdirectory(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(NEEDED_LIBS
    ${OCC_LIBS}
    OccEdged
    OccFaced
    OccSolidd
    OccShaped
    OccCylinderd
    OccBooleanSolidd
    OccSolidModifierd 
    OccModifiedSolidd
    OccSolidMakerd )

AddTest("PrimitiveSolidManagerTester" ${NEEDED_LIBS} PrimitiveSolidManager)
AddTest("CompoundSolidManagerTester" ${NEEDED_LIBS} CompoundSolidManager)

add_executable("SimpleTest" "SimpleTest.cpp")
target_link_libraries("SimpleTest" ${NEEDED_LIBS} PrimitiveSolidManager CompoundSolidManager)
