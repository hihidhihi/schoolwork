#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef struct Stack{
	char a[10];
	int bottom,top;
}Stack;
void init1(Stack *s){
	s->bottom=0;
	s->top=0;
}
void push1(Stack *s,char a){
	s->a[s->top++]=a;
}
char pop1(Stack *s){
	return s->a[--s->top];
}
int main(){
	Stack s;
	init1(&s);
	int flag=0;
	char str[20],left[]="{[(",right[]="}])";
	gets(str);
	for(int i=0;i<strlen(str);i++){
		if(str[i]=='{'||str[i]=='['||str[i]=='(')push1(&s,str[i]);
		else if(str[i]==')'||str[i]==']'||str[i]=='}'){
			int l,r;
			for(r=0;r<3;r++)if(str[i]==right[r])break;		
			if(s.top==0){printf("ȱ��%c����",left[r]);flag=1;continue;}
			char p=pop1(&s);
			for(l=0;l<3;l++)if(p==left[l])break;
			if(l==r)continue;
			else {printf("ȱ��%c����",left[r]);push1(&s,p);flag=1;continue;}
		}
	}
	if(s.top!=0){	
		flag=1;
		for(int i=0;i<strlen(s.a);i++)
		{
		char p=pop1(&s);
		for(int l=0;l<3;l++)if(p==left[l]){
		printf("ȱ��%c����",right[l]);
		break;
	}
	}
	}
	if(flag==0)printf("ƥ��ɹ�");
	return 0;
}
//"[(]"��"[)]"���� ������Ҫһ����ƥ��������ջ 
