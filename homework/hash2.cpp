#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;


struct Employee {
    string surname;     
    string position;     
    string birthDate;    
    string experience;   
    int salary;         
};

// класс, реализующий закрытую хеш-таблицу с линейным опробированием
class ClosedHashTable {
private:
    vector<Employee> table;      // массив для хранения элементов (сама хеш-таблица)
    vector<bool> occupied;       // массив флагов: true — ячейка занята, false — свободна
    vector<bool> deleted;        // массив флагов: true — элемент удалён (для корректного поиска)
    int size;                     // размер хеш-таблицы (количество ячеек)
    int count;                    // текущее количество элементов в таблице
    
    // вспомогательная хеш-функция: метод деления по году рождения
    // извлекает год из даты (первые 4 символа) и вычисляет остаток от деления на размер таблицы
    int hash1(string birthDate) {
        int year = stoi(birthDate.substr(0, 4)); // извлекаем год из даты (символы с 0 по 3)
        return year % size;                       // остаток от деления года на размер таблицы
    }
    
    // основная хеш-функция
    // h(x, i) = (hash1(x) + i) mod size
    // i — номер попытки (0, 1, 2, ...)
    int hashFunction(string birthDate, int i) {
        return (hash1(birthDate) + i) % size; // линейное смещение на i позиций
    }
    
public:
    //создаёт хеш-таблицу заданного размера
    ClosedHashTable(int tableSize) : size(tableSize), count(0) {
        table.resize(size);      // выделяем память под массив элементов
        occupied.resize(size, false); // изначально все ячейки свободны
        deleted.resize(size, false);  // изначально удалённых элементов нет
    }
    
    // вставка сотрудника в хеш-таблицу
    bool insert(Employee emp) {
        // проверяем, есть ли ещё место в таблице
        if (count >= size) {
            cout << "таблица заполнена, невозможно добавить сотрудника\n";
            return false;
        }
        
        // пробуем вставить элемент, перебирая возможные ячейки
        for (int i = 0; i < size; i++) {
            int index = hashFunction(emp.birthDate, i); // вычисляем индекс для i-й попытки
            // если ячейка свободна или ранее была удалена — занимаем её
            if (!occupied[index] || deleted[index]) {
                table[index] = emp;          // записываем сотрудника в ячейку
                occupied[index] = true;      // помечаем ячейку как занятую
                deleted[index] = false;      // снимаем флаг удаления, если он был
                count++;                     // увеличиваем счётчик элементов
                return true;                 // вставка прошла успешно
            }
        }
        // если все попытки исчерпаны, значит таблица заполнена
        cout << "не удалось вставить сотрудника (все ячейки заняты)\n";
        return false;
    }
    
    // поиск сотрудника по фамилии и вывод полной информации о нём
    bool search(string surname) {
        // для поиска нужно знать дату рождения, но у нас есть только фамилия
        // поэтому перебираем последовательно все ячейки таблицы
        for (int i = 0; i < size; i++) {
            // если ячейка занята и не удалена, проверяем фамилию
            if (occupied[i] && !deleted[i] && table[i].surname == surname) {
                // выводим полную информацию о найденном сотруднике
                cout << "         ИНФОРМАЦИЯ О СОТРУДНИКЕ        \n";
                cout << " фамилия:      " << table[i].surname << "\n";
                cout << " должность:    " << table[i].position << "\n";
                cout << " дата рождения:" << table[i].birthDate << "\n";
                cout << " стаж:         " << table[i].experience << "\n";
                cout << " зарплата:     " << table[i].salary << " руб.\n";
                cout << " ячейка таблицы: " << i << "\n"; // показываем, в какой ячейке найден
                return true; // сотрудник найден
            }
        }
        cout << "cотрудник не найден!\n"; // фамилия не найдена ни в одной занятой ячейке
        return false;
    }
    
    // удаление сотрудника по фамилии (помечаем ячейку как удалённую)
    bool remove(string surname) {
        // перебираем все ячейки таблицы
        for (int i = 0; i < size; i++) {
            // если ячейка занята и не удалена, и фамилия совпадает
            if (occupied[i] && !deleted[i] && table[i].surname == surname) {
                deleted[i] = true;   // помечаем ячейку как удалённую
                count--;             // уменьшаем счётчик элементов
                cout << "сотрудник удалён из ячейки " << i << "\n";
                // не прерываем цикл, так как могут быть однофамильцы
            }
        }
        // дополнительный проход для проверки, был ли удалён хотя бы один
        for (int i = 0; i < size; i++) {
            if (deleted[i] && table[i].surname == surname) {
                return true; // если нашли удалённого с такой фамилией, возвращаем успех
            }
        }
        cout << "сотрудник не найден!\n";
        return false;
    }
    
    // вывод всей хеш-таблицы на экран
    void display() {
        cout << "     ЗАКРЫТАЯ ХЕШ-ТАБЛИЦА (ЛИНЕЙНОЕ ОПРОБИРОВАНИЕ)     \n";
        cout << "     вспомогательная функция: метод деления по году     \n";
        cout << "     размер таблицы: " << size << " ячеек               \n";
        cout << "     занято: " << count << " элементов               \n\n";
        
        for (int i = 0; i < size; i++) { // обходим все ячейки таблицы
            cout << "ячейка " << i << ": ";
            if (!occupied[i]) {          // если ячейка свободна
                cout << "[пусто]\n";
            } else if (deleted[i]) {     // если элемент удалён
                cout << "[удалено]\n";
            } else {                     // если ячейка занята
                cout << table[i].surname << " (год:" << table[i].birthDate.substr(0, 4) << ")\n";
            }
        }
        cout << "\n";
    }
    
    // вывод статистики по таблице (для демонстрации заполнения)
    void stats() {
        int occupiedCount = 0;
        int deletedCount = 0;
        int freeCount = 0;
        for (int i = 0; i < size; i++) {
            if (!occupied[i]) freeCount++;
            else if (deleted[i]) deletedCount++;
            else occupiedCount++;
        }
        cout << "статистика: занято=" << occupiedCount 
             << ", удалено=" << deletedCount 
             << ", свободно=" << freeCount << "\n";
    }
};

// функция загрузки сотрудников из файла
vector<Employee> loadEmployees(string filename) {
    vector<Employee> employees;            // вектор для хранения считанных сотрудников
    ifstream file(filename);               // открываем файл для чтения
    
    if (!file.is_open()) {                 // если файл не удалось открыть,
        cerr << "не удалось открыть файл\n"; // выводим сообщение об ошибке
        return employees;                  // возвращаем пустой вектор
    }
    
    string line;
    while (getline(file, line)) {          // читаем файл построчно
        // строка имеет формат: Иванов;Инженер;1985-04-12;10 лет;45000
        Employee emp;                      // создаём временный объект сотрудника
        
        // ищем позиции разделителей ";"
        int pos1 = line.find(';');                     // конец фамилии
        int pos2 = line.find(';', pos1 + 1);           // конец должности
        int pos3 = line.find(';', pos2 + 1);           // конец даты рождения
        int pos4 = line.find(';', pos3 + 1);           // конец стажа
        
        // извлекаем подстроки по найденным позициям
        emp.surname = line.substr(0, pos1);                          // фамилия
        emp.position = line.substr(pos1 + 1, pos2 - pos1 - 1);       // должность
        emp.birthDate = line.substr(pos2 + 1, pos3 - pos2 - 1);      // дата рождения
        emp.experience = line.substr(pos3 + 1, pos4 - pos3 - 1);     // стаж
        emp.salary = stoi(line.substr(pos4 + 1));                    // зарплата (преобразуем в int)
        
        employees.push_back(emp);            // добавляем сотрудника в вектор
    }
    
    file.close();                            // закрываем файл
    cout << "загружено сотрудников: " << employees.size() << "\n\n";
    return employees;                        // возвращаем вектор с данными
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");        // настраиваем локаль для корректного вывода кириллицы
    
    // загружаем сотрудников из файла в вектор
    vector<Employee> employees = loadEmployees("employees_sorting_dataset_v3.txt");
    
    if (employees.empty()) {                 // если файл пуст или не найден,
        return 1;                            // завершаем программу с кодом ошибки
    }
    
    // определяем размер хеш-таблицы как ближайшее простое число, большее количества сотрудников
    // для примера возьмём 23 (простое число, не являющееся степенью двойки минус 1)
    int tableSize = 23;
    cout << "создаём закрытую хеш-таблицу размером " << tableSize << " ячеек\n\n";
    
    // создаём хеш-таблицу и заполняем её сотрудниками
    ClosedHashTable hashTable(tableSize);
    
    // вставляем сотрудников в таблицу
    int insertedCount = 0;
    for (const auto& emp : employees) {      // для каждого сотрудника из вектора
        if (!hashTable.insert(emp)) {        // пытаемся вставить
            cout << "не удалось вставить: " << emp.surname << "\n";
        } else {
            insertedCount++;
        }
    }
    cout << "успешно вставлено: " << insertedCount << " из " << employees.size() << "\n\n";
    
    // выводим содержимое хеш-таблицы на экран
    hashTable.display();
    hashTable.stats();
    
    // главное меню программы
    int choice;
    string surname;
    
    while (true) {                           // бесконечный цикл меню
        cout << "\n 1. Поиск сотрудника                    \n";
        cout << " 2. Удалить сотрудника                  \n";
        cout << " 3. Показать таблицу заново             \n";
        cout << " 4. Статистика таблицы                  \n";
        cout << " 0. Выход                                \n";

        cout << "Выберите действие: ";
        cin >> choice;                       // считываем выбор пользователя
        cin.ignore();                        // очищаем буфер ввода от символа новой строки
        
        switch (choice) {
            case 1: {                        // поиск сотрудника
                cout << "введите фамилию сотрудника: ";
                getline(cin, surname);       // считываем фамилию
                hashTable.search(surname);   // выполняем поиск по таблице
                break;
            }
            case 2: {                        // удаление сотрудника
                cout << "введите фамилию для удаления: ";
                getline(cin, surname);       // считываем фамилию
                hashTable.remove(surname);   // удаляем всех с такой фамилией
                break;
            }
            case 3: {                        // повторный вывод таблицы
                hashTable.display();
                break;
            }
            case 4: {                        // вывод статистики
                hashTable.stats();
                break;
            }
            case 0: {                        // выход из программы
                cout << "\nдо свидания\n";
                return 0;
            }
            default: {                       // неверный ввод
                cout << "неверный выбор\n";
            }
        }
    }
}