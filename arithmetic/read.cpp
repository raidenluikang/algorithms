
// in C++ integer types separated as signed and unsigned
// 1.  signed char   , unsigned char     ,  1 byte
// 2.  signed short  , unsigned short    ,  2 byte at most situations
// 3.  signed int    , unsigned int      ,  4 byte at most situations
// 4.  signed long   , unsigned long     ,  4 byte at most situations
// 5.  signed long long , unsigned long long, 8 bytes at most situations.
//   x bytes signed integer type interval is [ -2^(x*8) .. (2^(x*8) - 1)]
//   x bytes unsigned  integer type interval is [0 ... 2^(x*8) - 1]

// so   unsigned short   value interval is [0..2^16-1]
//      unsigned short sh  = 10000;  sh - value is 10000
//      unsigned short sh2 = 100000;  sh2 - value is not 100000, sh2 = 100000 MOD 2^16 = 44464

//      unsigned char c = 200; // c - 200
//      unsigned char c = 300; // c != 300,  c == 300 MOD 256 = 44.

//    signed - is optinonal:   signed int with int, 
//   signed short with short, signed long with long,
//   signed long long with long long are identical.
//   Only  signed char and char - are different types.  char may signed or unsigned, so valid char values are [0..127].


#include <stdio.h>
#include <iostream>
int main()
{
     signed char ic;
     unsigned char uc;
     
     signed short ish; // 
     unsigned short ush;
     
     signed int  ii;
     unsigned int ui;
     
     signed long il;
     unsigned long ul;
     
     signed long long ill;
     unsigned long long ull;

    // C  style
    scanf("%c",&ic);  scanf("%c",&uc);
    
    //scanf("%d",&ish);  short I don't know, yet )).
    
    scanf("%d", &ii);
    scanf("%u",&ui);
    
    scanf("%ld",&il);
    scaf("%lu",&ul);
    
    scanf("%lld",&ill);
    scanf("%llu",&ull);
    //------------------------
    
    
    //C++ style
    using std::cin; // or using namespace std;
    
    cin >> ic;
    cin >> uc;
    
    cin >> ish;
    cin >> ush;
    
    cin >> ii;
    cin >> ui;
    
    cin >> il;
    cin >> ul;
    
    cin >> ill;
    cin >> ull;
    
    // C++ style very easy, but it's slowly than C style reading.
    

}
