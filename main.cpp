#include <iostream>
#include <ctime>
#include "cmath"
#include <string>
#include <omp.h>

// Кузнецов Михаил Алексндрович БПИ199
// 13 вариант

uint32_t* a; // Массив A
uint32_t* b; // Массив B
uint32_t arrays_size; // Хранит размер массивов
uint32_t thread_num = 6; // Число потоков
uint32_t upperBound; // Верхняя граница длины массива

/// Генерирует длину массива
void generateSize() {
	arrays_size = rand() % (upperBound - 999) + 1000;
}

/// Генерирует массив
/// \return Сгенерированный массив
uint32_t* generateArray() {
	uint32_t* array = new uint32_t[arrays_size];
	for (int j = 0; j < arrays_size; ++j) {
		array[j] = (unsigned)rand();
	}
	return array;
}

/// Проверяет является ли переданное число простым
/// \param num Число для проверки на простоту
/// \return Результат проверки
bool isPrime(long num, int* value) {
	if (num == 0 || abs(num) == 1) return false;
	for (long j = 2; j <= sqrt(abs(num)); ++j) {
		if (num % j == 0) return false;
	}
	*value = num;
	return true;
}

/// Выводит массив
/// \param arrayName Название массива
/// \param ar Массив
void printArray(std::string arrayName, uint32_t* ar) {
	for (int j = 0; j < arrays_size; ++j) {
		std::cout << arrayName << "[" << j << "] = " << ar[j] << std::endl;
	}
}


int main(int argc, char** argv) {
	srand((unsigned)time(NULL));
	if (argc != 3) { // Проверяем число аргументов
		std::cout << "Wrong number of console arguments" << std::endl;
		return 1;
	}
	if (std::stol(argv[1]) < 1001) { // Проверяем верхнюю границу
		std::cout << "Wrong upper bound" << std::endl;
		return 1;
	}
	if (std::stol(argv[2]) <= 0) { // Проверяем число потоков
		std::cout << "Wrong thread number" << std::endl;
		return 1;
	}
	upperBound = std::stoul(argv[1]); // Получаем верхнюю границу
	thread_num = std::stoul(argv[2]); // Получаем число потоков

	generateSize();
	a = generateArray();
	b = generateArray();
	printArray("a", a);
	printArray("b", b);
	std::cout << "Set of indices i:" << std::endl;

	// Ищем индексы массивов подходящие под условия
#pragma omp parallel num_threads(thread_num) 
	{
		auto threadNum = omp_get_thread_num();
		int c;
		int primeValue;
#pragma omp for private(c,primeValue)
		for (int j = 0;j < arrays_size; j++)
		{
			c = j;
			if (isPrime(a[c] + b[c], &primeValue) || isPrime(a[c] - b[c], &primeValue)) {
#pragma omp  critical
				{
					std::cout << threadNum << " thread " << "i: " << c << " with prime result: " << primeValue << std::endl;
				}
			}
		}
	}
	delete[] a;
	delete[] b;
	return 0;
}
