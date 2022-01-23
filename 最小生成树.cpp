#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int a[6][6],v[6],e[6][6];
int main(){
	int w=0;
	for(int i=0;i<6;i++)
	{
	for(int j=0;j<6;j++)
	{
	scanf("%d",&a[i][j]);
	e[i][j]=0;
	}
	v[i]=0;
	}
	v[0]=1;
	for(int i=0;i<5;i++)
	{
	int x,y,min=100;
	for(int j=0;j<6;j++)
	if(v[j]==1)
	for(int k=0;k<6;k++)
	if(v[k]==0&&a[j][k]!=-1&&a[j][k]<min&&e[j][k]==0)
	{
	min=a[j][k];
	x=j;y=k;
	}
	w+=min;
	e[x][y]=1;
	v[y]=1;
	}
	printf("%d",w);
	return 0;
}
