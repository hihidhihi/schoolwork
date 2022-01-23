#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef struct node{
	int a;
	char s;
	char haff[7];
	struct node*lc,*rc;
}node;
void qianxu(node*k){
	if(k->lc==NULL)return;
	char l[]="0";char r[]="1";
	strcpy(k->lc->haff,k->haff);
	strcpy(k->rc->haff,k->haff);
	strcat(k->lc->haff,l);
	strcat(k->rc->haff,r);
	qianxu(k->lc);
	qianxu(k->rc);
}
int main(){
	node *a[8],b[8];
	for(int i=0;i<8;i++)
	{
		scanf("%d",&(b[i].a));
		b[i].s='A'+i;
		b[i].lc=NULL;
		b[i].rc=NULL;
		a[i]=&b[i];
		for(int j=i;j>0;j--){
			if(a[j]->a>=a[j-1]->a)break;
			node*p=a[j];
			a[j]=a[j-1];
			a[j-1]=p;
		}
	}
	for(int i=0;i<7;i++){
		node*n=(node*)malloc(sizeof(node));
		n->lc=a[i];
		n->rc=a[i+1];
		n->a=n->lc->a+n->rc->a;
		a[i+1]=n;
		for(int j=i+1;j<7;j++){
			if(a[j]->a<=a[j+1]->a)break;
			node*p=a[j];
			a[j]=a[j+1];
			a[j+1]=p;
		}
	}
	qianxu(a[7]);
	for(int i=0;i<8;i++)printf("%s",b[i].haff);
	return 0;
}
