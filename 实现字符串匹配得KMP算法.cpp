#include<stdio.h>
#include<string.h>
#include<stdlib.h>
/*void getNext(char*ps,int*next) {
    next[0]=-1;
    int j=0;
    int k=-1;
    while(j<strlen(ps)-1){
       if(k==-1||ps[j]==ps[k]){
           if(ps[++j]==ps[++k]){
              next[j]=next[k];
           }
		   else{
              next[j]=k;
           }
       }else k=next[k];
       
    }
} 
*/
void getNext(int *next,char* ps){ 
     next[0]=-1;
     int j=0;
     int k=-1;
     while(j<strlen(ps)-1){
        if(k==-1||ps[j]==ps[k]){
            next[++j]=++k;
        }else{
            k=next[k];
        }
     }
 }
int main(){
	char str[]="abacababdeadcde";
	char ps[10];
	gets(ps);
	const int kk=strlen(ps);
	int next[kk];
	getNext(next,ps);
	int i=0,j=0;
	while(i<15&&j<kk)
		{
			if(str[i]==ps[j]||j==-1)
			{
			i++;j++;
			}
			else j=next[j];
		}
	if(j==strlen(ps))printf("匹配成功\n"); 
	else printf("匹配未成功\n");
	for(int m=0;m<strlen(ps);m++)printf("%d",next[m]);
	return 0;
}
