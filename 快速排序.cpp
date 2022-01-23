#include<stdio.h>
#include<string.h>
int m[7][12];
void quick(int a,int b,int dep){
	dep++;
	if(a==b){
		for(int i=dep+1;i<7;i++)m[i][a]=m[dep][a];
		return;
			}
	int p=m[dep][a];
	int i=a,j=b;
	for(int kk=a;kk<=b;kk++)m[dep+1][kk]=m[dep][kk];
	while(j>i)
	{		
		while(m[dep+1][j]>=p&&j>i)j--;
		m[dep+1][i]=m[dep+1][j];			
		while(m[dep+1][i]<p&&j>i)i++;
		m[dep+1][j]=m[dep+1][i];
	}
	for(int w=dep+1;w<7;w++)m[w][i]=p;
	if(a<=i-1)quick(a,i-1,dep);
	if(i+1<=b)quick(i+1,b,dep);
}
int main(){
	for(int u=0;u<12;u++)scanf("%d",&m[0][u]);
	quick(0,11,-1);
	for(int h=1;h<7;h++){
	for(int q=0;q<11;q++)printf("%d ",m[h][q]);
	if(h!=6)printf("%d\n",m[h][11]);else printf("%d",m[h][11]);
	}
	return 0;
}
