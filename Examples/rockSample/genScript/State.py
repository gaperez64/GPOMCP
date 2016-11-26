from Coord import *
import itertools

class State:
	def __init__(self, robot, mines, observation):
		self.robot = robot
		self.mines = mines
		self.observation = observation


	def __repr__(self):
		return "N%sM%sT" % (str(self.robot),str(self.mines))
		# return "(R:%s,T:%s,A: %s,O:%s)" % (self.robot,self.target,self.automatonState,self.observation)

	def __str__(self):
		return self.__repr__()




	def __eq__(self, other):
		if not isinstance(other,State):
			return False
		return self.__repr__() == other.__repr__()
		# return self.robot == other.robot and self.target == other.target and self.observation == other.observation and self.automatonState == other.automatonState


	def __hash__(self):
		return hash(repr(self))


# Coords: origin has (0,0), then standard geometric interpretation, final and losing states have coords (-1,-1) and (-2,-2), respectively.
# We map integers to coords as follows:
# 12 13 14 15  
# 8 9 10 11
# 4 5 6 7
# 0 1 2 3
# 

obsDict = {0 : (1,0), 1 : (0.9,0.1), 2 : (0.8,0.2), 3 : (0.7,0.3), 4 : (0.6,0.4), 5 : (0.5,0.5)}

def distMine(cRobot, cMine):
	return (abs(cRobot.x-cMine.x) + abs(cRobot.y - cMine.y) )



def getObs(coord, mineString, mines, minePosToInt,sensetuples):
# What kind of observations I can get if I use the sensing action in a given state
# 	print(str(coord))
	mineIndObs = {}
	returnObs = {}
	retObsTemp = ""
	counter = 0
	for mine in mines:
		distToM = min(distMine(coord,mine), 5)
		mineID = minePosToInt.get(mine)
		corrProb = obsDict[distToM][0]
		incorrProb = obsDict[distToM][1]
		if int(mineString[mineID]) > 0:
			mineIndObs[counter] = (corrProb,incorrProb) # First item is the probability of sensing good, second of
			# sensing bad mine
		else:
			mineIndObs[counter] = (incorrProb,corrProb)
		counter+=1
	# counter = 0
	for senseTuple in sensetuples:
		prob = 1
		counter = 0
		for i in senseTuple:
			#retObsTemp += i
			if i == 'G':
				prob = prob*mineIndObs[counter][0]
			else:
				prob = prob*mineIndObs[counter][1]
			counter += 1
		index = "".join(str(e) for e in senseTuple)
		returnObs[index] = prob
	# return "o"+str(coord.x)+"x"+str(coord.y) + "x" + mineObs
	assert len(returnObs) == len(sensetuples)
	return returnObs


#
# def getObs(relPos):
#   if relPos==9:
#     return ["oNot"]
#   elif relPos ==8:
#     return ["oSE"]
#   elif relPos ==7:
#     return ["oSE","oSW"]
#   elif relPos ==6:
#     return ["oSW"]
#   elif relPos ==5:
#     return ["oSE","oNE"]
#   elif relPos ==4:
#     return ["oDetected"]
#   elif relPos ==3:
#     return ["oSW","oNW"]
#   elif relPos ==2:
#     return ["oNE"]
#   elif relPos ==1:
#     return ["oNE","oNW"]
#   elif relPos ==0:
#     return ["oNW"]
