import itertools
import random


from State import *
from Coord import *

# Here you can set up rewards for various type of transitions and other 

stepRew = -1
goodMineRew = 50
badMineRew = -25
illegalMoveRew = -100
discount = 0.98
sense = -5

assert not badMineRew == 0 and not goodMineRew == 0 and not badMineRew == goodMineRew

perfect_sense_dist = 0 # in which distance does sensing work perfectly
useless_sense_dist = 5
dist_decay = 0.5/(useless_sense_dist - perfect_sense_dist) 


# Input

dim = int(input('Dimension:'))
totalMines = int(input('Total number of mines:'))
assert totalMines <= dim*dim, "I cannot fit that many mines into the grid of size "+str(dim*dim)
goodMines = int(input('Number of good mines:'))
assert totalMines >= goodMines, "Number of good mines cannot be larger than total number of mines"
outFile = str(raw_input('Output file name:'))
# f = outFile






def sumOfSampleMined(mineString):
    num = 0
    for i in range(len(mineString)):
        if int(mineString[i]) == 2:
            num+=1
    return num


def filterValidEntries(mineString):
    goodNM = 0
    goodM = 0
    for i in mineString:
        if i == 1:
            goodNM+=1
        if i == 2:
            goodM+=1
    if goodNM + goodM <= goodMines:
        return True
    else:
        return False


# write a model in the old format
# def writeOldFormat():
#   filename = '../RS'+str(dimX)+'x'+str(dimY) + 'x'+ str(minesCount) +'x'+str(numOfGoodToSample) + '.txt'
#   print("Output written to: "+filename)
#   f = open(filename, 'w')
#   f.write("NAME: [generated] #\n")
#   f.write("OBSERVATIONS: [")
#   f.write(', '.join(str(e) for e in observations))
#   f.write("] #\n")
#   f.write("STATES: [")
#   print("Num of states: " + str(len(listOfStates)))
#   for state in listOfStates:
#       f.write("{%s;1;%s;F}," %(str(state),state.observation))
#
#   f.write("{NgoalMNoneToGoal;2;oGoal;F},")
#   f.write("{NloosingMNoneToLoosing;1;oLoosing;F},")
#   f.write("{NdStartMNoneToStart;1;oStart;T}")
#
#
#
#
#
#
#   f.write("] #\n")
#
#   f.write("ACTIONS: [")
#   f.write(', '.join(str(e) for e in actions))
#   f.write("] #\n")
#
#
#   transitionsMap = {}
#
#   for transition in listOfTransitions:
#
#     source,action,target = transition
#
#     if (source,action) in transitionsMap:
#         targets = transitionsMap[(source,action)]
#         targets.append(target)
#     else:
#       targets = [target]
#     transitionsMap[(source,action)]=targets
#
#
#   # print(transitionsMap)
#   #
#   for transition in transitionsMap.keys():
#     source,action = transition
#     f.write("TRANSITIONS: [%s,%s,{" %(source,action))
#     support = ';'.join(str(e) for e in transitionsMap[transition])
#     f.write(support)
#     f.write("}] # \n")
#   for action in actions:
#       f.write("TRANSITIONS: [NgoalMNoneToGoal,%s,{NgoalMNoneToGoal}]\n" %(action))
#       f.write("TRANSITIONS: [NloosingMNoneToLoosing,%s,{NloosingMNoneToLoosing}]\n" %(action))
#   f.close()

def writeNewFormat():
    f = open(outFile, 'w')
    f.write("discount: "+str(discount)+"\n")
    f.write("values: reward\n")
    f.write("states: %s %s " %(goalState,losingState))
    f.write(' '.join(str(e) for e in listOfStates))
    f.write(' ')
    f.write(' '.join("sen"+str(e[0]) for e in senseStates))
    f.write(' ')
    f.write(' '.join("min"+str(e[0])+"R"+str(e[1]) for e in mineStates))
    f.write('\n')
    f.write("\nactions: ")
    f.write(' '.join(str(e) for e in actions))
    f.write("\nobservations: ")
    f.write(' '.join(str(e) for e in observations))
#
# f.write("\n\nparity: 2 ")
# for state in listOfStates:
#     automatonState = state.automatonState
#     priority = getPriority(int(automatonState))
#     f.write("%s " %(priority))
#     # f.write("2 ")
# f.write("1")
# f.write("\n\ngoals: 1\n")
# Start line
# f.write("\nstart: 1.0 ")
# for state in listOfStates:
#        f.write("0.0 ")
# for state in senseStates:
#        f.write("0.0 ")
# # for the loosing state
# f.write("0.0 0.0")
    f.write("\n start include: " +' '.join(map(str,startStates)) +"\n\n")
#
#
#
# Transitions:
	for transition in listOfTransitions:
		source,robotAct,target, rew = transition
		#
		if isinstance(source, tuple):
			if source[1] == "sensing":
				prob = 1.0
				f.write("\nT: %s : %s : %s %.2f" % (robotAct,"sen"+str(source[0]),target,prob))
			else:
				prob = 1.0
				f.write("\nT: %s : %s : %s %.2f" % (robotAct,"min"+str(source[0])+"R"+str(source[1]),target,prob))
		elif isinstance(target, tuple):
			if target[1] == "sensing":
				prob = 1.0
				f.write("\nT: %s : %s : %s %.2f" % (robotAct,source,"sen"+str(target[0]),prob))
			else:
				prob = 1.0
				f.write("\nT: %s : %s : %s %.2f" % (robotAct,source,"min"+str(target[0])+"R"+str(target[1]),prob))
		else:
			prob = 1.0
			f.write("\nT: %s : %s : %s %.2f" % (robotAct,source,target,prob))
	#
	#
	#
	f.write("\nT: * : %s : %s 1.0" %(goalState,goalState))
	f.write("\nT: * : %s : %s 1.0" %(losingState,losingState))
#
#
# for obs in observations:
#     for action in actions:
#         if obs[-1] == "G" and action == "Sample":
#             f.write("\nE: %s : %s %d" %(obs,action, capacity))
#         else:
#             f.write("\nE: %s : %s -1.0" %(obs,action))
# prob = 1.0 / startStates.__len__()
#
# for state in startStates:
#     f.write("\nT: * : dStart : %s %f" %(state,prob))
#
#
#
# Observations:
    f.write("\n")
    for state in listOfStates:
        robotPos = state.robot
        formDiscObs = "o%sx%sx" % (str(robotPos.x), str(robotPos.y))
        f.write("\nO: * : %s : %s 1.0" %(state,formDiscObs))
    for state in senseStates:
        robotPos = state[0].robot
        obs = state[0].observation
        for key in obs:
            formDiscObs = "o%sx%sx%s" % (str(robotPos.x), str(robotPos.y), key)
            f.write("\nO: * : %s : %s %f" %("sen"+str(state[0]),formDiscObs, obs[key]))
    for state in mineStates:
        robotPos = state[0].robot
        if state[1] == 0:
            formDiscObs = "o%sx%sxMM" % (str(robotPos.x), str(robotPos.y))
        elif state[1] == badMineRew:
            formDiscObs = "o%sx%sxMB" % (str(robotPos.x), str(robotPos.y))
        else:
            formDiscObs = "o%sx%sxMG" % (str(robotPos.x), str(robotPos.y))
        f.write("\nO: * : %s : %s 1.0" %("min"+str(state[0])+"R"+str(state[1]),formDiscObs))
    f.write("\nO: * : %s : oLosing 1.0" %(losingState))
    f.write("\nO: * : %s : oGoal 1.0" %(goalState))
# f.write("\nO: * : dStart : oStart 1.0")
# f.write("\nR: * : * : * : * 1.0")
    f.write("\nR: * : %s : * : * 0.0" %(goalState))
    f.write("\nR: * : %s : * : * 0.0" %(losingState))
    for transition in listOfTransitions:
        source,robotAct,target, rew = transition
        if not isinstance(source,tuple):
            f.write("\nR: %s : %s : * : * %d" % (robotAct, source, rew))
        elif source[1] == "sensing":
            f.write("\nR: %s : %s : * : * %d" % (robotAct, "sen"+str(source[0]), rew))
        else:
            f.write("\nR: %s : %s : * : * %d" % (robotAct, "min"+str(source[0])+"R"+str(source[1]), rew))





# Set up observations

observationString = "oStart,oGoal,oLosing"
observations = set(observationString.split(","))
actionString = "N,E,S,W"
actions = actionString.split(",")
listOfStates = []


# Structure for holding mone locations and map from integer to tuple coords and vice versa

listMines = []
minePosToInt = {}
mineIntToPos = {}

def randMines(rdim, rtotalMines):
    print("Not a valid coordinate list. (Does the number of mines correspond to what was input above?)")
    print("Generating the list randomly.")
    listMinesR = random.sample(range(rdim*rdim), rtotalMines)
    listMines=[]
    counter=0
# Generate tuple coordinates of mines
    for intID in listMinesR:
        pos = getCoord(intID,dim)
        listMines.append(pos)
        mineIntToPos[counter]=pos
        minePosToInt[pos]=counter
        counter+=1
    return listMines



# Input mine coords, if not given, generate randomly

def can_evaluate(string):
    try:
        eval(string)
        return True
    except:
        return False

listMinesIn = input('Enter list of mine coordinates, as list of tuples, or some bogus to generate randomly')
listMinesCp = listMinesIn
if not can_evaluate(listMinesCp):
    listMines = randMines(dim, totalMines)
else:
    listMinesEv = eval(listMinesIn)
    if not isinstance(listMinesEv,list) or not len(listMinesEv) == totalMines:
        listMines = randMines(dim, totalMines)
    else:
        counter = 0
        for mineCoord in listMinesEv:
            if not isinstance(mineCoord,tuple) or not len(mineCoord) == 2:
                listMines = randMines(dim, totalMines)
            elif not (isinstance(mineCoord[0],int) and isinstance(mineCoord[1],int) and 0 <= mineCoord[0] and mineCoord[0] <= dim - 1 and mineCoord[1] <= dim - 1):
                listMines = randMines(dim, totalMines)
            else:
                listMines.append(mineCoord)
                newCoord = Coord(mineCoord[0],mineCoord[1],dim)
                mineIntToPos[counter]= newCoord
                minePosToInt[newCoord]=counter
                counter += 1



# Generate possible info about state of Mines 0 = bad, 1=good, 2= was good, but now is mined

tuplesList = itertools.product([0, 1, 2], repeat=len(listMines))
sensetuples = list(itertools.product(['G', 'B'], repeat=len(listMines)))

restrictedTuples = [x for x in tuplesList if filterValidEntries(x)]
# sensetuples =  [x for x in sensetuplesAll if x.count('G')==goodMines]


# restrictedTuples = tuples
# print(list(restrictedTuples))


# startState = State("dStart",None,"oStart")
goalState = State("goal",None,{"oGoal":1})
losingState = State("losing",None,{"oLosing":1})


# listOfStates = []


print("List of mines:"+str(listMines))
print("Indexes of mines:"+str(minePosToInt))

# Generate standard states

for robotPos in list(generateCoords(dim)):
    formDiscObs = "o%sx%sx" % (str(robotPos.x), str(robotPos.y))
    if not formDiscObs in observations:
        observations.add(formDiscObs)
    for mineStats in restrictedTuples:
        mineString = "".join(map(str,list(mineStats)))
        # obs is a map from strings of 'G' and 'B' to probabilities
        obs = getObs(robotPos, mineString, listMines, minePosToInt,sensetuples)
        for key in obs:
            formDiscObs = "o%sx%sx%s" % (str(robotPos.x), str(robotPos.y), key)
            if not formDiscObs in observations:
                observations.add(formDiscObs)
        if robotPos in listMines:
            formDiscObs = "o%sx%sxMG" % (str(robotPos.x), str(robotPos.y))
            if not formDiscObs in observations:
                observations.add(formDiscObs)
            formDiscObs = "o%sx%sxMM" % (str(robotPos.x), str(robotPos.y))
            if not formDiscObs in observations:
                observations.add(formDiscObs)
            formDiscObs = "o%sx%sxMB" % (str(robotPos.x), str(robotPos.y))
            if not formDiscObs in observations:
                observations.add(formDiscObs)
        state = State(robotPos,mineString,obs)
        # print(str(state.robot))
        listOfStates.append(state)

senseStates = []
for state in listOfStates:
    newState = (state,"sensing")
    senseStates.append(newState)
mineStates = []
#

# print(listOfStates)

# Transitions
listOfTransitions = []
#
for state in listOfStates:
    for robotAct in actions:
        newRCoord,bump = state.robot.getNext(robotAct)
        obs = getObs(newRCoord, state.mines, listMines, minePosToInt,sensetuples)
        newState = State(newRCoord,state.mines,obs)
        if bump:
            setRew = illegalMoveRew
        else:
            setRew = stepRew
        listOfTransitions.append((str(state),robotAct,str(newState),setRew))


    robotAct = "Sample"
    # sampleActionTaken = False
    coord = state.robot
    isOnMine = False
    for mine in listMines:
        if mine == coord:
            isOnMine = True
            mineID = minePosToInt.get(coord)
            mineVal = int(state.mines[mineID])
            if mineVal == 0:
                # Mining a bad mine
                newState = (state,badMineRew)
                listOfTransitions.append((state,robotAct,newState,0))
                # sampleActionTaken = True
            elif mineVal == 2:
                # Mining an already mined state makes it bad, we receive zero reward
                newMineString = ""
                for i in range(len(state.mines)):
                    if i == mineID:
                        newMineString+="0"
                    else:
                        newMineString+=state.mines[i]
                # nothing changes
                # listOfTransitions.append((state,robotAct,state))
                obs = getObs(state.robot, newMineString, listMines, minePosToInt,sensetuples)
                newState = (State(state.robot,newMineString,obs),0)
                listOfTransitions.append((state,robotAct,newState,0))
                # sampleActionTaken = True


            elif mineVal == 1:
                # Mining a good state makes it mined but not bad
                newMineString = ""
                for i in range(len(state.mines)):
                    if i == mineID:
                        newMineString+="2"
                    else:
                        newMineString+=state.mines[i]
                # if sumOfSampleMined(newMineString) == numOfGoodToSample:
                #     listOfTransitions.append((state,robotAct,goalState))
                #     sampleActionTaken = True
                # elif sumOfSampleMined(newMineString) < numOfGoodToSample:
                newState = (State(state.robot,newMineString,state.observation),goodMineRew)
                listOfTransitions.append((state, robotAct, newState, 0))
            if not newState in mineStates:
                mineStates.append(newState)
    if not isOnMine:
        listOfTransitions.append((state, robotAct, losingState, illegalMoveRew))
                # sampleActionTaken = True
    # if not sampleActionTaken:
    #     listOfTransitions.append((state,robotAct,losingState))

    robotAct = "Sense"
    newState = (state,"sensing")
    listOfTransitions.append((state, robotAct, newState, sense))

    robotAct = "Finish"
    newState = goalState
    listOfTransitions.append((state, robotAct, newState, 0))

actions.append("Sample")
actions.append("Finish")
actions.append("Sense")

for state in senseStates:
    for act in actions:
        listOfTransitions.append((state,act,state[0], 0))

for state in mineStates:
    for act in actions:
        listOfTransitions.append((state,act,state[0], state[1]))



startStates = []
tuples = itertools.product([0, 1], repeat=len(listMines))
restrictedTuples = [x for x in tuples if sum(x) == goodMines]
# print(restrictedTuples)
for rtuple in restrictedTuples:
    mineString = "".join(map(str,list(rtuple)))
    obs = getObs(Coord(0,0,dim),mineString,listMines,minePosToInt,sensetuples)
    newState = State(Coord(0,0,dim),mineString,obs)
    startStates.append(newState)
    # for action in actions:
    #    listOfTransitions.append((startState,action,newState))


# writeOldFormat()
# f = outFile
writeNewFormat()

totalStates = len(listOfStates)+len(senseStates)+len(mineStates)+2
totalObs = len(observations)

print("\n Total no of states: %d" % (totalStates))
print("\n Total no of Obs: %d" % (totalObs))








