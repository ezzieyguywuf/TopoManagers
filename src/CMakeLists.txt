add_library(PrimitiveSolidManager PrimitiveSolidManager.cpp)
target_link_libraries(PrimitiveSolidManager ${OCC_LIBS} OccSolidd)
