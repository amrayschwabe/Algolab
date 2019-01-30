# CMake generated Testfile for 
# Source directory: /Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week3/firsthit
# Build directory: /Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week3/firsthit
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(compilation_of__firsthit "/usr/local/Cellar/cmake/3.12.3/bin/cmake" "--build" "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week3/firsthit" "--target" "firsthit")
set_tests_properties(compilation_of__firsthit PROPERTIES  FIXTURES_SETUP "firsthit" LABELS "1._")
add_test(execution___of__firsthit "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week3/firsthit/firsthit")
set_tests_properties(execution___of__firsthit PROPERTIES  DEPENDS "compilation_of__firsthit" FIXTURES_REQUIRED "1._;firsthit" LABELS "1._" WORKING_DIRECTORY "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week3/firsthit/__exec_test_dir")
add_test(1.__SetupFixture "/usr/local/Cellar/cmake/3.12.3/bin/cmake" "-E" "copy_directory" "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week3/firsthit" "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week3/firsthit/__exec_test_dir")
set_tests_properties(1.__SetupFixture PROPERTIES  FIXTURES_SETUP "1._" LABELS "1._")
add_test(1.__CleanupFixture "/usr/local/Cellar/cmake/3.12.3/bin/cmake" "-E" "remove_directory" "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week3/firsthit/__exec_test_dir")
set_tests_properties(1.__CleanupFixture PROPERTIES  FIXTURES_CLEANUP "1._" LABELS "1._")
