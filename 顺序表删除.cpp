#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int main(){ 
	char a[8][5],str[40];
	int l=1;
	gets(str);
	int p=0,j=0,k=0;
	for(;p<strlen(str);p++){
		if(str[p]==' '){
			a[j][k]='\0';
			j++;
			k=0;
			l++;
			continue;
		}
		a[j][k]=str[p];
		k++;
	}
	if(strlen(str)==0)l=0;
	a[j][k]='\0';
	char e[]="x"; 
	for(int i=0;i<l;i++){ 
		if(strcmp(e,a[i])==0){ 
			for(int j=i;;j++){ 
				if(j==l-1){
				if(j==0)a[0][0]='\0';
				break;}
				else strcpy(a[j],a[j+1]);     
							 } 
			i--; l--; 
							 } 
						}
	for(int i=0;;i++){
	   printf("%s",a[i]); 
	   if(i<l-1)printf(" ");
	   else break;   }
	return 0;
		  }
