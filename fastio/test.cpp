#include <cstdio>
#include <cstring>
#include <chrono>



typedef long long ll;
static const int N = 1<<20;//10^5
static const int M = 500000 + 8;//5*10^5
static const int oo = 1000000000;

int a[N];
int n,m;

 
char in[1<<23];

char out[1<<23];

struct scanner
{
    char const* o;
    scanner(): o(in){
        in[fread(in,1,sizeof(in)-4,stdin)] = 0;
    }
    int readInt()
    { 
        unsigned u = 0, s=  0; 
        while(*o && *o <= 32)++o;
        if (*o == '-')s = ~0,++o;else if (*o == '+')++o;
        while(*o >='0' && *o <='9')u = (u << 3) + (u << 1) + (*o++ -'0'); 
        return (u^s) + !!s;
    }
    
};

struct writer
{
    char * w; 
    writer() : w(out){}
    ~writer(){ flush(); }
    void writeInt( ll u, char c)
    { 
        if (u < 0){*w++ = '-'; u = -u;}
        ll v = u; 
        int n = 0,m; 

        do ++n; while(v/=10); 
        m = n; 
        while(m-->0)w[m] = u%10 + '0',u/=10; 
        w+=n;
        *w++=c;
    }
    void flush(){ if (w != out) fwrite(out, 1, w - out, stdout), w  = out;}
};

struct auto_cpu_timer
{
    typedef std::chrono::high_resolution_clock ct;
    typedef ct::time_point tp;
    
    tp s,e;
    
    auto_cpu_timer(){ s = ct::now();}
    ~auto_cpu_timer(){ e = ct::now(); 
    long int n = std::chrono::duration_cast< std::chrono::milliseconds > ( e - s).count();
    
    fprintf(stderr,"\nelapsed time: %ld ms.\n", n);
    }
};

int solve()
{
    auto_cpu_timer cpu_timer;
    
    scanner sc;
    writer pw;
    n = sc.readInt();
    
    for(int i = 0; i < n; ++i)a[i] = sc.readInt();
    
    //sort(a, a + n);
    
    for(int i = 0; i < n; ++i)pw.writeInt(a[i], ' ');
    
    
    return 0;
}

int main()
{
#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
#endif 

	solve();
	
	return 0;
}
