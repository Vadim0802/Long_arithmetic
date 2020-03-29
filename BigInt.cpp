#include <iostream>
#include <string>

class BigInt {
	std::string value; // значение числа
	bool isNeg; // флаг отрицательности

public:
	BigInt(); //check
	BigInt(long x); //check
	BigInt(std::string &value); //check

	const std::string &getValue() const; //check
	const bool getIsNeg() const; // check

	const bool operator == (const BigInt &bigInt); //check
	const bool operator != (const BigInt &bigInt); //check
	const bool operator < (const BigInt &bigInt); //check проверил работоспособность
	const bool operator > (const BigInt &bigInt); // check
	const bool operator >= (const BigInt &bigInt);
	const bool operator <= (const BigInt &bigInt);

	BigInt &operator = (const BigInt &bigInt); //check

	BigInt &operator+();
	BigInt &operator-();

	friend std::istream &operator >> (std::istream &stream, BigInt &bigInt); //check
	friend std::ostream &operator << (std::ostream &stream, BigInt &bigInt); //check
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
	return ((value != bigInt.getValue()) && value < bigInt.getValue()) ? false : true;
}

BigInt &BigInt::operator = (const BigInt &bigInt) {
	value = bigInt.getValue();
	isNeg = bigInt.getIsNeg();

	return *this;
}

std::istream &operator >> (std::istream &stream, BigInt &bigInt) {
	std::string value;
	stream >> value;
	bigInt = BigInt(value);

	return stream;
}

std::ostream &operator << (std::ostream &stream, BigInt &bigInt) {
	if (bigInt.getIsNeg()) {
		stream << "-";
	}
	return stream << bigInt.getValue();
}

int main () {
	BigInt a;
	BigInt b;
	std::cin >> a; std::cin >> b;
	std::cout << a << "\n" << b << "\n";
	std::cout << (a < b) << "\n";
	return 0;
}