#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int a[12];
void print1(){
	for(int q=0;q<11;q++)printf("%d ",a[q]);
	printf("%d\n",a[11]);
}
void shell(int n){
	for(int i=0;i<n;i++)
		for(int j=i+n;j<12;j+=n)
		{int p=a[j],k;
		for(k=j-n;k>=0;k-=n)
		{
		if(a[k]>p)a[k+n]=a[k];else
		{
		a[k+n]=p;
		break;
		}
		}
		if(k==i-n)a[k+n]=p;
		}
}
int main(){
	for(int i=0;i<12;i++)scanf("%d",a+i);
		shell(3);
		print1();
		shell(2);
		print1();
		shell(1);
		print1();
}
