#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef struct LIST{
	char No;
	int pcs;
	struct LIST* next;
}LIST;
void create1(LIST*l){
	l->No='\0';
	char str[100];
	LIST*c=l;
	gets(str);
	for(int i=0;i<strlen(str);i+=5){
		c->next=(LIST*)malloc(sizeof(LIST));
		c=c->next;
		c->No=str[i];
		c->pcs=str[i+2]-48;
	}
	c->next=NULL;
}
void add1(LIST*l,LIST*ladd){
	LIST*c=l,*cadd=ladd->next;
	while(1){
		while(c->No<cadd->No&&c->next!=NULL&&c->next->No<=cadd->No)c=c->next;
		if(c->No<cadd->No&&c->next!=NULL&&c->next->No>cadd->No){
			LIST*k=cadd->next;
			cadd->next=c->next;
			c->next=cadd;
			cadd=k;
			c=c->next;
		}
		if(c->No<cadd->No&&c->next==NULL){c->next=cadd;break;}
		if(c->No==cadd->No){
			c->pcs=c->pcs+cadd->pcs;
			if(cadd->next!=NULL)cadd=cadd->next;
			else break;
		}
	}
}
void print1(LIST*l){
	LIST*c=l->next;
	while(c->next!=NULL){
		printf("%c %d, ",c->No,c->pcs);
		c=c->next;
	}
		printf("%c %d;",c->No,c->pcs);
}
int main(){
	LIST l;
	create1(&l);
	LIST ladd;
	create1(&ladd);
	add1(&l,&ladd);
	print1(&l);
	return 0;
}
