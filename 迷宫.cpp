#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
typedef struct GRID{
	int x,y,f=0,g=0,h=0,w=0,ifopen=0,ifclose=0,ifroad=0;
	struct GRID*prev;
}GRID;
typedef struct OPLIST{
	int a;
	struct OPLIST*next;
}OPLIST;
OPLIST*head;
int x,y,m,n;
void in(int xy,GRID*a){
	OPLIST*pp=(OPLIST*)malloc(sizeof(OPLIST));
	pp->a=xy;
	pp->next=NULL;
	if(head==NULL){
	head=pp;return;}
	if(a[xy].h<a[head->a].h){
		OPLIST*c=head;
		head=pp;
		head->next=c;
		return;
		}
	if(head->next==NULL){
		head->next=pp;
		return;
		}
	OPLIST*cc=head;
	for(;cc->next!=NULL;cc=cc->next)
		if(a[xy].h>=a[cc->a].h&&a[xy].h<a[cc->next->a].h){
			OPLIST*qq=cc->next;
			cc->next=pp;
			pp->next==qq;
			return;
		}
	cc->next=pp;
	return;
}
int out(){
	OPLIST*p=head;
	int k=head->a;
	head=head->next;
	free(p);
	return k;
}
void no(){
	OPLIST*p=head;
	if(p==NULL){
	printf("没有通路");
	exit(0);}
}
int main(){
	printf("输入迷宫长：");
	scanf("%d",&m);
	printf("输入迷宫宽：");
	scanf("%d",&n);
	const int p=m*n;
	GRID a[p];
	for(int i=0;i<m;i++)
	for(int j=0;j<n;j++){
		a[j*m+i].x=i;
		a[j*m+i].y=j;
		a[j*m+i].f=0;
		a[j*m+i].g=0;
		a[j*m+i].h=0;
	}//初始化 
	int w;
	printf("输入墙的数量：");
	scanf("%d",&w);
	srand(unsigned(time(NULL)));
	int r;
	for(int i=0;i<w;i++){	
		r=rand()%p;
		while(a[r].w!=0||r==p-1)
		r=(r+rand()%p)%p;
		a[r].w=1;
	}//随机造墙
	 for(int i=0;i<m+2;i++)printf("*");
	 printf("\n");
	 for(int i=0;i<n;i++){
		printf("*");
		for(int j=0;j<m;j++){
			if(a[i*m+j].w==0)printf(" ");
			else printf("*");
		}
		printf("*\n");
	 }
	 for(int i=0;i<m+2;i++)printf("*");
	 printf("\n");//绘制迷宫
	 for(int i=0;i==0;)
	 {
	 printf("输入起点横坐标（1~%d）:",m);
	 scanf("%d",&x);
	 printf("输入起点纵坐标（1~%d）:",n);
	 scanf("%d",&y);
	 if(a[x+y*m].w==1)printf("此位置为墙，请重新输入！\n");
	 else i=1;
	 }
	 x--;y--;//输入起点 
	 a[x+y*m].ifopen=1;
	 OPLIST *head;
	 in(x+y*m,a);//建立打开列表 
	 while(1){
     	 int k=out();
     	 a[k].ifclose=1;
		 if(a[k+1].ifopen==0&&a[k+1].ifclose==0&&(k+1)%m!=0&&a[k+1].w==0){
	 		a[k+1].ifopen=1;
	 		a[k+1].f=a[k].f+1;
	 		a[k+1].g=m+n-2-(k+1)%m-(k+1)/m;
	 		a[k+1].h=a[k+1].f+a[k+1].g;
	 		a[k+1].prev=&a[k];
			in(k+1,a);
			if(k+1==m*n-1)break;
		 }
		   if(a[k+m].ifopen==0&&a[k+m].ifclose==0&&(k+m)<m*n&&a[k+m].w==0){
	 		a[k+m].ifopen=1;
	 		a[k+m].f=a[k].f+1;
	 		a[k+m].g=m+n-2-(k+m)%m-(k+m)/m;
	 		a[k+m].h=a[k+m].f+a[k+m].g;
	 		a[k+m].prev=&a[k];
			in(k+m,a);
			if(k+m==m*n-1)break;
		 }
		  if(a[k-1].ifopen==0&&a[k-1].ifclose==0&&k%m!=0&&a[k-1].w==0){
	 		a[k-1].ifopen=1;
	 		a[k-1].f=a[k].f+1;
	 		a[k-1].g=m+n-2-(k-1)%m-(k-1)/m;
	 		a[k-1].h=a[k-1].f+a[k-1].g;
	 		a[k-1].prev=&a[k];
			in(k-1,a);
			if(k-1==m*n-1)break;
		 }
		  if(a[k-m].ifopen==0&&a[k-m].ifclose==0&&(k-m)>0&&a[k-m].w==0){
	 		a[k-m].ifopen=1;
	 		a[k-m].f=a[k].f+1;
	 		a[k-m].g=m+n-2-(k-m)%m-(k-m)/m;
	 		a[k-m].h=a[k-m].f+a[k-m].g;
	 		a[k-m].prev=&a[k];
			in(k-m,a);
			if(k-m==m*n-1)break;
		 }
		 no();
	 }//A*算法 
	 a[m*n-1].ifroad=1;
	 GRID *ccc=&a[p-1];
	 for(int i=0;i<a[p-1].h;i++){
	 	ccc=ccc->prev;
	 	ccc->ifroad=1;
	 }//最短路径记录 
	 for(int i=0;i<m+2;i++)printf("*");
	 printf("\n");
	 for(int i=0;i<n;i++){
		printf("*");
		for(int j=0;j<m;j++){
			if(a[i*m+j].w==1)printf("*");
			else if(a[i*m+j].ifroad==1)printf("+");
			else printf(" ");
		}
		printf("*\n");
	 }
	 for(int i=0;i<m+2;i++)printf("*");
	 printf("\n");//绘制最短路径 
	 return 0;
}
