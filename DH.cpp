#include<stdio.h>
#include<time.h>
#include<math.h>
#include<stdlib.h>
/*
int Random();
int isPrime(int odd);
int MoChongFu(int m, int e,int n);
int MoChongFua(int m, int e,int n);
int Miller_Rabin(int odd);
int yuangen(int yy);*/

 

//产生一个随机数
int Random()
{
	int odd = 0;
	//设置随机数种子，产生随机数 
	srand((unsigned)time(NULL));
	while (1)
	{
		odd = rand() % (16384) + 16384;
		if (odd % 2 != 0)
			break;
	}
	//printf("%d\n", odd);
	return odd;
}


//如果是素数的话返回1
int isPrime(int odd)
{
	int i, r, k = 0;
	int prime[9] = { 3, 5, 7, 11, 13, 17, 19, 23, 29 };
 	for (i = 0; i<9; i++)
	{
		r = odd % prime[i];
	//	printf("第%d次小素数%d试除的余数为%d.\n", i + 1, prime[i], r);
		if (r == 0)
		{ 
	//		printf("小素数试除判断%d不是素数。\n\n",odd);
		    return 0;
		}
	}	
//	printf("小素数试除判断%d是素数。\n\n",odd);
	return 1;
}


//模重复平方算法，计算b^n%m 
//将n进行二进制表示，b^n=b0^n0*b1^n1*...*b(k-1)^n(k-1) ，n0,n1,...为0或1； 
int MoChongFu(int m, int e,int n){ 
	int binary[22];
	int count=0,i;
	int a=1,b=m;
    //从低位到高位
	do{                            
		binary[count]=e%2;
		e=e/2;
		count++;
	}while(e!=0);
	for(i=0;i<count;i++)
	{
		if(binary[i]==1)
		{
			//a为余数，b平方 
			a=(a*b)%n;
			b=(b*b)%n;
		}
		if(binary[i]==0)
		{
			a=a;
			b=(b*b)%n;
		}
	
	}
	return a;
}

//米勒检测---大概率判断是否为素数 
/*1、要测定p，取一个较小的质数a，设s,t,满足2^s*t=p-1;
  2、先算a^t，不断平方进行二次探测(进行s次）；
  3、根据费马小定理a^(p-1)不等于1(mod p),p为合数； 
*/

int Miller_Rabin(int odd)
{
	int s=0,i,count=0;
	int a,b,t,num;
	num=odd-1;
	//把n-1转化成(2^s)*t，偶数都可以表示成 
	while(1)              
	{
		if(num%2==0)
		{
			s++;
			num=num/2;
		}
		else
		{
			t=num;
			break;
		}
	}
//	printf("将%d写成%d-1=2^%d*%d\t\t\n",odd,odd,s,t);
	a=rand()%(odd-3)+2;
//	printf("产生的随机数a是：%d\n",a);
	b=MoChongFu(a,t,odd);
//	printf("第1次算出的余值是：%d\n",b);	
	if(b%odd==1||b==(odd-1))
	{	
		return 1;
	}
	for(i=1;i<s;i++)
	{	
		b=(b*b)%odd;
	//	printf("第%d次算出的余值是：%d \n",i+1,b);
		if(b==(odd-1))
		{
			return 1;
		}	
	}	
	return 0;   
}



//欧拉函数的素因数和两者的商
int yuangen(int yy)
{
	int n=2,g=0,q,k,count=0,a[10];
	int gg;
	int c[10];
	
	q=yy-1;
	
	while(1)
	{
    	if (q%n==0)
		{
			a[count]=n;
			count++;
			while(!(q%n))
			{
				q=q/n;
			}
		}
		n++;
		if(q<n)
			break;
	}
	
//	printf("模%d的欧拉函数分解质因数为：\n",yy);
/*	for(n=0;n<count;++n)
	{
			printf("%d  ",a[n]);
		
	}
	
	printf("\n\n");
	printf("所以指数为：");*/
	for(n=0;n<count;++n)
	{
		c[n]=(yy-1)/a[n];
//		printf("%d  ",c[n]);
	}
//	printf("\n\n");
	
	for(g=2;;g++)
	{
		for(n=0;n<count;++n)
		{
			if(MoChongFu(g,c[n],yy)==1)
			{
//				printf("%d^%d=%d mod %d\n",g,c[n],MoChongFu(g,c[n],yy),yy);
				
				goto cd;
			}
			else
			{
//				printf("%d^%d=%d mod %d\n",g,c[n],MoChongFu(g,c[n],yy),yy);
			}
		}
		gg=g;
//     	printf("所以%d是最小原根。\n",gg);
		goto ab;
cd: ;
	}	
ab:	for(g=3;;g++)
	{
		//gcd(g,yy-1)=1即互素的话gg^g为原根 
		if((yy-1)%g!=0)
		{
			k=MoChongFu(gg,g,yy);
			if(k>32&&k<1024)
			{
//				printf("取介于2^5到2^10之间的一个原根值为：");
//				printf("%d\n\n",k);
				return k;
			}	
		}
	}
	return 0;	
}

