#include <iostream>
#include <string>

class BigInt {
	std::string value; // значение числа
	bool isNeg; // флаг отрицательности

public:
	BigInt(); //check
	BigInt(long x); //check
	BigInt(std::string &value); //check
	BigInt(const BigInt& bigInt);

	const std::string &getValue() const; //check
	const bool getIsNeg() const; // check

	const bool operator == (const BigInt &bigInt); //check
	const bool operator != (const BigInt &bigInt); //check
	const bool operator < (const BigInt &bigInt); //check проверил работоспособность
	const bool operator > (const BigInt &bigInt); // check
	const bool operator >= (const BigInt &bigInt);
	const bool operator <= (const BigInt &bigInt);

	BigInt &operator = (const BigInt &bigInt); //check

	BigInt operator + (const BigInt &bigInt);

	BigInt operator+(); //check
	BigInt operator-(); //check

	friend std::istream &operator >> (std::istream &stream, BigInt &bigInt); //check
	friend std::ostream &operator << (std::ostream &stream, const BigInt &bigInt); //check
};

BigInt::BigInt() {
	this->value = "0";
	this->isNeg = false;
}

BigInt::BigInt(long x) {
	this->isNeg = x < 0 ? true : false;
	this->value = std::to_string(isNeg ? -x : x);
}

BigInt::BigInt(std::string &value) {
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

	for (int i = 0; i < this->value.length(); i++) {
		if (this->value[i] < '0' || this->value[i] > '9') {
			throw;
		}
	}
}

BigInt::BigInt(const BigInt& bigInt) {
	this->value = bigInt.getValue();
	this->isNeg = bigInt.getIsNeg();
}

const std::string &BigInt::getValue() const {
	return value;
}

const bool BigInt::getIsNeg() const {
	return isNeg;
}

const bool BigInt::operator == (const BigInt &bigInt) {
	return ((value == bigInt.getValue() && isNeg == bigInt.getIsNeg()) ? true : false);
}

const bool BigInt::operator != (const BigInt &bigInt) {
	return (value != bigInt.getValue() || isNeg != bigInt.getIsNeg()) ? true : false; 
}

const bool BigInt::operator < (const BigInt &bigInt) {
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

const bool BigInt::operator > (const BigInt &bigInt) {
	return ((value != bigInt.getValue()) && value < bigInt.getValue()) ? true : false;
}

BigInt &BigInt::operator = (const BigInt &bigInt) {
	value = bigInt.getValue();
	isNeg = bigInt.getIsNeg();

	return *this;
}

BigInt BigInt::operator +() {
	return BigInt(*this);
}

BigInt BigInt::operator -() {
	std::string choice = isNeg ? value : std::string("-") + value;
	return BigInt(choice);
}

BigInt BigInt::operator + (const BigInt &bigInt) {
	if (isNeg == bigInt.getIsNeg()) {
		std::string num2 = bigInt.getValue();

		size_t len1 = value.length();
		size_t len2 = num2.length();
		size_t length = 1 + std::max(len1, len2);

		char* res = new char[length + 1];

		res[length - 1] = res[length] = '\0';

		for (int i = 0; i < length - 1; i++) {
			int j = length - 1 - i;
			res[j] += ((i < len2) ? (num2[len2 - 1 - i] - '0') : 0) + ((i < len1) ? (value[len1 - 1 - i] - '0') : 0);
			res[j - 1] = res[j] / 10;
			res[j] = res[j] % 10 + '0';
		}

		res[0] += '0';
		std::string result = isNeg ? std::string("-") + std::string(res) : std::string(res);
		delete [] res;
		return BigInt(result);
	} else {
		throw;//in progress
	}
}

std::istream &operator >> (std::istream &stream, BigInt &bigInt) {
	std::string value;
	stream >> value;
	bigInt = BigInt(value);

	return stream;
}

std::ostream &operator << (std::ostream &stream, const BigInt &bigInt) {
	if (bigInt.getIsNeg()) {
		stream << "-";
	}
	return stream << bigInt.getValue();
}

int main () {
	BigInt a;
	BigInt b;
	BigInt c;
	std::cin >> a;
	std::cin >> b;
	c = a + b;
	std::cout << a + b << "\n";
	
}