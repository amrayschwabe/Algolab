# CMake generated Testfile for 
# Source directory: /Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week12/hongkong
# Build directory: /Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week12/hongkong
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(compilation_of__hong_kong "/usr/local/Cellar/cmake/3.12.3/bin/cmake" "--build" "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week12/hongkong" "--target" "hong_kong")
set_tests_properties(compilation_of__hong_kong PROPERTIES  FIXTURES_SETUP "hong_kong" LABELS "1._")
add_test(execution___of__hong_kong "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week12/hongkong/hong_kong")
set_tests_properties(execution___of__hong_kong PROPERTIES  DEPENDS "compilation_of__hong_kong" FIXTURES_REQUIRED "1._;hong_kong" LABELS "1._" WORKING_DIRECTORY "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week12/hongkong/__exec_test_dir")
add_test(1.__SetupFixture "/usr/local/Cellar/cmake/3.12.3/bin/cmake" "-E" "copy_directory" "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week12/hongkong" "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week12/hongkong/__exec_test_dir")
set_tests_properties(1.__SetupFixture PROPERTIES  FIXTURES_SETUP "1._" LABELS "1._")
add_test(1.__CleanupFixture "/usr/local/Cellar/cmake/3.12.3/bin/cmake" "-E" "remove_directory" "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week12/hongkong/__exec_test_dir")
set_tests_properties(1.__CleanupFixture PROPERTIES  FIXTURES_CLEANUP "1._" LABELS "1._")
add_test(compilation_of__hong_kong_sol "/usr/local/Cellar/cmake/3.12.3/bin/cmake" "--build" "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week12/hongkong" "--target" "hong_kong_sol")
set_tests_properties(compilation_of__hong_kong_sol PROPERTIES  FIXTURES_SETUP "hong_kong_sol" LABELS "1._")
add_test(execution___of__hong_kong_sol "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week12/hongkong/hong_kong_sol")
set_tests_properties(execution___of__hong_kong_sol PROPERTIES  DEPENDS "compilation_of__hong_kong_sol" FIXTURES_REQUIRED "1._;hong_kong_sol" LABELS "1._" WORKING_DIRECTORY "/Users/amrayschwabe/Documents/ETH/Master/1. Semester/AlgoLab/week12/hongkong/__exec_test_dir")
