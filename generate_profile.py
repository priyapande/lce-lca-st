import subprocess, shutil
import os

naive_code_cmd = 'g++ LCE_Naive.cpp --std=c++17'
suffix_tree_code_cmd = 'g++ LCE.cpp --std=c++17'

pattern_size = [10, 100, 1000, 10000, 100000, 1000000, 10000000, 10000000, 1000000000]


for pattern in pattern_size:
    test_dir = "/Users/gaganganapathyas/Spring 22 - SBU/CSE 549 - BIO/Project/tests/Tests_" + str(pattern)
    
    test_output_dir_naive = "/Users/gaganganapathyas/Spring 22 - SBU/CSE 549 - BIO/Project/outputs/naive" + str(pattern)
    test_output_dir_suffix_tree = "/Users/gaganganapathyas/Spring 22 - SBU/CSE 549 - BIO/Project/outputs/suffix_tree" + str(pattern)

    os.mkdir(test_output_dir_naive)
    os.mkdir(test_output_dir_suffix_tree)

    test_files = []
    for (dirpath, dirnames, filenames) in os.walk(test_dir):
        test_files.extend(filenames)
        break

    for test_file in test_files:
        subprocess.call([naive_code_cmd])
        subprocess.call("./a.out {test_file} output_naive.txt")

        shutil('/Users/gaganganapathyas/Spring 22 - SBU/CSE 549 - BIO/Project/output_naive.txt', '/{test_output_dir_naive}/{test_file}')

        subprocess.call([suffix_tree_code_cmd])
        subprocess.call("./a.out {test_file} output.txt")

        shutil('/Users/gaganganapathyas/Spring 22 - SBU/CSE 549 - BIO/Project/output.txt', '/{test_output_dir_suffix_tree}/{test_file}')