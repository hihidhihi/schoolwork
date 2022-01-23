#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int n;char a[20];
void qianxu(int i){
	if(i>=n||a[i]=='0')return;
	printf("%c",a[i]);
	qianxu(2*i+1);
	qianxu(2*i+2);
}
void zhongxu(int i){
	if(i>=n||a[i]=='0')return;
	zhongxu(2*i+1);
	printf("%c",a[i]);
	zhongxu(2*i+2);
}
void houxu(int i){
	if(i>=n||a[i]=='0')return;
	houxu(2*i+1);
	houxu(2*i+2);
	printf("%c",a[i]);
}
int main(){
	gets(a);
	n=strlen(a);
	qianxu(0);
	printf("\n");
	zhongxu(0);
	printf("\n");
	houxu(0);
	return 0;
}
