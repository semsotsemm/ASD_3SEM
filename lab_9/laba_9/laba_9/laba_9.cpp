#include <iostream>
#include <vector> 
#include <ctime>   

using namespace std;


const vector<vector<int>> CITY_MAP = {
//   0   1   2   3   4   5   6   7   8
	{0, 25, 40, 60, 15, 88, 30, 45, 75}, // 0
	{15, 0, 18, 55, 32, 21, 62, 10, 50}, // 1
	{35, 20, 0, 27, 45, 30, 12, 55, 38}, // 2
	{50, 45, 20, 0, 10, 70, 48, 22, 16}, // 3
	{12, 35, 42, 10, 0, 50, 25, 30, 28}, // 4
	{80, 20, 35, 65, 52, 0, 90, 78, 11}, // 5
	{32, 60, 10, 50, 28, 95, 0, 40, 53}, // 6
	{40, 12, 60, 25, 35, 80, 42, 0, 18}, // 7
	{70, 55, 30, 18, 30, 15, 50, 15, 0}  // 8
};


void printRoute(vector<int> individ);
bool isNumberInRoute(const vector<int> individ, const int random_number);
int getRouteWeight(const vector<int>individ);
vector<int> mutation(vector<int> new_generation);
int chooseBestIndivid(vector<vector<int>> population);
vector<vector<int>> weedOutIndivids(vector<vector<int>> population, int new_population_size);
vector<vector<int>> evolvePopulation(vector<vector<int>> population, int new_population_size);


int main()
{
	srand(static_cast<unsigned>(time(0)));
	setlocale(LC_CTYPE, "Russian");		

	vector<vector<int>> population;
	int papulation_size;
	int child_elements;
	int number_generations;
	int number_of_cities = CITY_MAP[0].size();

	cout << "Введите размер популяции: ";
	cin >> papulation_size;
	cout << "Введите количество потомков у 2 родителей: ";
	cin >> child_elements;
	cout << "Введите количество поколений: ";
	cin >> number_generations;

	// Генерируем первое поколение случайно
	for (int i = 0; i < papulation_size; i++)
	{
		vector<int> individ;
		for (int j = 0; j < number_of_cities; j++)
		{
			int random_number = rand() % number_of_cities;
			while (!isNumberInRoute(individ, random_number))
			{
				random_number = rand() % number_of_cities;
			}
			individ.push_back(random_number);
		}
		population.push_back(individ);
	}

	// Эволюционируем n поколений
	for (int i = 0; i < number_generations; i++)
	{
		int min_weight;
		int weight_index;
		for (int i = 0; i < population.size(); i++)
		{
			int individ_weight = getRouteWeight(population[i]);
			if (i == 0 || min_weight > individ_weight)
			{
				min_weight = individ_weight;
				weight_index = i;
			}
		}
		cout << "Поколение " << i + 1 << ". Вес наилучшего маршрута сейчас: " << min_weight << ". Маршрут:";
		printRoute(population[weight_index]);

		int new_population_size = population.size() / 2;
		vector<vector<int>> new_population = evolvePopulation(population, new_population_size);
		population = new_population;
	}

	return 0;
}


bool isNumberInRoute(const vector<int> individ, const int random_number) {
	bool is_repeat = true;
	for (int i = 0; i < individ.size(); i++)
	{
		if (individ[i] == random_number)
		{
			is_repeat = false;
		}
	}
	return is_repeat;
}


int getRouteWeight(const vector<int>individ)
{
	int weight = 0;
	for (int i = 1; i < individ.size(); i++)
	{
		weight += CITY_MAP[individ[i - 1]][individ[i]];
	}
	return weight;
}


int chooseBestIndivid(vector<vector<int>> population)
{
	int min_weight_individ = 0;
	int min_weight = 0;

	for (int i = 0; i < population.size(); i++)
	{
		int individ_weight = getRouteWeight(population[i]);
		if (min_weight > individ_weight || i == 0)
		{
			min_weight = individ_weight;
			min_weight_individ = i;
		}
	}

	return min_weight_individ;
}


vector<vector<int>> weedOutIndivids(vector<vector<int>> population, int new_population_size)
{
	vector<vector<int>> new_population;
	for (int i = 0; i < new_population_size; i++)
	{
		int best_individ_index = chooseBestIndivid(population);
		new_population.push_back(population[best_individ_index]);
		population.erase(population.begin() + best_individ_index);
	}
	return new_population;
}


void printRoute(vector<int> individ)
{
	for (int i = 0; i < individ.size(); i++)
	{
		cout << individ[i] + 1;
		if (i != individ.size() - 1)
		{
			cout << "->";
		}
	}
	cout << "\n";
}

vector<vector<int>> evolvePopulation(vector<vector<int>> population, int new_population_size)
{
	vector<vector<int>> best_individs = weedOutIndivids(population, new_population_size);

	vector<vector<int>> new_generation = best_individs;

	int total_needed = population.size();
	int children_needed = total_needed - new_generation.size();

	for (int i = 0; i < children_needed; i++)
	{
		int p1_idx = rand() % best_individs.size();
		int p2_idx = rand() % best_individs.size();

		while (p1_idx == p2_idx && best_individs.size() > 1) {
			p2_idx = rand() % best_individs.size();
		}

		vector<int> parent1 = best_individs[p1_idx];
		vector<int> parent2 = best_individs[p2_idx];
		vector<int> child;

		int split_point = parent1.size() / 2;
		for (int j = 0; j < split_point; j++)
		{
			child.push_back(parent1[j]);
		}

		for (int j = 0; j < parent2.size(); j++)
		{
			int city = parent2[j];
			if (isNumberInRoute(child, city))
			{
				child.push_back(city);
			}
		}
		mutation(child);
		new_generation.push_back(child);
	}
	return new_generation;
}

vector<int> mutation(vector<int> child)
{
	int random_number = rand() % 100;
	if (random_number <= 5)
	{
		int first_mutated_gene = rand() % child.size();
		int second_mutated_gene = rand() % child.size();
		while (second_mutated_gene == first_mutated_gene)
		{
			second_mutated_gene = rand() % child.size();
		}
		swap(child[first_mutated_gene], child[second_mutated_gene]);
	}
	return child;
}