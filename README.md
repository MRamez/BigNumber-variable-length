# BigNumber-variable-length
Variable-length BigNumber implementation in C++11

This class has the same interface as the fixed-length BigNumber, but it does not impose a limit on the number of digits that a BigNumber can hold. As a result, it can be more memory efficient (depending on the size of numbers you are working on) but it's generally slower.
BigNumber has a very simple and natural interface and can be used like primitive types. The code below demonstrates its usage:
```C++
#include "BigNumber.h"

using namespace Big_Number;
using namespace std;

int main()
{
	ios_base::sync_with_stdio(false);
	BigNumber bn1("4");
	bn1 ^= bn1;
	cout<<bn1<<'\n';
	BigNumber bn2("-4");
	cout<<bn2<<'\n';
	bn2 *= bn2;
	cout<<bn2<<'\n';
	bn2 = BigNumber("2") ^ 32;
	cout<<bn2<<'\n';
	BigNumber bn3("38");
	BigNumber bn4("-7");
	cout<<(bn3 / bn4)<<'\n';
	cout<<(bn3 % bn4)<<'\n';
	cout<<(bn3 * bn4)<<'\n';
	cout<<(bn3 - bn4)<<'\n';
	cout<<(-bn3 - bn4)<<'\n';
	cout<<(bn3 - -bn4)<<'\n';
	cout<<(-bn3 - -bn4)<<'\n';
	cout<<(bn3 + bn4)<<'\n';
	cout<<(bn3 % -bn4)<<'\n';
	cout<<(-bn3 % bn4)<<'\n';
	cout<<(-bn3 % -bn4)<<'\n';
	cout<<(bn4 ^ bn3)<<'\n';
	bn3[0] = 4;
	cout<<bn3<<'\n';
	bn4 *= BigNumber("-1");
	cout<<bn4<<'\n';
	cin>>bn4;
	cout<<bn4<<'\n';
	bn4.at(0) = 3;
	for (BigNumber bi = 7; bi-- != 0; )
		cout<<bi<<'\n';
	cout<<(9 * bn4)<<'\n';
	cout<<(bn4 + "384958943")<<'\n';
	return 0;
}
```
Because this class is not a template, friend functions and methods other than inline ones are defined in `BigNumber.cpp`. So, to compile your code, you need to compile BigNumber.cpp and link it to your program. For example, if your code is in `my_code.cpp`, then you can compile it using the following command:
```
g++ my_code.cpp BigNumber.cpp -o output -std=c++11 -O2
```
If you use another compiler, the command should be similar to the one above (But you need to change `g++` to your compiler's name).
