#include <iostream>
#include <string>
#include <fstream>

class BigInt {
private:
	std::string value;
	bool isNeg;
public:
	BigInt(); //check
	BigInt(int x); //check
	BigInt(const std::string& value); //check

	const std::string& getValue() const; //check
	const bool getIsNeg() const; // check

	const bool operator == (const BigInt& bigInt) const; //check
	const bool operator != (const BigInt& bigInt) const; //check
	const bool operator < (const BigInt& bigInt) const; //check 
	const bool operator > (const BigInt& bigInt) const; // check

	BigInt& operator = (const BigInt& bigInt); //check

	BigInt operator + (std::string number);
	BigInt operator + (const BigInt& bigInt) const; // check
	BigInt operator - (const BigInt& bigInt) const; // check
	BigInt operator * (const BigInt& bigInt) const; // check
	BigInt operator / (const BigInt& bigInt) const; // check

	BigInt operator+() const; //check
	BigInt operator-() const; //check

	friend std::istream& operator >> (std::istream& stream, BigInt& bigInt); //check
	friend std::ostream& operator << (std::ostream& stream, const BigInt& bigInt); //check
};

BigInt::BigInt() {
	/*
	Конструктор по умолчанию, будет инициализироваться, если мы не передадим никаких параметров
	BigInt example;
	*/
	this->value = "0"; //Поле "значение" равно 0
	this->isNeg = false; //Поле "Отрицательное число" - ложь
}

BigInt::BigInt(int x) {
	/*
	Этот конструктор вызовется, если в качестве аргумента мы передадим int число
	BigInt example(81);
	*/
	this->isNeg = x < 0 ? true : false; //Тернарный оператор, (условие ? вернет это значение если истина : это значение если ложь)
	this->value = std::to_string(isNeg ? -x : x);
}

BigInt::BigInt(const std::string& value) {
	/*
	Основной конструктор, который инициализируется, если мы передаем в качестве аргумента строку.
	*/

	/*
	Проверяем, не пустая ли строка, переданная конструктору, если пустая, инициализируем по умолчанию.
	*/
	if (value.length() == 0) {
		this->value = "0";
		this->isNeg = false;
		return;
	}

	isNeg = value[0] == '-'; //Логическое условие, isNeg примет положение 0 или 1, если value[0] == '-', то isNeg = 1, и наоборот.
	this->value = value.substr(isNeg); //"обрезаем" нашу строку значения, избавляясь от минуса, информация об отрицательности числа
	int counter = 0;					//хранится в булевом значении isNeg

	/*
	Проверяем на то, есть ли нули в начале строки, так как оно может быть вида 000000156, нули нужно посчитать и обрезать
	*/
	while (this->value[counter] == '0' && (this->value.length() - counter) > 1) {
		counter++;
	}

	this->value = this->value.substr(counter); //удаляем нули

	try
	{
		/*
		Проверяем строку на корректность, все символы нашей строки должны быть от 0 до 9, если 
		есть хотя бы один не подходящий символ, завершаем выполнение программы (выбрасываем исключение throw)
		*/
		for (int i = 0; i < this->value.length(); i++) {
			if (this->value[i] < '0' || this->value[i] > '9') {
				throw this->value[i];
			}
		}
	}
	catch (const char err)
	{
		std::cout << "Incorrect symbol: " << err << "\n";
		exit(0);
	}
}

/*
Геттер, позволяющий получить доступ к приватным полям класса
*/
const std::string& BigInt::getValue() const {
	return value;
}

/*
Геттер, позволяющий получить доступ к приватным полям класса
*/
const bool BigInt::getIsNeg() const {
	return isNeg;
}


const bool BigInt::operator == (const BigInt& bigInt) const {
	/*
	Перегрузка оператора сравнения.
	Если левое и правое значение равно по значению и по знаку, вернет true, если нет, то false
	const перед функией означает, что возвращаемое значение будет не изменяемым.
	const после функции означает, что функция не может изменять переменные-объекта, которому она принадлежит, т.е. BigInt
	*/
	return ((value == bigInt.getValue() && isNeg == bigInt.getIsNeg()) ? true : false);
}

const bool BigInt::operator != (const BigInt& bigInt) const {
	/*
	Перегрузка оператора НЕ РАВНО
	Если левое и правое значение не равны ИЛИ не равны знаки, вернет true, если нет, то false
	*/
	return (value != bigInt.getValue() || isNeg != bigInt.getIsNeg()) ? true : false;
}

const bool BigInt::operator < (const BigInt& bigInt) const {
	std::string value2 = bigInt.getValue(); //Значение правого числа
	int len1 = value.length(); //длина первого числа
	int len2 = value2.length(); //длина второго числа

	if (isNeg == bigInt.getIsNeg()) { //Если знаки равны
		if (len1 != len2) { //Если длины не равны
			return (len1 < len2) ^ isNeg; // меньше число с меньшей длинной для положительных и с большей длиной для отрицательных
		}
		int i = 0;

		/*
		Если длины равны, то нужно найти разряд числа, которые отличаются друг от друга и сравнить их
		Пример:
		123456
		123457
		*/
		while (i < len1 && value[i] == value2[i]) {
			i++;
		}

		// если разряд найден, то меньше число с меньшей цифрой для положительных и с большей цифрой для отрицательных, иначе числа равны
		return (i < len1) && ((value[i] < value2[i]) ^ isNeg);
	}
	return isNeg; // знаки разные, если число отрицательное, то оно меньше, если положительное, то больше
}

const bool BigInt::operator > (const BigInt& bigInt) const {
	/*
		Перегрузка оператора >
		*this - указатель на объект данного класса
		Вернет true, если *this(левое число) НЕ меньше правого числа ИЛИ не равен правому числа
	*/
	return !(*this < bigInt || *this == bigInt);
}

BigInt& BigInt::operator = (const BigInt& bigInt) {
	/*
	Объекту this присваиваем значение после =
	*/
	value = bigInt.getValue();
	isNeg = bigInt.getIsNeg();

	return *this;
}

BigInt BigInt::operator +() const {
	/*унарный плюс
	просто возвращает само число
	*/
	return BigInt(*this);
}

BigInt BigInt::operator -() const {
	/*
	унарный минус
	проверяет, если число уже было отрицательным, то минус на минус даст плюс
	если не было отрицательным, то сделать число отрицательным
	*/
	return BigInt(isNeg ? value : std::string("-") + value);
}

BigInt BigInt::operator + (const BigInt& bigInt) const {
	if (isNeg == bigInt.getIsNeg()) { //Если знаки равны
		std::string num2 = bigInt.getValue(); //значение правого числа

		int len1 = value.length(); // длина первого числа
		int len2 = num2.length(); // длина второго числа
		int length = (len1 < len2) ? len2 + 1 : len1 + 1; //результат суммы равен максимальной длине одного из чисел (+ 1 из-за возможного смещения разряда)

		int* a = new int[length]; //массив для хранения разрядов числа
		int* b = new int[length]; 
		char* res = new char[length + 1]; //массив для хранения результата сложения
		res[length] = '\0';

		for (int i = 0; i < length; i++) {
			/*
			Формируем разряды
			Т.е., грубо говоря, превращаем строку в массив целых чисел
			было "1234"
			стало [1] [2] [3] [4]
			*/
			a[i] = (i < len1) ? (value[len1 - 1 - i] - '0') : 0; // -'0' означает перевод в целое число
			b[i] = (i < len2) ? (num2[len2 - 1 - i] - '0') : 0; // так же в этом цикле выравнием длины чисел, если они разной длины
			res[i] = 0;											// то добавляем 0
		}

		for (int i = 0; i < length; i++) {
			//выполняем сложение
			res[length - 1 - i] += a[i] + b[i]; //в элемент массива суммы записываем результат сложения
			res[length - 1 - 1 - i] += res[length - 1 - i] / 10; // в следующий элемент записываем разряд десяткой, если он есть
			//тут аналогия в сложением в столбик, в одной ячейке у нас должно быть только одно число, десяток мы запоминаем в след.разряд
			res[length - 1 - i] %= 10; //в элементе массива суммы, куда мы изначально записывали результат сложения, оставляем только разряд единиц
		}
		//переводим массив суммы к строковому виду
		for (int i = 0; i < length; i++) {
			res[length - 1 - i] += '0';
		}

		//проверяем на то, были ли числа отрицательные, если да, добавим минус
		//так как по сути отрицательное число минус отрицательное число, есть просто сумма, и этот случай мы тут учитываем
		std::string result = isNeg ? std::string("-") + std::string(res) : std::string(res);
		delete[] a;
		delete[] b;
		delete[] res;
		return BigInt(result);
	}
	else {
		//если знаки чисел не были равны, то смотрим, какой знак у первого числа, если положительный, то просто отправляем на вычитание
		//если знак был отрицательным, то меняем местами и так же отправляем на вычитание.
		return isNeg ? (bigInt - (-BigInt(*this))) : (*this - (-BigInt(bigInt)));
	}
}

BigInt BigInt::operator - (const BigInt& bigInt) const {
	if (*this == bigInt) {
		return 0;
		//если числа равны, то 0
	}
	if (!isNeg && !bigInt.getIsNeg()) { //если оба числа положительные, то выполняем вычитание
		std::string value2 = bigInt.getValue();// запоминаем значение второго числа

		int len1 = value.length();// запоминаем длину первого числа
		int len2 = value2.length(); // запоминаем длину второго числа
		int length = (len1 < len2 ? len2 : len1);// длина результата не превысит максимума длин чисел

		int* a = new int[length]; // массивы аргументов
		int* b = new int[length];

		bool isNegRes = bigInt > * this; // определяем знак результата

		char* res = new char[length + 1]; // строковый массив для результата
		res[length] = '\0';

		for (int i = 0; i < length; i++) { //формируем разряды
			a[i] = (i < len1) ? (value[len1 - 1 - i] - '0') : 0;
			b[i] = (i < len2) ? (value2[len2 - 1 - i] - '0') : 0;
			res[i] = 0; 
		}

		for (int i = 0; i < length; i++) {
			if (!isNegRes) { // если первое число больше второго, то от первого отнимаем второе
				if (a[i] - b[i] < 0) {
					a[i + 1] += -1;
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
			else { //если второе больше первого, то меняем местами, теперь из второго отнимаем первое
				if (b[i] - a[i] < 0) { //если результат вычитания разрядов меньше 0, то у элемента [i + 1] занимаем 1
					b[i + 1] += -1;//заняли единицу
					res[length - 1 - i] += 10 + (b[i] - a[i]);// 10 появилось из-за того, что заняли 
					res[length - 1 - i - 1] += res[length - 1 - i] / 10; //законсим разряд десятков в след. элем.
					res[length - 1 - i] %= 10; //оставляем разряд единиц в i элементе
				}
				else { //если результат вычитания разрядов не меньше 0, то тоже самое, только без занимания
					res[length - 1 - i] += (b[i] - a[i]); 
					res[length - 1 - i - 1] += res[length - 1 - i] / 10;
					res[length - 1 - i] %= 10;
				}
			}
		}
		//перевод в стороковый формат
		for (int i = 0; i < length; i++) {
			res[length - 1 - i] += '0';
		}

		//проверяем на знак, если первое число было больше второго, то вернем просто результат, если меньше, то вернем результат с -
		std::string result = isNegRes ? std::string("-") + std::string(res) : std::string(res);
		delete[] a;
		delete[] b;
		delete[] res;
		return BigInt(result);

	}
	else {// если оба числа отрицательные, то меняем местами,
		//меняем знаки и повторяем вычитание, а если знаки разные, то отправляем на сумму
		return isNeg && bigInt.getIsNeg() ? (-BigInt(bigInt) - (-BigInt(*this))) : (*this + -BigInt(bigInt));
	}
}

BigInt BigInt::operator * (const BigInt& bigInt) const {
	if (value == "0" || bigInt.getValue() == "0") {
		return 0; // если один из множителей равен нулю, то результат равен нулю
	}
	std::string value2 = bigInt.getValue(); // запоминаем значение второго числа

	int len1 = value.length(); // запоминаем длину первого числа
	int len2 = value2.length(); // запоминаем длину второго числа
	int length = len1 + len2 + 1; // резульат влезет в сумму длин + 1 из-за возможного переноса

	bool isNegRes = isNeg ^ bigInt.getIsNeg(); // флаг отрицательности результата - отрицательный, если числа разных знаков

	int* a = new int[length];
	int* b = new int[length];
	char* res = new char[length + 1];
	res[length] = '\0';

	for (int i = 0; i < length; i++) { //формируем разряды
		a[i] = (i < len1) ? (value[len1 - 1 - i] - '0') : 0;
		b[i] = (i < len2) ? (value2[len2 - 1 - i] - '0') : 0;
		res[i] = 0;
	}
	//тоже самое, что и сложение, только теперь число первого числа, надо умножать на каждое число второго числа
	for (int i = 0; i < len1; i++) {
		for (int j = 0; j < len2; j++) {
			res[length - 1 - (i + j)] += a[i] * b[j];
			res[length - 1 - 1 - (i + j)] += res[length - 1 - (i + j)] / 10;
			res[length - 1 - (i + j)] %= 10;
		}
	}

	for (int i = 0; i < length; i++) {
		res[length - 1 - i] += '0';
	}

	std::string result = isNegRes ? std::string("-") + std::string(res) : std::string(res);
	delete[] a;
	delete[] b;
	delete[] res;

	return BigInt(result);
}

BigInt BigInt::operator / (const BigInt& bigInt) const {
	std::string value1 = value;
	std::string value2 = bigInt.getValue(); // запоминаем значение второго числа
	try
	{
		if (value2 == "0") {
			throw; // нельзя делить на ноль
		}
	}
	catch (const std::exception & err)
	{
		std::cout << err.what() << "\n";
		exit(0);
	}

	if (value1 == "0") {
		return 0; // а вот ноль делить можно на всё, кроме нуля
	}

	if (value1 == "1") {
		return BigInt(bigInt.getIsNeg() ? -BigInt(*this) : *this); // делить на 1 можно, но смысл?
	}

	int zeroes = 0;
	while (value2[value2.length() - 1 - zeroes] == '0') {
		zeroes++; //считаем нули 
	}

	if (zeroes >= value.length()) {
		return 0;
	}

	if (zeroes) { // избавляемся от круглых чисел
		value1 = value1.substr(0, value.length() - zeroes);
		value2 = value2.substr(0, value2.length() - zeroes);
	}

	bool isNegRes = isNeg ^ bigInt.getIsNeg(); // считаем знак числа

	BigInt tmp(value2);
	int divider_length = value2.length(); // запоминаем длину делителя
	int length = value1.length();// получаем длину делимого
	int index = 0;// стартуем с нулевого индекса
	std::string div; // результат деления
	std::string v; // подстрока, которая делится на делитель

	// формируем начальное число для деления
	while (BigInt(v) < tmp && index < length) {
		v += value[index++];
	}

	do {
		int count = 0;  // результат деления подчисла на делитель

		// если можем разделить, то делим
		if (BigInt(v) > tmp || BigInt(v) == tmp) {
			BigInt mod = v;

			//делим с помощью вычитания
			while (mod > tmp || mod == tmp) {
				mod = mod - tmp;
				count++;
			}

			v = mod.getValue();
		}

		div += count + '0'; // если не делили, то добавили ноль к результату, иначе добавили результат дедения

		if (index <= length) {
			v += value1[index++]; // формируем новое значение для подчисла
		}
	} while (index <= length);

	// возвращаем результат учитывая знак и возможное равенство нулю
	return BigInt(isNegRes && div != "0" ? std::string("-") + div : div); 
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

	std::cout << "-------------------------------------------------------------------" << "\n";
	std::cout << "-------------------------------------------------------------------" << "\n";
	std::cout << "-------------------------------------------------------------------" << "\n";
	std::cout << "-------------------------------------------------------------------" << "\n";
	std::cout << "-------------------------------------------------------------------" << "\n";
	std::cout << a + b << "\n";
	std::cout << "-------------------------------------------------------------------" << "\n";
	std::cout << a - b << "\n";
	std::cout << "-------------------------------------------------------------------" << "\n";
	std::cout << a * b << "\n";
	std::cout << "-------------------------------------------------------------------" << "\n";
	std::cout << a / b << "\n";
	std::cout << "-------------------------------------------------------------------" << "\n";
}