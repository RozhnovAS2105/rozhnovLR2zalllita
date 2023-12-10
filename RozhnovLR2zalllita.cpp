#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>
#include <Windows.h>
#include <locale>

using namespace std;
void print_menu()
{
    cout << "1 - Зашифровать" << "\n";
    cout << "2 - Дешифровать" << "\n";
    cout << "3 - Выход" << "\n";
};

int main()
{
    locale::global(locale(""));
    //SetConsoleCP(1251);
    //SetConsoleOutputCP(1251);

    ifstream in;
    ofstream out;
    string inputFileName, outputFileName, keyword;

    int option = -1;
    while (option) {
        print_menu();
        cin >> option;
        switch (option)
        {
        case 1:
        {
            cout << "\nВведите название файла, который хотите зашифровать: ";
            cin >> inputFileName;
            cout << "Введите название файла, в который будет зашифровываться текст: ";
            cin >> outputFileName;
            while (true)
            {
//бинарный режим помогает корректно обрабатывать текст, предотвращая искажения в тексте.
//(связанных с символами новой строки и другими специальными символами.)
                in.open(inputFileName + ".txt", ios::binary);
                out.open(outputFileName + ".txt", ios::binary);

                if (!in.is_open() || !out.is_open())
                {
                    cout << "\nОшибка! \nПроверьте существование файла с таким названием." << endl;
                    cout << "\nВведите название файла, который хотите зашифровать: ";
                    cin >> inputFileName;
                    cout << "Введите название файла, в который будет зашифровываться текст: ";
                    cin >> outputFileName;
                }
                else
                {
                    break; // Выход из цикла, если файлы успешно открыты
                }
            }

            cout << "Введите ключевоe слово" << "\n";
            cin >> keyword;
            //k - символ ключа, symbol - итоговый символ, n - символ из входного файла
            unsigned char symbol;
            unsigned char n;
            //unsigned char k;
            int keyi = 0;




            while (in.is_open() && out.is_open())
            {
                //получение сивмола из входного файла
                n = in.get();
                symbol = (unsigned char)(n + keyword[keyi]);
                //запись итогового(зашифрованного) символа в файл вывода
                out << symbol;
                ++keyi;
                if (keyi == keyword.size())
                {
                    keyi = 0;
                }
                //если достигнут конец файла
                if (in.peek() == EOF)
                {
                    break;
                }
            };
            in.close();
            out.close();
            break;

        };
        case 2:
        {
            cout << "\nВведите название файла, который хотите расшифровать: ";
            string cryptfile;
            cin >> cryptfile;
            ifstream fin(cryptfile + ".txt", ios::binary);
//пара итераторов обозначает диапазон символов, которые будут использоваться для инициализации вектора.
//в данном векторе будут хранится последовательно все символы из файла ввода
            vector<unsigned char> crypt_Text((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
            fin.close();
            double M = 0;
            int M_ind;
            // нахождение длины ключа: метод индекса совпадений
            for (int i = 1; i < 26; i++)
            {
                float INDsovpadenia = 0.0;
                float n = crypt_Text.size() / i; //разделение всего текста на длину ключа
                vector<int> letters(256, 0);
                for (int j = 0; j < 256; j++)
                {
                //благодаря этому циклу, мы проходим по каждому i-му символу 
                    for (int k = 0; k < crypt_Text.size(); k += i)
                    {
                        if (crypt_Text[k] == (unsigned char)j)
                        {
                            letters[j]++;
                        }
                    }

                    if (letters[j] != 0)
                    {
                        //формула индекса совпадений
                        INDsovpadenia += (float)(letters[j] * (letters[j] - 1)) / (n * (n - 1)); 
                    }
                }
                cout << i << ": " << INDsovpadenia << endl;
                if (INDsovpadenia >= M) {
                    M = INDsovpadenia;
                    M_ind = i;
                }
            }
            cout << "Наибольшее значение " << M << " соответствует длине: " << M_ind << endl;
            //..cout << "\nВыберите длину ключа: "; 
            //..int keySize;
            //..cin >> keySize;

            //int M_ind;
            //cin >> M_ind;
            // 
//нахождение самой встречающейся буквы в тексте.
            cout << "\nВведите эталонный файл: ";
            string testfile;
            cin >> testfile;
            fin = ifstream(testfile + ".txt", ios::binary);
            //такая же ситуация, как выше, т.е. вектор с символами из эталлоного файла
            vector<unsigned char> reference_Text((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
            fin.close();

            //подсчёт для наиболее частичной буквы через сортировки 

            vector<pair<unsigned char, int>> reference_Letters(256);//256 - размер вектора
            for (int i = 0; i < 256; i++)
            {
                //i - символ, 0 - частота
                reference_Letters[i] = make_pair(i, 0);
            }
        //проходимяя по всему вектору эталонного файла
            for (int i = 0; i < reference_Text.size(); i++)
            {
                reference_Letters[reference_Text[i]].second++;
            }
            //сортируем по убыванию частоты
            sort(reference_Letters.begin(), reference_Letters.end(), [](auto& left, auto& right) { return left.second > right.second; });
            //выводим первый компонент первого элемента, что и будет являться наиболее частой буквой
            unsigned char most_in_reference = reference_Letters[0].first;
            cout << "\nНаиболее частая буква в эталоне: '" << most_in_reference << "'" << '\n';


            
//нахождение ключа
            vector<unsigned char> key;
            //для каждой
            for (int i = 0; i < M_ind; i++)
            {
//вектор будет использоваться для подсчета частоты встречаемости символов в зашифрованном тексте.
                vector<int> crypt_Letters(256, 0);

                for (int j = i; j < crypt_Text.size(); j += M_ind) //j += M_ind - шаг
                {
                    crypt_Letters[crypt_Text[j]]++;
                }
                //сравнение и присваивание наибольшей частоты встречаемости
                unsigned char most_in_crypt = 0;
                int M = 0;
                for (int j = 0; j < 256; j++)
                {
                    if (crypt_Letters[j] > M)
                    {
                        M = crypt_Letters[j];
                        most_in_crypt = j;
                    }
                }

                key.push_back(256 + most_in_crypt - most_in_reference);
            }
//после обхода всех символов, вычисляется разница между имволом с максимальным количеством вхождений
//в зашифрованном тексте (most_in_crypt) и символом с максимальным количеством вхождений в эталонном тексте (most_in_reference).
            cout << "\nПолучен ключ: ";

            for (const auto& sym : key)
            {
                cout << sym;
            }
            cout << '\n';


            cout << "\nВведите файл для дешифровки: ";
            string decryptfile;
            cin >> decryptfile;
            ofstream out;
            out.open(decryptfile + ".txt", ios::binary);
            if (out.is_open())
            {
                int i = 0;
                for (const auto& sym : crypt_Text)
                {
                    out << (unsigned char)(sym - key[i]);
                    i++;
                    if (i >= key.size())
                    {
                        i = 0;
                    }
                }
                out.close();
            }
            break;
        };
        case 3:
        {
            return 0;
        }
        }
    }
}
