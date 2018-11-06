# CMake generated Testfile for 
# Source directory: /Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week7/Maximize_it
# Build directory: /Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week7/Maximize_it
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(compilation_of__maximize "/usr/local/Cellar/cmake/3.12.3/bin/cmake" "--build" "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week7/Maximize_it" "--target" "maximize")
set_tests_properties(compilation_of__maximize PROPERTIES  FIXTURES_SETUP "maximize" LABELS "1._")
add_test(execution___of__maximize "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week7/Maximize_it/maximize")
set_tests_properties(execution___of__maximize PROPERTIES  DEPENDS "compilation_of__maximize" FIXTURES_REQUIRED "1._;maximize" LABELS "1._" WORKING_DIRECTORY "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week7/Maximize_it/__exec_test_dir")
add_test(1.__SetupFixture "/usr/local/Cellar/cmake/3.12.3/bin/cmake" "-E" "copy_directory" "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week7/Maximize_it" "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week7/Maximize_it/__exec_test_dir")
set_tests_properties(1.__SetupFixture PROPERTIES  FIXTURES_SETUP "1._" LABELS "1._")
add_test(1.__CleanupFixture "/usr/local/Cellar/cmake/3.12.3/bin/cmake" "-E" "remove_directory" "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week7/Maximize_it/__exec_test_dir")
set_tests_properties(1.__CleanupFixture PROPERTIES  FIXTURES_CLEANUP "1._" LABELS "1._")
