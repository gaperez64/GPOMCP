# Generates a POMDP file with a Hallway instance from a given maze map in a text file.
# Usage: python genHallway.py input_map.txt output_file [discount_factor]

# Hallway: a classic POMDP robot navigation benchmark. We have a maze with walls, traps and goals.
# The robot can move forward or turn left and right and it can only sense walls around itself (it cannot sense
# traps). The goal is to find the target as fast as possible (i.e. actions have zero reward until
# target is reached, after which a large number is recieved, discounted by the number of steps).

# The maze is given as a text file, each row representing a row of a maze,
# individual cells in a row separated by spaces. See the following example:

# 1 1 1 1 1 1 
# 1 0 + + 0 1 
# 1 1 + + 1 1 
# 1 0 0 0 0 1 
# 1 1 g x 1 1 
# 1 1 1 1 1 1 

# 1s are walls, 0s are empty cells, R are reloading places (this is from a different application domain,
# we probably don't need this), g is a goal state, x is a trap which we have to avoid
# (state with a self-loop), + is a possible starting location. The robot starts in a starting location
# chosen uniformly at random, looking southwards.

# FOR THE SCRIPT TO FUNCTION, THE MAZE HAS TO BE SURROUNDED BY WALLS

# NW corner of the maze is the coordinate origin, coordinates increase with 
# each step to the east and to the south.

# The output is given in the standard POMDP file format (http://www.pomdp.org/code/pomdp-file-spec.html)


__author__ = 'mchmelik, edited by pnovotny'
import sys

# Test for optional input (discount factor), otherwise take default.

if len(sys.argv)>3:
	discount = sys.argv[3]
else:
	discount="0.95"

# How does the orientation change when turning around.

left = {'N':'W','W':'S','S':'E','E':'N'}
right = {'N':'E','E':'S','S':'W','W':'N'}

# Coordinate change after moving forward with a given orientation.

changeMap = {'N':[-1,0],'S':[1,0],'E':[0,1],'W':[0,-1]}


# Is there a wall in front of the robot?

def isWall(i,j,direction):
    if(direction.__eq__('N')):
        return (maze[i-1][j] == '1')
    if(direction.__eq__('E')):
        return (maze[i][j+1] == '1')
    if(direction.__eq__('W')):
        return (maze[i][j-1] == '1')
    if(direction.__eq__('S')):
        return (maze[i+1][j] == '1')

def isTrap(i,j,direction):
    if(direction.__eq__('N')):
        return (maze[i-1][j] == 'x')
    if(direction.__eq__('E')):
        return (maze[i][j+1] == 'x')
    if(direction.__eq__('W')):
        return (maze[i][j-1] == 'x')
    if(direction.__eq__('S')):
        return (maze[i+1][j] == 'x')

# Retrieve a new orientation after a turn is made.

def turnLeft(orient):
    return left[orient]

def turnRight(orient):
    return right[orient]

# Produce observation for the current state. The only information about the environment
# the robot gets is, for each direction (forward,left,right,behind), whether there is a wall next to the robot
# in that direction. Hence, an observation can be characterized by a word from
# {F,f}.{L,l}.{R,r}.{B,b}, where a capital letter indicates that there _is_ a wall.

def getObs(i,j,orientation):
    if(orientation.__eq__('N')):
        return ['F' if isWall(i,j,'N') else 'f', 'L' if isWall(i,j,'W') else 'l', 'R' if isWall(i,j,'E') else 'r', 'B' if isWall(i,j,'S') else 'b']
    if(orientation.__eq__('E')):
        return ['F' if isWall(i,j,'E') else 'f', 'L' if isWall(i,j,'N') else 'l', 'R' if isWall(i,j,'S') else 'r', 'B' if isWall(i,j,'W') else 'b']
    if(orientation.__eq__('S')):
        return ['F' if isWall(i,j,'S') else 'f', 'L' if isWall(i,j,'E') else 'l', 'R' if isWall(i,j,'W') else 'r', 'B' if isWall(i,j,'N') else 'b']
    if(orientation.__eq__('W')):
        return ['F' if isWall(i,j,'W') else 'f', 'L' if isWall(i,j,'S') else 'l', 'R' if isWall(i,j,'N') else 'r', 'B' if isWall(i,j,'E') else 'b']


# print(sys.argv[1])
# print(sys.argv[2])
fIn = open(sys.argv[1], 'r');
fOut = open(sys.argv[2], 'w');

# fIn = open("hallway2.txt", 'r');
# fOut = open("hal.txt", 'w');
maze = [];
for line in fIn.readlines():
    maze.append(line.split());

width = len(maze[0]);
height = len(maze);


actions = ["f", "l", "r"]
states = ["absorb", "goal", "goal2"]

stateToObsMap = {"absorb":"Oabs", "goal":"Ogoal", "goal2":"Ogoal2"}

observations = {"Oabs", "Ogoal", "Ogoal2"}
reloads = set()
transitions = ""
observationEntries = ""

# Add self-loops under all actions for goal and trap states. We have two states for goal, one entered upon
# reaching goal and the other, with a self-loop, immediately after that.

for action in {'f','l','r'}:
    transitions += "T: %s : absorb : absorb 1.0 \n" %action
    transitions += "T: %s : goal : goal2 1.0 \n" %action
    transitions += "T: %s : goal2 : goal2 1.0 \n" %action

initialStates = set()

# For each possible robot state, generate outgoing transitions and the corresponding observation.
# State names have a format sixjxo, where [i,j] are coordinates and o is an oritentation.

for i in range(0, height):
    for j in range(0, width):
        element = maze[i][j]
        for orient in {'N','E','S','W'}:
			# Standard states, where there are all movement actions enabled.
            if element in {"0","+","R","x"}:
                # print("i: "+str(i)+" j: " + str(j) + " orient: " + orient)
                # print(''.join(getObs(i,j,orient)))
                # Forward
                # Move forward
                if isWall(i,j,orient):
                        target = "absorb"
                        transitions += "T: f : s%dx%dx%s : %s 1.0 \n" % (i,j,orient,target)
                elif isTrap(i,j,orient):
                        target = "s%dx%dx%s" % (i+changeMap[orient][0],j+changeMap[orient][1],'N')
                        transitions += "T: f : s%dx%dx%s : %s 0.25 \n" % (i,j,orient,target)
                        target = "s%dx%dx%s" % (i+changeMap[orient][0],j+changeMap[orient][1],'S')
                        transitions += "T: f : s%dx%dx%s : %s 0.25 \n" % (i,j,orient,target)
                        target = "s%dx%dx%s" % (i+changeMap[orient][0],j+changeMap[orient][1],'W')
                        transitions += "T: f : s%dx%dx%s : %s 0.25 \n" % (i,j,orient,target)
                        target = "s%dx%dx%s" % (i+changeMap[orient][0],j+changeMap[orient][1],'E')
                        transitions += "T: f : s%dx%dx%s : %s 0.25 \n" % (i,j,orient,target)
                else:
                        target = "s%dx%dx%s" % (i+changeMap[orient][0],j+changeMap[orient][1],orient);
                        transitions += "T: f : s%dx%dx%s : %s 1.0 \n" % (i,j,orient,target);
                        
                #target = "s%dx%dx%s" % (i+changeMap[orient][0],j+changeMap[orient][1],orient) if not isWall(i,j,orient) else "absorb";
                #target2 = "s%dx%dx%s" % (i,j,orient);
                #transitions += "T: f : s%dx%dx%s : %s 1.0 \n" % (i,j,orient,target)
                # transitions += "T: f : s%dx%dx%s : %s 0.02 \n" % (i,j,orient,target2)
                # transitions += "TRANSITIONS: [s%dx%dx%s,F,{%s}] #\n" % (i,j,orient,target);
                # Left
                # Turn left
                target = "s%dx%dx%s" % (i,j,left[orient]);
                transitions += "T: l : s%dx%dx%s : %s 1.0 \n" % (i,j,orient,target)
                # transitions += "T: l : s%dx%dx%s : %s 0.02 \n" % (i,j,orient,target2)

                # transitions += "TRANSITIONS: [s%dx%dx%s,L,{%s}] #\n" % (i,j,orient,target);
                # Or not turn at all
                # Right
                # Turn right
                target = "s%dx%dx%s" % (i,j,right[orient]);
                transitions += "T: r : s%dx%dx%s : %s 1.0\n" % (i,j,orient,target)
                # transitions += "T: r : s%dx%dx%s : %s 0.02\n" % (i,j,orient,target2)
                # transitions += "TRANSITIONS: [s%dx%dx%s,R,{%s}] #\n" % (i,j,orient,target);
                # Or not turn at all
                #obs = ''.join(getObs(i,j,orient))
                #observations.add(obs);

			# If in goal or trap location, go to the corresponding abstract state.			

            if element in {"g"}:
                transitions += "T: f : s%dx%dx%s : goal 1.0\n" % (i,j,orient)
                transitions += "T: l : s%dx%dx%s : goal 1.0\n" % (i,j,orient)
                transitions += "T: r : s%dx%dx%s : goal 1.0\n" % (i,j,orient)



            #if element in {"x"}:
             #   for orient1 in {'N','E','S','W'}:
              #          target = "s%dx%dx%s" % (i+changeMap[orient1][0],j+changeMap[orient1][1],orient1) if not isWall(i,j,orient1) else "absorb";
               #         transitions += "T: f : s%dx%dx%s : %s 0.25 \n" % (i,j,orient,target)
                #        target = "s%dx%dx%s" % (i,j,left[orient1]);
                 #       transitions += "T: l : s%dx%dx%s : %s 0.25 \n" % (i,j,orient,target)
                  #      target = "s%dx%dx%s" % (i,j,right[orient1]);
                   #     transitions += "T: r : s%dx%dx%s : %s 0.25\n" % (i,j,orient,target)



#            if element in {"R"}:
#                reloads.add("s%dx%dx%s" %(i,j,orient))



            if element in {"0","+","g","x","R"}:
                #if element in {"R"}:
                #    obs = ''.join(getObs(i,j,orient)) + "R"
                #else:
				
				#compute an observation and add it to an state-to-obs map
                obs = ''.join(getObs(i,j,orient))
                observations.add(obs);
                state = "s%dx%dx%s" % (i,j,orient)
                states.append(state)
                stateToObsMap[state]=obs
                # states+=", {s%dx%dx%s; 1; %s; F}" % (i,j,orient,obs)


        if element in {"+"}:
               initialStates.add("s%dx%dxS" % (i,j))




#prob = 1.0 / initialStates.__len__()

#for state in initialStates:
#    transitions+="T: f : st : %s %f \n" % (state,prob);
#    transitions+="T: l : st : %s %f \n" % (state,prob);
#    transitions+="T: r : st : %s %f \n" % (state,prob);

# Observations are deterministic and action-independent.

for state in states:
    observationEntries+="O: * : %s : %s 1.0\n" % (state,stateToObsMap.get(state))


# energyEntries = ""
# for state in states:
#    for action in actions:
#        obs = stateToObsMap.get(state)
#        if state in reloads:
#            energyEntries+="E: %s : %s 15.0\n" %(obs,action)
#        else:
#            energyEntries+="E: %s : %s -1.0\n" %(obs,action)



# Dump everything to an output file.


#fOut.write("capacity: 10\n\n\n")
fOut.write("discount: "+discount+"\n")
fOut.write("values: reward\n")
fOut.write("states: "+' '.join(states)+"\n\n")
fOut.write("actions: "+' '.join(actions)+"\n\n")

fOut.write("observations: " + ' '.join(observations) + " \n\n")

#fOut.write("goals: 2\n")
#fOut.write("start: 1.0")
#for _ in range(states.__len__()-1):
#    fOut.write(" 0.0")
#fOut.write("\n\n")

fOut.write("start include: " +' '.join(initialStates) +"\n\n")




# fOut.write("STATES: ["+states +"] #\n")
# fOut.write("ACTIONS:[" + actions +"] #\n")
fOut.write(transitions+"\n")
fOut.write(observationEntries+"\n")
#fOut.write(energyEntries+"\n")



fOut.write("R: * : * : * : * 0.0\n")
fOut.write("R: * : goal : goal2 : * 10000.0\n")





print("Num of states: " + str(len(states)))
print("Width", width);
print("Heigth", height);
print("Succesfully generated a POMDP format file.")
# print(transitions)



