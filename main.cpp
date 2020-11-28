#include <iostream>
#include <ctime>
#include "cmath"
#include <string>
#include <omp.h>

// �������� ������ ������������ ���199
// 13 �������

uint32_t* a; // ������ A
uint32_t* b; // ������ B
uint32_t arrays_size; // ������ ������ ��������
uint32_t thread_num = 6; // ����� �������
uint32_t upperBound; // ������� ������� ����� �������

/// ���������� ����� �������
void generateSize() {
	arrays_size = rand() % (upperBound - 999) + 1000;
}

/// ���������� ������
/// \return ��������������� ������
uint32_t* generateArray() {
	uint32_t* array = new uint32_t[arrays_size];
	for (int j = 0; j < arrays_size; ++j) {
		array[j] = (unsigned)rand();
	}
	return array;
}

/// ��������� �������� �� ���������� ����� �������
/// \param num ����� ��� �������� �� ��������
/// \return ��������� ��������
bool isPrime(long num, int* value) {
	if (num == 0 || abs(num) == 1) return false;
	for (long j = 2; j <= sqrt(abs(num)); ++j) {
		if (num % j == 0) return false;
	}
	*value = num;
	return true;
}

/// ������� ������
/// \param arrayName �������� �������
/// \param ar ������
void printArray(std::string arrayName, uint32_t* ar) {
	for (int j = 0; j < arrays_size; ++j) {
		std::cout << arrayName << "[" << j << "] = " << ar[j] << std::endl;
	}
}


int main(int argc, char** argv) {
	srand((unsigned)time(NULL));
	if (argc != 3) { // ��������� ����� ����������
		std::cout << "Wrong number of console arguments" << std::endl;
		return 1;
	}
	if (std::stol(argv[1]) < 1001) { // ��������� ������� �������
		std::cout << "Wrong upper bound" << std::endl;
		return 1;
	}
	if (std::stol(argv[2]) <= 0) { // ��������� ����� �������
		std::cout << "Wrong thread number" << std::endl;
		return 1;
	}
	upperBound = std::stoul(argv[1]); // �������� ������� �������
	thread_num = std::stoul(argv[2]); // �������� ����� �������

	generateSize();
	a = generateArray();
	b = generateArray();
	printArray("a", a);
	printArray("b", b);
	std::cout << "Set of indices i:" << std::endl;

	// ���� ������� �������� ���������� ��� �������
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
