#include<stdio.h>
#include<string.h>
void heap1(int p,int*a,int n){
	if((2*p+1)>=n)return;
	if((2*p+2)>=n){
		if(a[p]<a[2*p+1]){
			int k=a[p];
			a[p]=a[2*p+1];
			a[2*p+1]=k;
		}
		return;
	}
	if(a[p]<a[2*p+1]||a[p]<a[2*p+2]){
		if(a[2*p+1]>=a[2*p+2]){
			int k=a[p];
			a[p]=a[2*p+1];
			a[2*p+1]=k;
			heap1(2*p+1,a,n);
		}else{
			int k=a[p];
			a[p]=a[2*p+2];
			a[2*p+2]=k;
			heap1(2*p+2,a,n);
		}
	}else return;
}
int main(){
	int a[15];int n=0;
	char s[30];
	gets(s);
	for(int i=0;i<strlen(s);i++){
		if(s[i]>='0'&&s[i]<='9'&&s[i+1]>='0'&&s[i+1]<='9'){
			a[n]=(s[i]-'0')*10+(s[i+1]-'0');
			n++;
			i++;
			continue;
		}
		if(s[i]>='0'&&s[i]<='9'){
			a[n]=s[i]-'0';
			n++;
			continue;
		}
	}
	for(int i=n/2-1;i>=0;i--)heap1(i,a,n);
	int c;
	for(c=0;c<n-1;c++)printf("%d ",a[c]);
	printf("%d",a[c]);
	return 0;
}
