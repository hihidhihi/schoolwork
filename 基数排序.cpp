#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef struct node{
	int a;
	struct node*next;
}node;
int main(){
	node m[10];
	for(int i=0;i<10;i++)m[i].next=NULL;
	for(int i=0;i<12;i++){
		node*c=(node*)malloc(sizeof(node));
		scanf("%d",&(c->a));
		c->next=NULL;
		node*k=&m[c->a%10];
		while(k->next!=NULL)k=k->next;
		k->next=c;
	}
	int f=0;
	for(int i=0;i<10;i++){
		node*k=&m[i];
		while(k->next!=NULL){
			k=k->next;
			if(f==11)printf("%d\n",k->a);
			else printf("%d ",k->a);
			f++;
		}
	}
	f=0;
	for(int i=0;i<10;i++){
		for(int j=0;j<10;j++){
			node*k=&m[j];
			while(k->next!=NULL){
				k=k->next;
				if(k->a/10==i){
					if(f==11)printf("%d",k->a);
					else printf("%d ",k->a);
					f++;
					break;
				}
			}
		}
	}
	return 0;
}
