PROJECT("progetto-cpp" CXX)
CMAKE_MINIMUM_REQUIRED(VERSION 3.5)


ADD_COMPILE_OPTIONS("-std=c++17")

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    add_compile_options("-Weverything")
    add_compile_options("-Wno-c++98-compat")
    add_compile_options("-Wno-c++98-compat-pedantic")
    add_compile_options("-Wno-padded")
    add_compile_options("-Wno-undefined-func-template")
    add_compile_options("-Wno-documentation-unknown-command")
    add_compile_options("-Wno-global-constructors")
    add_compile_options("-Wno-exit-time-destructors")
    add_compile_options("-Wno-sign-conversion")
    add_compile_options("-Wno-weak-vtables")
    add_compile_options("-Wno-weak-template-vtables")
    add_compile_options("-Wno-documentation") ##########
elseif (CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options("-Wall")
    add_compile_options("-Wextra")
    add_compile_options("-Wpedantic")
endif()

add_compile_options("-Werror")

# ------------------------------------------------------------------------------
### BUILD TYPE ###

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options("-O3")
    add_compile_options("-DNDEBUG")

elseif (CMAKE_BUILD_TYPE STREQUAL "Debug")
    if (CMAKE_COMPILER_IS_GNUCXX)
        add_compile_options("-ggdb3")
    elseif (CMAKE_CXX_COMPILER_ID EQUAL "Clang")
        add_compile_options("-ggdb")
    else()
        add_compile_options("-g")
    endif()

elseif (CMAKE_BUILD_TYPE STREQUAL "Coverage")
    if (CMAKE_COMPILER_IS_GNUCXX)
        set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "--coverage")
        add_compile_options("--coverage")
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        add_compile_options("-fprofile-instr-generate")
        add_compile_options("-fcoverage-mapping")
        set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS}
                                  "-fprofile-instr-generate -fcoverage-mapping")
    endif()

elseif (CMAKE_BUILD_TYPE STREQUAL "SANITIZE" AND
        CMAKE_CXX_COMPILER_ID EQUAL "Clang")

    add_compile_options("-O1")
    add_compile_options("-g")
    add_compile_options("-fno-omit-frame-pointer")
    add_compile_options("-fno-optimize-sibling-calls")

    add_compile_options("-fsanitize=address")
    add_compile_options("-fsanitize=leak")
    add_compile_options("-fsanitize=memory")
    add_compile_options("-fsanitize-memory-track-origins")
    add_compile_options("-fsanitize=undefined")
    add_compile_options("-fsanitize=integer")
    add_compile_options("-fsanitize=nullability")

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
endif()



# ------------------------------------------------------------------------------
### BUILD LIBRARY ###

SET(CMAKE_EXPORT_COMPILE_COMMANDS ON) # for clang-tidy

include_directories(${PROJECT_SOURCE_DIR}/include
                    ${PROJECT_SOURCE_DIR}/lib)

file(GLOB_RECURSE SRCS ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_library(collections ${SRCS})

add_executable(collections_test EXCLUDE_FROM_ALL
               ${PROJECT_SOURCE_DIR}/test/test.cpp)
target_link_libraries(collections_test collections)

install (TARGETS collections DESTINATION lib)
install (DIRECTORY include/
         DESTINATION include/collections)

# ------------------------------------------------------------------------------
### CUSTOM TARGETS ###

add_custom_target(clean-all
                  COMMAND rm -rf ${PROJECT_SOURCE_DIR}/build/*
                  COMMAND rm -rf ${PROJECT_SOURCE_DIR}/doc/*
                  COMMENT "scan-build Static Analysis")

if (CMAKE_COMPILER_IS_GNUCXX)
    add_custom_target(coverage
                      COMMAND lcov --capture --directory
                                   ${PROJECT_SOURCE_DIR}/include
                                   --output-file coverage.info
                      COMMENT "COVERAGE")

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    find_program(SCANBUILD_FOUND scan-build)
    if (SCANBUILD_FOUND)
        add_custom_target(scan
                          COMMAND make clean
                          COMMAND scan-build make
                          COMMENT "scan-build Static Analysis")
    endif()

    find_program(CLANGTIDY_FOUND clang-tidy)
    if (CLANGTIDY_FOUND)
        add_custom_target(tidy
                          COMMAND clang-tidy -p=.
                                  ../src/*.cpp ../test/*.cpp
                          COMMENT "clang-tidy Linter")
    endif()

    find_program(LLVMCOV_FOUND llvm-cov)
    if (LLVMCOV_FOUND)
        add_custom_target(coverage
                          COMMAND llvm-profdata merge -sparse default.profraw
                                                -o default.profdata
                          COMMAND llvm-cov show -format=html
                                  -instr-profile default.profdata
                                  -output-dir=${PROJECT_SOURCE_DIR}/doc/coverage
                                  -Xdemangler=c++filt
                                  ./collections_test
                          COMMENT "COVERAGE")
    endif()
endif()

find_program(CLOC_FOUND cloc)
if (CLOC_FOUND)
    add_custom_target(stats
                      COMMAND cloc ${PROJECT_SOURCE_DIR}/src
                                   ${PROJECT_SOURCE_DIR}/include
                                   ${PROJECT_SOURCE_DIR}/test
                      COMMENT "Code statistics")
endif()

find_program(LYZARD_FOUND lizard)
if (LYZARD_FOUND)
    add_custom_target(cyclo
                      COMMAND lizard ${PROJECT_SOURCE_DIR}/src/
                                     ${PROJECT_SOURCE_DIR}/include/
                      COMMENT "Code Complexity")
endif()

find_program(DOXYGEN_FOUND doxygen)
if (DOXYGEN_FOUND)
    add_custom_target(doc
                      COMMAND doxygen ${PROJECT_SOURCE_DIR}/doxygen.cfg
                      COMMENT "Code documentation")
endif()

# ------------------------------------------------------------------------------
### TESTS ###

enable_testing()

ADD_TEST(NAME CompileTest
         WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/build
         COMMAND make collections_test)

ADD_TEST(NAME ContiguousFifoTest
         WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/build
         COMMAND ./collections_test [ContiguousFifoTest] -x 1024)

ADD_TEST(NAME LinkedFifoTest
         WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/build
         COMMAND ./collections_test [LinkedFifoTest] -x 1024)

ADD_TEST(NAME MultilinkedFifoTest
         WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/build
         COMMAND ./collections_test [MultilinkedFifoTest] -x 1024)

ADD_TEST(NAME ConcatFifoTest
          WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/build
          COMMAND ./collections_test [ConcatFifoTest] -x 1024)

ADD_TEST(NAME AssignmentTest
          WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/build
          COMMAND ./collections_test [AssignmentTest] -x 1024)

ADD_TEST(NAME MismatchTest
          WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/build
          COMMAND ./collections_test [MismatchTest] -x 1024)


find_program(VALGRIND_FOUND valgrind)
if (VALGRIND_FOUND)
    ADD_TEST(NAME ValgrindTest1
             WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/build
             COMMAND valgrind --leak-check=full --show-leak-kinds=all
                              --track-origins=yes
                              ./collections_test [ContiguousFifoTest] -x 1024)

    ADD_TEST(NAME ValgrindTest2
             WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/build
             COMMAND valgrind --leak-check=full --show-leak-kinds=all
                              --track-origins=yes
                              ./collections_test [LinkedFifoTest] -x 1024)

    ADD_TEST(NAME ValgrindTest3
             WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/build
             COMMAND valgrind --leak-check=full --show-leak-kinds=all
                              --track-origins=yes
                              ./collections_test [MultilinkedFifoTest] -x 1024)

    ADD_TEST(NAME ValgrindTest4
           WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/build
           COMMAND valgrind --leak-check=full --show-leak-kinds=all
                            --track-origins=yes
                            ./collections_test [ConcatFifoTest] -x 1024)

    ADD_TEST(NAME ValgrindTest5
           WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/build
           COMMAND valgrind --leak-check=full --show-leak-kinds=all
                            --track-origins=yes
                            ./collections_test [AssignmentTest] -x 1024)

    ADD_TEST(NAME ValgrindTest6
           WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/build
           COMMAND valgrind --leak-check=full --show-leak-kinds=all
                            --track-origins=yes
                            ./collections_test [MismatchTest] -x 1024)

endif()
