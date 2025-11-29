#include <iostream>
#include <string> 

#include <windows.h>

using namespace std;
const int INF = INT_MAX;

bool check_input(string user_input);
void print_graph(int** graph, int size);
void print_graph_without(int** graph, int size, int index_i, int index_j);
void floyd_warshall(int** graph, int size);

int main()
{
	setlocale(LC_CTYPE, "Russian");
	int var = 0;
	string user_input;
	int N;
	int** graph = nullptr;

	do {
		cout << "Выберите вариант задания матрицы:\n1) Задать матрицу в ручную\n2) Взять готовую матрицу\n";
		cin >> user_input;
		if (!check_input(user_input)) {
			cout << "Ошибка ввода, повторите попытку\n";
		}
		else 
		{
			var = stoi(user_input);
		}
	} while (!(check_input(user_input) && var >= 1 && var <= 2));

	if (var == 1)
	{
		do {
			cout << "Введите N (размер графа): ";
			cin >> user_input;
			if (!check_input(user_input)) {
				cout << "Ошибка ввода, повторите попытку\n";
			}
		} while (!check_input(user_input));
		N = stoi(user_input);
		graph = new int* [N];
		for (int i = 0; i < N; i++)
		{
			graph[i] = new int[N];
		}
		cout << "Матрица задается через граф смежности, введите элементы\n";
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				graph[i][j] = 0;
			}
		}
		cout << "Граф:\n";
		print_graph(graph, N);
	}
	else 
	{
		N = 6;
		graph = new int* [N];
		for (int i = 0; i < N; ++i) {
			graph[i] = new int[N]; 
		}
		graph[0][0] = 0;
		graph[0][1] = 28;
		graph[0][2] = 21;
		graph[0][3] = 59;
		graph[0][4] = 12;
		graph[0][5] = 27;
		graph[1][0] = 7;
		graph[1][1] = 0;
		graph[1][2] = 24;
		graph[1][3] = INF;
		graph[1][4] = 21;
		graph[1][5] = 9;
		graph[2][0] = 9;
		graph[2][1] = 32;
		graph[2][2] = 0;
		graph[2][3] = 13;
		graph[2][4] = 11;
		graph[2][5] = INF;
		graph[3][0] = 8;
		graph[3][1] = INF;
		graph[3][2] = 5;
		graph[3][3] = 0;
		graph[3][4] = 16;
		graph[3][5] = INF;
		graph[4][0] = 14;
		graph[4][1] = 13;
		graph[4][2] = 15;
		graph[4][3] = 10;
		graph[4][4] = 0;
		graph[4][5] = 22;
		graph[5][0] = 15;
		graph[5][1] = 18;
		graph[5][2] = INF;
		graph[5][3] = INF;
		graph[5][4] = 6;
		graph[5][5] = 0;
	}
		floyd_warshall(graph, N);
		for (int i = 0; i < N; i++) {
			delete[] graph[i];
		}
		delete[] graph;
}


bool check_input(string user_input)
{
	for (int i = 0; i < user_input.length(); i++)
	{
		if (!(user_input[i] >= '0' && user_input[i] <= '9'))
		{
			return false;
		}
	}
	return (user_input == "" || user_input == "0") ? false : true;
}

void print_graph_without(int** graph, int size, int index_i, int index_j) 
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (i == index_i && j == index_j) 
			{
				SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
				cout << graph[i][j] << " ";
				SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
			else 
			{
				cout << graph[i][j] << " ";
			}
		}
		cout << endl;
	}
}

void floyd_warshall(int** graph, int size) 
{
	int** S = new int* [size];
	for (int i = 0; i < size; i++)
	{
		S[i] = new int[size];
		for (int j = 0; j < size; j++)
		{
			S[i][j] = j+1;
		}
	}
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			for (int k = 0; k < size; k++)
			{
				if (i == j || j == k || k == i) 
				{
					continue;
				}
				cout << "Путь из " << j + 1 << " в " << k + 1 << " вершину = " << graph[j][k] << " а через вершину " << i + 1 << " : " << graph[j][i] << "+" << graph[i][k] << "=" << graph[j][i] + graph[i][k];
				if (graph[j][i] + graph[i][k] < graph[j][k])
				{
					cout << " Записываем новое расстояние";
				}
				cout << endl;
				if (graph[j][k] > graph[j][i] + graph[i][k] && graph[j][i] + graph[i][k] > 0)
				{

					S[j][k] = i + 1;
					graph[j][k] = graph[j][i] + graph[i][k];
				}
			}
		}
	}
	cout << "Матрица путей S:\n";
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			cout << S[i][j] << " ";
		}
		cout << endl;
	}
	cout << "Матрица ррасстояний\n";
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			cout << graph[i][j] << " ";
		}
		cout << endl;
	}
}


void print_graph(int** graph, int size) 
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (i == j) 
			{
				graph[i][j] = 0;
			}
			else
			{
				print_graph_without(graph, size, i, j);
				cin >> graph[i][j];

				for (int k = 0; k < size+1; ++k) {
					std::cout << "\033[F";
					std::cout << "\033[2K";
				}
			}
		}
	}
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			cout << graph[i][j] << " ";
		}
		cout << '\n';
	}
}