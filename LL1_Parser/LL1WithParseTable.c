#include <stdio.h>
#include <ctype.h>
#include <string.h>

//================================================================================================================================

char NON_TERMINALS[10],TERMINALS[10];
int nt_count=0,t_count=0,rules=0;
char production[10][10];
char FirstPos[10][10],FollowPos[10][10],ParseTable[10][11];
int prod_num = -1;

//================================================================================================================================

int push_hash_to_last(char *arr){
	for (int i = 0; arr[i] != '\0'; ++i)
	{
		if (arr[i]=='#' && arr[i+1] != '\0')
		{
			int j;
			for (j = i; arr[j+1] != '\0'; ++j)
			{
				arr[j]=arr[j+1];
			}
			arr[j]='#';
			return 1;
		}
	}

	return 0;
}

//================================================================================================================================

int not_in_arr(char* arr1,char* arr2){
	for (int i = 0; arr1[i]!='\0'; ++i)
	{
		for(int j=0; arr2[j]!='\0'; ++j){
			if (arr1[i]==arr2[j])
			{
				return 0;
			}
		}
	}
	return 1;
}

//================================================================================================================================

int remove_duplicate(int i){
	for (int j = 1; FirstPos[i][j] != '\0'; ++j)
	{
		char temp = FirstPos[i][j];
		for (int k = j+1; FirstPos[i][k] != '\0'; ++k)
		{
			if(FirstPos[i][k]==FirstPos[i][j]){
				for (int l = k; FirstPos[i][l] != '\0'; ++l)
				{
					FirstPos[i][l] = FirstPos[i][l+1];
				}
			}
		}
	}
	return 0;
}

//================================================================================================================================

int not_in(char temp,char* arr){
	int i;
	for(i=0;arr[i]!='\0';++i){
		if (temp==arr[i])
		{
			return 0;	//return position of similarity
		}
	}
	return 1;
}

//================================================================================================================================

int initialise(char* arr, int size){
	int i;
	for (i = 0; i < size; ++i)
	{
		arr[i]='\0';
	}
	
	if(i==size)
		return 1;
	return 0;
}

//================================================================================================================================

int FindFollow(int i){
	
	if(i==0)
		FollowPos[i][0] = '$';

	for (int j = 0; j < rules; ++j)
	{
		for (int k = 2; production[j][k] != '\0'; ++k)
		{
			if(production[j][k] == NON_TERMINALS[i])
			{
				++k;
				if(production[j][k]=='\0')	//if last in RHS, FollowPos = FirstPos of LHS
				{
					int m;
					for (m = 0; i < nt_count; ++m)
					{
						if(NON_TERMINALS[m]==production[j][0])
							break;
					}
					

					if(NON_TERMINALS[m] != NON_TERMINALS[i])
					{
						
						if(strlen(FollowPos[m])==0)
							FindFollow(m);
						
						for (int v = 0; FollowPos[m][v] != '\0'; ++v)
						{
							if(not_in(FollowPos[m][v],FollowPos[i]))
							{
								int len = strlen(FollowPos[i]);
								FollowPos[i][len] = FollowPos[m][v];
							}
						}
					}
				}

				else if(!not_in(production[j][k],NON_TERMINALS))	// FollowPos = FirstPos of rest;
				{
					int cont=1;
					while(!not_in(production[j][k],NON_TERMINALS) && cont == 1)
					{
						cont=0;
						int m;
						for (m = 0; i < nt_count; ++m)
						{
							if(NON_TERMINALS[m]==production[j][k])
								break;
						}
						for (int v = 0; FirstPos[m][v] != '\0'; ++v)
						{
							if(not_in(FirstPos[m][v],FollowPos[i]))
							{
								int len = strlen(FollowPos[i]);
								FollowPos[i][len] = FirstPos[m][v];
							}
						}
						
						if(!not_in('#',FollowPos[i]))
						{
							cont=1;
							int len = strlen(FollowPos[i]);
							FollowPos[i][--len] = '\0';
							if(!not_in(production[j][k+1],TERMINALS))
							{
								if(not_in(production[j][k+1],FollowPos[i]))
								FollowPos[i][len] = production[j][k+1];
							}

							else if(production[j][k+1]=='\0')
							{
								FollowPos[i][len] = production[j][k+1];
								int m;
								for (m = 0; m < nt_count; ++m)
								{
									if(NON_TERMINALS[m]==production[j][0])
										break;
								}

								if(NON_TERMINALS[m] != NON_TERMINALS[i])
								{
									if(strlen(FollowPos[m])==0)
										FindFollow(m);
									
									for (int v = 0; FollowPos[m][v] != '\0'; ++v)
									{
										if(not_in(FollowPos[m][v],FollowPos[i]))
										{
											int ln = strlen(FollowPos[i]);
											FollowPos[i][ln] = FollowPos[m][v];
										}
									}
								}

							}	
						}
						++k;
					}
				}

				else if(!not_in(production[j][k],TERMINALS))
				{
					if(not_in(production[j][k],FollowPos[i]))
					{
						int len = strlen(FollowPos[i]);
						FollowPos[i][len] = production[j][k];
					}
				}
			}
		}
	}

	return 0;
}

//================================================================================================================================

int FindFirst(int i, int level, char search)
{
		char nt=NON_TERMINALS[i];
		int terms=0;

		for (int j = 0; j < rules; ++j)
		{
			int index=2;
			if (nt!=production[j][0])
				continue;

			if(!not_in(production[j][2],TERMINALS))	// if term is in TERMINALS; add to FirstPos...
			{
				if(not_in(production[j][2],FirstPos[i]))
				{
					FirstPos[i][terms]=production[j][2];
					terms++;
				}

				if(!not_in(search,FirstPos[i]))
				{
					for (int p = 0; p < strlen(FirstPos[i]); ++p)
					{
						if (FirstPos[i][p] == search)
						{
							prod_num = j;
						}
					}
				}

			}
		
			else if (!not_in(production[j][2],NON_TERMINALS))	// if term is in NON_TERMINALS; jump to productions of those NON_TERMINALS
			{
				int m;
				for (m = 0; m < nt_count; ++m)
				{
					if(NON_TERMINALS[m]==production[j][2])
						break;
				}
				if (strlen(FirstPos[m])==0 || level ==0)
				{
					terms+=FindFirst(m,1,search);
					strcat(FirstPos[i],FirstPos[m]);
				}

				for (int v = 0; FirstPos[m][v] != '\0'; ++v)
				{
					if(not_in(FirstPos[m][v],FirstPos[i]))
					{
						int ln = strlen(FirstPos[i]);
						FirstPos[i][ln] = FirstPos[m][v];
					}
					terms=strlen(FirstPos[i]);
				}
				
				while(FirstPos[i][terms-1]=='#' && production[j][index+1]!='\0')
				{	
					FirstPos[i][--terms]='\0';

					if(!not_in(production[j][index+1],TERMINALS))
					{
						if(not_in(production[j][index+1],FirstPos[i]))
							FirstPos[i][terms++]=production[j][index+1];

					}

					else if(!not_in(production[j][index+1],NON_TERMINALS))
					{
						index++;
						int m;
						for (m = 0; m < nt_count; ++m)
						{
							if(NON_TERMINALS[m]==production[j][index])
								break;
						}
						
						if (strlen(FirstPos[m])==0)
						{
							terms+=FindFirst(m,1,search);
							strcat(FirstPos[i],FirstPos[m]);
						}
						
						for (int v = 0; FirstPos[m][v] != '\0'; ++v)
						{
							if(not_in(FirstPos[m][v],FirstPos[i]))
							{
								int ln = strlen(FirstPos[i]);
								FirstPos[i][ln] = FirstPos[m][v];
							}
						}
					}
				}

				if(!not_in(search,FirstPos[i]))
				{
					for (int p = 0; p < strlen(FirstPos[i]); ++p)
					{
						if (FirstPos[i][p] == search)
						{
							prod_num = j;
						}
					}
				}

			}
		
			else if (production[j][2]=='#')
			{
				if (not_in(production[j][2],FirstPos[i]))
				{
					FirstPos[i][terms]=production[j][2];
					terms++;
				}

				if(!not_in(search,FirstPos[i]))
				{
					for (int p = 0; p < strlen(FirstPos[i]); ++p)
					{
						if (FirstPos[i][p] == search)
						{
							prod_num = j;
						}
					}
				}

			}
		}
	push_hash_to_last(FirstPos[i]);
	return terms;
}

//================================================================================================================================

int main(void)
{
/*
	A LIST OF TERMINALS AND NON_TERMINALS.
	Both strings end with a '\0'.
*/
	initialise(NON_TERMINALS,10);
	initialise(TERMINALS,10);

	scanf("%d %d %d",&nt_count,&t_count,&rules);

	for (int i = 0; i < rules; ++i)
	{
		scanf("%s",production[i]);
	}
	printf("\n");
	int n=0,t=0;

	for (int i = 0; i < rules; ++i)
	{
		for (int j = 0; production[i][j] != '\0'; ++j)
		{
			if(j==0)
			{
				if(not_in(production[i][j],NON_TERMINALS)){
					NON_TERMINALS[n]=production[i][j];
					n++;
				}
			}

			else if(!isupper(production[i][j]) && production[i][j]!='#' && production[i][j]!=':')
			{
				if(not_in(production[i][j],TERMINALS)){
					TERMINALS[t]=production[i][j];
					t++;
				}
			}
		}
	}

	printf("\nnt_count: %d, NON_TERMINALS: ", nt_count); puts(NON_TERMINALS);
	printf("t_count: %d, TERMINALS: ", t_count); puts(TERMINALS);
	
/*
	CALCULATING THE FirstPos OF THE DIFFERENT NON_TERMINALS
*/

	for (int i = 0; i < nt_count; ++i)
		initialise(FirstPos[i],t_count);

	for (int i = 0; NON_TERMINALS[i] !='\0' ; ++i){
		if(strlen(FirstPos[i])==0)
		{
			FindFirst(i,0,':');
			remove_duplicate(i);
		}
	}

	printf("\nFirstPos:\n----------\n");
	

	for (int i = 0; i < nt_count; ++i)
	{
		printf("%c >> ", NON_TERMINALS[i]);
		puts(FirstPos[i]);
	}

/*
	CALCULATING LastPos OF THE DIFFERENT NON_TERMINALS
*/

	for (int i = 0; i < nt_count; ++i)
		initialise(FollowPos[i],t_count);

	for (int i = 0; i < nt_count; ++i){
		FindFollow(i);
	}

	printf("\nFollowPos:\n----------\n");
	

	for (int i = 0; i < nt_count; ++i)
	{
		printf("%c >> ", NON_TERMINALS[i]);
		puts(FollowPos[i]);
	}

/*
	CALCULATING ParseTable
*/

	for (int i = 0; i < nt_count; ++i)
	{
		initialise(ParseTable[i],t_count+1);
	}

	for (int i = 0; i < nt_count; ++i)
	{
		for (int j = 0; j < t_count; ++j)
		{
			if (not_in(TERMINALS[j],FirstPos[i]))
			{
				ParseTable[i][j] = '*';
			}
			else
			{
				//Find which production gives the corresponding 'FirstPos'
				FindFirst(i,0,TERMINALS[j]);
				ParseTable[i][j] = (prod_num+48);
			}
		}
	}

	printf("\nParseTable:\n----------\n    ");
	for (int i = 0; i < t_count; ++i)
	{
		printf("%c ", TERMINALS[i]);
	}
	printf("\n----------\n");

//=====================================================================

	char first_prod[rules][t_count];
	for(int ap=0; ap < rules;ap++){
		int destiny = 0;
		int k = 2;
		int ct = 0;
		char tem[100];
		while(production[ap][k] != '\0'){
			if(!isupper(production[ap][k])){
				tem[ct++] = production[ap][k];
				tem[ct++] = '_';
				tem[ct++] = '\0';
				k++;
				break;
			}
			else{
				int zap=0;
				int tuna = 0;
				for(zap=0;zap<rules;zap++){
					if(NON_TERMINALS[zap] == production[ap][k]){
						for(tuna=0;tuna<100;tuna++){
							if(FirstPos[zap][tuna] != '\0'){
								tem[ct++] = FirstPos[zap][tuna];
							}
							else
								break;
						}
					break;
					}
				}
				tem[ct++] = '_';
			}
			k++;
		}
		int zap = 0,tuna;		
		for(tuna = 0;tuna<ct;tuna++){
			if(tem[tuna] == '#'){
				zap = 1;
			}
			else if(tem[tuna] == '_'){
				if(zap == 1){
					zap = 0;
				}
				else
					break;
			}
			else{
				first_prod[ap][destiny++] = tem[tuna];
			}
		}
	}
	for(int ap = 0;ap < rules; ap++){
		printf("%s\n",first_prod[ap]);
	}
	char table[nt_count][t_count+1];
	int ptr = -1;
	for(int ap = 0; ap < nt_count ; ap++){
		for(int kay = 0; kay < (t_count + 1) ; kay++){
			table[ap][kay] = '!';
		}
	}
	for(int ap = 0; ap < rules ; ap++){
		int ck = production[ap][0];
		int xxx = 0;
		for(int kay = 0; kay <= ptr; kay++)
			if(ck == table[kay][0])
				xxx = 1;
		if (xxx == 1)
			continue;
		else{
			ptr = ptr + 1;
			table[ptr][0] = ck;
		}
	}
	for(int ap = 0; ap < rules ; ap++){
		int tuna = 0;
		while(first_prod[ap][tuna] != '\0'){
			int to,ni=0;
			for(to=0;to<t_count;to++){
				if(first_prod[ap][tuna] == TERMINALS[to]){
					ni = 1;
				}
			}
			if(ni == 1){
				char xz = production[ap][0];
				int cz=0;
				while(table[cz][0] != xz){
					cz = cz + 1;
				}
				int vz=0;
				while(TERMINALS[vz] != first_prod[ap][tuna]){
					vz = vz + 1;
				}
				table[cz][vz+1] = (char)(ap + 65);
			}
			tuna++;
		}
	}
	for(int k=0;k<t_count;k++){
		for(int kay=0;kay<100;kay++){
			if(FirstPos[k][kay] == '\0'){
				break;
			}
			else if(FirstPos[k][kay] == '#'){
				int fz = 0;
				while(FollowPos[k][fz] != '\0'){
					char xz = production[k][0];
					int cz=0;
					while(table[cz][0] != xz){
						cz = cz + 1;
					}
					int vz=0;
					while(TERMINALS[vz] != FollowPos[k][fz]){
						vz = vz + 1;
					}
					table[k][vz+1] = '#';
					fz++;	
				}
				break;
			}
		}
	}
	for(int ap = 0; ap < nt_count ; ap++){
		printf("\t\t\t   %c\t|\t",table[ap][0]);
		for(int kay = 1; kay < (t_count + 1) ; kay++){
			if(table[ap][kay] == '!')
				printf("\t\t");
			else if(table[ap][kay] == '#')
				printf("%c=#\t\t",table[ap][0]);
			else{
				int mum = (int)(table[ap][kay]);
				mum -= 65;
				printf("%s\t\t",production[mum]);
			}
		}
		printf("\n");
		printf("\t\t\t---------------------------------------------------------------------------------------------------------------------");
		printf("\n");
	}



	return 0;
}

//================================================================================================================================

/*

4 3 8
S:BD
S:d
B:CDaB
B:#
C:cC
C:#
D:a
D:#

FirstPos:
----------
S >> cad
B >> ca#
C >> c#
D >> a#

FollowPos:
----------
S >> $
B >> a$
C >> a
D >> $a


3 3 5
S:BC
S:d
B:aB
B:#
C:cC

FirstPos:
----------
S >> acd
B >> a#
C >> c

FollowPos:
----------
S >> $
B >> c
C >> $


3 4 5
S:aABb
A:c
A:#
B:d
B:#

FirstPos:
----------
S >> a
A >> c#
B >> d#

FollowPos:
----------
S >> $
A >> db
B >> b


6 6 9
S:aBDh
B:cC
C:bC
C:#
D:EF
E:g
E:#
F:f
F:#

FirstPos:
----------
S >> a
B >> c
C >> b#
D >> gf#
E >> g#
F >> f#

FollowPos:
----------
S >> $
B >> gfh
C >> gfh
D >> h
E >> fh
F >> h


6 5 10
S:ABCDE
A:a
A:#
B:b
B:#
C:c
D:d
D:#
E:e
E:#

FirstPos:
----------
S >> abc
A >> a#
B >> b#
C >> c
D >> d#
E >> e#

FollowPos:
----------
S >> $
A >> bc
B >> c
C >> de$
D >> e$
E >> $

3 4 6
S:Bb
S:Cd
B:aB
B:#
C:cC
C:#

FirstPos:
----------
S >> abcd
B >> a#
C >> c#

FollowPos:
----------
S >> $
B >> b
C >> d


5 5 8
E:TY
Y:+TY
Y:#
T:FZ
Z:*FZ
Z:#
F:i
F:(E)

FirstPos:
----------
E >> i(
Y >> +#
T >> i(
Z >> *#
F >> i(

FollowPos:
----------
E >> $)
Y >> $)
T >> +$)
Z >> +$)
F >> *+$)


5 5 9
E:TY
Y:+TY
Y:#
T:FZ
Z:*FZ
Z:#
F:i
F:(E)
F:#

FirstPos:
----------
E >> i(*+#
Y >> +#
T >> i(*#
Z >> *#
F >> i(#

FollowPos:
----------
E >> $)
Y >> $)
T >> +$)
Z >> +$)
F >> *+$)


4 5 9
S:ACB
S:CbB
S:Ba
A:da
A:BC
B:g
B:#
C:h
C:#

FirstPos:
----------
S >> dghba#
A >> dgh#
B >> g#
C >> h#

FollowPos:
----------
S >> $
A >> hg$
B >> $ahg
C >> g$bh


9 5 14
E:TY
Y:+TY
Y:#
T:FZ
Z:*FZ
Z:#
F:ACD
F:B
F:#
A:i
A:#
B:(E)
C:#
D:#

FirstPos:
----------
E >> i(*+#
Y >> +#
T >> i(*#
Z >> *#
F >> i(#
A >> i#
B >> (
C >> #
D >> #

FollowPos:
----------
E >> $)
Y >> $)
T >> +$)
Z >> +$)
F >> *+$)
A >> *+$)
B >> *+$)
C >> *+$)
D >> *+$)

*/
