#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAX 	10		// MAXIMUM SYMBOL TABLE ENTRIES

struct SYMBTAB
{
	char Symb[10];
	int Addr;	
}S[MAX];

struct LITTAB
{
	char Lit[5];
	int Addr;
}L[MAX];

int POOLTAB[MAX],SymbCnt,LitCnt,PoolCnt,TokCnt,LC,i,j,k;
char Tok1[MAX],Tok2[MAX],Tok3[MAX],Tok4[MAX],Tok5[MAX],Buffer[80],temp[MAX];
FILE *fp,*fp1,*fp2;  // fp1 for icfile, fp2 - TC

void DispSymbTab()
{
	int i;
	//LISTING SYMBOL TABLE
	printf("\n------------------------------------------");
	printf("\nSymb\tAddr");
	printf("\n------------------------------------------");
	for(i=0;i<SymbCnt;i++)
		printf("\n%s\t%d",S[i].Symb,S[i].Addr);
	printf("\n------------------------------------------");

}// END OF DispSymbTab()

void DispLitTab()
{
	int i;
	printf("\n-------------------------------");
	printf("\nLit#\tLit\tAddr");
	printf("\n-------------------------------");
	for(i=0;i<LitCnt;i++)
		printf("\n%02d\t%s\t%d",i,L[i].Lit,L[i].Addr);
	printf("\n-------------------------------");
}// END OF DispLitTab()

void DispPoolTab()
{
	int i;
	printf("\n--------------------");
	printf("\nPool#\tPool Base");
	printf("\n--------------------");
	for(i=0;i<PoolCnt;i++)
		printf("\n%02d\t%d",i,POOLTAB[i]);
	printf("\n--------------------");
}// END OF DispPoolTab()

void PrintFile(char *fn)
{
	fp=fopen(fn,"r");
	if( fp == NULL)
	{
		printf("\nCannot Open File %s",fn);
		exit(0);
	}
	while(fgets(Buffer,80,fp))
		printf("%s",Buffer);
	fclose(fp);
}// END OF PrintFile()

void PassTwo()
{
	int i,j,k;
	static int ind = 0;
	char temp[MAX];
	LC=0;
	fp1 = fopen("ic.txt","r");
	fp2 = fopen("tc.txt","w");
	
	if((fp1==NULL || fp2 == NULL))
	{
		printf("\nCannot Open File ");
		exit(0);
	}

	while(fgets(Buffer,80,fp1))
	{
		TokCnt = sscanf(Buffer,"%s %s %s %s %s",Tok1,Tok2,Tok3,Tok4,Tok5);
		Tok1[strlen(Tok1)-1]='\0';
		Tok2[strlen(Tok2)-1]='\0';
		Tok3[strlen(Tok3)-1]='\0';
		Tok4[strlen(Tok4)-1]='\0';
		Tok5[strlen(Tok5)-1]='\0';
		switch(TokCnt)
		{
		
			case 2:
			
			if((strcmp(Tok1+1,"AD")==0)&&(atoi(Tok2)==1 || atoi(Tok2)==4))	
			{ //ONLY for END AND LTORG
					for(i=POOLTAB[ind];i<POOLTAB[ind+1];i++)
					{
						strcpy(temp,L[i].Lit);
						temp[strlen(temp)-1]='\0';
						fprintf(fp2,"%d) + 00 0 %03d\n",
												 LC++,atoi(strstr(temp,"'")+1));
					}//inner for
					ind++;
				LC--;
			}
			
			else if(strcmp(Tok1+1,"IS")==0)// STOP
					fprintf(fp2,"%d) + 00 0 000\n",LC);
			
			break;
				
				
				
				
				
				

			case 4:

				if(strcmp(Tok1+1,"AD")==0 )	// START, ORIGIN
						LC = atoi(Tok4)-1;
				else if(strcmp(Tok1+1,"IS")==0)	// READ, PRINT
						fprintf(fp2,"%d) + %02d 0 %03d\n",LC,atoi(Tok2),S[atoi(Tok4)].Addr);
				else if(strcmp(Tok1+1,"DL")==0)
				{
					if(atoi(Tok2)==1) // DS
					{
						for(i=0;i<atoi(Tok4);i++)
							fprintf(fp2,"%d)\n",LC++);
						LC--;
					}
					else if(atoi(Tok2)==0) // DC
					{
						fprintf(fp2,"%d) + 00 0 %03d\n",LC,atoi(Tok4));
					}
				}
				break;

			case 5:
								
				if(Tok4[1]=='S')		// ADD-DIV with symbols
					fprintf(fp2,"%d) + %02d %d %03d\n",LC,atoi(Tok2),atoi(Tok3+1),S[atoi(Tok5)].Addr);
				else if(Tok4[1]=='L')	// ADD-DIV with literals
					fprintf(fp2,"%d) + %02d %d %03d\n",LC,atoi(Tok2),atoi(Tok3+1),L[atoi(Tok5)].Addr);
				break;
		}//switch case
		LC++;
	}
	fclose(fp1);
	fclose(fp2);
}

void createSymbTab()
{
    printf("\n Enter total number of Symbols :");
    scanf("%d", &SymbCnt);
    printf("\nEnter symbols and address\n");
    
    for( int i =0; i < SymbCnt; i++ )
        scanf("%s %d", S[i].Symb, &S[i].Addr);  
}

void createLitTab()
{
    printf("\n Enter total number of Literals :");
    scanf("%d", &LitCnt);
    printf("\nEnter Literals and address\n");
    
    for( int i =0; i < LitCnt; i++ )
        scanf("%s %d", L[i].Lit, &L[i].Addr);  
}

void createPoolTab()
{
    printf("\n Enter total number of Pools :");
    scanf("%d", &PoolCnt);
    printf("\nEnter Literal reference\n");
    
    for( int i =0; i < PoolCnt; i++ )
        scanf("%d", &POOLTAB[i]);  
}


void generate_dataStructures()
{
	createSymbTab();
	DispSymbTab();
    createLitTab();
    DispLitTab();
	createPoolTab();
    DispPoolTab();	
}

int main()
{
        generate_dataStructures();
		PassTwo();
		printf("\nTarget Code\n");
		PrintFile("tc.txt");
	
	return 0;
}
