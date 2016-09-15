import sys
import math

threshold_string = "threshold = "
reward_string = "Obtained an accum reward of: "

if len(sys.argv) != 5:
    print "4 Arguments required: input_file1 "\
        "input_file2 and output_file and avg_out_file"
    exit(0)

in_filename1 = sys.argv[1]
in_filename2 = sys.argv[2]
out_filename = sys.argv[3]
avg_out_filename = sys.argv[4]
in_file1 = open(in_filename1, 'r')
in_file2 = open(in_filename2, 'r')
out_file = open(out_filename, 'w')
avg_out_file = open(avg_out_filename, 'w')
t = "N/A"
r = "N/A"
s = dict()
c = dict()

data = []
for line in in_file1.readlines():
    line_data = line.split()
    assert(len(line_data) == 3)
    t = line_data[0].strip()
    r = line_data[1] .strip()
    data.append((t, r))
    if t not in s:
        c[t] = 1
        s[t] = float(r)
    else:
        c[t] += 1
        s[t] += float(r)

for line in in_file2.readlines():
    line_data = line.split()
    assert(len(line_data) == 3)
    t = line_data[0].strip()
    r = line_data[1] .strip()
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
    avg_out_file.write(t + " " + str(float(s[t]) / c[t]) + " " +
                       str(std_dev[t]) + "\n")

in_file1.close()
in_file2.close()
out_file.close()
avg_out_file.close()
