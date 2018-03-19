function(AddTest SrcName)
    add_executable(${SrcName} "${SrcName}.cpp")
    target_link_libraries(${SrcName} "gtest_main" ${ARGN})
    add_test(NAME "TEST_${SrcName}" COMMAND $<TARGET_FILE:${SrcName}>)
endfunction(AddTest)

function(AddLib SrcName)
    add_library(${SrcName} SHARED "${SrcName}.cpp")
    target_link_libraries(${SrcName} ${ARGN})
endfunction(AddLib)
