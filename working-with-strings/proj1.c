#include <stdio.h>
#include <string.h>
#include <stdbool.h>







//funkce pro porovnani cisel		
int porovnani_kontaktu(char *argument, char *comparison)			
{
	int posun=1;
	int i=0;
	int j=0;
	bool odcykleni = false;
	for(; comparison[j] != '\0'; j++, odcykleni = false)	
	{
				
		if(argument[i] == comparison[j])
		{
			j++;
			i++;			
			for(;argument[i] != '\0'; j++, i++, posun++)
			{
				if(argument[i] != comparison[j])
				{
					i -= posun;
					j -= posun;
					posun = 1;
					odcykleni = true;
					break;		
				}	
			}
			if(!odcykleni)		
				return 1;		
		}
	}
	return 0;
}
//funcke na prevod z pismen do cisel
int latter_to_num(char *argument, char *jmeno)
{
	int handover_for_cmp;
	char cis_jmeno[102];
	int k=0;
	for(;jmeno[k]!= '\0'; k++)
	{
		if((jmeno[k] >= 'A' && jmeno[k] <= 'C') || (jmeno[k] >= 'a' && jmeno[k] <= 'c'))
			cis_jmeno[k] = '2';	
		
		else if((jmeno[k] >= 'D' && jmeno[k] <= 'F') || (jmeno[k] >= 'd' && jmeno[k] <= 'f'))
			cis_jmeno[k] = '3';		
		
		else if((jmeno[k] >= 'G' && jmeno[k] <= 'I') || (jmeno[k] >= 'g' && jmeno[k] <= 'i'))
			cis_jmeno[k] = '4';
		
		else if((jmeno[k] >= 'J' && jmeno[k] <= 'L') || (jmeno[k] >= 'j' && jmeno[k] <= 'l'))
			cis_jmeno[k] = '5';	
		
		else if((jmeno[k] >= 'M' && jmeno[k] <= 'O') || (jmeno[k] >= 'm' && jmeno[k] <= 'o'))
			cis_jmeno[k] = '6';
		
		else if((jmeno[k] >= 'P' && jmeno[k] <= 'S') || (jmeno[k] >= 'p' && jmeno[k] <= 's'))
			cis_jmeno[k] = '7';	
			
		else if((jmeno[k] >= 'T' && jmeno[k] <= 'V') || (jmeno[k] >= 't' && jmeno[k] <= 'v'))
			cis_jmeno[k] = '8';		
		
		else if((jmeno[k] >= 'W' && jmeno[k] <= 'Z') || (jmeno[k] >= 'w' && jmeno[k] <= 'z'))
			cis_jmeno[k] = '9';
		
		else if(jmeno[k] == '+')
			cis_jmeno[k] = '0';
		
		else if(jmeno[k] == '\n')
		{
			cis_jmeno[k] = '\0';
			break;
		}	
		
		else
			cis_jmeno[k]= ' ';
		
	}
	//predavani ciselneho stringu na porovnani	
	handover_for_cmp = porovnani_kontaktu(argument, cis_jmeno);

	if(handover_for_cmp == 1)
		return 1;
	else
		return 0;
	
}
void del_new_line(char *kontakt)
{
	char *change_char;

	change_char = strchr(kontakt, '\n');	
	*change_char = ',';	
}




void vypsani_kontaktu(char (*kontakt)[102], int load_contacts)
{

	for(int i=0; i<load_contacts; i += 2)
	{
		
		del_new_line(kontakt[i]);
		printf("%s %s", kontakt[i], kontakt[i+1]);
	}				
}


		

int main(int argc, char *argv[])
{	
	char kontakt[100][102];
	int cmp_kontakt;
	int cmp_jmeno;
	bool neuspech = true;

	//kontrola argumentu
	if(argc > 2)
		printf("Spatny vstup\n");
	else
	{	
		//nacteni vsech kontaktu ze souboru
		int load_contacts = 0;
		while (fgets(kontakt[load_contacts], 102, stdin) != NULL && load_contacts < 99)
		{
			//kontrola jestli string neni delsi jak sto znaku
			if(strlen(kontakt[load_contacts]) == 101 && kontakt[load_contacts][101] != '\n')
			{
				printf("V souboru je nepodporovany kontakt!\n");
				return 1;
			}
			
			load_contacts++;			
		}	
		printf("%d\n", load_contacts);
		if(argc == 1)
		{	
				vypsani_kontaktu(kontakt, load_contacts);		
		}	
		else
		{	
			//kontrola vstupu pro znaky ruzne od cisel
			for(int i=0;argv[1][i] != '\0'; i++)
                	{
                        	if(argv[1][i] < '0' || argv[1][i] > '9')
                        	{
                        	        printf("Spatny vstup\n");
                        	        return 1;
                        	}
                	}
               			
					
			for(int k = 0;k<load_contacts;k += 2)
			{	
				//predavani jmena funkci na porovnani
				cmp_jmeno = latter_to_num(argv[1], kontakt[k]);
				//predavani tel. cisla funkci na porovnani
				cmp_kontakt = porovnani_kontaktu(argv[1], kontakt[k+1]);
				
				if(cmp_jmeno == 1 || cmp_kontakt == 1)
				{
					del_new_line(kontakt[k]);
					printf("%s %s", kontakt[k], kontakt[k+1]);
						neuspech = false;
				}
			
			
			}
			if(neuspech)		
				printf("Not found\n");
		}
	}
	return 0;					


}




















