import sys

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

for line in in_file.readlines():
    if line.find(threshold_string) >= 0:
        t = line[len(threshold_string):].strip()
    if line.find(reward_string) >= 0:
        r = line[len(reward_string):].strip()
        out_file.write(t + " " + r + "\n")
        if t not in s:
            c[t] = 1
            s[t] = float(r)
        else:
            c[t] += 1
            s[t] += float(r)
for t in s:
    avg_out_file.write(t + " " + str(float(s[t]) / c[t]) + "\n")

in_file.close()
out_file.close()
avg_out_file.close()
