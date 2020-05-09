#include <iostream>
#include <string>
#include <time.h>
#include <vector>
#include <numeric>
#include <algorithm>
#include <iterator>
#include <omp.h>
using namespace std;


void task_1();
void printMatrix(const vector<vector<int>> matrix);
vector<vector<int>> generateMatrix(vector<vector<int>> matrix);

int main()
{
	task_1();

	system("pause");
	return 0;
}



void task_1()
{
	constexpr int m = 8, n = 11;
	vector<vector<int>> matrix(m, vector<int>(n));

	matrix = generateMatrix(matrix);
	printMatrix(matrix);

	omp_lock_t lock;
	omp_init_lock(&lock);
	#pragma omp parallel num_threads(m) shared(matrix) 
	{
		int countOfZeros = 0;
		#pragma omp for
		for (int i = 0; i < m; i++)
		{
			while (!omp_test_lock(&lock)) {
				cout << "Thread " << omp_get_thread_num() << " is waiting for unlocking" << endl;
			}
			cout << "Thread " << omp_get_thread_num() << " locked resources" << endl;
			countOfZeros = count_if(matrix[i].begin(), matrix[i].end(), [](int a) {return a < 0; });
			omp_unset_lock(&lock);
		}
		#pragma omp critical 
		{
			cout << "\nThread #" << omp_get_thread_num() << " has counted negative values in a row #" << omp_get_thread_num() << ". It is " << countOfZeros << endl;
		}
	}
	omp_destroy_lock(&lock);
	cout << endl;
}


vector<vector<int>> generateMatrix(vector<vector<int>> matrix)
{
	srand(time(NULL));
	for (int i = 0; i < matrix.size(); i++)
		generate(matrix[i].begin(), matrix[i].end(), []() { return rand() % 100 - 50; });
	return matrix;
}


void printMatrix(const vector<vector<int>> matrix)
{
	cout << "Matrix:";
	for (int i = 0; i < matrix.size(); i++)
	{
		cout << endl;
		copy(matrix[i].begin(), matrix[i].end(), ostream_iterator<int>(cout, "\t"));
	}
	cout << endl;
}