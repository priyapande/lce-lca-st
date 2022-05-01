import re
import os
import pandas as pd


def main():
    pattern_size = [10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000]
    test_files = []
    test_files_op = []
    df = pd.DataFrame(columns=["alphabet_size", "size", "memory_dump"])
    df2 = pd.DataFrame(columns=["alphabet_size", "size", "memory_dump"])

    for pattern in pattern_size:
        test_dir_naive = "/Users/priya/Documents/Spring 2022/Comp Bio/lce-lca-st/outputs_mem/naive/" + str(pattern)
        test_dir_lca = "/Users/priya/Documents/Spring 2022/Comp Bio/lce-lca-st/outputs_mem/suffix_tree/" + str(pattern)
        for (dir_path, dir_names, filenames) in os.walk(test_dir_naive):
            for name in filenames:
                test_files.append(dir_path + '/' + name)
        for (dir_path, dir_names, filenames) in os.walk(test_dir_lca):
            for name in filenames:
                test_files_op.append(dir_path + '/' + name)

    for file in test_files:
        my_lines = []
        with open(file, 'r') as my_file:
            for line in my_file:
                my_lines.append(line.rstrip('\n'))
        s = []
        temp = file.split('_')
        t2 = temp[-1].split('.')
        s.extend([t2[0], temp[-2]])
        for line in my_lines:
            if line.find("total heap usage") != -1:
                w = [int(s) for s in re.findall(r'-?\d+\.?\d*', line)]
                t = str(w[-2]) + str(w[-1])
                s.extend([t])
                df.loc[len(df.index)-1] = s

    for file in test_files_op:
        my_lines = []
        with open(file, 'r') as my_file:
            for line in my_file:
                my_lines.append(line.rstrip('\n'))
        s = []
        temp = file.split('_')
        t2 = temp[-1].split('.')
        s.extend([t2[0], temp[-2]])
        for line in my_lines:
            if line.find("total heap usage") != -1:
                w = [int(s) for s in re.findall(r'-?\d+\.?\d*', line)]
                t = str(w[-2]) + str(w[-1])
                s.extend([t])
                df2.loc[len(df2.index)-1] = s
    df.to_csv("/Users/priya/Documents/Spring 2022/Comp Bio/lce-lca-st/scripts/naive_op_mem.csv")
    df2.to_csv("/Users/priya/Documents/Spring 2022/Comp Bio/lce-lca-st/scripts/lca_op_mem.csv")


if __name__ == "__main__":
    main()