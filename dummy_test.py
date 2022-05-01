import random

sizes = [5000, 1000, 7000, 5000]
t_size = 10000

s = ''
for i in range(len(sizes)):
    sz = sizes[i]
    if i % 2:
        for i in range(sz):
            s += 'a'
    else:
        for i in range(sz):
            s += 'b'


f = open('t.txt', 'w')

f.write(s + '\n')
f.write(str(t_size) + '\n')

for i in range(t_size):
    q1 = random.randint(1, sum(sizes))
    q2 = random.randint(1, sum(sizes))

    f.write(str(q1) + " " + str(q2) + "\n")

f.close()