#include "abdrive.h"                          // Include abdrive header
#include "ping.h"                             // Include ping header
#include "mylib.h"        
int main()                                    // main function
{
   drive_goto(30,30);
   visit[0][0]=1;
   depthfirst(0,0,1);
    for (int i=0;i<=4;i++)
        for (int j=0;j<=4;j++)
           distFromStart[i][j]=100;
    distFromStart[0][0]=0;
    queue[0][0]=0; 
    queue[0][1]=0;
        
   buildDistance();
   int idxpath = buildPath();
   goEnd(idxpath);
}


