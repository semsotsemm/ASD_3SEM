#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <windows.h>
#include <climits> 

using namespace std;
const int INF = INT_MAX;

int find_set(int v, vector<int>& parent) {
	if (v == parent[v])
		return v;
	return parent[v] = find_set(parent[v], parent);
}

void union_sets(int a, int b, vector<int>& parent) {
	a = find_set(a, parent);
	b = find_set(b, parent);
	if (a != b)
		parent[b] = a;
}

struct Edge {
	int u, v, weight;
};

bool compare_edges(const Edge& a, const Edge& b) {
	return a.weight < b.weight;
}

bool check_input(string user_input);
void print_graph(int** graph, int size);
void prima_algorithm(int** graph, int size);
void kruskal_algorithm(int** graph, int size);

int main()
{
	setlocale(LC_CTYPE, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

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
		cout << "Матрица задается через граф смежности, введите элементы (0 - нет ребра, INF - используйте очень большое число): \n";

		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				if (i == j) {
					graph[i][j] = 0;
					continue;
				}
				cout << "Введите вес ребра [" << i + 1 << "][" << j + 1 << "]: ";
				cin >> graph[i][j];
				if (graph[i][j] == 0) graph[i][j] = INF; 
			}
		}
		cout << "Введенный граф:\n";
		print_graph(graph, N);
	}
	else
	{
		N = 8;
		graph = new int* [N];
		for (int i = 0; i < N; ++i) {
			graph[i] = new int[N];
		}

		vector<vector<int>> matrix = {
			{0, 2, INF, 8, 2, INF, INF, INF},
			{2, 0, 3, 10, 5, INF, INF, INF},
			{INF, 3, 0, INF, 12, INF, INF, 7},
			{8, 10, INF, 0, 14, 3, 1, INF},
			{2, 5, 12, 14, 0, 11, 4, 8},
			{INF, INF, INF, 3, 11, 0, 6, INF},
			{INF, INF, INF, 1, 4, 6, 0, 9},
			{INF, INF, 7, INF, 8, INF, 9, 0}
		};

		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				graph[i][j] = matrix[i][j];
			}
		}
		cout << "Готовый граф:\n";
		print_graph(graph, N);
	}

	prima_algorithm(graph, N);
	kruskal_algorithm(graph, N);

	for (int i = 0; i < N; i++) {
		delete[] graph[i];
	}
	delete[] graph;

	return 0;
}


void prima_algorithm(int** graph, int size) {
	cout << "  1. АЛГОРИТМ ПРИМА\n";

	vector<int> min_dist(size, INF);
	vector<int> parent(size, -1);
	vector<bool> in_mst(size, false);

	int start_node = 0;
	min_dist[start_node] = 0;

	vector<pair<int, int>> mst_edges;
	long long total_weight = 0; 

	for (int count = 0; count < size; ++count) 
	{
		int u = -1;
		int min_weight = INF;

		for (int v = 0; v < size; ++v) 
		{
			if (!in_mst[v] && min_dist[v] < min_weight) 
			{
				min_weight = min_dist[v];
				u = v;
			}
		}

		if (u == -1) break; 

		in_mst[u] = true;

		if (parent[u] != -1) {
			mst_edges.push_back({ parent[u], u });
			total_weight += graph[parent[u]][u];
		}

		for (int v = 0; v < size; ++v) {
			if (graph[u][v] != INF && !in_mst[v] && graph[u][v] < min_dist[v]) {
				parent[v] = u;
				min_dist[v] = graph[u][v];
			}
		}

		if (parent[u] != -1) { 
			cout << "Ребро(" << parent[u] + 1 << " - " << u + 1
				<< "): вес " << min_weight << ",общий вес: " << total_weight << endl;
		}
	}

	cout << "Вес графа: " << total_weight << endl;
	cout << "Список ребер:\n";
	for (const auto& edge : mst_edges) {
		cout << "  Ребро (" << edge.first + 1 << " - " << edge.second + 1 << "), Вес: " << graph[edge.first][edge.second] << endl;
	}
}


void kruskal_algorithm(int** graph, int size) {
	cout << "  2. АЛГОРИТМ КРАСКАЛА\n";
	vector<Edge> edges;
	for (int i = 0; i < size; ++i) {
		for (int j = i + 1; j < size; ++j) { 
			if (graph[i][j] != INF) {
				edges.push_back({ i, j, graph[i][j] });
			}
		}
	}
	sort(edges.begin(), edges.end(), compare_edges);

	vector<int> parent(size);
	for (int i = 0; i < size; ++i) {
		parent[i] = i;
	}

	vector<pair<int, int>> mst_edges;
	long long total_weight = 0;
	int edges_used = 0;

	for (const auto& edge : edges) {
		if (edges_used == size - 1) break;

		int root_u = find_set(edge.u, parent);
		int root_v = find_set(edge.v, parent);

		if (root_u != root_v) {
			union_sets(edge.u, edge.v, parent);
			mst_edges.push_back({ edge.u, edge.v });
			total_weight += edge.weight;
			edges_used++;

			cout << "Ребро (" << edge.u + 1 << " - " << edge.v + 1
				<< "), вес: " << edge.weight << ", общий вес: " << total_weight << endl;
		}
		else {
			cout << "Пропущено ребро (" << edge.u + 1 << " - " << edge.v + 1
				<< ") с весом " << edge.weight << " (Образует цикл).\n";
		}
	}

	cout << "Вес графа: " << total_weight << endl;
	cout << "Список ребер:\n";
	for (const auto& edge : mst_edges) {
		cout << "  Ребро (" << edge.first + 1 << " - " << edge.second + 1 << "), Вес: " << graph[edge.first][edge.second] << endl;
	}
}


bool check_input(string user_input)
{
	if (user_input.empty()) return false;
	for (char c : user_input)
	{
		if (!isdigit(c))
		{
			return false;
		}
	}
	return (user_input != "0");
}

void print_graph(int** graph, int size)
{
	cout << "  ";
	for (int i = 0; i < size; ++i) {
		cout << i + 1 << " ";
	}
	cout << "\n";
	for (int i = 0; i < size; i++)
	{
		cout << i + 1 << " ";
		for (int j = 0; j < size; j++)
		{
			if (graph[i][j] == INF) {
				cout << "I "; 
			}
			else {
				cout << graph[i][j] << " ";
			}
		}
		cout << '\n';
	}
	cout << '\n';
}