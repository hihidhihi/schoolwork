#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define SIZE 5
typedef struct node{
	int a;
	int u;
	int h;
	struct node*lc,*rc,*parent;
}node;
void uh(node*c){
	if(c->lc!=NULL&&c->rc!=NULL)
			{
				c->u=c->lc->h-c->rc->h;
				c->h=c->lc->h>=c->rc->h?(c->lc->h+1):(c->rc->h+1);
			}
			if(c->lc==NULL&&c->rc!=NULL)
			{
				c->u=0-c->rc->h;
				c->h=c->rc->h+1;
			}
			if(c->rc==NULL&&c->lc!=NULL)
			{
				c->u=c->lc->h;
				c->h=c->lc->h+1;
			}
			if(c->rc==NULL&&c->lc==NULL)
			{
				c->u=0;
				c->h=1;
			}
}
node*rroll(node*t){
	node*c=t->lc;
	t->lc=c->rc;
	if(c->rc!=NULL)c->rc->parent=t;
	c->rc=t;
	c->parent=t->parent;
	if(t->parent!=NULL)(t->parent->lc==t)?(t->parent->lc=c):(t->parent->rc=c);
	t->parent=c;			
	uh(t);
	uh(c);
	return c;
}
node*lroll(node*t){
	node*c=t->rc;
	t->rc=c->lc;
	if(c->lc!=NULL)c->lc->parent=t;
	c->lc=t;
	c->parent=t->parent;
	if(t->parent!=NULL)(t->parent->rc==t)?(t->parent->rc=c):(t->parent->lc=c);
	t->parent=c;			
	uh(t);
	uh(c);
	return c;
}
node* roll(node*t){
	if(t->u>0)
	{
		if(t->lc->u>0)return rroll(t);else return(rroll(lroll(t->lc)->parent));
	}
	else
	{
		if(t->rc->u<0)return lroll(t);else return(lroll(rroll(t->rc)->parent));
	}
}
void initnode(node*t){
	t->h=1;
	t->u=0;
	t->lc=NULL;
	t->rc=NULL;
	t->parent=NULL;
}
node* create1(node*t){
	initnode(t);
	scanf("%d",&(t->a));
	node*c=t,*par=t;
	for(int i=0;i<4;i++){
		int p,f;
		scanf("%d",&p);
		while(c!=NULL){
		par=c;
		if(p<=c->a){
		c=c->lc;f=0;}
		else {
		c=c->rc;f=1;}
		}
		node*cc=(node*)malloc(sizeof(node));
		initnode(cc);
		cc->a=p;
		if(f==0)par->lc=cc;else par->rc=cc;
		cc->parent=par;
		c=cc;
		while(c->parent!=NULL){
			c=c->parent;
			uh(c);
			if(c->u>1||c->u<-1)c=roll(c);	
		}
	}
	return c;
}
typedef struct Queue{
	node* a[SIZE];
	int f,r;
}Queue;
void init1(Queue*q){
	q->f=0;
	q->r=0;
}
void in(Queue*q,node* a){
	if((q->r+1)%SIZE!=q->f){
	q->a[q->r]=a;
	q->r=(q->r+1)%SIZE;
	}
	else {
	printf("¶ÓÁÐÒÑÂú");
	exit(0);
	}
}
node* out(Queue*q){
	node* m=q->a[q->f];
	q->f=(q->f+1)%SIZE;
	return m;
}
void print1(node*t){
	Queue q;
	init1(&q);
	node* tt[5];
	int i=0;
	in(&q,t);
	while(q.f!=q.r){
		tt[i]=out(&q);
		if(tt[i]->lc!=NULL)in(&q,tt[i]->lc);
		if(tt[i]->rc!=NULL)in(&q,tt[i]->rc);
		i++;
	}
	for(int qq=0;qq<4;qq++)printf("%d ",tt[qq]->a);
	printf("%d",tt[4]->a);
}
int main(){
	node t;
	print1(create1(&t));
}
