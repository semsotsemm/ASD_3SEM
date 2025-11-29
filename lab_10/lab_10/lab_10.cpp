#include <iostream>
#include <vector>
#include <random>
#include <limits>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <string>
#include <clocale>

#define INF 0x3f3f3f3f

using namespace std;

struct CityCoord {
    double x;
    double y;
};

struct BestResult {
    double length = INF;
    vector<int> route;
};

void write_data_to_file(const string& filename, const vector<CityCoord>& coords, const BestResult& best, int iter) {
    ofstream file(filename, ios::app);

    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return;
    }

    file << "ITERATION," << iter << endl;
    file << "LENGTH," << fixed << setprecision(2) << best.length << endl;

    file << "COORDS,";
    for (size_t i = 0; i < coords.size(); i++) {
        file << coords[i].x << ";" << coords[i].y;
        if (i < coords.size() - 1) file << "|";
    }
    file << endl;

    file << "ROUTE,";
    for (size_t i = 0; i < best.route.size(); i++) {
        file << best.route[i];
        if (i < best.route.size() - 1) file << "->";
    }
    file << "->" << best.route[0]; 
    file << endl;

    file.close();
}

int main() {
    setlocale(LC_ALL, ""); 
    std::random_device rd;
    std::mt19937 generator(rd());

    int number_of_cities;
    double alpha;
    double beta;
    double initial_pheromone;
    int iterations;
    double evaporation = 0.5; 
    double Q = 100.0;

    cout << "Введите количество городов: ";
    cin >> number_of_cities;
    cout << "Введите начальное значение феромонов (например 0.1): ";
    cin >> initial_pheromone;
    cout << "Введите альфа (например 1): ";
    cin >> alpha;
    cout << "Введите бета (например  2): ";
    cin >> beta;
    cout << "Введите количество итераций: ";
    cin >> iterations;

    std::uniform_real_distribution<> coord_gen(0.0, 100.0);
    vector<CityCoord> city_coords;

    // Генерируем координаты для кадого города
    for (int i = 0; i < number_of_cities; i++) 
    {
        city_coords.push_back({ coord_gen(generator), coord_gen(generator) });
    }

    // Вычисляем расстояния между городами
    vector<vector<int>> distance_matrix(number_of_cities, vector<int>(number_of_cities));
    for (int i = 0; i < number_of_cities; i++) 
    {
        for (int j = i; j < number_of_cities; j++) 
        {
            if (i == j) 
            {
                distance_matrix[i][j] = INF;
            }
            else 
            {
                double dx = city_coords[i].x - city_coords[j].x;
                double dy = city_coords[i].y - city_coords[j].y;
                int d = static_cast<int>(round(sqrt(dx * dx + dy * dy)));

                distance_matrix[i][j] = d;
                distance_matrix[j][i] = d;
            }
        }
    }

    vector<vector<double>> cities_tau(number_of_cities, vector<double>(number_of_cities, initial_pheromone));
    vector<vector<double>> cities_eta(number_of_cities, vector<double>(number_of_cities));
    for (int i = 0; i < number_of_cities; i++) 
    {
        for (int j = 0; j < number_of_cities; j++) 
        {
            if (i != j)
            {
                cities_eta[i][j] = 1.0 / (double)distance_matrix[i][j];
            }
            else
            {
                cities_eta[i][j] = 0.0;
            }
        }
    }

    BestResult global_best;
    std::uniform_real_distribution<> distribution_double(0.0, 1.0);

    ofstream clear_file("aco_data.txt", ios::trunc);
    clear_file.close();

    cout << "----------------------------------------------------" << endl;
    cout << setw(5) << "Итер." << " | " << setw(10) << "Мин. маршр." << " | " << "Маршрут" << endl;
    cout << "----------------------------------------------------" << endl;

    // Цикл поколений
    for (int iter = 1; iter <= iterations; iter++) 
    {
        vector<vector<double>> delta_tau(number_of_cities, vector<double>(number_of_cities, 0.0));

        // Проходим по всем городам
        for (int ant = 0; ant < number_of_cities; ant++) 
        {
            vector<int> path;
            vector<bool> visited(number_of_cities, false);

            int current_city = ant;
            path.push_back(current_city);
            visited[current_city] = true;
            double path_length = 0;

            // Шаги мураытя
            for (int step = 0; step < number_of_cities - 1; step++) 
            {

                vector<int> candidates;
                double denominator = 0.0;

                for (int j = 0; j < number_of_cities; j++) 
                {
                    if (!visited[j]) 
                    {
                        denominator += pow(cities_tau[current_city][j], alpha) * pow(cities_eta[current_city][j], beta);
                        candidates.push_back(j);
                    }
                }

                if (candidates.empty())
                {
                    break;
                }

                int next_city = -1;
                double r = distribution_double(generator);
                double current_slice = 0.0;

                for (int city_idx : candidates) 
                {
                    double numerator = pow(cities_tau[current_city][city_idx], alpha) * pow(cities_eta[current_city][city_idx], beta);
                    double prob;
                    if ((denominator > 0))
                    {
                        prob = (numerator / denominator);
                    }
                    else 
                    {
                        prob = 0;
                    }

                    current_slice += prob;
                    if (r <= current_slice) {
                        next_city = city_idx;
                        break;
                    }
                }

                if (next_city == -1)
                {
                    next_city = candidates.back();
                }

                path_length += distance_matrix[current_city][next_city];
                current_city = next_city;
                path.push_back(current_city);
                visited[current_city] = true;
            }

            path_length += distance_matrix[path.back()][path[0]];

            if (path_length < global_best.length) 
            {
                global_best.length = path_length;
                global_best.route = path;
                write_data_to_file("aco_data.txt", city_coords, global_best, iter);
            }

            double deposit = Q / path_length;
            for (size_t k = 0; k < path.size() - 1; k++)
            {
                int u = path[k];
                int v = path[k + 1];
                delta_tau[u][v] += deposit;
                delta_tau[v][u] += deposit;
            }
            delta_tau[path.back()][path[0]] += deposit;
            delta_tau[path[0]][path.back()] += deposit;
        }

        for (int i = 0; i < number_of_cities; i++) 
        {
            for (int j = 0; j < number_of_cities; j++) 
            {
                cities_tau[i][j] = (1.0 - evaporation) * cities_tau[i][j] + delta_tau[i][j];
                if (cities_tau[i][j] < 0.0001)
                {
                    cities_tau[i][j] = 0.0001;
                }
            }
        }

        cout << setw(5) << iter << " | " << fixed << setprecision(2) << setw(10) << global_best.length << " | ";

        if (number_of_cities <= 20) {
            for (size_t i = 0; i < global_best.route.size(); i++) {
                cout << global_best.route[i];
                if (i < global_best.route.size() - 1) cout << "->";
            }
            cout << "->" << global_best.route[0];
        }
        else {
            cout << "[Маршрут слишком длинный...]";
        }
        cout << endl;
    }

    cout << "----------------------------------------------------" << endl;
    cout << "\nФинальный лучший путь: " << global_best.length << endl;
    return 0;
}