#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <iostream>


using namespace std;



vector<int> convertToVector(const string line)
{
	vector<int> sequence_numbers;
	string not_converted_number = "";
	int vector_size;

	if (line[0] == '~')
	{
		srand(time(0));
		for (int i = 0; i < line.length(); i++)
		{
			if (line[i] != ',' && line[i] != ' ' && line[i] != '~')
			{
				not_converted_number += line[i];
			}
			else if(not_converted_number != "")
			{
				vector_size = stoi(not_converted_number);
				break;
			}
		}
		vector_size = stoi(not_converted_number);
		for (int i = 0; i < vector_size; i++)
		{
			sequence_numbers.push_back(rand() % 102 + 1);
		}
	}
	else
	{
		for (int i = 0; i < line.length(); i++)
		{
			if (line[i] != ',' && line[i] != ' ')
			{
				not_converted_number += line[i];
			}
			else if (not_converted_number != "")
			{
				sequence_numbers.push_back(stoi(not_converted_number));
				not_converted_number = "";
			}
		}
		sequence_numbers.push_back(stoi(not_converted_number));
	}
	return sequence_numbers;
}


bool checkUserInput(const string user_input)
{
	if ((user_input.length() == 0) || (user_input[0] <= '0' && user_input[0] >= '9' && user_input[0] != '~'))
	{
		return false;
	}
	else
	{
		for (int i = 0; i < user_input.length(); i++)
		{
			if (user_input[i] != ' '&& user_input[i] != ','&& user_input[i] != '~'&& (user_input[i] <= '0'&& user_input[i] >= '9'))
			{
				return false;
			}
		}
	}
	return true;
}


void printVector(const vector<int> vec)
{
	cout << '{';
	for (int i = 0; i < vec.size(); i++)
	{
		cout << ((i == vec.size() - 1) ? to_string(vec[i]) : to_string(vec[i]) + ", ");
	}
	cout << "}\n";
}


int main()
{
	setlocale(LC_CTYPE, "Russian");

	int max_subsequences_length = 0;
	vector<int> sequence_numbers, subsequence_lengths(1, 1), longest_subsequence;
	string user_input;

	while (true)
	{
		cout << "Введите последовательность чисел через запятую, или ~длинна последовательности (чтобы задать последовательность случайно): ";
		getline(cin, user_input);

		if (!checkUserInput(user_input))
		{
			cout << "Проверьте ввод";
		}
		else
		{
			sequence_numbers = convertToVector(user_input);

			cout << "Ваша последовательность: ";
			printVector(sequence_numbers);
			cout << endl;
			cout << "==============================================================\n";
			cout << endl;

			// Ищем длины подпоследовательностей 
			for (int i = 1; i < sequence_numbers.size(); i++)
			{
				int max_subsequence_length = 0;
				for (int j = i - 1; j >= 0; j--)
				{
					if (sequence_numbers[j] < sequence_numbers[i] && max_subsequence_length < subsequence_lengths[j])
					{
						max_subsequence_length = subsequence_lengths[j];
					}
				}
				subsequence_lengths.push_back(max_subsequence_length + 1);
			}

			// Получаем наибольшую самой длиной
			for (int i = 0; i < subsequence_lengths.size(); i++)
			{
				if (subsequence_lengths[i] > max_subsequences_length)
				{
					max_subsequences_length = subsequence_lengths[i];
				}
			}
			cout << "Наибольшая длина возрастающей подпоследовательности: " << max_subsequences_length << "\n";

			// Восстанавливаем самую длинную
			for (int i = subsequence_lengths.size() - 1; i >= 0; i--)
			{
				if (subsequence_lengths[i] == max_subsequences_length)
				{
					longest_subsequence.insert(longest_subsequence.begin(), sequence_numbers[i]);
					max_subsequences_length--;
				}
			}
			cout << "Подпоследовательность: ";
			printVector(longest_subsequence);
			cout << endl;
		}
	}
}