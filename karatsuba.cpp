#define NDEBUG 1

#include <cstdio>

#include <cassert>
#include <vector>

#include <chrono>
#include <cstring>

#define PP_VALIDATE 1


namespace
{
    constexpr int base = 1000000000; // 10^9
    constexpr int base_digits = 9;



    void validate(const int a[], const int n)
    {
#ifdef NDEBUG
        (void)0; // do nothing.
#else
        assert(n > 0);

        for (int i = 0; i < n; i++)
        {
            assert(a[i] >= 0);

            assert(a[i] < base);
        }
        assert( (n == 1) || (a[n-1] > 0) ); // last part must not be 0.
#endif

    }

    int add_bigint(const int a [], const int n, const int b[], const int m, int c[])
    {
        int q = 0;
        int i = 0;

        validate(a, n);
        validate(b, m);

        while (i < n && i < m)
        {

            q += a[i] + b[i]; // 1 + 10^9 + 10^9 = 2*10^9 + 1 < 2^31 = MAX_INT
            if (q < base) // likely
            {
                c[i] = q;
                q = 0;
            }
            else
            {
                c[i] = q - base;
                q = 1;
            }
            i += 1;
        }

        while (i < n)
        {
            q += a[i];

            if (q < base)
            {
                c[i] = q;
                q = 0;
            }
            else
            {
                c[i] = q - base;
                q = 1;
            }
            i += 1;
        }

        while (i < m)
        {
            q += b[i];
            if (q < base)
            {
                c[i] = q;
                q = 0;
            }
            else
            {
                c[i] = q - base;
                q = 1;
            }
            i += 1;
        }

        if (q > 0)
        {
            c[i] = q;
            i += 1;
        }

        validate(c, i);



        return i;
    }

    int sub_bigint(const int a [], const int n, const int b[], const int m, int c[])
    {
        int q = 0;
        int i = 0;

        assert(n >= m);
        // there must be a >= b
        validate(a, n);
        validate(b, m);

        while (i < m)
        {
            q += a[i] - b[i];
            if (q >= 0)
            {
                c[i]  = q;
                q = 0;
            }
            else
            {
                c[i] = q + base;
                q = -1;
            }
            i += 1;
        }

        while (i < n && q != 0)
        {
            q += a[i];

            if (q >= 0)
            {
                c[i] = q;
                q = 0;
            }
            else
            {
                c[i] = q + base;
                q = -1;
            }
            i += 1;
        }

        if (q != 0)
        {
            printf("Assertion\n");
        }
        assert(q == 0);

        while (i < n)
        {
            c[i] = a[i];
            i += 1;
        }

        //  11121 - 11120 = 00001 -> 1
        while (i > 1 && c[i-1] == 0)
        {
            i -= 1;
        }
        validate(c, i);
        return i;
    }

    int mul_x(const int x, const int a[], const int n, int c[])
    {
        assert(x >= 0 && x < base);

        validate(a, n);

        //x = 0 or a = 0 cases.
        if (x == 0)
        {
            // 0 * a = 0
            c[ 0 ] = 0;
            return 1;
        }

        if (n == 1 && a[0] == 0)
        {
            // x * 0 = 0
            c[0] = 0;
            return 1;
        }

        if (x == 1)
        {
            //1 * a = a
            std::memcpy(c, a, n * sizeof(int));
            return n;
        }

        int64_t q = 0;
        int64_t y = x;
        int i = 0;
        while (i < n)
        {
            q += y * a[i];
            c[i] = static_cast<int>(q % base);
            q /= base;

            i += 1;
        }
        if (q > 0)
        {
            c[i] = static_cast<int>(q % base);
           // q /= base;
            i += 1;
        }
        validate(c, i);
        return i;
    }

    int mul_bigint(const int a [], const int n, const int b[], const int m, int c[])
    {
        validate(a, n);
        validate(b, m);

        if (n == 1 )
        {
            return mul_x(a[0], b, m, c);
        }

        if (m == 1)
        {
            return mul_x(b[0], a, n, c);
        }

        // common cases where a > 0 and b > 0
        const int cn = n + m;

        std::memset(c, 0, cn * sizeof( int ) );

        for (int i = 0; i < n; i++)
        {
            const int64_t ai = a[i];

            int64_t q = 0;

            for (int j = 0; j < m; j ++)
            {
                q += ai * b[j] + c[i + j];

                c[i + j] = static_cast< int >( q % base ) ;

                q /= base;
            }

            if (q > 0)
            {
                int j = m;
                while (q > 0)
                {
                    q += c[i + j];
                    c[i + j] = static_cast<int>( q % base );
                    q /= base;
                    j += 1;
                }
            }
        }

        int ret = cn;

        while (ret > 1 && c[ret - 1] == 0)
        {
            ret -= 1;
        }
        validate(c, ret);
        return ret;
    }

    int mul_karatsuba(const int a [], const int n, const int b[], const int m, int c[], int tmp[])
    {
        if (n < 32 && m < 32)
        {
            return mul_bigint(a, n, b, m, c);
        }

        if (n < 4)
        {
            return mul_bigint(a, n, b, m, c);
        }

        if (m < 4)
        {
            return mul_bigint(b, m, a, n, c);
        }

        const int t = std::min(n,m)/2;


        // a = a1 * 10^t + a2    ,  0 <= a2 < 10^t
        // b = b1 * 10^t + b2    ,  0 <= b2 < 10^t

        // a * b = a1 * b1 * 10^2t + (a1*b2 + a2*b1)*10^t + a2*b2
        //  a1*b2 + a2*b1 = (a1+a2)*(b1+b2) - a1*b1 - a2*b2

        //1. (a1 + a2) ->
        int ns1 = add_bigint(a, t, a + t, n - t, c);
        assert(ns1 > 0 && ns1 <= n + 1); // t + (n-t) + 1 = n + 1

        //2. (b1 + b2) ->
        int ns2 = add_bigint(b, t, b + t, m - t, c + ns1);
        assert(ns2 > 0 && ns2 <= m + 1);

        //3. (a1 + a2)*(b1+b2) ->
        int ns3 = mul_karatsuba(c, ns1, c + ns1, ns2, tmp, tmp + ns1 + ns2);

        assert(ns3 > 0 && ns3 <= ns1 + ns2);

        // tmp = (a1 + a2)*(b1+b2)

        //4. a1 * b1 ->
        int ns4 = mul_karatsuba(a + t, n - t, b + t, m - t,  c + 2 * t, tmp + ns3);
        assert(ns4 > 0 && ns4 <= (n-t) + (m-t) );

        // c[2*t ..) = a1*b1

        //5. a2*b2 ->
        int ns5 = mul_karatsuba(a, t, b, t, c, tmp + ns3);
        assert(ns5 > 0 && ns5 <= t + t);

        // c[0..2*t) = a1*b1
        if (ns5 < 2*t)
        {
            // agar ns5 < 2*t -> urtadagi sonlarni 0 ga uzlashtirib chiqamiz.
            std::memset(c + ns5, 0, (2*t - ns5) * sizeof(int));
        }

        //6. (a1+a2) * (b1+b2) - a1*b1
        ns3 = sub_bigint(tmp, ns3, c + 2*t, ns4, tmp);

        //7. (a1+a2)*(b1+b2) - a1*b1  - a2*b2
        ns3 = sub_bigint(tmp, ns3, c, ns5, tmp);

        //8. tmp  ni c[t .. ] qushamiz.
        int ns6 = add_bigint(tmp, ns3, c + t, t + ns4, c + t);

        return ns6 + t;
    }

    std::vector<int> pow2_karatsuba(const int n)
    {
        constexpr double log10_2 = 0.30102999566398119521373889472449;
        const int digits_count = 1 + static_cast<int>( n * log10_2 ) ;
        const int len = 2 *  (1 + digits_count / base_digits);
        // log2(len) * len
        std::vector<int> va, vc, vtmp, vf;

        va.resize(len );
        vc.resize(len);
        vf.resize(len);

        vtmp.resize(len * 32);

        int*  a = va.data();
        int*  c = vc.data();
        int* const tmp = vtmp.data();
        int*  f = vf.data();


        a[ 0 ] = 2; // a = 2
        c[ 0 ] = 1; // c = 1

        int fn;

        int an = 1;
        int cn = 1;

        int rn = n;
        while (rn > 1)
        {
            if (rn & 1)
            {
                // c = c * a
                fn = mul_karatsuba(a, an, c, cn, f, tmp);

                //copy f -> c
                std::swap(f, c);

                cn = fn;
            }
            // a = a * a
            fn = mul_karatsuba(a, an, a, an, f, tmp);

            //copy f --> a
            std::swap(a, f);

            an = fn;

            rn /= 2;
        }

        if (rn == 1)
        {
            // c = c * a
            fn = mul_karatsuba(a, an, c, cn, f, tmp);
            //copy f -> c
            std::swap(c, f);

            cn = fn;
        }

        assert(c == vc.data() || c == vf.data() || c == va.data());

        if (c == vf.data())
        {
            std::swap(vc, vf);
        } else if (c == va.data())
        {
            std::swap(vc, va);
        }
        vc.resize(cn);
        return vc;
    }

    int pow2_old(int n,  int d[])
    {
        int dn = 1;
        d[0] = 1;

        for (int i = 1; i <= n; i++)
        {
            int q = 0;
            for (int j = 0; j < dn; j++)
            {
                q += d[j] << 1;
                d[j] = q % base;
                q /= base;
            }
            if (q > 0)
            {
                d[dn++] = q;
            }
        }
        return dn;
    }

    int println_bigint(const char* msg, const int a[], const int n)
    {
        validate(a, n);
        printf("%s %d", msg, a[n-1]);
        for (int i = n - 2; i >= 0; --i)
        {
            printf("%09d", a[i]);
        }
        printf("\n");
        return 0;
    }

    void test_old(int n)
    {
        const double log10_2 = 0.30102999566398119521373889472449;
        const int digits_count = 1 + static_cast<int>( n * log10_2 ) ;
        const int len = 1 + digits_count / base_digits;


        const auto start_time = std::chrono::system_clock::now();

        std::vector<int> digits( len  );

        const int dn = pow2_old(n, digits.data());

        const auto finish_time = std::chrono::system_clock::now();
        const auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(finish_time - start_time);

        printf("2^%d",n);
       // println_bigint(" = ", digits.data(), dn);

        printf("\nelapsed time: %ld milliseconds\n", static_cast<long>(elapsed_ms.count()));


    }

    void test_karatsuba(int n)
    {
        const auto start_time = std::chrono::system_clock::now();

        std::vector<int> digits = pow2_karatsuba(n);

        int dn = digits.size();

        const auto finish_time = std::chrono::system_clock::now();
        const auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(finish_time - start_time);

        printf("2^%d",n);
        //println_bigint(" = ", digits.data(), dn);

        printf("\nelapsed time: %ld milliseconds\n", static_cast<long>(elapsed_ms.count()));

       
    }
} // anonymous namespace


int main() {

#ifdef __GNUC__
    printf("GCC Version: %d.%d.%d\n", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
#endif

    int const n = 1000;
    test_old(n);

    int const m = 1000000;
    test_karatsuba(m);
    return 0;
}
