#ifndef _DH_H
#define _DH_H
#include<stdio.h>
#include<time.h>
#include<math.h>
#include<stdlib.h>

int Random();
int isPrime(int odd);
int MoChongFu(int m, int e,int n);
int Miller_Rabin(int odd);
int yuangen(int yy);

#endif
