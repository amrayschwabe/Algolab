# CMake generated Testfile for 
# Source directory: /Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week8/h1n1
# Build directory: /Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week8/h1n1
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(compilation_of__h1n1 "/usr/local/Cellar/cmake/3.12.3/bin/cmake" "--build" "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week8/h1n1" "--target" "h1n1")
set_tests_properties(compilation_of__h1n1 PROPERTIES  FIXTURES_SETUP "h1n1" LABELS "1._")
add_test(execution___of__h1n1 "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week8/h1n1/h1n1")
set_tests_properties(execution___of__h1n1 PROPERTIES  DEPENDS "compilation_of__h1n1" FIXTURES_REQUIRED "1._;h1n1" LABELS "1._" WORKING_DIRECTORY "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week8/h1n1/__exec_test_dir")
add_test(1.__SetupFixture "/usr/local/Cellar/cmake/3.12.3/bin/cmake" "-E" "copy_directory" "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week8/h1n1" "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week8/h1n1/__exec_test_dir")
set_tests_properties(1.__SetupFixture PROPERTIES  FIXTURES_SETUP "1._" LABELS "1._")
add_test(1.__CleanupFixture "/usr/local/Cellar/cmake/3.12.3/bin/cmake" "-E" "remove_directory" "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week8/h1n1/__exec_test_dir")
set_tests_properties(1.__CleanupFixture PROPERTIES  FIXTURES_CLEANUP "1._" LABELS "1._")
