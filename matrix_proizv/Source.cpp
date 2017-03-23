#include <fstream>
#include <iostream>
#include <ctime>
#include <string>

#define SIZE 1000 //размерность матриц, т.е. 1000х1000 

using namespace std;
/*
Процедура заполнения файла
матрицей 1000х1000 с рандомными числами.
*/
void createMatrix(string nameTxt) {
	srand(time(0));

	ofstream fout(nameTxt, ios_base::out | ios_base::trunc);

	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			fout << rand() << " ";

			if (j < SIZE-1)
			{
				fout << " ";
			}
			else if (i < SIZE)
			{
				fout << endl;
			}
		}
	}
}
/*
Процедура записи в файл массива с затратами 
времени на умножение матриц с размерностями 
от 3х3 до 1000х1000.
*/
void writeTime(double t[SIZE]) {
	srand(time(0));

	ofstream fout("result_time1.txt", ios_base::out | ios_base::trunc);

	for (int i = 0; i < SIZE; i++)
	{
		fout << t[i] << endl;
	}
	fout.close();
}
/*
Процедура работы в консоли с пользователем.
*/
void askToCreateMatrix() {
	char answer;
	cin >> answer;
	if (answer == 'y')
	{
		createMatrix("matrix_1.txt");
		createMatrix("matrix_2.txt");
	}
	else if (answer == 'n') {}
	else
	{
		cout << "Некорректно введен ответ. Введите снова (y/n): ";
		askToCreateMatrix();
	}
}
/*
Считывание файла и заполнение двумерного 
динамического массива.
*/
void readMatrixFromTxt(string nameTxt, int **matrix)
{
	ifstream fin(nameTxt, ios_base::in);
	int number;
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			fin >> (int) matrix[i][j];
		}
	}
	fin.close();
}
/*
вывод на экран двумерного динамического массива
(матрицы) желаемого размера N
*/
void printMatrix(int **matrix, int N) {
	for (size_t i = 0; i < N; i++)
	{
		for (size_t j = 0; j < N; j++)
		{
			cout << "\t" << matrix[i][j];
		}
		cout << endl;
	}
}
/*
Инициализация двумерного динамического массива с размерностью N
*/
void initializeMatrix(int **matrix, int N) {
	for (size_t i = 0; i < N; i++)
	{
		for (size_t j = 0; j < N; j++)
		{
			matrix[i][j]=0;
		}
		cout << endl;
	}
}
/*
перемножение двух матриц, с распараллеливанием или нет.
*/
int** matrixProduct(int **matrix_1, int**matrix_2, int isParallel) {
	//объявляем двумерный динамический 
	//массив, который содержит результат перемножения 
	int **matrix_result = new int *[SIZE];
	initializeMatrix(matrix_result, SIZE);
	//выделяем память динамическим массивам
	for (int i = 0; i < SIZE; i++) {
		matrix_1[i] = new int[SIZE];
		matrix_2[i] = new int[SIZE];
		matrix_result[i] = new int[SIZE];
	}
	//заполняем матрицы
	readMatrixFromTxt("matrix_1.txt", matrix_1);
	readMatrixFromTxt("matrix_2.txt", matrix_2);
	
	double timeArray[SIZE];
	clock_t t1, t2;

	for (size_t size = 2; size < SIZE; size++)
	{
		int i, j, k;
		t1 = clock();
#pragma omp parallel for private(j,k) if(isParallel)	
		for (i = 0; i<size; i++)
		{
			for (j = 0; j<size; j++)
				for (k = 0; k<size; k++)
					matrix_result[i][j] += matrix_1[i][k] * matrix_2[k][j];
		}
		t2 = clock();
		timeArray[size] = (double)(t2 - t1) / (double)CLOCKS_PER_SEC;
		cout << "size of matrix: " << size << " Time: " << timeArray[size] << '\n';
	}
	//Записываем время всех перемножений в файл.
	writeTime(timeArray);
	//Записываем результат перемножения в файл.
	ofstream fout("resultMatrix1.txt", ios_base::out | ios_base::trunc);
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			fout << matrix_result[i][j] << " ";
			if (j < SIZE - 1)
			{
				fout << " ";
			}
			else if (i < SIZE)
			{
				fout << endl;
			}
		}
	}
	return matrix_result;
}

//считывание от 3х3 до 1000х1000 и сделать график затраченного времени в эксель
int main() {
	setlocale(LC_ALL, "ru");
	cout << "Создать новые матрицы? (y/n): ";
	askToCreateMatrix();

	int **matrix_1 = new int*[SIZE];
	int **matrix_2 = new int*[SIZE];

	for (int i = 0; i < SIZE; i++) {
		matrix_1[i] = new int[SIZE];
		matrix_2[i] = new int[SIZE];
	}

	//заполняем матрицы
	readMatrixFromTxt("matrix_1.txt", matrix_1);
	readMatrixFromTxt("matrix_2.txt", matrix_2);
	
	//вычисляем с распараллеливанием
	matrixProduct(matrix_1, matrix_2, 1);
	//вычисляем без распараллеливания
	matrixProduct(matrix_1, matrix_2, 0);

	system("pause");
}