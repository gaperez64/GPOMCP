import sys

threshold_string = "threshold = "
reward_string = "Obtained an accum reward of: "

if len(sys.argv) != 3:
    print "2 Arguments required: input_file and output_file"
    exit(0)

in_filename = sys.argv[1]
out_filename = sys.argv[2]
in_file = open(in_filename, 'r')
out_file = open(out_filename, 'w')
t = "N/A"
r = "N/A"

for line in in_file.readlines():
    if line.find(threshold_string) >= 0:
        t = line[len(threshold_string):].strip()
    if line.find(reward_string) >= 0:
        r = line[len(reward_string):].strip()
        out_file.write(t + " " + r + "\n")

in_file.close()
out_file.close()
