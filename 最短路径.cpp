#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int a[5][5],visited[5],dist[5];
int main(){
	for(int i=0;i<5;i++)
	{
	for(int j=0;j<5;j++)
	scanf("%d",&a[i][j]);
	visited[i]=0;
	dist[i]=1000;
	}
	int m=0;
	dist[m]=0;
	for(int p=0;p<5;p++){
		visited[m]=1;
		for(int i=0;i<5;i++)if(visited[i]==0&&a[m][i]+dist[m]<dist[i]&&a[m][i]!=-1)dist[i]=a[m][i]+dist[m];
		int min=1000;
		for(int i=0;i<5;i++)if(visited[i]==0&&dist[i]<min){
			min=dist[i];
			m=i;
		}
	}
	printf("%d %d %d %d %d",dist[0],dist[1],dist[2],dist[3],dist[4]);
	return 0;
}
