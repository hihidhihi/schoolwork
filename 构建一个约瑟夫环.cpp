#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef struct LIST{
	int a,No;
	struct LIST* next;
}LIST;
LIST*head,*c;
void init(LIST*l){
	l->a=NULL;
	head=l;
	c=l;
}
void create1(int n){
	for(int i=0;i<n;i++){
	c->next=(LIST*)malloc(sizeof(LIST));
	c=c->next;
	printf("�����%d���˵����룺",i+1);
	scanf("%d",&c->a);
	c->No=i+1;
	}
	c->next=head->next;
	c=head;
}
void delete1(int n,int m){
	int live=n;
	for(int i=0;i<n;i++){
		int k;
		if(i==0)k=m;else k=c->a;
		for(int j=0;j<k-1;j++)c=c->next;
		LIST*pp=c->next;
		c->next=pp->next;
		printf("��%d�����е���Ϊ������е�%d�ţ�\n",i+1,pp->No);
		free(pp);
	}
}
int main(){
	int n;
	printf("������������");
	scanf("%d",&n);
	LIST Jsph;
	init(&Jsph);
	create1(n);
	printf("���뱨������m��");
	int m;
	scanf("%d",&m);
	delete1(n,m);
	return 0;
}
