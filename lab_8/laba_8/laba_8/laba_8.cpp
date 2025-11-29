#include <set>
#include <vector>
#include <iostream>
#include <windows.h>
#include <algorithm>

using namespace std;


struct Item {
    string name;
    int price;
    int weight;
};


struct Backpack {
    int volume;
    vector<Item> items;
};


int main() {
    setlocale(LC_CTYPE, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int user_input;
    Backpack backpack;
    vector<Item> items;
    bool is_successful_input = true;

    while (true)
    {
        cout << "Сделайте выбор\n";
        cout << "1 - Заполнить рюкзак автоматически\n";
        cout << "2 - Ввести значения вручную\n";
        cin >> user_input;
        switch (user_input)
        {
        case 1:
        {
            backpack.volume = 50;
            Item apple = { "Яблоко", 5, 2 };
            Item book = { "Книга", 20, 10 };
            Item laptop = { "Ноутбук", 500, 30 };
            Item bublic = { "Бублик", 70, 50 };
            Item water = { "Бутылка воды", 9, 5 };

            /* 0 1 2 3 4 5 6
            0  0 0 0 0 0 0 0
            1  0 0 0 0 0 0 0  
            2  0 5 0 0 0 0 0
            3  0 5 0 0 0 0 0
            3  0 5 0 0 0 0 0
            4  0 5 0 0 0 0 0
            5  0 5 0 0 0 0 9
            6  0 5 0 0 0 0 9
            7  0 14 0 0 0 0 14
            8
            9
           10
           ..
           51
            */
            items = { apple ,book ,laptop , bublic , water };
            is_successful_input = true;
        }
        break;
        case 2:
            cout << "Введите объем рюкзака: ";
            cin >> backpack.volume;
            while (true)
            {
                Item newItem;
                cout << "Введите информацию о новом предмете в формате Название Стоимость Вес (разделяя пробелами), 0 для того чтобы закончить ввод: ";
                cin >> newItem.name;
                if (newItem.name == "0")
                {
                    break;
                }
                cin >> newItem.price;
                cin >> newItem.weight;
                items.push_back(newItem);
                cout << "Новый предмет успешно добавлен в список предметов.\n";
                cout << "Информация о новом предмете:\n";
                cout << "========================================\n";
                cout << "Название предмета: " << newItem.name << "\n";
                cout << "Цена предмета: " << newItem.price << "\n";
                cout << "Вес предмета: " << newItem.weight << "\n";
                cout << "========================================\n";
                is_successful_input = true;
            }
            break;
        default:
            cout << "Ошибка ввода\n";
            is_successful_input = false;
            break;
        }
        if (is_successful_input)
        {

            vector<vector<int>> best_choices(backpack.volume + 1, vector<int>(items.size() + 1, 0));

            cout << "Рюкзак (объем: " << backpack.volume << ")" << endl;
            for (int w = 1; w < best_choices.size(); w++)
            {
                for (int j = 1; j < best_choices[w].size(); j++)
                {
                    int previous_items_set = best_choices[w][j - 1];
                    int this_item_price = items[j - 1].price;
                    int this_item_weight = items[j - 1].weight;
                    if (this_item_weight <= w)  // Влезет ли новый 
                    {
                        best_choices[w][j] = max(previous_items_set, this_item_price + best_choices[w - this_item_weight][j - 1]); // Можем ли еще что-то доложить?
                    }
                    else
                    {
                        best_choices[w][j] = previous_items_set;  // Берем предыщие предметы 
                    }
                }
            }

            // Восстанавливаем вошедшие предметы 
            vector<string> vector_incoming_items;
            for (int i = 0; i < backpack.volume; i++)
            {
                int act_price = best_choices[i][items.size()];
                if (act_price != 0)
                {
                    for (int j = 0; j < items.size(); j++)
                    {
                        if (items[j].price == act_price)
                        {
                            vector_incoming_items.push_back(items[j].name);
                        }
                    }
                }
            }
            set<string> uniqueValues(vector_incoming_items.begin(), vector_incoming_items.end());
            vector<string> gone_items(uniqueValues.begin(), uniqueValues.end());

            cout << "Максимальная стоимость предметов в рюкзаке: " << best_choices[backpack.volume][items.size()] << "\n";
            cout << "Предметы в рюкзаке: ";
            cout << "{ ";
            for (int i = 0; i < gone_items.size(); i++)
            {
                cout << gone_items[i] << " ";
            }
            cout << "}\n";
        }

    }
    return 0;
}