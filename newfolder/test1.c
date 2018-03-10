#include "abdrive.h"                          // Include abdrive header
#include "ping.h"                             // Include ping header
/*
we can use if-ifelse-else replace if-if-else.
reduce tick error by:
int forwardTickCount=0;
goForward()
{
    if(forwardTickCount%3==0)
    {
        dirve_goto(129,129);
    }
    else
    {
        dirve_goto(130,130);
    }
}
*/



int viz[5][5]; // matrix that we will use so that we will not visit a square more than once
int neighbour[5][5][5]; // a 3D array where we store the x,y coordonates and the direction  ( 1- front, 2 -right, 3 - left , 4 - back); 
//we use this 3D array in order to build the map of the maze
int x, y;
int dist[5][5];   //Matrix we use in order to calculate the distace between the start square and the current square7
    
int directionStack[6][6],pathStack[17][2],indexPath=0;  // variables used in order to build our final path
int queue[202][2],current,last;
// the depth first search algorithm is used in order to traverse the maze 
// we visit the squares only once
//we get the coordonates of the squares and the direction
//the sensors used are PING and inflared( IR)
// when we see that the neighbour square are not visited( and we can go there) we move to the next square, call the dfs recurively on that square
// after the robot cannot go anywhere, it starts to go back

void dfs(int x,int y,int direction) {
    int irRight,irLeft,distance;
    distance = ping_cm(8);
    freqout(11, 1, 38000);                      // Left IR LED light
    irLeft = input(10);                         // Check left IR detector
    freqout(1, 1, 38000);                       // Repeat for right detector
    irRight = input(2);
   
    if (irLeft==1) {//starts to check on he sides
        if (direction==1){
            neighbour[x][y][3]=1;
            neighbour[x][y-1][2]=1;
        }
       if (direction==2) {
            neighbour[x][y][1]=1;
            neighbour[x+1][y][4]=1;
       }
       if (direction==3) {
           neighbour[x][y][4]=1;
           neighbour[x-1][y][1]=1;
       }
       if (direction==4) {
           neighbour[x][y][2]=1;
           neighbour[x][y+1][3]=1;
       }
    }
    if (irRight==1) {
        if (direction==1){
            neighbour[x][y][2]=1;
            neighbour[x][y+1][3]=1;
        }
       if (direction==2) {
            neighbour[x][y][4]=1;
            neighbour[x-1][y][1]=1;
       }
       if (direction==3) {
           neighbour[x][y][1]=1;
           neighbour[x+1][y][4]=1;
       }
       if (direction==4) {
           neighbour[x][y][3]=1;
           neighbour[x][y-1][2]=1;
       }
    }
     if (distance>=40) {    //checks on the front
        if (direction==1) {
              if (viz[x+1][y]==0) {          
                  neighbour[x+1][y][4]=1;
                  neighbour[x][y][1]=1;
                  drive_goto(130,130);
                  viz[x+1][y]=viz[x][y]+1;
                  dfs(x+1,y,1);
                  drive_goto(-130,-130);
                  }
        }
        if (direction==2) {
            if (viz[x][y+1]==0) {      
              drive_goto(130,130);
              neighbour[x][y+1][3]=1;
              neighbour[x][y][2]=1;
              viz[x][y+1]=viz[x][y]+1;
              dfs(x,y+1,2);
              drive_goto(-130,-130);
              }
        }
        if (direction==3) {
            if (y-1>=0) {
                if (viz[x][y-1]==0) {  
                  neighbour[x][y-1][2]=1;
                  neighbour[x][y][3]=1;
                  drive_goto(130,130);
                  viz[x][y-1]=viz[x][y]+1;
                  dfs(x,y-1,3);
                  drive_goto(-130,-130);
                  }
            }
        }
        if (direction==4) {
            if (x-1>=0) {
                if (viz[x-1][y]==0) {
                    neighbour[x][y][4]=1;
                    neighbour[x-1][y][1]=1;
                    drive_goto(130,130);
                    viz[x-1][y]=viz[x][y]+1;
                    dfs(x-1,y,4);
                    drive_goto(-130,-130);
                }
            }
        }
     }
     if (irLeft==1) {
        if (direction==1) {
            if (viz[x][y-1]==0) {
            drive_goto(-26,25);
            dfs(x,y,3);
            drive_goto(26,-25);
            }
        }
        if (direction==2) {
            if (viz[x+1][y]==0) {
            drive_goto(-26,25);
            dfs(x,y,1);
            drive_goto(26,-25);
            }
        }
        if (direction==3) {
            if (viz[x-1][y]==0) {
            drive_goto(-26,25);
            dfs(x,y,4);
            drive_goto(26,-25);
            }
        }
        if (direction==4) {
            if (viz[x][y+1]==0) {
            drive_goto(-26,25);
            dfs(x,y,2);
            drive_goto(26,-25);
            }
        }
     }
     if (irRight==1) {
        if (direction==1) {
            if (viz[x][y+1]==0) {
            drive_goto(26,-25);
            dfs(x,y,2);
            drive_goto(-26,25);
            }
        }
        if (direction==2) {
            if (viz[x-1][y]==0) {
            drive_goto(26,-25);
            dfs(x,y,4);
            drive_goto(-26,25);
            }
        }
        if (direction==3) {
            if (viz[x+1][y]==0) {
            drive_goto(26,-25);
            dfs(x,y,1);
            drive_goto(-26,25);
            }
        }
        if (direction==4) {
            if (viz[x][y-1]==0) {
            drive_goto(26,-25);
            dfs(x,y,3);
           drive_goto(-26,25);
            }
        }

     }
}


//functions used to move 
void doRight()
{
  drive_goto(-26,25);
  drive_goto(130,130);
}

void doLeft()
{
  drive_goto(26,-25);
  drive_goto(130,130);
}

void doBack()
{
  drive_goto(104,-102);
  drive_goto(130,130);
 } 
 
 void doFront()
 {
   drive_goto(130,130);
 }      
  
 //calculates the distances as well as the directionStack
 void buildDistance()
 {
   while(current<=last) {
        x=queue[current][0];
        y=queue[current][1];
        if (neighbour[x][y][1]) {
            if (dist[x+1][y]>dist[x][y]+1) {
                dist[x+1][y]=dist[x][y]+1;
                directionStack[x+1][y]=4;
                queue[++last][0]=x+1;
                queue[last][1]=y;
            }
        }
       if (neighbour[x][y][2]) {
          if (dist[x][y+1]>dist[x][y]+1) {
              dist[x][y+1]=dist[x][y]+1;
              directionStack[x][y+1]=3;
              queue[++last][0]=x;
              queue[last][1]=y+1;
          }
       }
       if (neighbour[x][y][3]) {
           if (dist[x][y-1]>dist[x][y]+1) {
               dist[x][y-1]=dist[x][y]+1;
               directionStack[x][y-1]=2;
               queue[++last][0]=x;
               queue[last][1]=y-1;
           }
       }
       if (neighbour[x][y][4]) {
           if (dist[x-1][y]>dist[x][y]+1) {
              dist[x-1][y]=dist[x][y]+1;
              directionStack[x-1][y]=1;
              queue[++last][0]=x-1;
              queue[last][1]=y;
           }
       }
       current++;
    }
  }
  
  //gets the shortest path and the length of it
  
  void buildPath(int x, int y)
  {
    x=4,y=3;//top corner
    while(x>0 || y>0) { 
      pathStack[++indexPath][0]=x;
      pathStack[indexPath][1]=y;
      if (directionStack[x][y]==1)
        x++;
      else if (directionStack[x][y]==2)
        y++;
      else if (directionStack[x][y]==3)
        y--;
      else if (directionStack[x][y]==4)
        x--;
    }
  }
  
  //makes the robot move on the shortest path 
  
  void goEnd( int x, int y, int direction)
  {
    while(indexPath>0) {
        if (pathStack[indexPath][0]==x+1) {
            if (direction==1)
                doFront();
            if (direction==2) 
                doRight();
            if (direction==3) 
                doLeft();
            if (direction==4) 
                doBack();
            direction=1;
        }
        if (pathStack[indexPath][0]==x-1) {
            doFront();
            if (direction==1) 
                doBack();
            if (direction==2)
              doLeft();
            if (direction==3) 
                doRight();
            if (direction==4)
                doFront();
            direction=4;
        }
        if (pathStack[indexPath][1]==y+1) {
            if (direction==1) 
            doLeft();
            
            if (direction==2)
                doFront();
            if (direction==3)
                doBack();
            if (direction==4) 
                doRight();
          direction=2;
        }
        if (pathStack[indexPath][1]==y-1) {
            if (direction==1) 
                doLeft();
            if (direction==2) 
                doBack();
            if (direction==3)
                doFront();
            if (direction==4) 
                doRight();
            direction=3;
        }
        x=pathStack[indexPath][0];
        y=pathStack[indexPath][1];
        indexPath--;
    }
  }           
int main()                                    // main function
{
   viz[0][0]=1;
   //depth first search algorithm - we use it in order to see the neighbours of each square
   dfs(0,0,1);
    //we initailize the distance matrix with 100 ( the maximum possible is 16 )
    for (int i=0;i<=4;i++)
        for (int j=0;j<=4;j++)
           dist[i][j]=100;
    dist[0][0]=0;// we initialize the distance from the start position and itself as 0
    queue[0][0]=0; // we put the square of the coordonate 0 0 the first on our priority queue
    queue[0][1]=0;
   
    current=0;    //the index of the current element from the queue
    last=0;       //the last element of the queue
    // we start the djakstra algorithm. basically, we will have a matrix that store the distance between the squre of coordonates 00 and the rest of the squares
    //we will use a queue to traverse the matrix so that we will not visit a square more than once
    //the directionStack matrix will help us to build the final path between the 2 points
    
   buildDistance();
   buildPath( 3,  4);
   goEnd(0,0,1);
}


