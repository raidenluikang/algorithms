#include <fstream>    // std::ifstream, std::ofstream
#include <vector>     // std::vector
#include <string>     // std::string
#include <iterator>   // std::ifstream_iterator, std::oftream_iterator
#include <algorithm>  // std::sort
#include <numeric>    // std::numeric
#include <cassert>     // assert

struct op
{
    int* p ;
    
    unsigned long long operator()(unsigned long long u, char c)const
    {
        if (c == '.' ) 
        {
            assert(p != NULL);
            assert(*p >= 8);
        
            *p -= 8 ;
            
            return u << 8 ;
        }
        
        int b = ( u & 0xff ) * 10 + ( c - '0' );
        
        assert(  ( c >= '0' && c <= '9' ) ) ;
        assert(  b < 256   ) ;
        
        return  ( u & ~0xffull ) +  b  ;
    }
};

static unsigned long long to_int(const std::string& s){
    int n = 56;
    struct op o = {&n};
    unsigned long long result = std::accumulate(s.begin(), s.end(), 0ULL , o ) ;
    return result << n ;
}

static bool version_comparator(const std::string& lhs, const std::string& rhs ) { return to_int(lhs) < to_int(rhs); }
 

int main( int argc, char* argv[] )
{
    std::ifstream input("version.txt");
    std::ofstream output("sorted_version.txt");

    std::vector< std::string > versions(   (std::istream_iterator< std::string >(input)), std::istream_iterator< std::string >() ) ;
    std::sort(versions.begin(), versions.end() , &version_comparator);
    std::copy(versions.begin(), versions.end(), std::ostream_iterator<std::string>(output, "\n") ) ;
}
 
