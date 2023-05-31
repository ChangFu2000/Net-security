#include <iostream>
#include"DES.cpp"
#include"DH.h"
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int p() {
	int yy=0;
	int gg;
	int A,B,Key,Key1,Key2;
	int SA,SB;
	
	int  i,flag1,flag2;
	do
	{
	//	printf("**********************\n\n\n");
q:		while(yy == Random());
		yy = Random();
	//	printf("产生的随机数是:%d\n\n",yy);
		flag1=!isPrime(yy);
		for(i=0;i<5;i++)
			
		{
			flag2=!Miller_Rabin(yy);
			if(flag2)
			{
			//	printf("第%d次米勒检测未通过。\n\n",i+1);
			//	printf("因为第%d次米勒检测没有通过，所以随机数%d不是素数，\n产生下一个随机数，先用小素数试除，再进行5次米勒检测，如果小素数试除通过并且5次都通过，则说明该随机数是大素数。",i+1,yy);
			//	printf("\n");
				goto q;
			}
		/*	else
				printf("第%d次米勒检测通过。\n\n",i+1);*/
		}		
	}while(flag1||flag2);
	return yy;
}
int g(int p){
	int gg=yuangen(p);
	return gg;
}
void psmbit()
{
	for(int i=0;i<64;i++)printf("%d ",smbit[i]);
	puts("");
}
char* final(int Key){
    init();
	int num=0,i;
	printf("jiami key:%d\n",Key); 
	for(i=7;i>=0;i--){
		if(Key!=0){
			num=Key%10;
			miyao[i]=num+'0';
			Key/=10;
		}else{
			miyao[i]='0';
		}
	}
    change(miyao,2);
    char iv[9]="12345678";
//	char key_tmp[maxn]="12345678";
    printf("\n请输入8位明文:");
    scanf("%s",mingwen);
	for(int k=0;k<8;k++)
	{
		//printf("%c-%c\n",mingwen[k],iv[k]);
		mingwen[k]^=iv[k];
		//printf("明文:%d\n",mingwen[k]);
	}
	//printf("明文：%s\n",mingwen);
    change(mingwen,1);
    //scanf("%s",secret_key);
    jiami();
    psmbit();
    printf("加密完成,得到十六进制密文:\n");
    printf("%s\n",miwen);
	return miwen;
}
void result(char* message,int key){
	init();
	int num=0,i,k=0;
	printf("jiemi key:%d\n",key); 
	for(i=7;i>=0;i--){
		if(key!=0){
			num=key%10;
			miyao[i]=num+'0';
			key/=10;
		}else{
			miyao[i]='0';
		}
	}	
	int l=0,tmp=0;
    change(miyao,2);
	for(int i=0;i<16;i++)
	{
		if(message[i]>='A'&&message[i]<='Z')tmp=message[i]-'A'+10;
		else tmp=message[i]-'0';
		for(int j=3;j>=0;j--)smbit[l++]=(tmp&(1<<j))?1:0;
	}
	psmbit();
	jiemi();
    printf("\n解密完成,得到明文:\n");
    char iv[9]="12345678";
    for(i=0;i<64;i++)
    {
        if((i+1)%8==0)
        {
            int ans=smbit[i-7]*128+smbit[i-6]*64+smbit[i-5]*32+smbit[i-4]*16+smbit[i-3]*8+smbit[i-2]*4+smbit[i-1]*2+smbit[i];
            ans^=iv[k++];
            printf("%c",ans);
        }
    }
    printf("\n");
}
int ran(){
	srand(time(NULL));
	int r = rand()%(96)+32;
	printf("选择的随机数是：%d\n",r);
	return r;
}
int key(int gg,int r,int p){
	int S=MoChongFu(gg,r,p);
	printf("\n");
//	printf("计算key=(g^r) mod p:\n");
	return S;
}

