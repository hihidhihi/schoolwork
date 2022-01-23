#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef struct Queue{
	int a;
	struct Queue* next;
}Queue;
void in1(Queue**r,int a){
	(*r)->next=(Queue*)malloc(sizeof(Queue));
	(*r)=(*r)->next;
	(*r)->a=a;
	(*r)->next=NULL;
}
int out1(Queue**f){
	Queue *qq=*f;
	*f=(*f)->next;
	//free(qq);
	return (*f)->a;
}
int see(Queue*r){
	return r->a;
}
int see1(Queue*f){
	if(f->next!=NULL)return f->next->a;
	else return -1;
}
int main(){
	Queue a,b,c;
	a.next=b.next=c.next=NULL;
	Queue*af,*ar,*bf,*br,*cf,*cr;
	af=ar=&a;bf=br=&b;cf=cr=&c;
	char str[20];
	gets(str);
	for(int i=strlen(str)-1;i>=0;i-=2){
			int pp=see(ar);
			pp=see(br);
			pp=see(cr);
			int cc=0,max=0;
			if(af->next!=NULL&&see(ar)<str[i]-48&&see(ar)>max){
				cc=1;max=see(ar);
			}
			if(bf->next!=NULL&&see(br)<str[i]-48&&see(br)>max){
				cc=2;max=see(br);
			}
			if(cf->next!=NULL&&see(cr)<str[i]-48&&see(cr)>max){
				cc=3;max=see(cr);
			}
			switch(cc){
				case 0:
					if(af->next==NULL)in1(&ar,str[i]-48);
					else if(bf->next==NULL)in1(&br,str[i]-48);
					else if(cf->next==NULL)in1(&cr,str[i]-48);
					else {printf("÷ÿ≈≈ ß∞‹");exit(0);}
					break;
				case 1:
					in1(&ar,str[i]-48);break;
				case 2:
					in1(&br,str[i]-48);break;
				case 3:
					in1(&cr,str[i]-48);break;
			}
		
	}
	int pr[5];
	for(int i=0;i<5;i++){
		int cc=0,min=10;
		if(af->next!=NULL&&see1(af)<min){
			cc=1;min=see1(af);
		}
		if(bf->next!=NULL&&see1(bf)<min){
			cc=2;min=see1(bf);
		}
		if(cf->next!=NULL&&see1(cf)<min){
			cc=3;min=see1(cf);
		}
		switch(cc){
			case 1:
				pr[i]=out1(&af);
				break;
			case 2:
				pr[i]=out1(&bf);
				break;
			case 3:
				pr[i]=out1(&cf);
				break;
		}
	}
	printf("%d %d %d %d %d",pr[4],pr[3],pr[2],pr[1],pr[0]);
	return 0;
}
