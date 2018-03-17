add_library(PrimitiveSolidManager SHARED PrimitiveSolidManager.cpp)
target_link_libraries(PrimitiveSolidManager ${OCC_LIBS} OccSolidd)

add_library(CompoundSolidManager CompoundSolidManager.cpp)
target_link_libraries(CompoundSolidManager ${OCC_LIBS} OccSolidd OccModifiedSolidd)
