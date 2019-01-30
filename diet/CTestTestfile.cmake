# CMake generated Testfile for 
# Source directory: /Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week7/diet
# Build directory: /Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week7/diet
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(compilation_of__diet "/usr/local/Cellar/cmake/3.12.3/bin/cmake" "--build" "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week7/diet" "--target" "diet")
set_tests_properties(compilation_of__diet PROPERTIES  FIXTURES_SETUP "diet" LABELS "1._")
add_test(execution___of__diet "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week7/diet/diet")
set_tests_properties(execution___of__diet PROPERTIES  DEPENDS "compilation_of__diet" FIXTURES_REQUIRED "1._;diet" LABELS "1._" WORKING_DIRECTORY "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week7/diet/__exec_test_dir")
add_test(1.__SetupFixture "/usr/local/Cellar/cmake/3.12.3/bin/cmake" "-E" "copy_directory" "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week7/diet" "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week7/diet/__exec_test_dir")
set_tests_properties(1.__SetupFixture PROPERTIES  FIXTURES_SETUP "1._" LABELS "1._")
add_test(1.__CleanupFixture "/usr/local/Cellar/cmake/3.12.3/bin/cmake" "-E" "remove_directory" "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week7/diet/__exec_test_dir")
set_tests_properties(1.__CleanupFixture PROPERTIES  FIXTURES_CLEANUP "1._" LABELS "1._")
