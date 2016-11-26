#dimX = 6
#dimY = 6
# dimY = 2

class Coord:
    # (0,0) bottom left corner
    def __init__(self,x,y,dim):
        self.x = x
        self.y = y
        self.dim = dim

    def setWalls(self,walls):
        self.walls = walls

    def __str__(self):
        return "%dx%d" % (self.x, self.y)

    def __repr__(self):
        return "%dx%d" % (self.x, self.y)

    def __eq__(self, other):
        if other==None :
          return False

        return self.x == other.x and self.y == other.y

    def __hash__(self):
        return hash(repr(self))


    def getNorth(self):
        if self.y<self.dim-1 :
            return (Coord(self.x,self.y+1,self.dim),False)
        else:
            return (Coord(self.x,self.y,self.dim),True)

    def getSouth(self):
        if self.y>0 :
            return (Coord(self.x,self.y-1,self.dim),False)
        else:
            return (Coord(self.x,self.y,self.dim),True)

    def getWest(self):
        if self.x>0:
            return (Coord(self.x-1,self.y,self.dim),False)
        else:
            return (Coord(self.x,self.y,self.dim),True)

    def getEast(self):
        if self.x<self.dim-1:
            return (Coord(self.x+1,self.y,self.dim),False)
        else:
            return (Coord(self.x,self.y,self.dim),True)

    def getNext(self,direction, walls=None):

        if direction == "N":
            targetCoord,bump = self.getNorth()
        elif direction == "E":
            targetCoord,bump = self.getEast()
        elif direction == "S":
            targetCoord,bump = self.getSouth()
        elif direction =="W":
            targetCoord,bump = self.getWest()
        else:
            targetCoord,bump = (self,False)

        if walls != None:
            blockedByAWall = self.checkWall(targetCoord,walls)
            if blockedByAWall:
                return self,False
        return (targetCoord,bump)




    def checkWall(self,targetCoord,walls):
        currentCoord = self
        if frozenset([currentCoord,targetCoord]) in walls:
           return 1
        else:
           return 0




def generateCoords(dim):
    for i in range(dim*dim):
        yield getCoord(i,dim)
        # Coord(i // dimX,i % dimX)

def getPossibleNeighbors(robotCoord,dim):
  possibleNeighbors = []
  diff = [lambda x: x+1,lambda x: x, lambda x: x-1]
  for diffX in diff:
    for diffY in diff:
      newX = diffX(robotCoord.x)
      newY = diffY(robotCoord.y)
      if(newX >= 0 and newX < dim and newY >=0 and newY < dim and robotCoord != Coord(newX,newY,dim)):
        possibleNeighbors.append(Coord(newX,newY,dim))
  return possibleNeighbors

def getCoord(integer, dim):
    y = integer//dim
    x = integer%dim
    return Coord(x,y,dim)
