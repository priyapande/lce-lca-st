import re
import os
import pandas as pd


def main():
    pattern_size = [10, 100, 1000, 10000]
    test_files = []
    test_files_op = []
    df = pd.DataFrame(columns=["queries", "size", "exec_time"])
    df2 = pd.DataFrame(columns=["ST_construction_time", "LCA_Preprocess", "queries", "size", "exec_time"])

    for pattern in pattern_size:
        test_dir_naive = "/Users/priya/Documents/Spring 2022/Comp Bio/lce-lca-st/outputs/naive/" + str(pattern)
        test_dir_lca = "/Users/priya/Documents/Spring 2022/Comp Bio/lce-lca-st/outputs/suffix_tree/" + str(pattern)
        for (dir_path, dir_names, filenames) in os.walk(test_dir_naive):
            for name in filenames:
                test_files.append(dir_path + '/' + name)
        for (dir_path, dir_names, filenames) in os.walk(test_dir_lca):
            for name in filenames:
                test_files_op.append(dir_path + '/' + name)

    for file in test_files:
        inp = open(file, 'r')
        sentence = inp.readline()
        s = [int(s) for s in re.findall(r'-?\d+\.?\d*', sentence)]
        df.loc[len(df.index)-1] = s

    for file in test_files_op:
        inp = open(file, 'r')
        sentence = inp.readline()
        s1 = inp.readline()
        s2 = inp.readline()
        s = [int(s) for s in re.findall(r'-?\d+\.?\d*', sentence)]
        s_1 = ([int(s) for s in re.findall(r'-?\d+\.?\d*', s1)])
        s.extend(s_1)
        s_1 = ([int(s) for s in re.findall(r'-?\d+\.?\d*', s2)])
        s.extend(s_1)
        df2.loc[len(df2.index)-1] = s

    df.to_csv("/Users/priya/Documents/Spring 2022/Comp Bio/lce-lca-st/naive_op.csv")
    df2.to_csv("/Users/priya/Documents/Spring 2022/Comp Bio/lce-lca-st/lca_op.csv")


if __name__ == "__main__":
    main()