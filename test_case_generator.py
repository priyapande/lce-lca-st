str = open('t.txt', 'r').readline().strip()


qs = open('q.txt', 'w')

qs.write(len(ts))
for t in ts:
    qs.write(str(t[0]) + " " + str(t[1]) + " " + str(t[2]))