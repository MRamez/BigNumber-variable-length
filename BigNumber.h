#ifndef BIGNUMBER_H
#define BIGNUMBER_H

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <type_traits>
#include <sstream>
#include <cctype>

namespace Big_Number
{

	class DivisionByZero: public std::invalid_argument
	{
		public:
			explicit DivisionByZero(const std::string& msg): std::invalid_argument(msg) {}
	};

	class NegativePower: public std::invalid_argument
	{
		public:
			explicit NegativePower(const std::string& msg): std::invalid_argument(msg) {}
	};

	class BigNumber
	{
		std::vector<unsigned char> digits;
		bool sign;

		public:
			BigNumber(): sign(true) { digits.push_back(0); }
			BigNumber(const std::string&);
			BigNumber(const char* ca): BigNumber(std::string(ca)) {}
			BigNumber(int n): BigNumber(std::to_string(n)) {} //This is required for "BigNumber(0)"
			/*All of the following constructors are required (not just one of them) because otherwise calls to BigNumber constructor with
			certain integral types would have been ambiguous.*/
			BigNumber(unsigned n): BigNumber(std::to_string(n)) {}
			BigNumber(long n): BigNumber(std::to_string(n)) {}
			BigNumber(unsigned long n): BigNumber(std::to_string(n)) {}
			BigNumber(long long n): BigNumber(std::to_string(n)) {}
			BigNumber(unsigned long long n): BigNumber(std::to_string(n)) {}
			
			unsigned char& operator[](std::size_t i) { return digits[i]; }
			const unsigned char& operator[](std::size_t i) const { return digits[i]; }
			unsigned char& at(std::size_t i) { return digits.at(i); }
			const unsigned char& at(std::size_t i) const { return digits.at(i); }
			std::size_t size() const { return digits.size(); }
			void reserve(std::size_t n) { digits.reserve(n); }
			BigNumber& operator+=(const BigNumber&);
			BigNumber& operator<<=(std::size_t);
			BigNumber& operator*=(const BigNumber&);
			BigNumber& operator>>=(std::size_t);
			BigNumber& operator/=(const BigNumber&);
			BigNumber operator-() const;
			BigNumber& operator-=(const BigNumber&);
			BigNumber& operator%=(const BigNumber&);
			BigNumber& operator^=(const BigNumber&);
			BigNumber& operator++() { return *this += big1; }
			BigNumber& operator--() { return *this -= big1; }
			BigNumber operator++(int);
			BigNumber operator--(int);

		private:
			static const BigNumber big0;
			static const BigNumber big1;
			static const BigNumber big2;

			BigNumber mult_digit(unsigned char) const; //accepts only a digit.
			void discard_leading_zeros();

		friend BigNumber operator<<(const BigNumber&, std::size_t);
		friend BigNumber operator+(const BigNumber&, const BigNumber&);
		friend BigNumber operator*(const BigNumber&, const BigNumber&);
		friend BigNumber operator>>(const BigNumber&, std::size_t);
		friend BigNumber operator/(const BigNumber&, const BigNumber&);
		friend BigNumber operator-(const BigNumber&, const BigNumber&);
		friend BigNumber operator%(const BigNumber&, const BigNumber&);
		friend BigNumber abs(const BigNumber&);
		friend BigNumber max(const BigNumber&, const BigNumber&);
		friend BigNumber min(const BigNumber&, const BigNumber&);
		friend bool is_even(const BigNumber&);
		friend bool is_odd(const BigNumber&);
		friend BigNumber operator^(const BigNumber&, const BigNumber&);
		friend bool operator<(const BigNumber&, const BigNumber&);
		friend bool operator>(const BigNumber&, const BigNumber&);
		friend bool operator<=(const BigNumber&, const BigNumber&);
		friend bool operator>=(const BigNumber&, const BigNumber&);
		friend bool operator==(const BigNumber&, const BigNumber&);
		friend bool operator!=(const BigNumber&, const BigNumber&);
		friend std::ostream& operator<<(std::ostream&, const BigNumber&);
		friend std::istream& operator>>(std::istream&, BigNumber&);
		friend void swap(BigNumber&, BigNumber&);
	};

	//definition of inline member functions
	inline BigNumber BigNumber::operator-() const
	{
		if (*this == big0)
			return big0;
		BigNumber ret = *this;
		if (ret.sign)
			ret.sign = false;
		else
			ret.sign = true;
		return ret;
	}

	inline BigNumber& BigNumber::operator%=(const BigNumber& rhs)
	{
		*this -= (*this / rhs) * rhs;
		return *this;
	}

	inline BigNumber BigNumber::operator++(int)
	{
		BigNumber ret = *this;
		++*this;
		return ret;
	}

	inline BigNumber BigNumber::operator--(int)
	{
		BigNumber ret = *this;
		--*this;
		return ret;
	}

	inline void BigNumber::discard_leading_zeros() //Assumption: end != 0
	{
		auto riter = std::find_if(digits.rbegin(), digits.rend(), [] (unsigned char c) { return c != 0; });
		digits.erase(riter.base(), digits.end());
		if (size() == 0)
		{
			digits.push_back(0);
			sign = true;
		}
	}

	//definition of inlined friend functions
	inline BigNumber operator<<(const BigNumber& lhs, std::size_t n)
	{
		BigNumber ret = lhs;
		ret <<= n;
		return ret;
	}

	inline BigNumber operator+(const BigNumber& lhs, const BigNumber& rhs)
	{
		BigNumber temp = lhs;
		temp += rhs;
		return temp;
	}

	inline BigNumber operator*(const BigNumber& lhs, const BigNumber& rhs)
	{
		BigNumber ret = lhs;
		ret *= rhs;
		return ret;
	}

	inline BigNumber operator>>(const BigNumber& lhs, std::size_t n)
	{
		BigNumber ret = lhs;
		ret >>= n;
		return ret;
	}

	inline BigNumber operator/(const BigNumber& lhs, const BigNumber& rhs)
	{
		BigNumber ret = lhs;
		ret /= rhs;
		return ret;
	}

	inline BigNumber operator-(const BigNumber& lhs, const BigNumber& rhs)
	{
		BigNumber ret = lhs;
		ret -= rhs;
		return ret;
	}

	inline BigNumber operator%(const BigNumber& lhs, const BigNumber& rhs)
	{
		BigNumber ret = lhs;
		ret %= rhs;
		return ret;
	}

	inline BigNumber abs(const BigNumber& bn)
	{
		if (bn.sign)
			return bn;
		else
			return -bn;
	}

	inline BigNumber max(const BigNumber& bn1, const BigNumber& bn2)
	{
		if (bn1 < bn2)
			return bn2;
		else
			return bn1;
	}

	inline BigNumber min(const BigNumber& bn1, const BigNumber& bn2)
	{
		if (bn1 < bn2)
			return bn1;
		else
			return bn2;
	}

	inline bool is_even(const BigNumber& bn)
	{
		return bn.digits[0] % 2 == 0;
	}

	inline bool is_odd(const BigNumber& bn)
	{
		return !is_even(bn);
	}

	inline BigNumber operator^(const BigNumber& lhs, const BigNumber& rhs)
	{
		BigNumber ret = lhs;
		ret ^= rhs;
		return ret;
	}

	inline bool operator>(const BigNumber& lhs, const BigNumber& rhs)
	{
		return rhs < lhs;
	}

	inline bool operator<=(const BigNumber& lhs, const BigNumber& rhs)
	{
		return !(rhs < lhs);
	}

	inline bool operator>=(const BigNumber& lhs, const BigNumber& rhs)
	{
		return !(lhs < rhs);
	}

	inline bool operator==(const BigNumber& lhs, const BigNumber& rhs)
	{
		return !(lhs < rhs) && !(rhs < lhs);
	}

	inline bool operator!=(const BigNumber& lhs, const BigNumber& rhs)
	{
		return !(lhs == rhs);
	}

	inline std::ostream& operator<<(std::ostream& os, const BigNumber& bn)
	{
		if (!bn.sign)
			os<<'-';
		for (std::size_t i = bn.size(); i-- != 0;)
			os<<(unsigned)(bn.digits[i]);
		return os;
	}

	inline std::istream& operator>>(std::istream& is, BigNumber& bn)
	{
		std::string s;
		is>>s;
		bn = BigNumber(s);
		return is;
	}

	inline void swap(BigNumber& lhs, BigNumber& rhs)
	{
		using std::swap;
		swap(lhs.digits, rhs.digits);
		swap(lhs.sign, rhs.sign);
	}

} //end of namespace Big_Number

namespace std
{
	template <>
	struct hash<Big_Number::BigNumber>
	{
		typedef size_t result_type;
		typedef Big_Number::BigNumber argument_type;

		size_t operator()(const Big_Number::BigNumber&) const;
	};
}

#endif
