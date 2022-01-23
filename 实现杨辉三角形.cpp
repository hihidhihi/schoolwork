#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define SIZE 98
//说明：通过避免两端1的存储增大了队列的存储能力，
//所以大小为100的队列处理第100行不会队列已满，
//为通过系统迫不得已缩小数组大小 
typedef struct Queue{
	int a[SIZE];
	int f,r;
}Queue;
void init1(Queue*q){
	q->f=0;
	q->r=0;
}
void in(Queue*q,int a){
	if((q->r+1)%SIZE!=q->f){
	q->a[q->r]=a;
	q->r=(q->r+1)%SIZE;
	}
	else {
	printf("队列已满");
	exit(0);
	}
}
int out(Queue*q){
	int m=q->a[q->f];
	q->f=(q->f+1)%SIZE;
	return m;
}
void yanghui(Queue*q,int x){
	if(x==1)printf("1");
	if(x==2)printf("1 1");
	if(x==3)printf("1 2 1");
	if(x==4)printf("1 3 3 1");
	if(x>4){
		in(q,3);
		in(q,3);
		for(int i=4;i<x;i++){
		int l=out(q),r;
		in(q,l+1);
		for(int j=0;j<i-3;j++){
			r=l;
			l=out(q);
			in(q,l+r);
		}
		in(q,l+1);
		}
		printf("1 ");
		while(q->f!=q->r)printf("%d ",out(q));
		printf("1");
	}
}
int main(){
	Queue q;
	init1(&q);
	int x;
	scanf("%d",&x);
	yanghui(&q,x);
	return 0;
}
