#include <iostream>

using namespace std;

struct Node
{
    int data;
    int height;
    Node* left;
    Node* right;
};

// создание нового узла
Node* createNode(int value)
{
    Node* node = new Node;

    node->data = value;
    node->height = 1;
    node->left = nullptr;
    node->right = nullptr;

    return node;
}

// получение высоты
int getHeight(Node* node)
{
    if (node == nullptr)
        return 0;

    return node->height;
}

// поиск максимума
int getMax(int a, int b)
{
    if (a > b)
        return a;

    return b;
}

// вычисление баланса
int getBalance(Node* node)
{
    if (node == nullptr)
        return 0;

    return getHeight(node->left) - getHeight(node->right);
}

// правый поворот
Node* rotateRight(Node* y)
{
    Node* x = y->left;
    Node* temp = x->right;

    x->right = y;
    y->left = temp;

    y->height = getMax(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = getMax(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

// левый поворот
Node* rotateLeft(Node* x)
{
    Node* y = x->right;
    Node* temp = y->left;

    y->left = x;
    x->right = temp;

    x->height = getMax(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = getMax(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

// вставка элемента
Node* insert(Node* root, int value)
{
    // обычная вставка
    if (root == nullptr)
        return createNode(value);

    if (value < root->data)
        root->left = insert(root->left, value);
    else if (value > root->data)
        root->right = insert(root->right, value);
    else
        return root;

    // обновляем высоту
    root->height = 1 + getMax(getHeight(root->left), getHeight(root->right));

    // считаем баланс
    int balance = getBalance(root);

    // левый левый случай
    if (balance > 1 && value < root->left->data)
        return rotateRight(root);

    // правый правый случай
    if (balance < -1 && value > root->right->data)
        return rotateLeft(root);

    // левый правый случай
    if (balance > 1 && value > root->left->data)
    {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }

    // правый левый случай
    if (balance < -1 && value < root->right->data)
    {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

// поиск минимального элемента
Node* findMin(Node* root)
{
    Node* current = root;

    while (current->left != nullptr)
    {
        current = current->left;
    }

    return current;
}

// удаление элемента
Node* deleteNode(Node* root, int value)
{
    if (root == nullptr)
        return root;

    // поиск узла
    if (value < root->data)
    {
        root->left = deleteNode(root->left, value);
    }
    else if (value > root->data)
    {
        root->right = deleteNode(root->right, value);
    }
    else
    {
        // если один ребенок или нет детей
        if (root->left == nullptr || root->right == nullptr)
        {
            Node* temp;

            if (root->left != nullptr)
                temp = root->left;
            else
                temp = root->right;

            // если нет детей
            if (temp == nullptr)
            {
                temp = root;
                root = nullptr;
            }
            else
            {
                *root = *temp;
            }

            delete temp;
        }
        else
        {
            // если два ребенка
            Node* temp = findMin(root->right);

            root->data = temp->data;

            root->right = deleteNode(root->right, temp->data);
        }
    }

    if (root == nullptr)
        return root;

    // обновляем высоту
    root->height = 1 + getMax(getHeight(root->left), getHeight(root->right));

    // считаем баланс
    int balance = getBalance(root);

    // левый левый случай
    if (balance > 1 && getBalance(root->left) >= 0)
        return rotateRight(root);

    // левый правый случай
    if (balance > 1 && getBalance(root->left) < 0)
    {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }

    // правый правый случай
    if (balance < -1 && getBalance(root->right) <= 0)
        return rotateLeft(root);

    // правый левый случай
    if (balance < -1 && getBalance(root->right) > 0)
    {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

// симметричный обход
void inorder(Node* root)
{
    if (root == nullptr)
        return;

    inorder(root->left);

    cout << root->data << " ";

    inorder(root->right);
}

// подсчет листьев
int countLeaves(Node* root)
{
    if (root == nullptr)
        return 0;

    // если это лист
    if (root->left == nullptr && root->right == nullptr)
        return 1;

    return countLeaves(root->left) + countLeaves(root->right);
}

int main()
{
    setlocale(LC_ALL, "russian");

    Node* root = nullptr;

    int n;

    cout << "введите количество элементов: ";
    cin >> n;

    cout << "введите элементы дерева: ";

    for (int i = 0; i < n; i++)
    {
        int value;
        cin >> value;

        root = insert(root, value);
    }

    cout << "симметричный обход дерева: ";
    inorder(root);
    cout << endl;

    cout << "колво листьев: ";
    cout << countLeaves(root) << endl;

    int newValue;

    cout << "введите элемент для добавления: ";
    cin >> newValue;

    root = insert(root, newValue);

    cout << "новое дерево после добавления: ";
    inorder(root);
    cout << endl;

    int deleteValue;

    cout << "введите элемент для удаления: ";
    cin >> deleteValue;

    root = deleteNode(root, deleteValue);

    cout << "новое дерево после удаления: ";
    inorder(root);
    cout << endl;

    cout << "новое колво листьев: ";
    cout << countLeaves(root) << endl;


}