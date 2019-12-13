
#include <cstdio>
#include <cstddef>
#include <cstdint>
#include <climits>
#include <limits>
#include <cmath>
#include <cassert>
#include <tuple>
#include <queue>
#include <set>
#include <vector>
#include <functional>
#include <memory>

using Int = std::int64_t;

static bool canMul(Int a, Int b)
{
	//1. if a or b is zero, a*b = 0
	if (a == 0 || b == 0 )
	   return true;
	
	//2. if a=1 --> a*b = b,  or  b = 1,  a*b = a*1=a
	if (a == 1 || b == 1 )
	   return true;
	
	if (a == -1)
		return (b != std::numeric_limits<Int>::min());
	if (b == -1)
	    return (a != std::numeric_limits<Int>::min());
	    
	//3. if  a  or b   is min or max value of Int, so can't mul they.
	if (a == std::numeric_limits<Int>::max() || a == std::numeric_limits<Int>::min() )
	  return false;
	
	if (b == std::numeric_limits<Int>::max() || b == std::numeric_limits<Int>::min() )
	  return false;
	
	//4. if both a and b are negative, so a*b == (-a)*(-b)     
	if (a < 0 && b < 0)
	  return canMul(-a, -b);
	
	//5. a < 0 && b > 0
	if (a < 0 )
	  return canMul(-a,b) && canMul((-a) * b , Int{-1});
	
	//6. a > 0 && b < 0
	if (b < 0 )
	  return canMul(a, -b) && canMul(a*(-b), Int{-1});
	
	//7. a > 1 && b > 1
	// a * b > Int::max --> a > Int::max / b
	if (a > std::numeric_limits<Int>::max() / b )
	  return false;
	
	
	return true;
	
}

static bool canAdd(Int a, Int b)
{
	if (a == 0 || b == 0)
	 return true;
	
	//   min <= a <= max
	//   min <= b <= max
	//  check   min <= a + b <= max
	if (a>0)
	  return std::numeric_limits<Int>::max() - a >= b;
    if (b>0)
      return true;//a <0 -->  a+b = b - |a|
    
    // a<0 && b < 0
    if (a == std::numeric_limits<Int>::min() ||
        b == std::numeric_limits<Int>::min()
       )
       return false;

   // min <= a + b --> min - a <= b
   return std::numeric_limits<Int>::min() - a <= b;  
}

static bool canSqrt(Int a)
{
	if (a <0)
	  return false;
	
	if ( a <10)
	  return (a == 0 || a == 1 || a==4 || a == 9);
	
	Int s = 0;
	for(int i = 32; i >= 0; --i)
	{
		Int pow2 = Int{1}  << i;
		if (canAdd(s, pow2))
		{
			Int ss = s + pow2;
			if (canMul(ss, ss) && ss * ss <= a )
			{
				
				s = ss;
			}
		}
	}
	return canMul(s,s) && s * s == a;  
}

/*
static bool canFactorial(Int n)
{
	if (n<0 || n > 30) return false;
	Int f = 1;
	while( n > 0 )
	{
		if (!canMul(f,n)) return false;
		f *= n;
		--n;
	}
	return true;	
}
*/

static bool canFactorial(Int n, Int k)
{
	if (n < 0 || k < 1) return false;
	Int f = 1;
	while(n>0)
	{
		if (!canMul(f,n)) return false;
		f *= n;
		n -= k;
	}
	return true;
}

static bool canPower(Int a, Int n)
{
	// 0^0 -> ??
	if (a == 0 && n == 0 )
	   return false;
	   
	 // 0^n = 0  
	if (a == 0)
	  return true;
	
	//1^n = 1
	if (a == 1)
	  return true;
	
	//a^0 = 1
	if (n==0)
	  return true;
	  
	  //(-1)^n = 1 or -1
	if (a == -1)
	  return true;  
	  
	// |a| > 1,  if n < 0  -->  1/a  - is not integer.
	if (n < 0)
	  return false;   
    
   // n == 1   a^1 = a
   if (n == 1)
     return true;
     
   // |a| >=2  -->  n <= 64
   if (n >= 64)
     return false;
  
   Int p{1};
   while(n>0)
   {
	   if (!canMul(p,a))
	     return false;
	   p *= a;
	   --n;
   }  	 
   return true; 
}
static Int Power(Int x, Int n)
{
	assert(canPower(x,n));
	
	   
	 // 0^n = 0  
	if (x == 0)
	  return 0;
	
	//1^n = 1
	if (x == 1)
	  return 1;
	
	//a^0 = 1
	if (n==0)
	  return 1;
	  
	  //(-1)^n = 1 or -1
	if (x == -1)
	  return ( n % 2 != 0) ? -1 : 1;  
	  
	// |a| > 1,  if n < 0  -->  1/a  - is not integer.
	//if (n < 0)
	//  return false;   
    
   // n == 1   a^1 = a
   if (n == 1)
     return x;
     
   // |a| >=2  -->  n <= 64
   //if (n >= 64)
   //  return false;
  
   Int p{1};
   while(n>0)
   {
	   p *= x;
	   --n;
   }  	 
   return p; 
}

static bool canSub(Int a, Int b)
{
	// a - b = a + (-b)
	if (b == 0) return true;
	if (b > 0) return canAdd(a,-b);
	if (b == std::numeric_limits<Int>::min()) {
		// -b = max + 1
		// a - b = a + max + 1
		if(a >= 0)
		  return false;
		return canAdd(a+1, std::numeric_limits<Int>::max());
	}
	return canAdd(a,-b);
}
/*
static Int Factorial(Int n)
{
	Int f = 1;
	while(n>0) f *= n, --n;
	return f;
}
* */
static Int Factorial(Int n, std::size_t k)
{
	//assert(canFactorial(n,k));
	if (!canFactorial(n,k)){
		printf("Factorial(%ld, %zu) can't calculate\n", n, k);
		fflush(stdout);
		exit(-1);
	}
	Int f = 1;
	while(n>0) f *= n, n -= k;
	return f;
}

static Int Sqrt(Int n)
{
	assert(canSqrt(n));
	if (n == 0 || n == 1 )
	   return n;
	   
	Int s{0};
	for(int i= 32; i>=0; --i)
	{
		Int pow2 = Int{1} << i;
		if(canAdd(s,pow2))
		{
			Int ss = s + pow2;
			if (canMul(ss,ss) && ss * ss <= n )
			   s = ss;
		}
	}
	
	return s;
}

static bool canMod(Int a, Int b)
{
	if(b == 0)
	 return false;
	return true;
}
static bool canDiv(Int a, Int b){
	return canMod(a,b) && a % b == 0;
}

struct Number
{
	enum class Kind
	{
		raw,
		plus,
		minus,
		mul,
		div,
		mod,
		sqrt_,
		factorial,
		factorial_2,
		factorial_3,
		factorial_4,
		factorial_5,
		power,
		
		unknown = 99
		
	};
	
	Int value = 0;
	
	std::size_t usedCount = 0;
	
	enum Kind kind = Kind::unknown;
	
	std::shared_ptr<Number> left;
	std::shared_ptr<Number> right;
	
	
	friend bool operator < (const Number& lhs, const Number& rhs)
	{
		return std::make_pair(lhs.value, lhs.usedCount) < std::make_pair(rhs.value, rhs.usedCount);
	}
	
	static char symbol(enum Kind kind)
	{
		switch(kind)
		{
			case Kind::plus: return '+';
			case Kind::minus: return '-';
			case Kind::mul: return '*';
			case Kind::div : return '/';
			case Kind::mod : return '%';
			case Kind::power: return '^';
			default: return '\0';
		}
	}
	
	static Number  fromRaw(Int value)
	{
		Number res;
		res.kind = Kind::raw;
		res.value = value;
		res.left = nullptr;
		res.right = nullptr;
		res.usedCount  = 1;
		return res;
	}
	
	explicit operator bool()const noexcept
	{
		return kind != Kind::unknown;
	}
};

void printNumber(const Number& number)
{
	bool l_raw, r_raw;
	l_raw = number.left != nullptr && ( number.left->kind == Number::Kind::raw || number.left->kind == Number::Kind::factorial || number.left->kind == Number::Kind::sqrt_);
	r_raw = number.right != nullptr && (number.right->kind == Number::Kind::raw|| number.right->kind == Number::Kind::factorial || number.right->kind == Number::Kind::sqrt_);
	
	
	switch(number.kind)
	{
		case Number::Kind::raw  : 
			printf("%ld", number.value); 
			break;
		case Number::Kind::plus :
		case Number::Kind::minus :
		case Number::Kind::mul :
		case Number::Kind::div :
		case Number::Kind::power :
		case Number::Kind::mod :
		{
			const bool lo = !l_raw &&  !(number.kind == Number::Kind::plus || number.kind == Number::Kind::minus);
			const bool ro = !r_raw &&  !(number.kind == Number::Kind::plus);
			if (lo)
			 printf("(");
		
			printNumber(*number.left);
		
			if (lo)
			  printf(")");
			  
			printf(" %c ", Number::symbol(number.kind));
			
			if (ro)
			  printf("(");
			  
			printNumber(*number.right);
			
			if(ro)
			  printf(")");
		  }
		break;
		case Number::Kind::sqrt_ :
			printf("sqrt(");
			printNumber(*number.left);
			printf(")");
		break; 
		case Number::Kind::factorial:
		case Number::Kind::factorial_2:
		case Number::Kind::factorial_3:
		case Number::Kind::factorial_4:
		case Number::Kind::factorial_5:
		{
			const bool o = !l_raw;
			
			if (o)
			  printf("(");
			printNumber(*number.left);
			
			if (o)
			  printf(")");
			
			for(int k = (int)Number::Kind::factorial; k <= (int)number.kind; ++k)
			  printf("!");
		  }
		break;
		case Number::Kind::unknown:
		printf("UNKNOWN!");
		throw std::logic_error("error");
		break;
	}
}

template< typename CheckerOp, typename Op >
Number unaryOpNumber(const Number& a, CheckerOp cho, Op op, Number::Kind kind )
{
	Number result{};
	
	if (cho(a.value))
	{
		result.kind = kind;
		result.value = op(a.value);
		result.usedCount = a.usedCount;
		result.left = std::make_shared<Number>(a);
		result.right = nullptr;
	}
	return result;
}

template< typename CheckerOp, typename Op>
Number binaryOpNumber(const Number& a, const Number& b, CheckerOp cho, Op op, Number::Kind kind)
{
	Number result{};
	if (cho(a.value, b.value))
	{
		result.kind = kind;
		result.value = op(a.value, b.value);
		result.usedCount = a.usedCount + b.usedCount;
		result.left = std::make_shared<Number>(a);
		result.right = std::make_shared<Number>(b);
	}
	return result;
}

Number operator + (const Number& a, const Number & b)
{
	return binaryOpNumber(a, b, canAdd, std::plus<Int>{}, Number::Kind::plus);
}

Number operator - (const Number& a, const Number& b)
{
	return binaryOpNumber(a, b, canSub, std::minus<Int>{}, Number::Kind::minus);
}

Number operator * (const Number& a, const Number & b)
{
	return binaryOpNumber(a, b, canMul, std::multiplies<Int>{}, Number::Kind::mul);
}

Number operator / (const Number& a, const Number & b)
{
	return binaryOpNumber(a, b, canDiv, std::divides<Int>{}, Number::Kind::div);
}

Number operator % (const Number& a, const Number & b)
{
	return binaryOpNumber(a, b, canMod, std::modulus<Int>{}, Number::Kind::mod);
}

Number Sqrt(const Number& a)
{
	return unaryOpNumber(a, static_cast<bool(*)(Int)>(&canSqrt), static_cast<Int(*)(Int)>(&Sqrt), Number::Kind::sqrt_);
}

Number Power(const Number& a, const Number& n)
{
	return binaryOpNumber(a, n, canPower, static_cast<Int(*)(Int, Int)>(&Power), Number::Kind::power);
}

Number Factorial(const Number& n, std::size_t k)
{
	auto check = [k](Int n){ return canFactorial(n, k);};
	auto fact = [k](Int n){ return Factorial(n,k);};
	
	return unaryOpNumber(n, check, fact, static_cast<Number::Kind>((int)Number::Kind::factorial + k - 1));
}

using TSet = std::set<Number>;
struct TOperation
{
	using TUnary = std::function<Number(const Number&)>;
	using TBinary = std::function<Number(const Number&, const Number&)>;
	
	TUnary unary;
	TBinary binary;
	
	template< typename Function>
	TOperation(Function  f)
	{
		if constexpr (std::is_invocable_v<Function, const Number&>)
		{
			unary = f;
		}
		
		if constexpr (std::is_invocable_v<Function, const Number&, const Number&>)
		{
			binary = f;
		}
		
		
	}
	
	Number operator ()(const Number& n)const 
	{
		if (unary) return unary(n);
		else return {};
	}
	
	Number operator ()(const Number& a, const Number & b)const
	{
		if (binary)return binary(a,b);
		return {};
	}
};	



TSet find(TSet const& set, std::size_t maxUsedCount, std::vector<TOperation> const& ops )
{
	TSet memory;
	auto lambda = [&](const Number& r)
	{	
		if ( !r || r.usedCount > maxUsedCount)
		  return ;
		  
		if ( !memory.count(r) && !set.count(r))
		{
			//printf("usedCount: %zu  r = %ld\n", r.usedCount, r.value);
		   memory.insert(r);
	    }
	};
	
	for(const auto& x : set)
	{
		for(auto&& op : ops )
			   lambda( op(x) );
		
		for(const auto& y : set )
		{
			for(auto && op : ops)
			   lambda(op(x, y));
		}  
	}
	
	return memory;
} 

Number solve(Int result, Int a, std::size_t n, std::vector<TOperation> const& ops)
{
	Number resNumber = Number::fromRaw(result);
	resNumber.usedCount = n;
	
	
	TSet cache;
	cache.insert(Number::fromRaw(a)); 

	while(true)
	{
		TSet memory = find(cache, n,  ops);
		if (memory.empty())
		  break;
		
		cache.insert(memory.begin(), memory.end());
		
		if (cache.count(resNumber))
	    	break;
	}
	if (cache.count(resNumber))
	   return *cache.find( resNumber);
	   
	return {};
}

Number solve(Int result, Int a, std::size_t n)
{
	
	auto add = std::plus<Number>{};
	auto sub = std::minus<Number>{};
	auto mul = std::multiplies<Number>{};
	auto div = std::divides<Number>{};
	auto mod = std::modulus<Number>{};
	
	auto fact = [](const Number & n ){ return Factorial(n,1);};
	auto fact2 = [](const Number & n ) { return Factorial(n,2);};
	auto fact3 = [](const Number & n ) { return Factorial(n,3);};
	auto fact4 = [](const Number & n ){ return Factorial(n,4);};
	auto fact5 = [](const Number & n) { return Factorial(n,5);};
	
	auto sqrt_ = [](const Number& n ){ return Sqrt(n);};
	
	auto pow_  = [](const Number& x, const Number& n) { return Power(x,n);};
	
	//const std::vector<TOperation> ops = {add, sub, mul, div, mod, pow_, sqrt_};
	const std::vector<TOperation> ops ={add, sub, mul, div, mod, pow_, sqrt_, fact, fact2, fact3, fact4, fact5};
	std::vector<TOperation> oo;
	for(auto o : ops)
	{
		oo.push_back(o);
		
		auto res =  solve(result, a, n, oo);
		
		if (res)
			return res;
		
	} 
	   
	return {};
	  
}

void solveAndPrint(Int result, Int a, std::size_t n)
{
	Number res = solve(result, a, n);
	if (res.kind == Number::Kind::unknown){
		printf("result = %ld, a = %ld  n = %zu   - CAN'T REPRESENT\n", result, a, n);
	} else {
		printf("%4ld = ", result);
		printNumber(res);
		printf("\n");
		fflush(stdout);
	}
}

int main()
{
	for(int i= 0; i <= 20; ++i)
	{
		//printf("%2d: ",i);
		solveAndPrint(6, i, 3);
	}
	  
	 
	return 0;
}
