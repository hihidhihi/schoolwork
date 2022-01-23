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
			if(s.top==0){printf("缺少%c括号",left[r]);flag=1;continue;}
			char p=pop1(&s);
			for(l=0;l<3;l++)if(p==left[l])break;
			if(l==r)continue;
			else {printf("缺少%c括号",left[r]);push1(&s,p);flag=1;continue;}
		}
	}
	if(s.top!=0){	
		flag=1;
		for(int i=0;i<strlen(s.a);i++)
		{
		char p=pop1(&s);
		for(int l=0;l<3;l++)if(p==left[l]){
		printf("缺少%c括号",right[l]);
		break;
	}
	}
	}
	if(flag==0)printf("匹配成功");
	return 0;
}
//"[(]"和"[)]"问题 可能需要一个不匹配右括号栈 
