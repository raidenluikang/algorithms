

/***
   Find all possible expressions which have five digits of 5, evaluate result is 100.
   
(5 + 5) ^ ((5 + 5) / 5) = 100
(5 - 5 / 5) * 5 * 5 = 100
5 * (5 + 5 + 5 + 5) = 100
5 * 5 * (5 - 5 / 5) = 100
5 * 5 * 5 - 5 * 5 = 100
*/


#include <cstdint>
#include <string>
#include <utility>
#include <vector>
#include <stdio.h>
#include <limits>
#include <set>

using i64 = std::int64_t;

using list = std::vector< std::pair<i64, std::string> > ;

std::set<std::string> ss;

std::string bracket(std::string x, std::string y, char op)
{
	if (op == '+')
		return x +  " + " + y;
	
	if (op == '-')
	{
		int b = 0;
		bool has = false;
		for (size_t i = 0; i < y.size(); ++i){
			if (y[i] == '(') ++ b;
			else if (y[i] == ')') -- b;
			else if ( (y[i] == '-' || y[i] == '+') && b == 0){
				has = true;
				break;
			}
		}
		if (has)
			return x + " - (" + y + ")";
		else
			return x + " - " + y;
	}
	
	if (op == '*')
	{
		int b = 0;
		bool has_y = false;
		for (size_t i = 0; i < y.size(); ++i)
		{
			if (y[i] == '(') ++ b;
			else if (y[i] == ')') -- b;
			else if (y[i] == '+' || y[i] == '-' || y[i] == '/'){
				if ( b == 0){
					has_y = true;
					break;
				}
			}
		}
		
		b = 0;
		bool has_x = false;
		for (size_t i = 0; i < x.size(); ++i){
			if (x[i] == '(') ++ b;
			else if (x[i] == ')') --b;
			else if (x[i] == '+' || x[i] == '-'  || x[i] == '/') {
				if (b == 0){
					has_x = true;
					break;
				}
			}
		}
		
		if (has_y) y = "(" + y + ")";
		if (has_x) x = "(" + x + ")";
		return x + " * " + y;
	}
	
	if (op == '/')
	{
		int b = 0;
		bool has_y = false;
		for (size_t i = 0; i < y.size(); ++i)
		{
			if (y[i] == '(') ++ b;
			else if (y[i] == ')') -- b;
			else if (y[i] == '+' || y[i] == '-' || y[i] == '/' || y[i] == '*' ){
				if ( b == 0){
					has_y = true;
					break;
				}
			}
		}
		
		b = 0;
		bool has_x = false;
		for (size_t i = 0; i < x.size(); ++i){
			if (x[i] == '(') ++ b;
			else if (x[i] == ')') --b;
			else if (x[i] == '+' || x[i] == '-'  || x[i] == '/' || x[i] == '*' ) {
				if (b == 0){
					has_x = true;
					break;
				}
			}
		}
		
		if (has_y) y = "(" + y + ")";
		if (has_x) x = "(" + x + ")";
		return x + " / " + y;
	}
	
	if (op == '^')
	{
		int b = 0;
		bool has_y = false;
		for (size_t i = 0; i < y.size(); ++i)
		{
			if (y[i] == '(') ++ b;
			else if (y[i] == ')') -- b;
			else if (y[i] == '+' || y[i] == '-' || y[i] == '/' || y[i] == '*' || y[i] == '^' ){
				if ( b == 0){
					has_y = true;
					break;
				}
			}
		}
		
		b = 0;
		bool has_x = false;
		for (size_t i = 0; i < x.size(); ++i){
			if (x[i] == '(') ++ b;
			else if (x[i] == ')') --b;
			else if (x[i] == '+' || x[i] == '-'  || x[i] == '/' || x[i] == '*' || x[i] == '^') {
				if (b == 0){
					has_x = true;
					break;
				}
			}
		}
		
		if (has_y) y = "(" + y + ")";
		if (has_x) x = "(" + x + ")";
		return x + " ^ " + y;
	}
	
	printf("Incorrect op = %c\n", op);
	exit(0);
}
void print_all(const list& v)
{
	if (v.empty())return;
	if (v.size() == 1){
		if (v[0].first == 100)
		{
			//printf("%s = 100\n", v[0].second.c_str());
			ss.insert(v[0].second);
		}
		return;
	}
	
	for (size_t x = 0; x < v.size(); ++x)
	{
		for (size_t y = x + 1; y < v.size(); ++y)
		{
			list w = v;
			w.erase(w.begin() + y);
			w.erase(w.begin() + x);
			auto xp = v[x];
			auto yp = v[y];
			auto zp = std::make_pair(i64{0}, std::string{});
			
			//1. Add xp + yp
			zp.first = xp.first + yp.first;
			zp.second = bracket(xp.second, yp.second, '+');  //"(" + xp.second + ") + (" + yp.second + ")";
			
			w.push_back(zp);
			print_all(w);
			w.pop_back();
			
			//2. Sub xp - yp
			zp.first = xp.first - yp.first;
			zp.second = bracket(xp.second, yp.second, '-');  //"(" + xp.second + ") - (" + yp.second + ")";
			
			w.push_back(zp);
			print_all(w);
			w.pop_back();
			
			//3. Sub yp - xp
			zp.first = yp.first - xp.first;
			zp.second = bracket(yp.second, xp.second, '-'); //"(" + yp.second + ") - (" + xp.second + ")";
			
			w.push_back(zp);
			print_all(w);
			w.pop_back();
			
			
			//4. Mul xp * yp
			
			zp.first = xp.first * yp.first;
			zp.second = bracket(xp.second, yp.second, '*');//"(" + xp.second + ") * (" + yp.second + ")";
			
			w.push_back(zp);
			print_all(w);
			w.pop_back();
			
			
			//5. Div xp / yp
			if (yp.first != 0 && xp.first % yp.first == 0)
			{
				zp.first = xp.first / yp.first;
				zp.second = bracket(xp.second, yp.second, '/'); //"(" + xp.second + ") / (" + yp.second + ")";
				
				w.push_back(zp);
				print_all(w);
				w.pop_back();
				
			}
			
			//6. Div yp / xp
			if (xp.first != 0 && yp.first % xp.first == 0)
			{
				zp.first = yp.first / xp.first;
				zp.second = bracket(yp.second, xp.second, '/'); //"(" + yp.second + ") / (" + xp.second + ")";
				
				w.push_back(zp);
				print_all(w);
				w.pop_back();
				
			}
			
			//7. Pow  xp ^ yp
			if (yp.first >= 0 && yp.first <= 100)
			{
				i64 max = i64(1e15);
				zp.first = 1;
				bool fail = false;
				for (i64 i = 0; i < yp.first; ++i)
				{
					if (zp.first * xp.first > max){
						fail = true;
						break;
					}
					zp.first *= xp.first;
				}
				
				if (!fail)
				{
					zp.second = bracket(xp.second, yp.second, '^');//"(" + xp.second + ") ^ (" + yp.second + ")";
					w.push_back(zp);
					print_all(w);
					w.pop_back();
				}
			}
			
			//8. Pow yp ^ xp
			if (xp.first >= 0 && xp.first <= 100)
			{
				i64 max = i64(1e15);
				zp.first = 1;
				bool fail = false;
				for (i64 i = 0; i < xp.first; ++i)
				{
					if (zp.first * yp.first > max){
						fail = true;
						break;
					}
					zp.first *= yp.first;
				}
				
				if (!fail)
				{
					zp.second = bracket(yp.second, xp.second, '^');//"(" + yp.second + ") ^ (" + xp.second + ")";
					w.push_back(zp);
					print_all(w);
					w.pop_back();
				}
			}
		}
	}
}


int main(){

	const list v = { {5, "5" }, {5, "5" }, {5, "5" }, {5, "5" }, {5, "5" } };
	ss.clear();
	print_all(v);
	for (const std::string& s : ss )
	{
		printf("%s = 100\n", s.c_str());
	}
}
