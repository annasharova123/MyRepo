#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

ifstream in("employees_sorting_dataset_v3.txt"); // аналог std::cin и std::cout для работы с файлами
ofstream out("output.txt");                     // создает объект in и out, которые читают и выводят файл

struct date{
    int yy, mm, dd;
};

struct employee{
    string surname;
    string position;
    date birth;
    int experience;
    int salary;
};

date str_to_date(string s){ // запись даты рождения в структуру date
    date d;
    d.yy = stoi(s.substr(0,4));
    d.mm = stoi(s.substr(5,2));
    d.dd = stoi(s.substr(8,2));
    return d;
}

// чтение файла
vector<employee> infile(){
    vector<employee> a;
    string line;

    while(getline(in,line)){
        stringstream ss(line); // stringstream берет всю строку с инфой о работнике и читает ее
        string temp;
        employee e;

        getline(ss,e.surname,';'); // читаем до первой ;
        getline(ss,e.position,';'); // до второй ;

        getline(ss,temp,';');
        e.birth = str_to_date(temp);

        getline(ss,temp,';'); // стаж "10 лет"
        e.experience = stoi(temp);

        getline(ss,temp,';');
        e.salary = stoi(temp);

        a.push_back(e);
    }
    return a;
}

// вывод в файл
void print(vector<employee> a){
    for(auto x : a){
        out<<x.surname<<";" // выводит отсортированный массив, используя out(аналог std::cout)
           <<x.position<<";"
           <<x.birth.yy<<"-"<<x.birth.mm<<"-"<<x.birth.dd<<";"
           <<x.experience<<" лет;"
           <<x.salary<<endl;
    }
}


// №3
void radix_sort_salary(vector<employee>& a){

    int max_salary = 0;

    for(auto x : a)
        if(x.salary > max_salary)
            max_salary = x.salary;

    for(int exp = 1; max_salary/exp > 0; exp *= 10){

        vector<vector<employee>> bucket(10);

        for(auto x : a){
            int digit = (x.salary / exp) % 10;
            bucket[digit].push_back(x);
        }

        int k = 0;
        for(int i=0;i<10;i++)
            for(auto x : bucket[i])
                a[k++] = x;
    }
}

// №8

bool cmp8(employee a, employee b){
    if(a.salary < b.salary) return true;
    if(a.salary == b.salary && a.birth.yy < b.birth.yy) return true;
    return false;
}

void merge(vector<employee>& a,int l,int m,int r){

    vector<employee> left(a.begin()+l, a.begin()+m+1);
    vector<employee> right(a.begin()+m+1, a.begin()+r+1);

    int i=0,j=0,k=l;

    while(i<left.size() && j<right.size()){

        if(cmp8(left[i],right[j]))
            a[k++] = left[i++];
        else
            a[k++] = right[j++];
    }

    while(i<left.size())
        a[k++] = left[i++];

    while(j<right.size())
        a[k++] = right[j++];
}

void merge_sort(vector<employee>& a,int l,int r){

    if(l>=r) return;

    int m=(l+r)/2;

    merge_sort(a,l,m);
    merge_sort(a,m+1,r);

    merge(a,l,m,r);
}


// №11

bool cmp11(employee a, employee b){

    if(a.position < b.position) return true;

    if(a.position == b.position && a.experience < b.experience)
        return true;

    if(a.position == b.position &&
       a.experience == b.experience &&
       a.salary < b.salary)
        return true;

    return false;
}

void bubble_sort(vector<employee>& a){

    for(int i=0;i<a.size()-1;i++)
        for(int j=0;j<a.size()-i-1;j++)
            if(!cmp11(a[j],a[j+1]))
                swap(a[j],a[j+1]);
}



int main(){

    vector<employee> a = infile();

    int choice;

    cout<<"1 - №3"<<endl;
    cout<<"2 - №8"<<endl;
    cout<<"3 - №11"<<endl;

    cin>>choice;

    if(choice==1)
        radix_sort_salary(a);

    if(choice==2)
        merge_sort(a,0,a.size()-1);

    if(choice==3)
        bubble_sort(a);

    print(a);

}