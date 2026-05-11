#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

// структура для хранения данных о сотруднике
struct Employee {
    string surname;      // фамилия сотрудника
    string position;     // должность
    string birthDate;    // дата рождения в формате 1985-04-12
    string experience;   // стаж работы ("10 лет")
    int salary;          // зп в рублях
};

// узел двусвязного списка т е элемент для цепочки при коллизии
struct Node {
    Employee data;  // данные сотрудника
    Node* next;     // указатель на следующий узел в цепочке
    Node* prev;     // указатель на предыдущий узел в цепочке
    
    // конструктор узла который заполняет данные и обнуляет указатели
    Node(Employee emp) : data(emp), next(nullptr), prev(nullptr) {}
};

// этот класс реализует двусвязный список для хранения сотрудников в одной ячейке хеш-таблицы
class DoublyLinkedList {
public:
    Node* head;  // ук на первый узел списка (голову)
    
    //  создаём пустой список
    DoublyLinkedList() : head(nullptr) {}
    
    // добавление нового сотрудника в начало списка
    void addFront(Employee emp) {
        Node* newNode = new Node(emp); // создаём новый узел с данными сотрудника
        if (head == nullptr) {         // если список пуст,
            head = newNode;            // новый узел становится головой
        } else {                       // если в списке уже есть элементы,
            newNode->next = head;      // следующий за новым узлом этл старая голова
            head->prev = newNode;      // предыдущий для старой головы это новый узел
            head = newNode;            // голова списка смещается на новый узел
        }
    }
    
    // удаление узла по фамилии сотрудника 
    bool removeByName(string surname) {
        Node* current = head;          // начинаем обход с головы списка
        while (current != nullptr) {   // пока не дошли до конца списка
            if (current->data.surname == surname) { // если фамилия совпала,
                // если удаляемый узел — голова списка
                if (current->prev == nullptr) {
                    head = current->next; // головой становится следующий узел
                }
                // если удаляемый узел не голова
                else {
                    current->prev->next = current->next; // перебрасываем ссылку предыдущего узла
                }
                // если удаляемый узел не хвост
                if (current->next != nullptr) {
                    current->next->prev = current->prev; // перебрасываем ссылку следующего узла
                }
                delete current; // освобождаем память, занятую узлом
                return true;    // удаление прошло успешно
            }
            current = current->next; // переходим к следующему узлу
        }
        return false; // сотрудник с такой фамилией не найден
    }
    
    // вывод фамилий всех сотрудников в данной цепочке
    void display() {
        Node* current = head;        // начинаем с головы
        if (current == nullptr) {    // если цепочка пуста,
            cout << "  пусто\n";   // выводим сообщение
            return;
        }
        while (current != nullptr) { // пока есть узлы,
            cout << "  " << current->data.surname; // выводим фамилию
            current = current->next; // переходим к следующему
        }
        cout << "\n"; // завершаем строку
    }
    
    // поиск сотрудника по фамилии и вывод полной информации о нём
    bool findAndDisplay(string surname) {
        Node* current = head;        // начинаем просмотр с головы
        while (current != nullptr) { // обходим список
            if (current->data.surname == surname) { // если нашли нужную фамилию,
                // выводим полную информацию о сотруднике
                cout << "         ИНФОРМАЦИЯ О СОТРУДНИКЕ        \n";
                cout << " фамилия:      " << current->data.surname << "\n";
                cout << " должность:    " << current->data.position << "\n";
                cout << " дата рождения:" << current->data.birthDate << "\n";
                cout << " стаж:         " << current->data.experience << "\n";
                cout << " зарплата:     " << current->data.salary << " руб.\n";
                return true; // данные найдены и выведены
            }
            current = current->next; // двигаемся дальше
        }
        return false; // сотрудник не найден в этом списке
    }
    
    // полная очистка списка с освобождением памяти
    void clear() {
        while (head != nullptr) { // пока список не пуст,
            Node* temp = head;    // запоминаем текущую голову
            head = head->next;    // смещаем голову на следующий узел
            delete temp;          // удаляем старую голову
        }
    }
    
    //  автоматически вызывает очистку списка при уничтожении объекта
    ~DoublyLinkedList() {
        clear();
    }
};

// класс хеш-таблицы с открытым хешированием (метод цепочек)
class HashTable {
private:
    vector<DoublyLinkedList> table;  // массив из 12 двусвязных списков (по числу месяцев)
    int size;                         // размер таблицы (количество ячеек)
    
public:
    //  создаём таблицу из 12 ячеек (январь–декабрь)
    HashTable() : size(12) {
        table.resize(size);  // выделяем память под 12 списков
    }
    
    // хеш-функция т е она извлекает номер месяца из даты рождения и возвращает индекс ячейки (0–11)
    //месяц находится на позициях 5–6
    int hashFunction(string birthDate) {
        int month = stoi(birthDate.substr(5, 2)); // выделяем подстроку с месяцем и преобразуем в число
        return month - 1;  // переводим номер месяца в индекс массива (январь = 1 - индекс 0)
    }
    
    // вставка сотрудника в хеш-таблицу
    void insert(Employee emp) {
        int index = hashFunction(emp.birthDate); // вычисляем индекс по дате рождения
        table[index].addFront(emp);              // добавляем сотрудника в соответствующий список
    }
    
    // поиск сотрудника по фамилии во всей хеш-таблице
    bool search(string surname) {
        for (int i = 0; i < size; i++) {         // перебираем все ячейки таблицы
            Node* current = table[i].head;       // получаем голову цепочки в i-й ячейке
            while (current != nullptr) {         // обходим цепочку
                if (current->data.surname == surname) { // если фамилия совпала,
                    return table[i].findAndDisplay(surname); // выводим данные и выходим
                }
                current = current->next;         // иначе двигаемся по цепочке
            }
        }
        cout << "Сотрудник не найден!\n";        // фамилия не найдена ни в одной цепочке
        return false;
    }
    
    // удаление всех сотрудников с заданной фамилией из хеш-таблицы
    bool remove(string surname) {
        bool found = false;                      // флаг: был ли найден хотя бы один сотрудник
        for (int i = 0; i < size; i++) {         // обходим все ячейки
            while (table[i].removeByName(surname)) { // пока в цепочке есть сотрудники с такой фамилией,
                found = true;                    // отмечаем, что удаление произошло
            }
        }
        if (found) {
            cout << "сотрудник(и) удален(ы)\n";  // сообщаем об успешном удалении
        } else {
            cout << "сотрудник не найден!\n";    // ни одного сотрудника с такой фамилией нет
        }
        return found;
    }
    
    // вывод всей хеш-таблицы на экран
    void display() {
        cout << "ХЕШ-ТАБЛИЦА (ПО МЕСЯЦАМ)\n";
        
        // массив названий месяцев для наглядного вывода
        string months[] = {
            "Январь (01)", "Февраль (02)", "Март (03)", "Апрель (04)",
            "Май (05)", "Июнь (06)", "Июль (07)", "Август (08)",
            "Сентябрь (09)", "Октябрь (10)", "Ноябрь (11)", "Декабрь (12)"
        };
        
        for (int i = 0; i < size; i++) {          // для каждой ячейки таблицы
            cout << "║ " << months[i] << ": ";    // выводим название месяца
            cout.width(26 - (int)months[i].length()); // выравниваем вывод для красоты
            cout << left << "";
            table[i].display();                   // выводим содержимое цепочки
        }
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
    
    // создаём хеш-таблицу и заполняем её сотрудниками
    HashTable hashTable;
    for (const auto& emp : employees) {      // для каждого сотрудника из вектора
        hashTable.insert(emp);               // вставляем его в хеш-таблицу
    }
    
    // выводим содержимое хеш-таблицы на экран
    hashTable.display();
    
    // главное меню программы
    int choice;
    string surname;
    
    while (true) {                           // бесконечный цикл меню
        cout << " 1. Поиск сотрудника                    \n";
        cout << " 2. Удалить сотрудника                  \n";
        cout << " 3. Показать таблицу заново             \n";
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