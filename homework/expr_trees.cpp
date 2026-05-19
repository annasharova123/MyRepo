#include <iostream>
#include <string>
using namespace std;

struct tree {
    char inf;
    tree *left;
    tree *right;
    tree *parent;
};

// создать новый узел с символом x
tree *node(char x) {
    tree *n = new tree;
    n->inf = x;
    n->left = nullptr;
    n->right = nullptr;
    n->parent = nullptr;
    return n;
}


// выражение без скобок, только однозначные числа
tree *create_tree(string str) {
    tree *tr = nullptr;

    for (int i = 0; i < (int)str.length(); i++) {
        // пропускаем пробелы
        if (str[i] == ' ') continue;

        tree *n = node(str[i]);

        if (str[i] == '+' || str[i] == '-') {
            // + и - имеют низкий приоритет и все текущее дерево становится левым поддеревом
            tr->parent = n;
            n->left = tr;
            tr = n;
        }
        else if (str[i] == '*' || str[i] == '/') {
            if (tr == nullptr || isdigit(tr->inf)) {
                // первый знак в выражении цифра становится левым ребёнком
                tr->parent = n;
                n->left = tr;
                tr = n;
            }
            else {
                // добавляем справа от корня текущий правый ребёнок корня становится левым ребёнком нового узла
                n->parent = tr;
                n->left = tr->right;
                tr->right->parent = n;
                tr->right = n;
            }
        }
        else if (isdigit(str[i])) {
            if (!tr) {
                // первая цифра - корень
                tr = n;
            }
            else if (!tr->right) {
                // у корня нет правого ребёнка  становится им
                n->parent = tr;
                tr->right = n;
            }
            else {
                // ищем узел без правого ребёнка в правом поддереве
                tree *x = tr->right;
                while (x->right)
                    x = x->right;
                n->parent = x;
                x->right = n;
            }
        }
    }

    return tr;
}

// прямой обхо  корень - левое - правое  даёт префиксную запись
void preorder(tree *tr) {
    if (tr) {
        cout << tr->inf << " ";
        preorder(tr->left);
        preorder(tr->right);
    }
}

// обратный обход левое - правое - корень  даёт постфиксную запись
void postorder(tree *tr) {
    if (tr) {
        postorder(tr->left);
        postorder(tr->right);
        cout << tr->inf << " ";
    }
}

// симметричный обход левое  корень - правое  даёт инфиксную запись
void inorder(tree *tr) {
    if (tr) {
        inorder(tr->left);
        cout << tr->inf << " ";
        inorder(tr->right);
    }
}

int main() {
    string str;
    cout << "введите выражение (однозначные числа, без скобок): ";
    getline(cin, str);

    tree *tr = create_tree(str);

    cout << "\nпрефиксная запись (прямой обход):     ";
    preorder(tr);
    cout << endl;

    cout << "инфиксная запись (симметричный обход): ";
    inorder(tr);
    cout << endl;

    cout << "постфиксная запись (обратный обход):   ";
    postorder(tr);
    cout << endl;

}
