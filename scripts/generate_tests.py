import os
import random


def generate_instances(size, num_alpha):
    min_lc = ord(b'a')
    ba = bytearray(os.urandom(size))
    for i, b in enumerate(ba):
        ba[i] = min_lc + b % num_alpha
    return ba.decode()


def main():
    alpha_size = [2, 4, 5, 10, 11, 17, 26]
    pattern_size = [10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000]
    test_size = [10, 100, 1000, 10000, 100000, 500000, 500000, 500000, 500000]

    # Parent Directory path
    parent_dir = "/Users/priya/Documents/Spring 2022/Comp Bio/lce-lca-st/tests"
    i = 0
    for p_s in pattern_size:  # fixing pattern size
        directory = "Tests_" + str(p_s)
        # Path
        path = os.path.join(parent_dir, directory)
        if not os.path.isdir(path):
            os.mkdir(path)
        for s in alpha_size:  # fixing set of chars
            f_name = "strings_" + str(p_s) + "_" + str(s)
            f_out = open(path + "/" + f_name + ".txt", 'w')
            print("Executing " + str(p_s) + " " + str(s))
            p = generate_instances(int(p_s / 2), s)
            shuffle = ''.join(random.sample(p, len(p)))
            f_out.write(p + shuffle + "\n")
            test = test_size[i]
            f_out.write(str(test) + "\n")
            for m in range(test):
                q1 = random.randint(1, len(p))
                q2 = q1 + len(p)
                f_out.write(str(q1) + " " + str(q2) + "\n")
            f_out.close()
            print("Executed " + str(p_s) + " " + str(s))
        i = i + 1


if __name__ == "__main__":
    main()