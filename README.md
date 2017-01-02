# BigNumber-variable-length
Variable-length BigNumber implementation in C++11

This class has the same interface as the fixed-length BigNumber, but it does not impose a limit on number of digits that a BigNumber can hold. As a result, it can be more memory efficient (depending on the size of numbers you are working on) but it's generally slower.
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
