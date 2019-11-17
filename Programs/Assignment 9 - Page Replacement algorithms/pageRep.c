#include<stdio.h>

//Global declarations
int n,nf;  		// storing total length & number of frames
int in[25];	// variable storing the input 
int p[10];		// variable storing the page frames output
int pgHitCnt, pgFaultCnt;  // variable storing the count of pageFault and pageHit
int i,j,k;	
int near[10], least[10];  //variable storing near future/past references for all pages in the frames

//Function prototypes
void getData();
void initialize();
int isHit(int);
void dispPages();
void dispPgFaultsHits();
void fifo();
void optimal();
void findNearFuturePage();
void replacePageOptimal();
void lru();
void findLeastPastPage();
void replacePageLRU();

//Function definitions
void getData()
{
	printf("\nEnter length of page reference sequence:");
	scanf("%d",&n);

	printf("\nEnter the page reference sequence:");
	for(i=0; i<n; i++)
		scanf("%d",&in[i]);

	printf("\nEnter no of frames:");
	scanf("%d",&nf);
}//end getData()
 
void initialize()
{
	pgFaultCnt = 0;
	pgHitCnt = 0;
	for(i=0; i<nf; i++)
		p[i]=9999;
}//end initialize
 
int isHit(int data)
{
	int flag=0;
	for(j=0; j<nf; j++)
	{
		if(p[j]==data)
		{
			pgHitCnt++;
			flag=1;
			break;
		}//end if
	}//end for

	return flag;
}//end  isHit
 
void dispPages()
{
	for (k=0; k<nf; k++)
		if(p[k] != 9999)
			printf(" %d",p[k]);
}//end dispPages
 
void dispPgFaultsHits()
{
	printf("\n\nTotal no of page faults:%d",pgFaultCnt);
	printf("\nTotal no of page hits:%d\n",pgHitCnt);
}//end dispPgFaultsHits
 
void fifo()
{
	initialize();
	printf("\n FIFO:");
	for(i=0; i<n; i++) { //for entire input reference string
		printf("\nFor %d :",in[i]);

		if(isHit(in[i])==0) //check for a Page hit
		{
			for(k=0; k<nf-1; k++) { p[k]=p[k+1]; } //shift the indices 

			p[k]=in[i];
			pgFaultCnt++;
			dispPages();
		}
		else
			printf("No page fault");
	}//for (i=0)
	dispPgFaultsHits();
}//end fifo
 
 
void optimal()
{	
	initialize();
	printf("\nOptimal:");
	for(i=0; i<n; i++) //Process all the pages
	{ 
		printf("\nFor %d :",in[i]);
 		if(isHit(in[i])==0) // No Hit
       	{ 
       		findNearFuturePage();
			replacePageOptimal();            
              }
		else //else part of isHit?
	            printf("No page fault");
	}//end for
    dispPgFaultsHits();
}//end optimal

void lru()
{
	initialize();
	printf("\nLRU:");
	for(i=0; i<n; i++) //Process all the pages
	{
		printf("\nFor %d :",in[i]);

		if(isHit(in[i])==0) // No Hit
		{
			findLeastPastPage();
			replacePageLRU();		
		}
		else
			printf("No page fault!");
	}//end for
	dispPgFaultsHits();
}//end lru
 
void findNearFuturePage()
{
	for(j=0; j<nf; j++)
	{
		int pg=p[j];  //store current page in variable pg
		int found=0;
		for(k=i; k<n; k++)//search current page in near future
		{
			if(pg==in[k])
			{
				near[j]=k;
				found=1;
				break;
			}
			else
			found=0;
		}//end for(k=i)
		if(!found)
			near[j]=9999;
	}//outer for(j=0)
}//end findNearFuturePage()

void replacePageOptimal()
{
	int max=-9999;	// variable storing the index of later use and replacement
	int repindex;		// variable storing replacement index

	for(j=0; j<nf; j++)
	{
		if(near[j]>max) //Select the page which will be referred later for replacement
		{
			max=near[j];
			repindex=j;
		}
	}
	p[repindex]=in[i];
	pgFaultCnt++;

	dispPages();
}//end replacePageOptimal

void findLeastPastPage()
{
	for(j=0; j<nf; j++)
	{
		int pg=p[j];
		int found=0;
		for(k=i-1; k>=0; k--){
			if(pg==in[k])	{
				least[j]=k;
				found=1;
				break;
			}
			else
				found=0;
		}//end inner for
		if(!found)
			least[j]=-9999;
	}//end outer for
	
}//end findLeastPastPage

void replacePageLRU(){
	int min=9999;
	int repindex;
	for(j=0; j<nf; j++)
	{
		if(least[j]<min)
		{
			min=least[j];
			repindex=j;
		}//end if
	}//end for
	p[repindex]=in[i];
	pgFaultCnt++;
	dispPages();
}//end replacePageLRU

int main()
{
	int choice;
	while(1)
	{
		printf("\nPage Replacement Algorithms\n1.Enter data\n2.FIFO\n3.Optimal\n4.LRU\n5.Exit\nEnter your choice:");
		scanf("%d",&choice);
		switch(choice)
		{
			case 1: 
				getData();
				break;
			case 2:
				fifo();
				break;
			case 3:
				optimal();
				break;
			case 4:
				lru();
				break;
			default:
				return 0;
		}//end switch
	}//end while
}//end program

