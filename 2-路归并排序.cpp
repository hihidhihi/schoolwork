#include<stdio.h>
#include<string.h>
int m[5][12];
void merge1(int a,int b,int dep){
	dep++;
	if(a==b){
	m[dep][a]=m[4][a];
	return;
	}
	int x=(b-a+1)/2+a;
	merge1(a,x-1,dep);
	merge1(x,b,dep);
	int i=a,j=x,p=a;
	while(i<=x-1&&j<=b)
	{
		if(m[dep+1][i]<=m[dep+1][j]){
			m[dep][p]=m[dep+1][i];
			i++;
		}
		else{
			m[dep][p]=m[dep+1][j];
			j++;
		}
		p++;
	}
	if(i==x)for(int r=j;r<=b;r++,p++)m[dep][p]=m[dep+1][r];
	if(j==b+1)for(int r=i;r<=x-1;r++,p++)m[dep][p]=m[dep+1][r];
	return;
}
int main(){
	for(int u=0;u<12;u++)scanf("%d",&m[4][u]);
	merge1(0,11,-1);
	for(int h=3;h>=0;h--){
	
	for(int q=0;q<11;q++)printf("%d ",m[h][q]);
	printf("%d\n",m[h][11]);
	}
	return 0;
}
