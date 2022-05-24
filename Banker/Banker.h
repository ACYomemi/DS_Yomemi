#include <stdio.h>
#include <stdlib.h>
//bool, true and false is NOT pre-defined in standrad C, so I have to define them by hand.
#define bool int
#define true 1
#define false 0
#define LENGTH 100
int n,m;
int max[LENGTH][LENGTH];
int allocation[LENGTH][LENGTH];
int need[LENGTH][LENGTH];
int available[LENGTH];
int tmpAvailable[LENGTH];
bool finish[LENGTH]={false};
int safe[LENGTH]={-1};
int finishCount=0;
bool isGoodCompare(int needy[],int avaliablity[])
{
        int i;
        for (i=0;i<m;i++)
        {
                if (needy[i]>avaliablity[i])
                        return false;
        }
        return true;
}
bool isSafeAllocation(void)
{
        int i;
        for (i=0;i<m;i++)
                tmpAvailable[i]=available[i];
        for (i=0;i<n;i++)
        {
                if (finish[i])
                        continue;
                if (isGoodCompare(need[i],tmpAvailable))
                {
                        finish[i]=true;
                        int j;
                        for (j=0;j<m;j++)
                                tmpAvailable[j]+=allocation[i][j];
                        safe[finishCount]=i;
                        finishCount++;
                        i=-1;
                }
        }
        return (finishCount==n);
}

