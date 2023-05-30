///DES算法加密
#include<cstdio>
#include<cstring>
#define maxn 512
char mingwen[maxn],miwen[maxn],miyao[maxn];///输入的明文密文密钥
int smbit[maxn],miyaobit[maxn],mibit[maxn],zhongbit[maxn];///明文，密文，密钥的二进制，与转换中需要作为媒介的
int mip[maxn],rr[maxn],SYa[maxn],er[10];///mip经过ip初始置换得到rrE盒扩展后得到,SYas盒压缩得到,er在s盒的转换过程中使用
int Left[maxn],Right[maxn];///左右两部分
int miyaopc1[maxn],miyaopc2[maxn],c[maxn],d[maxn];///c,d密钥置换得到
int zimi[16][maxn];///产生的子密钥
int LL[17][64],RR[17][64],cnt;///用来记录每一次的左右部分
int times,xz[]= {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1}; ///循环左移位,times用来计算是第几次移位
void init()///初始化
{
    times=cnt=0;
    memset(smbit,0,sizeof(smbit));
    memset(LL,0,sizeof(LL));
    memset(RR,0,sizeof(RR));
	memset(miyaobit,0,sizeof(miyaobit));
	memset(mibit,0,sizeof(mibit)); 
	memset(mip,0,sizeof(mip)); 
    memset(c,0,sizeof(c));
    memset(d,0,sizeof(d));
    memset(LL,0,sizeof(LL));
    memset(RR,0,sizeof(RR));
    memset(mip,0,sizeof(mip));
    memset(SYa,0,sizeof(SYa));
    memset(zimi,0,sizeof(zimi));
    memset(Left,0,sizeof(Left));
    memset(Right,0,sizeof(Right));
    memset(miyao,0,sizeof(miyao));
    memset(mingwen,0,sizeof(mingwen));
    memset(miyaopc1,0,sizeof(miyaopc1));
    memset(miyaopc2,0,sizeof(miyaopc2));
    memset(miyaobit,0,sizeof(miyaobit));
}
extern const int IP[]=///ip置换表,用1维存的原因是方便置换。下面的都一样
{
    58,50,42,34,26,18,10,2,
    60,52,44,36,28,20,12,4,
    62,54,46,38,30,22,14,6,
    64,56,48,40,32,24,16,8,
    57,49,41,33,25,17, 9,1,
    59,51,43,35,27,19,11,3,
    61,53,45,37,29,21,13,5,
    63,55,47,39,31,23,15,7,
};
extern const int E[]=///扩展E盒
{
    32,1,2,3,4,5,
    4,5,6,7,8,9,
    8,9,10,11,12,13,
    12,13,14,15,16,17,
    16,17,18,19,20,21,
    20,21,22,23,24,25,
    24,25,26,27,28,29,
    28,29,30,31,32,1,
};
extern const int P[]=///p盒置换表
{
    16,7,20,21,29,12,28,17,1,15,23,26,5,18,31,10,
    2,8,24,14,32,27,3,9,19,13,30,6,22,11,4,25,
};
extern const int PC_1[]=///密钥置换选择PC_1盒
{
    57,49,41,33,25,17,9,1,58,50,42,34,26,18,
    10,2,59,51,43,35,27,19,11,3,60,52,44,36,
    63,55,47,39,31,23,15,7,62,54,46,38,30,22,
    14,6,61,53,45,37,29,21,13,5,28,20,12,4,
};
extern const int PC_2[]=///密钥置换选择PC_2盒
{
    14,17,11,24,1,5,3,28,15,6,21,10,
    23,19,12,4,26,8,16,7,27,20,13,2,
    41,52,31,37,47,55,30,40,51,45,33,48,
    44,49,39,56,34,53,46,42,50,36,29,32
};
extern const int S[8][4][16]=///8个s盒
{
    ///s1
    14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
    0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
    4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
    15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13,
    ///s2
    15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
    3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
    0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
    13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9,
    ///s3
    10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
    13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
    13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
    1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12,
    ///s4
    7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
    13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
    10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
    3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14,
    ///s5
    2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
    14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
    4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
    11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3,
    ///s6
    12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
    10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
    9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
    4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13,
    ///s7
    4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
    13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
    1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
    6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12,
    ///s8
    13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
    1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
    7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
    2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11,
};
extern const int IP_1[]=///逆初始置换表
{
    40,8,48,16,56,24,64,32,
    39,7,47,15,55,23,63,31,
    38,6,46,14,54,22,62,30,
    37,5,45,13,53,21,61,29,
    36,4,44,12,52,20,60,28,
    35,3,43,11,51,19,59,27,
    34,2,42,10,50,18,58,26,
    33,1,41,9,49,17,57,25,
};
//int smbit[]={0,0,0,1,0,1,0,0, 0,1,0,0,1,1,1,0, 1,1,0,1,0,1,0,0, 1,1,1,0,1,1,1,1,0,0,0,1,0,0,0,1, 0,0,1,0,1,1,0,0, 1,1,1,0,0,1,1,0, 0,0,0,0,1,1,1,0};
void change(char s[],int flag)///转换为二进制,（验证正确）
{
    int l=0;
    for(int i=0; i<8; i++)
    {
        int n=s[i],time=0;
        memset(zhongbit,0,sizeof(zhongbit));
        while(n!=0)
        {
            zhongbit[time++]=n%2;
            n/=2;
        }
        if(flag==1)
        {
            for(int j=7; j>=0; j--)
                smbit[l++]=zhongbit[j];
        }
        else if(flag==2)
        {
            for(int j=7; j>=0; j--)
                miyaobit[l++]=zhongbit[j];
        }
    }
}
void fen()///将64bit的minip分为左右两部分(验证正确）
{
    for(int i=0; i<32; i++)
    {
        Left[i]=mip[i];
        Right[i]=mip[32+i];
    }
    for(int i=0; i<32; i++)
    {
        LL[cnt][i]=Left[i];
        RR[cnt][i]=Right[i];
    }
    cnt++;
}
void ipzhihuan()///第一次循环使用的ip置换，ip置换(验证正确）
{
    for(int i=0; i<64; i++)
        mip[i]=smbit[IP[i]-1];///下标从0开始
    fen();
    memset(smbit,0,sizeof(smbit));
}
void EK()///E盒扩展（验证正确）
{
    memset(rr,0,sizeof(rr));
    for(int i=0; i<48; i++)
        rr[i]=Right[E[i]-1];///下标从0开始
}
void SY(int k)///异或及S盒压缩(验证正确）
{
    for(int i=0; i<48; i++)
        rr[i]^=zimi[k][i];
    int c=0;
    for(int i=0; i<48; i++)
    {
        if((i+1)%6==0)///进入下个s盒
        {
            int w=(i+1)/6,h,l;///哪个盒,哪一行那一列
            h=rr[i-5]*2+rr[i]*1;
            l=rr[i-4]*8+rr[i-3]*4+rr[i-2]*2+rr[i-1]*1;
            int ans=S[w-1][h][l];
            int tt=0;
            memset(er,0,sizeof(er));
            while(ans)
            {
                er[tt++]=ans%2;
                ans/=2;
            }
            for(int j=3; j>=0; j--)
                SYa[c++]=er[j];
        }
    }
}
void PHe()///P盒置换
{
    for(int i=0; i<32; i++)
        Right[i]=Left[i]^SYa[P[i]-1];
    for(int i=0; i<32; i++)
        Left[i]=mip[32+i];
    for(int i=0; i<32; i++)
    {
        mip[i]=Left[i];
        mip[32+i]=Right[i];
    }
    fen();///分为左右两部分
}
void nizhihuan()///逆初始置换表(验证正确）
{
    for(int i=0; i<64; i++)
        smbit[i]=mip[IP_1[i]-1];
}
void PC_1zhi()///置换选择pc1盒及循环移位(验证正确）
{
    if(times==0)///只有当是第一次的时候才会进行PC置换盒1
    {
        for(int i=0; i<56; i++)
            miyaopc1[i]=miyaobit[PC_1[i]-1];///下标从0开始
    }
    for(int i=0; i<28; i++) ///分为两部分,循环移位
    {
        int ans=i+xz[times];
        if(ans>27)
            ans-=28;
        c[i]=miyaopc1[ans];///得到前半部分
        d[i]=miyaopc1[28+ans];///得到后半部分
    }
    for(int i=0; i<28; i++)
    {
        miyaopc1[i]=c[i];
        miyaopc1[28+i]=d[i];
    }
    times++;
}
void PC_2zhi()///16轮置换，pc2盒得到子密钥（验证正确）
{
    for(int k=0; k<16; k++)
    {
        PC_1zhi();
        for(int i=0; i<48; i++)
            zimi[k][i]=miyaopc1[PC_2[i]-1];///将每一次产生的子密钥存下来
    }
}
void zhuan()///将二进制转换成16进制密文
{
    int ans=0,l=0;
    for(int i=0; i<64; i++)
    {
        if((i+1)%4==0)
        {
            ans=smbit[i-3]*8+smbit[i-2]*4+smbit[i-1]*2+smbit[i];
            if(ans>9)
                miwen[l++]='A'+ans-10;
            else
                miwen[l++]=ans+'0';
            ans=0;
        }
    }
}
void jiami()///加密算法
{
    PC_2zhi();///子密钥产生(正确）
    ipzhihuan();///ip置换（正确）
    for(int i=0; i<16; i++)
    {
        EK();///E盒扩展（正确）
        SY(i);///S盒压缩（正确）
        PHe();///p盒置换
    }
    for(int i=0; i<32; i++)
    {
        mip[32+i]=LL[16][i];
        mip[i]=RR[16][i];
    }
    nizhihuan();
    zhuan();
}
void jiemi()
{
    PC_2zhi();
    ipzhihuan();///ip置换（正确）
    for(int i=0; i<16; i++)
    {
        EK();///E盒扩展（正确）
        SY(15-i);///S盒压缩（正确）
        PHe();///p盒置换
    }
    for(int i=0; i<32; i++)
    {
        mip[32+i]=LL[16][i];
        mip[i]=RR[16][i];
    }
    nizhihuan();
}
/* 
int main()
{
    init();
    printf("请输入8位明文:");
    scanf("%s",mingwen);
    change(mingwen,1);
    printf("\n请输入8位密钥:");
    scanf("%s",miyao);
    change(miyao,2);
    jiami();
    printf("加密完成,得到十六进制密文:\n");
    printf("%s\n",miwen);
    jiemi();
    printf("\n解密完成,得到明文:\n\n");
    for(int i=0;i<64;i++)
    {
        if((i+1)%8==0)
        {
            int ans=smbit[i-7]*128+smbit[i-6]*64+smbit[i-5]*32+smbit[i-4]*16+smbit[i-3]*8+smbit[i-2]*4+smbit[i-1]*2+smbit[i];
            printf("%c",ans);
        }
    }
    printf("\n");
    return 0;
}*/
