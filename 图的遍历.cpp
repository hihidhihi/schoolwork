#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define SIZE 9
int a[9][9],visited1[9],visited2[9];
typedef struct Queue{
	int a[SIZE];
	int f,r;
}Queue;
Queue q;
void init1(Queue*q){
	q->f=0;
	q->r=0;
}
void in(Queue*q,int a){
	visited2[a]=1;
	if((q->r+1)%SIZE!=q->f){
	q->a[q->r]=a;
	q->r=(q->r+1)%SIZE;
	}
	else {
	printf("¶ÓÁÐÒÑÂú");
	exit(0);
	}
}
int out(Queue*q){
	int m=q->a[q->f];
	q->f=(q->f+1)%SIZE;
	return m;
}
void dfs(int n){
	printf("%c",'A'+n);
	visited1[n]=1;
	for(int i=0;i<9;i++)if(visited1[i]==0&&a[n][i]==1)dfs(i);
}
void wfs(int n){
	in(&q,n);
	while(1){
		int m=out(&q);
		printf("%c",'A'+m);
		for(int i=0;i<9;i++)if(a[m][i]==1&&visited2[i]==0)in(&q,i);
		if(q.f==q.r)return;
	}
}
int main(){
	init1(&q);
	for(int i=0;i<9;i++)
	{
	for(int j=0;j<9;j++)
	scanf("%d",&a[i][j]);
	visited1[i]=0;
	visited2[i]=0;
	}
	for(int i=0;i<9;i++)if(visited1[i]==0)dfs(i);
	printf("\n");
	for(int i=0;i<9;i++)if(visited2[i]==0)wfs(i);
	return 0;
}
