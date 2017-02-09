#include "BigNumber.h"

namespace Big_Number
{

	//definition of static members
	const BigNumber BigNumber::big0;
	const BigNumber BigNumber::big1("1");
	const BigNumber BigNumber::big2("2");

	//definition of member functinos
	BigNumber::BigNumber(const std::string& s): sign(true)
	{
		if (s.empty() || (s.size() == 1 && (s[0] == '-' || s[0] == '+')))
		{
			digits.push_back(0);
			return;
		}
		if (s[0] == '-')
			sign = false;
		std::size_t pos = 0;
		if (!std::isdigit(s[0]))
			pos = 1;
		digits.reserve(s.size() - pos);
		for (std::size_t i = s.size(); i-- != pos; )
			digits.push_back(s[i] - '0');
		discard_leading_zeros();
	}

	BigNumber& BigNumber::operator+=(const BigNumber& rhs)
	{
		if (this->sign != rhs.sign)
		{
			if (this->sign)
				*this -= -rhs;
			else
				*this = rhs - -*this;
		}
		else
		{
			digits.reserve(std::max(size(), rhs.size()) + 1);
			unsigned char carry = 0;
			for (std::size_t i = 0; i < size() && i < rhs.size(); ++i)
			{
				digits[i] = digits[i] + rhs.digits[i] + carry;
				carry = digits[i] / 10;
				digits[i] %= 10;
			}
			if (size() > rhs.size())
			{
				for (std::size_t i = rhs.size(); i < size(); ++i)
				{
					digits[i] = digits[i] + carry;
					carry = digits[i] / 10;
					digits[i] %= 10;
				}
			}
			else if (rhs.size() > size())
			{
				for (std::size_t i = size(); i < rhs.size(); ++i)
				{
					digits.push_back(rhs.digits[i] + carry);
					carry = digits[i] / 10;
					digits[i] %= 10;
				}
			}
			if (carry)
				digits.push_back(carry);
		}
		return *this;
	}

	BigNumber& BigNumber::operator<<=(std::size_t n)
	{
		digits.reserve(size() + n);
		std::size_t initial_size = size();
		if (n > initial_size)
		{
			digits.insert(digits.end(), n - initial_size, 0);
			for (std::size_t i = 0; i < initial_size; ++i)
				digits.push_back(digits[i]);
			std::fill(digits.begin(), digits.begin() + initial_size, 0);
		}
		else
		{
			for (std::size_t i = 0; i < n; ++i)
				digits.push_back(digits[initial_size + i - n]);
			for (std::size_t i = initial_size; i-- != n; )
				digits[i] = digits[i - n];
			std::fill(digits.begin(), digits.begin() + n, 0);
		}
		discard_leading_zeros(); //useful when shifting zero
		return *this;
	}

	BigNumber& BigNumber::operator*=(const BigNumber& rhs)
	{
		BigNumber product;
		product.digits.reserve(size() + rhs.size());
		bool initial_this_sign = this->sign;
		bool initial_rhs_sign = rhs.sign; //because rhs can be *this!
		for (std::size_t i = rhs.size(); i-- != 0; )
		{
			product <<= 1;
			product += this->mult_digit(rhs.digits[i]);
		}
		*this = std::move(product); //sign of product is true
		if (initial_rhs_sign != initial_this_sign && *this != big0)
			this->sign = false;
		return *this;
	}

	BigNumber& BigNumber::operator>>=(std::size_t n)
	{
		if (size() <= n)
			return *this = big0;
		for (std::size_t i = 0; i < size() - n; ++i)
			digits[i] = digits[i + n];
		digits.erase(digits.end() - n, digits.end());
		return *this;
	}

	BigNumber& BigNumber::operator/=(const BigNumber& rhs)
	{
		if (rhs == big0)
			throw DivisionByZero("Error: Division by zero.");
		bool initial_this_sign = this->sign;
		bool initial_rhs_sign = rhs.sign; //because rhs can be *this!
		std::size_t initial_shift;
		if (size() <= rhs.size())
			initial_shift = 0;
		else
			initial_shift = size() - rhs.size();
		BigNumber denom = rhs << initial_shift;
		denom.sign = true;
		BigNumber num = std::move(*this);
		num.sign = true;
		BigNumber q;
		for (std::size_t i = 0; i <= initial_shift; ++i)
		{
			unsigned char q_digit = 0;
			while (num >= denom)
			{
				num -= denom;
				++q_digit;
			}
			q <<= 1;
			q.digits[0] = q_digit;
			denom >>= 1;
		}
		*this = std::move(q); //sign of q is true
		if (initial_this_sign != initial_rhs_sign && *this != big0)
			this->sign = false;
		return *this;
	}

	BigNumber& BigNumber::operator-=(const BigNumber& rhs)
	{
		if (this->sign != rhs.sign)
		{
			bool final_sign = this->sign;
			this->sign = true;
			*this += abs(rhs);
			this->sign = final_sign;
		}
		else
		{
			bool final_sign;
			const BigNumber* mi;
			const BigNumber* ma;
			if (*this >= rhs)
			{
				final_sign = true;
				if (!this->sign)
				{
					mi = this;
					ma = &rhs;
				}
				else
				{
					mi = &rhs;
					ma = this;
				}
			}
			else
			{
				final_sign = false;
				if (this->sign)
				{
					mi = this;
					ma = &rhs;
				}
				else
				{
					mi = &rhs;
					ma = this;
				}
			}
			bool borrow = false;
			for (std::size_t i = 0; i < ma->size(); ++i)
			{
				unsigned char sub = borrow;
				if (i < mi->size())
					sub += mi->digits[i];
				unsigned char resulted_digit;
				if (sub > ma->digits[i])
				{
					borrow = true;
					resulted_digit = 10 + ma->digits[i] - sub;
				}
				else
				{
					borrow = false;
					resulted_digit = ma->digits[i] - sub;
				}
				if (i == this->size()) //no need to write i >= this->size() because this->size() is growing with i
					this->digits.push_back(resulted_digit);
				else
					this->digits[i] = resulted_digit;
			}
			discard_leading_zeros();
			this->sign = final_sign;
		}
		return *this;
	}

	BigNumber& BigNumber::operator^=(const BigNumber& rhs)
	{
		//digits.reserve is not practical here, because a^b has floor(b * log10(a)) + 1 digits and b is a BigNumber.
		if (!rhs.sign)
			throw NegativePower("Error: Power is negative.");
		if (rhs == big0)
			return *this = big1;
		if (rhs == big1)
			return *this;
		if (is_even(rhs))
		{
			*this ^= rhs / big2;
			*this *= *this;
		}
		else
		{
			BigNumber temp = *this; //because rhs can be *this!
			*this ^= (rhs - big1) / big2;
			*this *= *this;
			*this *= temp;
		}
		if (is_even(rhs))
			this->sign = true;
		return *this;
	}

	BigNumber BigNumber::mult_digit(unsigned char n) const //n should belong to [0, 9]
	{
		if (n == 0)
			return big0;
		BigNumber ret = *this;
		ret.sign = true;
		unsigned char carry = 0;
		for (std::size_t i = 0; i < size(); ++i)
		{
			ret.digits[i] = n * digits[i] + carry;
			carry = ret.digits[i] / 10;
			ret.digits[i] %= 10;
		}
		if (carry)
			ret.digits.push_back(carry);
		return ret;
	}

	//definition of friend functions
	bool operator<(const BigNumber& lhs, const BigNumber& rhs)
	{
		if (!lhs.sign && rhs.sign)
			return true;
		else if (lhs.sign && !rhs.sign)
			return false;
		else if (!lhs.sign && !rhs.sign)
			return -rhs < -lhs;
		else
		{
			if (lhs.size() < rhs.size())
				return true;
			else if (lhs.size() > rhs.size())
				return false;
			else
			{
				for (std::size_t i = lhs.size(); i-- != 0; )
				{
					if (lhs.digits[i] < rhs.digits[i])
						return true;
					else if (rhs.digits[i] < lhs.digits[i])
						return false;
				}
				return false;
			}
		}
	}

} //end of namespace Big_Number

namespace std
{
	size_t hash<Big_Number::BigNumber>::operator()(const Big_Number::BigNumber& number) const
	{
		ostringstream oss;
		oss << number;
		return hash<string>()(oss.str());
	}
}
