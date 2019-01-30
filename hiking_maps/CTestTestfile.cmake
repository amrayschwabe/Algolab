# CMake generated Testfile for 
# Source directory: /Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week4/HikingMaps
# Build directory: /Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week4/HikingMaps
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(compilation_of__main "/Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake" "--build" "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week4/HikingMaps" "--target" "main")
set_tests_properties(compilation_of__main PROPERTIES  FIXTURES_SETUP "main" LABELS "1._")
add_test(execution___of__main "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week4/HikingMaps/main")
set_tests_properties(execution___of__main PROPERTIES  DEPENDS "compilation_of__main" FIXTURES_REQUIRED "1._;main" LABELS "1._" WORKING_DIRECTORY "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week4/HikingMaps/__exec_test_dir")
add_test(1.__SetupFixture "/Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake" "-E" "copy_directory" "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week4/HikingMaps" "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week4/HikingMaps/__exec_test_dir")
set_tests_properties(1.__SetupFixture PROPERTIES  FIXTURES_SETUP "1._" LABELS "1._")
add_test(1.__CleanupFixture "/Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake" "-E" "remove_directory" "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week4/HikingMaps/__exec_test_dir")
set_tests_properties(1.__CleanupFixture PROPERTIES  FIXTURES_CLEANUP "1._" LABELS "1._")
