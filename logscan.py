import sys
import math

threshold_string = "threshold = "
reward_string = "Obtained an accum reward of: "

if len(sys.argv) != 4:
    print "3 Arguments required: input_file and output_file and avg_out_file"
    exit(0)

in_filename = sys.argv[1]
out_filename = sys.argv[2]
avg_out_filename = sys.argv[3]
in_file = open(in_filename, 'r')
out_file = open(out_filename, 'w')
avg_out_file = open(avg_out_filename, 'w')
t = "N/A"
r = "N/A"
s = dict()
c = dict()

data = []
for line in in_file.readlines():
    if line.find(threshold_string) >= 0:
        t = line[len(threshold_string):].strip()
    if line.find(reward_string) >= 0:
        r = line[len(reward_string):].strip()
        # out_file.write(t + " " + r + "\n")
        data.append((t, r))
        if t not in s:
            c[t] = 1
            s[t] = float(r)
        else:
            c[t] += 1
            s[t] += float(r)

total_squared_difs = dict()
for (t, r) in data:
    if t not in total_squared_difs:
        total_squared_difs[t] = (float(r) - float(s[t]) / c[t]) ** 2
    else:
        total_squared_difs[t] += (float(r) - float(s[t]) / c[t]) ** 2

std_dev = dict()
for t in c:
    std_dev[t] = math.sqrt(float(total_squared_difs[t]) / c[t])

for (t, r) in data:
    out_file.write(t + " " + r + " " + str(std_dev[t]) + "\n")

for t in s:
    avg_out_file.write(t + " " + str(float(s[t]) / c[t]) + "\n")

in_file.close()
out_file.close()
avg_out_file.close()
