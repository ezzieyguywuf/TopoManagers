include(${CMAKE_HELPER_DIR}/functions.cmake)

add_subdirectory(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(NEEDED_LIBS
    ${OCC_LIBS}
    OccEdge
    OccFace
    OccSolid
    OccShape
    OccCylinder
    OccBooleanSolid
    OccSolidModifier
    OccModifiedSolid
    OccSolidMaker)

AddTest("PrimitiveSolidManagerTester" ${NEEDED_LIBS} PrimitiveSolidManager)
AddTest("CompoundSolidManagerTester" ${NEEDED_LIBS} CompoundSolidManager)
AddTest("ISolidManagerTester" ${NEEDED_LIBS} PrimitiveSolidManager)

add_executable("SimpleTest" "SimpleTest.cpp")
target_link_libraries("SimpleTest" ${NEEDED_LIBS} PrimitiveSolidManager CompoundSolidManager)
