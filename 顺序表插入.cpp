#include<stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct{
	char a[40];
	int l;}LIST;
void create1(LIST *q);
void insert1(LIST *q);
void print1(LIST *q);
int main(){
	LIST m;
	create1(&m);
	insert1(&m);
	print1(&m);
	return 0;
}
void create1(LIST *q){
		char str[100];
		gets(str);
		q->l=0;
		for(int i=0;;i++){
			char c=str[i];
			if((c<'0'||c>'9')&&c!=' ')break;
			if(c<'0'||c>'9')continue;
			q->a[q->l]=c;
			q->l++;
		}
	}
void insert1(LIST *q){
		char str[10];
		gets(str);
		int p;
		for(int i=strlen(str)-1;;i--)
		if(str[i]>='0'&&str[i]<='9'){
			p=str[i]-48;
			break;
		}
		for(int i=q->l;i>=p;i--)q->a[i]=q->a[i-1];
		q->a[p-1]='x';
		q->l++;
	}
	void print1(LIST *q){
		for(int i=0;;i++){
		printf("%c",q->a[i]);
		if(i<q->l-1)printf(" ");
		else break;
		}
	}
