#include "Banker.h"
int main(void)
{
	scanf("%d",&n);
	scanf("%d",&m);
	int i=0,j=0;
	for (i=0;i<n;i++)
	{
		for (j=0;j<m;j++)
			scanf("%d",&max[i][j]);
	}
	for (i=0;i<n;i++)
        {
                for (j=0;j<m;j++)
                        scanf("%d",&allocation[i][j]);
        }
	for (i=0;i<n;i++)
        {
                for (j=0;j<m;j++)
                        need[i][j]=max[i][j]-allocation[i][j];
        }
	for (i=0;i<m;i++)
		scanf("%d",&available[i]);
	printf("need[][] as follow:\n");
	for (i=0;i<n;i++)
	{
		for (j=0;j<m;j++)
			printf("%d ",need[i][j]);
		printf("\n");
	}
	if (isSafeAllocation())
	{
		printf("Allocation given is safe. One possiblity as follow:\n");
		for (i=0;i<n;i++)
			printf("%d ",safe[i]);
		printf("\n");
	}
	else
	{
		printf("Allocation given is unsafe. Max allocation count=%d\n",finishCount);
	}
	return 0;
}
