#include "abdrive.h"                          // Include abdrive header
#include "ping.h"                             // Include ping header

int rightCount=0;
int leftCount=0;

int visit[5][5]; // matrix that we will use so that we will not visit a square more than once
int neighbours[5][5][5]; // a 3D array where we store the x,y coordonates and the direction  ( 1- front, 2 -right, 3 - left , 4 - back); 
//we use this 3D array in order to build the map of the maze
int x, y;
int distFromStart[5][5];   //Matrix we use in order to calculate the distace between the start square and the current square7
    
int direStack[6][6],pathsStack[20][2];  // variables used in order to build our final path

int queue[300][2];

void depthfirst(int x,int y,int direction) {
    int irRight,irLeft,distance;
    distance = ping_cm(8);
    drive_goto(-25, 26);
    irLeft =  ping_cm(8);
    drive_goto(51, -51);
    irRight =  ping_cm(8);
    drive_goto(-26, 25);
   
   if (irLeft<=40) {//starts to check on he sides
        if (direction==1){
            neighbours[x][y][3]=-1;
        }
        else if (direction==2) {
            neighbours[x][y][1]=-1;
       }
       else if (direction==3) {
           neighbours[x][y][4]=-1;
       }
       else {
           neighbours[x][y][2]=-1;
       }
    }
    if (irLeft<=40) {//starts to check on he sides
        if (direction==1){
            neighbours[x][y][3]=-1;
        }
        else if (direction==2) {
            neighbours[x][y][1]=-1;
       }
       else if (direction==3) {
           neighbours[x][y][4]=-1;
       }
       else {
           neighbours[x][y][2]=-1;
       }
    }
    if (irLeft>=40) {//starts to check on he sides
        if (direction==1){
            if (neighbours[x][y][3]!= -1)
                neighbours[x][y][3]=1;
            neighbours[x][y-1][2]=1;
        }
        else if (direction==2) {
            if (neighbours[x][y][1]!= -1)
            neighbours[x][y][1]=1;
            neighbours[x+1][y][4]=1;
       }
       else if (direction==3) {
           if (neighbours[x][y][4]!= -1)
           neighbours[x][y][4]=1;
           neighbours[x-1][y][1]=1;
       }
       else {
           if (neighbours[x][y][2]!= -1)
           neighbours[x][y][2]=1;
           neighbours[x][y+1][3]=1;
       }
    }
    if (irRight>=40) {
        if (direction==1){
             if (neighbours[x][y][2]!= -1)
            neighbours[x][y][2]=1;
            neighbours[x][y+1][3]=1;
        }
       else if (direction==2) {
            if (neighbours[x][y][4]!= -1)
            neighbours[x][y][4]=1;
            neighbours[x-1][y][1]=1;
       }
       else if (direction==3) {
            if (neighbours[x][y][1]!= -1)
           neighbours[x][y][1]=1;
           neighbours[x+1][y][4]=1;
       }
       else  {
            if (neighbours[x][y][3]!= -1)
           neighbours[x][y][3]=1;
           neighbours[x][y-1][2]=1;
       }
    }
     if (distance>=50) {    //checks on the front
        if (direction==1) {
              if (visit[x+1][y]==0) {
                  drive_goto(123,123);          
                  neighbours[x+1][y][4]=1;
                  neighbours[x][y][1]=1;
                  visit[x+1][y]=visit[x][y]+1;
                  depthfirst(x+1,y,1);
                  drive_goto(-123,-123);
                  }
        }
        else if (direction==2) {
            if (visit[x][y+1]==0) {      
              drive_goto(123,123);
              neighbours[x][y+1][3]=1;
              neighbours[x][y][2]=1;
              visit[x][y+1]=visit[x][y]+1;
              depthfirst(x,y+1,2);
              drive_goto(-123,-123);
              }
        }
        else if (direction==3) {
            if (y-1>=0) {
                if (visit[x][y-1]==0) {
                  drive_goto(123,123);  
                  neighbours[x][y-1][2]=1;
                  neighbours[x][y][3]=1;
                  visit[x][y-1]=visit[x][y]+1;
                  depthfirst(x,y-1,3);
                  drive_goto(-123,-123);
                  }
            }
        }
        else  {
            if (x-1>=0) {
                if (visit[x-1][y]==0) {
                    drive_goto(123,123);
                    neighbours[x][y][4]=1;
                    neighbours[x-1][y][1]=1;
                    visit[x-1][y]=visit[x][y]+1;
                    depthfirst(x-1,y,4);
                    drive_goto(-123,-123);
                }
            }
        }
     }
     if (irLeft>=40) {
        if (direction==1) {
            if (visit[x][y-1]==0) {
            drive_goto(-26,25);
            depthfirst(x,y,3);
            drive_goto(26,-25);
            }
        }
        else if (direction==2) {
            if (visit[x+1][y]==0) {
            drive_goto(-26,25);
            depthfirst(x,y,1);
            drive_goto(26,-25);
            }
        }
        else if (direction==3) {
            if (visit[x-1][y]==0) {
            drive_goto(-26,25);
            depthfirst(x,y,4);
            drive_goto(26,-25);
            }
        }
        else  {
            if (visit[x][y+1]==0) {
            drive_goto(-26,25);
            depthfirst(x,y,2);
            drive_goto(26,-25);
            }
        }
     }
     if (irRight>=40) {
        if (direction==1) {
            if (visit[x][y+1]==0) {
            drive_goto(26,-25);
            depthfirst(x,y,2);
            drive_goto(-26,25);
            }
        }
        else if (direction==2) {
            if (visit[x-1][y]==0) {
            drive_goto(26,-25);
            depthfirst(x,y,4);
            drive_goto(-26,25);
            }
        }
        else if (direction==3) {
            if (visit[x+1][y]==0) {
            drive_goto(26,-25);
            depthfirst(x,y,1);
            drive_goto(-26,25);
            }
        }
        else  {
            if (visit[x][y-1]==0) {
            drive_goto(26,-25);
            depthfirst(x,y,3);
           drive_goto(-26,25);
            }
        }

     }
}


//functions used to move 
void goLeft()
{
    if (rightCount%2==0)
    {
        drive_goto(-26,25);
    }
    else
    {
        drive_goto(-25,26);
    }
    drive_goto(123,123);
    rightCount+=1;
}

void goRight()
{
    if (leftCount%2==0)
    {
        drive_goto(26,-25);
    }
    else
    {
        drive_goto(25,-26);
    }
  drive_goto(123,123);
  leftCount+=1;
}

void goBack()
{
  drive_goto(104,-102);
  drive_goto(123,123);
 } 
 
 void goFront()
 {
   drive_goto(123,123);
 }      
  
 //calculates the distances as well as the direStack
 void buildDistance()
 {
    int current,last;
    while(current<=last) {
        x=queue[current][0];
        y=queue[current][1];
        if (neighbours[x][y][1]==1) {
            if (distFromStart[x+1][y]>distFromStart[x][y]+1) {
                distFromStart[x+1][y]=distFromStart[x][y]+1;
                direStack[x+1][y]=4;
                queue[++last][0]=x+1;
                queue[last][1]=y;
            }
        }
       if (neighbours[x][y][2]==1) {
          if (distFromStart[x][y+1]>distFromStart[x][y]+1) {
              distFromStart[x][y+1]=distFromStart[x][y]+1;
              direStack[x][y+1]=3;
              queue[++last][0]=x;
              queue[last][1]=y+1;
          }
       }
       if (neighbours[x][y][3]==1) {
           if (distFromStart[x][y-1]>distFromStart[x][y]+1) {
               distFromStart[x][y-1]=distFromStart[x][y]+1;
               direStack[x][y-1]=2;
               queue[++last][0]=x;
               queue[last][1]=y-1;
           }
       }
       if (neighbours[x][y][4]==1) {
           if (distFromStart[x-1][y]>distFromStart[x][y]+1) {
              distFromStart[x-1][y]=distFromStart[x][y]+1;
              direStack[x-1][y]=1;
              queue[++last][0]=x-1;
              queue[last][1]=y;
           }
       }
       current++;
    }
  }
  
  //gets the shortest path and the length of it
  
  int buildPath()
  {
    int indexPath=0;
    int a=4;
    int b =3;//top corner
    while(a>=1 || b>=1) { 
      pathsStack[++indexPath][0]=a;
      pathsStack[indexPath][1]=b;
      if (direStack[a][b]==1)
        a++; 
      else if (direStack[a][b]==2)
        b++;
      else if (direStack[a][b]==3)
        b--;
      else
        a--;
    }
    return indexPath;
  }
  
  //makes the robot move on the shortest path 
  
  void goEnd(int idxpath)
  {
      int a=0; 
      int b=0; 
      int dir=1;
      int c = 0;
      int strightpath[20];
      
    while(idxpath>0) {
        if (pathsStack[idxpath][0]==a+1) {
            if (dir==1)
                // goFront();
                {strightpath[c] = 1;
                printf("1,1  ");}
            else if (dir==2) 
                // goLeft();
                {strightpath[c] = 2;
                printf("2,1  ");}
            else if (dir==3) 
                // goRight();
                {strightpath[c] = 3;
                printf("3,1  ");}
            else  
                // goBack();
                {strightpath[c] = 4;
                printf("4,1  ");}
            dir=1;
        }
        else if (pathsStack[idxpath][0]==a-1) {
            // goFront();
            
            if (dir==1) 
                // goBack();
                {strightpath[c] = 4;
                printf("1,4  ");}
            else if (dir==2)
            //   goRight();
                {strightpath[c] = 3;
                printf("2,4  ");}
            else if (dir==3) 
                // goLeft();
                {strightpath[c] = 2;
                printf("3,4  ");}
            else 
                // goFront();
                {strightpath[c] = 1;
                printf("4,4  ");}
            dir=4;
        }
        else if (pathsStack[idxpath][1]==b+1) {
            
            if (dir==1) 
                // goRight();
                {strightpath[c] = 3;
                printf("1,2  ");}
            else if (dir==2)
                // goFront();
                {strightpath[c] = 1;
                printf("2,2  ");}
            else if (dir==3)
                // goBack();
                {strightpath[c] = 4;
                printf("3,2  ");}
            else 
                // goLeft();
                {strightpath[c] = 2;
                printf("4,2  ");}
          dir=2;
        }
        else{
            if (dir==1) 
                // goRight();
                {strightpath[c] = 2;
                printf("1,3  ");}
            else if (dir==2) 
                // goBack();
               { strightpath[c] = 4;
               printf("2,3  ");}
            else if (dir==3)
                // goFront();
                {strightpath[c] = 1;
                printf("3,3  ");}
            else 
                // goLeft();
                {strightpath[c] = 3;
                printf("4,3  ");}
            dir=3;
        }
        a=pathsStack[idxpath][0];
        b=pathsStack[idxpath][1];
        idxpath--;
        c = c + 1;
    }
    for (int i=0; i<c; i++)
    {
        if (strightpath[i] == 1)
        {
            int m = 0;
            int k=i;
            for (k; k<c; k++)
            {
                if (strightpath[k] ==1) 
                {
                m++;}
                else
                break;
            }
            drive_goto(123*m,123*m);
            i = k-1;
        }
        else if (strightpath[i] == 2)
        {
            drive_goto(-26,25);
            int m = 1;
            int k=i+1;
            for (k; k<c; k++)
            {
                if (strightpath[k] ==1) 
                {
                m++;}
                else
                break;
            }
            drive_goto(123*m,123*m);
            i = k-1;
            

        }
        else if (strightpath[i] == 3)
        {
            drive_goto(26,-25);
            int m = 1;
            int k=i+1;
            for (k; k<c; k++)
            {
                if (strightpath[k] ==1) 
                {
                m++;}
                else
                break;
            }
            drive_goto(123*m,123*m);
            i = k-1;
            
        }
        else
        goBack();
    }
  }           

