AddLib(ISolidManager OccModifiedSolid)
AddLib(PrimitiveSolidManager ISolidManager)
AddLib(CompoundSolidManager ISolidManager OccBooleanSolid)

#add_library(PrimitiveSolidManager STATIC ISolidManager PrimitiveSolidManager.cpp)
#target_link_libraries(PrimitiveSolidManager ${OCC_LIBS} OccSolidd)

#add_library(CompoundSolidManager STATIC CompoundSolidManager.cpp)
#target_link_libraries(CompoundSolidManager ${OCC_LIBS} ISolidManager OccSolidd OccModifiedSolidd)
