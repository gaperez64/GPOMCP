import random
import numpy


def isIn(i,j,num_rows,num_cols):
    b = 1
    if (i < 0) or (j < 0) or (i >= num_rows) or (j >= num_cols):
        b=0;
    return b;



#input
num_rows = int(input("Rows-2: "))
num_cols = int(input("Columns-2: "))
num_nowall = int(input("Number of cells that are not walls: "))
num_trap = int(input("Number of traps: "))
num_initial = int(input("Number of possible initial locations: "))
num_goal = int(input("Number of reward locations: "))

#generate a maze
M = numpy.chararray((num_rows,num_cols))
for i in range(0,num_rows):
    for j in range(0,num_cols):
        M[i,j] = '1';

#set all to 1's as we'll change only non-wall cells


#variables used for generating a random maze
current = random.randint(1, num_rows*num_cols)
current_x = current % num_cols
current_y = (current - current_x) / num_cols

count = num_nowall - 1
M[current_x,current_y] = '0'
stack = []
stack.append([current_x,current_y])

#those cells with no walls
nowalls = []
nowalls.append([current_x,current_y])

current_direction = 'D'

while count > 0:
    adjacent = [];
    if (isIn(current_x-1,current_y,num_rows,num_cols)==1) and (M[current_x-1,current_y] == '1'): adjacent.append('L');
    if (isIn(current_x+1,current_y,num_rows,num_cols)==1) and (M[current_x+1,current_y] == '1'): adjacent.append('R');
    if (isIn(current_x,current_y-1,num_rows,num_cols)==1) and (M[current_x,current_y-1] == '1'): adjacent.append('D');
    if (isIn(current_x,current_y+1,num_rows,num_cols)==1) and (M[current_x,current_y+1] == '1'): adjacent.append('U');
    
    if (current_direction in adjacent):
        if (current_direction.__eq__('U')):
            count = count - 1
            current_y = current_y + 1
            M[current_x,current_y] = '0'
            nowalls.append([current_x,current_y])
            stack.append([current_x,current_y])

        elif (current_direction.__eq__('D')):
            count = count - 1
            current_y = current_y - 1
            M[current_x,current_y] = '0'
            nowalls.append([current_x,current_y])
            stack.append([current_x,current_y])

        elif (current_direction.__eq__('L')):
            count = count - 1
            current_x = current_x - 1
            M[current_x,current_y] = '0'
            nowalls.append([current_x,current_y])
            stack.append([current_x,current_y])

        elif (current_direction.__eq__('R')):
            count = count - 1
            current_x = current_x + 1
            M[current_x,current_y] = '0'
            nowalls.append([current_x,current_y])
            stack.append([current_x,current_y])


    elif (len(adjacent)>0):
        direction = random.choice(adjacent)

        if (direction.__eq__('U')):
            count = count - 1
            current_y = current_y + 1
            M[current_x,current_y] = '0'
            nowalls.append([current_x,current_y])
            stack.append([current_x,current_y])
            current_direction = 'U'

        elif (direction.__eq__('D')):
            count = count - 1
            current_y = current_y - 1
            M[current_x,current_y] = '0'
            nowalls.append([current_x,current_y])
            stack.append([current_x,current_y])
            current_direction = 'D'

        elif (direction.__eq__('L')):
            count = count - 1
            current_x = current_x - 1
            M[current_x,current_y] = '0'
            nowalls.append([current_x,current_y])
            stack.append([current_x,current_y])
            current_direction = 'L'

        elif (direction.__eq__('R')):
            count = count - 1
            current_x = current_x + 1
            M[current_x,current_y] = '0'
            nowalls.append([current_x,current_y])
            stack.append([current_x,current_y])
            current_direction = 'R'

    else:
        del stack[-1]
        u = stack[-1]
        current_x = u[0]
        current_y = u[1]

#no we will just allocate traps, initial locations and goals to some cells randomly
random.shuffle(nowalls)


for i in range(0,num_trap):
    alpha=nowalls[i]
    M[alpha[0],alpha[1]] = 'x'

for i in range(0,num_initial):
    alpha=nowalls[num_trap+i]
    M[alpha[0],alpha[1]] = '+'

for i in range(0,num_goal):
    alpha=nowalls[num_trap+num_initial+i]
    M[alpha[0],alpha[1]] = 'g'


#output
s1 = '1'
for i in range(0,num_cols+1):
    s1 = s1 + ' 1'
f = open('E:\workfile.txt', 'w')
f.write(s1+"\n")
for i in range(0,num_rows-1):
    s = '1 ' + M[i,0]
    for j in range(1,num_cols):
        s = s + ' ' + M[i,j]
    s = s + ' 1'
    f.write(s+"\n")
    
s = '1 ' + M[num_rows-1,0]
for j in range(1,num_cols):
    s = s + ' ' + M[num_rows-1,j]
s = s + ' 1'
f.write(s+"\n")
f.write(s1)
f.close()
