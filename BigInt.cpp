#include <iostream>
#include <string>
#include <algorithm>

class BigInt {
	std::string value; // значение числа
	bool isNeg; // флаг отрицательности

public:
	BigInt(); //check
	BigInt(long x); //check
	BigInt(std::string& value); //check
	BigInt(const BigInt& bigInt);

	const std::string& getValue() const; //check
	const bool getIsNeg() const; // check

	const bool operator == (const BigInt& bigInt) const; //check
	const bool operator != (const BigInt& bigInt) const; //check
	const bool operator < (const BigInt& bigInt) const; //check 
	const bool operator > (const BigInt& bigInt) const; // check

	BigInt& operator = (const BigInt& bigInt); //check

	BigInt operator + (const BigInt& bigInt) const;
	BigInt operator - (const BigInt& bigInt) const;
	BigInt operator * (const BigInt& bigInt) const;

	BigInt operator+(); //check
	BigInt operator-() const&&; //check

	friend std::istream& operator >> (std::istream& stream, BigInt& bigInt); //check
	friend std::ostream& operator << (std::ostream& stream, const BigInt& bigInt); //check
};

BigInt::BigInt() {
	this->value = "0";
	this->isNeg = false;
}

BigInt::BigInt(long x) {
	this->isNeg = x < 0 ? true : false;
	this->value = std::to_string(isNeg ? -x : x);
}

BigInt::BigInt(std::string& value) {
	if (value.length() == 0) {
		this->value = "0";
		this->isNeg = false;
		return;
	}

	isNeg = value[0] == '-';
	this->value = value.substr(isNeg);
	int counter = 0;
	while (this->value[counter] == '0' && (this->value.length() - counter) > 1) {
		counter++;
	}

	this->value = this->value.substr(counter);

	for (size_t i = 0; i < this->value.length(); i++) {
		if (this->value[i] < '0' || this->value[i] > '9') {
			throw;
		}
	}
}

BigInt::BigInt(const BigInt& bigInt) {
	this->value = bigInt.getValue();
	this->isNeg = bigInt.getIsNeg();
}

const std::string& BigInt::getValue() const {
	return value;
}

const bool BigInt::getIsNeg() const {
	return isNeg;
}

const bool BigInt::operator == (const BigInt& bigInt) const {
	return ((value == bigInt.getValue() && isNeg == bigInt.getIsNeg()) ? true : false);
}

const bool BigInt::operator != (const BigInt& bigInt) const {
	return (value != bigInt.getValue() || isNeg != bigInt.getIsNeg()) ? true : false;
}

const bool BigInt::operator < (const BigInt& bigInt) const {
	std::string value2 = bigInt.getValue();
	int len1 = value.length();
	int len2 = value2.length();

	if (isNeg == bigInt.getIsNeg()) {
		if (len1 != len2) {
			return (len1 < len2) ^ isNeg;
		}
		int i = 0;

		while (i < len1 && value[i] == value2[i]) {
			i++;
		}

		return (value[i] < value2[i]) ^ isNeg ? true : false;
	}
	return isNeg;
}

const bool BigInt::operator > (const BigInt& bigInt) const {
	return !(*this < bigInt || *this == bigInt);;
}

BigInt& BigInt::operator = (const BigInt& bigInt) {
	value = bigInt.getValue();
	isNeg = bigInt.getIsNeg();

	return *this;
}

BigInt BigInt::operator +() {
	return BigInt(*this);
}

BigInt BigInt::operator -() const&& {
	std::string result = isNeg ? value : std::string("-") + value;
	return BigInt(result);
}

BigInt BigInt::operator + (const BigInt& bigInt) const {
	if (isNeg == bigInt.getIsNeg()) {
		std::string num2 = bigInt.getValue();

		size_t len1 = value.length(); // 2
		size_t len2 = num2.length(); // 2
		size_t length = 1 + std::max(len1, len2); // 3 //результат суммы равен максимальной длине одного из чисел (+ 1 из-за возможного смещения разряда)

		int* a = new int[length];
		int* b = new int[length];
		char* res = new char[length + 1];
		res[length] = '\0';

		for (size_t i = 0; i < length; i++) {
			a[i] = (i < len1) ? (value[len1 - 1 - i] - '0') : 0;
			b[i] = (i < len2) ? (num2[len2 - 1 - i] - '0') : 0;
			res[i] = 0;
		}

		for (size_t i = 0; i < length; i++) {
			res[length - 1 - i] += a[i] + b[i];
			res[length - 1 - 1 - i] += res[length - 1 - i] / 10;
			res[length - 1 - i] %= 10;
		}

		for (size_t i = 0; i < length; i++) {
			res[length - 1 - i] += '0';
		}

		std::string result = isNeg ? std::string("-") + std::string(res) : std::string(res);
		delete[] res;
		return BigInt(result);
	}
	else {
		return isNeg ? (bigInt - (-BigInt(*this))) : (*this - (-BigInt(bigInt)));
	}
}

BigInt BigInt::operator - (const BigInt& bigInt) const {
	if (*this == bigInt) {
		return 0;
	}
	if (!isNeg && !bigInt.getIsNeg()) {
		std::string value2 = bigInt.getValue();

		size_t len1 = value.length();
		size_t len2 = value2.length();
		size_t length = std::max(len1, len2);

		int* a = new int[length];
		int* b = new int[length];

		bool isNegRes = bigInt > * this;

		char* res = new char[length + 1];
		res[length] = '\0';

		for (size_t i = 0; i < length; i++) {
			a[i] = (i < len1) ? (value[len1 - 1 - i] - '0') : 0;
			b[i] = (i < len2) ? (value2[len2 - 1 - i] - '0') : 0;
			res[i] = 0;
		}

		for (size_t i = 0; i < length; i++) {
			if (!isNegRes) {
				if (a[i] - b[i] < 0) {
					a[i + 1] = -1;
					res[length - 1 - i] += 10 + (a[i] - b[i]);
					res[length - 1 - i - 1] += res[length - 1 - i] / 10;
					res[length - 1 - i] %= 10;
				}
				else {
					res[length - 1 - i] += (a[i] - b[i]);
					res[length - 1 - i - 1] += res[length - 1 - i] / 10;
					res[length - 1 - i] %= 10;
				}
			}
			else {
				if (b[i] - a[i] < 0) {
					b[i + 1] = -1;
					res[length - 1 - i] += 10 + (b[i] - a[i]);
					res[length - 1 - i - 1] += res[length - 1 - i] / 10;
					res[length - 1 - i] %= 10;
				}
				else {
					res[length - 1 - i] += (b[i] - a[i]);
					res[length - 1 - i - 1] += res[length - 1 - i] / 10;
					res[length - 1 - i] %= 10;
				}
			}
		}

		for (size_t i = 0; i < length; i++) {
			res[length - 1 - i] += '0';
		}

		std::string result = isNegRes ? std::string("-") + std::string(res) : std::string(res);
		return BigInt(result);

	}
	else {
		return isNeg && bigInt.getIsNeg() ? (-BigInt(bigInt) - (-BigInt(*this))) : (*this + -BigInt(bigInt));
	}
}

BigInt BigInt::operator * (const BigInt& bigInt) const {
	if (value == "0" || bigInt.getValue() == "0") {
		return 0;
	}
	std::string value2 = bigInt.getValue();

	size_t len1 = value.length(); // 2
	size_t len2 = value2.length(); // 2
	size_t length = len1 + len2 + 1; // 5

	bool isNegRes = isNeg ^ bigInt.getIsNeg();

	int* a = new int[length];
	int* b = new int[length];
	char* res = new char[length + 1];
	res[length] = '\0';

	for (size_t i = 0; i < length; i++) {
		a[i] = (i < len1) ? (value[len1 - 1 - i] - '0') : 0;
		b[i] = (i < len2) ? (value2[len2 - 1 - i] - '0') : 0;
		res[i] = 0;
	}

	for (size_t i = 0; i < len1; i++) {
		for (size_t j = 0; j < len2; j++) {
			res[length - 1 - (i + j)] += a[i] * b[j];
			res[length - 1 - (i + j + 1)] += res[length - 1 - (i + j)] / 10;
			res[length - 1 - (i + j)] %= 10;
		}
	}
	for (size_t i = 0; i < length; i++) {
		res[length - 1 - i] += '0';
	}
	std::string result = isNegRes ? std::string("-") + std::string(res) : std::string(res);
	delete[] a;
	delete[] b;
	delete[] res;

	return BigInt(result);
}

std::istream& operator >> (std::istream& stream, BigInt& bigInt) {
	std::string value;
	stream >> value;
	bigInt = BigInt(value);

	return stream;
}

std::ostream& operator << (std::ostream& stream, const BigInt& bigInt) {
	if (bigInt.getIsNeg()) {
		stream << "-";
	}
	return stream << bigInt.getValue();
}

int main() {
	BigInt a;
	BigInt b;
	std::cin >> a;
	std::cin >> b;
	std::cout << a - b;
	//std::cout << a - b << "\n";
	//std::cout << a * b << "\n";

}