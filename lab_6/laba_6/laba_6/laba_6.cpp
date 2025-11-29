#include <iostream>
#include <map>
#include <string>
#include <vector>


using namespace std;



struct Node 
{
    Node* left;
    Node* right;
    int value;
    string name;
    Node()
    {
        left = right = nullptr;
        value = 0;
        name = "";
    }
    Node(string node_name, int node_value)
    {
        left = right = nullptr;
        value = node_value;
        name = node_name;
    }
};


pair<string, int> foundMinValue(const map<string, int>& count_symbols, map<string, int>::iterator target_value)
{
    pair<string, int> min_value = *count_symbols.begin();

    for (auto it = count_symbols.begin(); it != count_symbols.end(); ++it)
    {
        if (it->second < min_value.second && it != target_value)
        {
            min_value = *it; 
        }
    }
    auto it = count_symbols.begin();
    if(min_value == *it && it == target_value)
    {
        it++;
        min_value = *it;
    }
    return min_value; 
}


void get_symbol_codess(map<string, string>& symbol_codess, Node* symbol_tree, string symbol_codes)
{
    if (symbol_tree->left != nullptr || symbol_tree->right != nullptr)
    {
        if (symbol_tree->left != nullptr)
        {
            get_symbol_codess(symbol_codess, symbol_tree->left, symbol_codes + "0");
        }
        if (symbol_tree->right != nullptr)
        {
            get_symbol_codess(symbol_codess, symbol_tree->right, symbol_codes + "1");
        }
    }
    else 
    {
    symbol_codess[symbol_tree->name] = symbol_codes;
    }
}


int replace(string& line, char wildcard_symbol, char replacement_symbol = '~')
{
    int count = 0;
    string new_line = "";
    if (replacement_symbol == '~')
    {
        for (int i = 0; i < line.length(); i++)
        {
            if (line[i] != wildcard_symbol) 
            {
                new_line += line[i];
                count++;
            }
        }
    }
    else
    {
        for (int i = 0; i < line.length(); i++)
        {
            if (line[i] == wildcard_symbol)
            {
                new_line += line[i];
                count++;
            }
        }
    }
    line = new_line;
    return count;
}


int toLowerCase(string& line) 
{
    int count = 0;
    for (int i = 0; i < line.length(); i++)
    {
        if ((line[i] >= 'A' && line[i] <= 'Z') || (line[i] >= 'А' && line[i] <= 'Я') || line[i] == 'Ё')
        {
            line[i] = line[i] + 32;
        }
    }
    return count;
}


int main()
{
    setlocale(LC_ALL, "Russian");
    system("chcp 1251");

    string userInput;
    vector<Node*> symbol_tree;
    map<string, string> symbol_codes;
    map<string, int> count_symbols;

    while (true)
    {
        cout << "Введите строку, которую хотите закодировать: ";
        getline(cin, userInput);
        replace(userInput, ' ');
        if (userInput == "")
        {
            cout << "Строка не может быть пустой, повторите ввод\n";
        }
        else 
        {
            replace(userInput, ' ');
            toLowerCase(userInput);

            // Составляем словарь вошедших символов
            for (int i = 0; i < userInput.length(); i++)
            {
                string key(1, userInput[i]); 
                if (count_symbols[key])
                {
                    count_symbols[key]++;
                }
                else 
                {
                    count_symbols[key] = 1;
                }
            }

            // Инициплизируем дерево первыми вошедшими символами
            for (auto ptr = count_symbols.begin(); ptr != count_symbols.end(); ++ptr)
            {
                Node* new_node = new Node;
                new_node->value = ptr->second;
                new_node->name = ptr->first;
                symbol_tree.push_back(new_node);
            }

            int vector_size = symbol_tree.size();

            // Основной код создания дерева
            for (int i = 0; i < vector_size - 1; i++)
            {
                pair<string, int> min_value = foundMinValue(count_symbols, count_symbols.end());
                pair<string, int> submin_value = foundMinValue(count_symbols, count_symbols.find(min_value.first));
                count_symbols.erase(min_value.first);
                count_symbols.erase(submin_value.first);
                Node* new_node = new Node(min_value.first + submin_value.first, min_value.second + submin_value.second);
                count_symbols[new_node->name] = new_node->value;

                // Ищем корни для новой вершины
                bool foundLeft = false;
                bool foundRight = false;
                for (int j = 0; j < symbol_tree.size(); j++)
                {
                    if (!foundLeft && symbol_tree[j]->name == min_value.first && symbol_tree[j]->value == min_value.second)
                    {
                        new_node->left = symbol_tree[j];
                        foundLeft = true;
                        continue; 
                    }

                    if (!foundRight && symbol_tree[j]->name == submin_value.first && symbol_tree[j]->value == submin_value.second)
                    {
                        new_node->right = symbol_tree[j];
                        foundRight = true;
                    }
                }
                symbol_tree.push_back(new_node);
            }

            // Подбираем код для каждого символа, идя по порядку с вершины дерева
            get_symbol_codess(symbol_codes, symbol_tree[symbol_tree.size() - 1], "");
            cout << "Таблица встречающихся символов и коды для них:\n";
            cout << "Поз.\tСимв.\tКол-во.\tКод символа\n";
            for (int i = 0; i < vector_size; i++)
            {
                cout << i + 1 <<  "\t" << symbol_tree[i]->name << "\t" << symbol_tree[i]->value << "\t" << symbol_codes[symbol_tree[i]->name]<<"\n";
            }
            cout << "\nЗакодированное сообщение:\n";
            for (int i = 0; i < userInput.length(); i++)
            {
                cout << symbol_codes[string(1, userInput[i])] << ' ';
            }
            cout << "\n";

            symbol_tree.clear();
            symbol_codes.clear();
            count_symbols.clear();
        }
    }
    return 0;
}
