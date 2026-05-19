#include <iostream>

using namespace std;

enum Color
{
    RED,
    BLACK
};

struct Node
{
    int data;
    Color color;
    Node* left;
    Node* right;
    Node* parent;
};

// создание нового узла
Node* createNode(int value)
{
    Node* node = new Node;

    node->data = value;
    node->color = RED;
    node->left = nullptr;
    node->right = nullptr;
    node->parent = nullptr;

    return node;
}

// левый поворот
void rotateLeft(Node*& root, Node* node)
{
    Node* child = node->right;
    node->right = child->left;

    if (child->left != nullptr)
        child->left->parent = node;

    child->parent = node->parent;

    if (node->parent == nullptr)
        root = child;
    else if (node == node->parent->left)
        node->parent->left = child;
    else
        node->parent->right = child;

    child->left = node;
    node->parent = child;
}

// правый поворот
void rotateRight(Node*& root, Node* node)
{
    Node* child = node->left;
    node->left = child->right;

    if (child->right != nullptr)
        child->right->parent = node;

    child->parent = node->parent;

    if (node->parent == nullptr)
        root = child;
    else if (node == node->parent->right)
        node->parent->right = child;
    else
        node->parent->left = child;

    child->right = node;
    node->parent = child;
}

// балансировка после вставки
void fixInsert(Node*& root, Node* node)
{
    while (node != root && node->parent->color == RED)
    {
        Node* parent = node->parent;
        Node* grandparent = parent->parent;

        // родитель слева
        if (parent == grandparent->left)
        {
            Node* uncle = grandparent->right;

            // если дядя красный
            if (uncle != nullptr && uncle->color == RED)
            {
                parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;
                node = grandparent;
            }
            else
            {
                // левый-правый случай
                if (node == parent->right)
                {
                    node = parent;
                    rotateLeft(root, node);
                }

                // левый-левый случай
                parent->color = BLACK;
                grandparent->color = RED;
                rotateRight(root, grandparent);
            }
        }
        else
        {
            Node* uncle = grandparent->left;

            // если дядя красный
            if (uncle != nullptr && uncle->color == RED)
            {
                parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;
                node = grandparent;
            }
            else
            {
                // правый-левый случай
                if (node == parent->left)
                {
                    node = parent;
                    rotateRight(root, node);
                }

                // правый-правый случай
                parent->color = BLACK;
                grandparent->color = RED;
                rotateLeft(root, grandparent);
            }
        }
    }

    root->color = BLACK;
}

// вставка элемента
void insert(Node*& root, int value)
{
    Node* newNode = createNode(value);

    Node* parent = nullptr;
    Node* current = root;

    while (current != nullptr)
    {
        parent = current;

        if (value < current->data)
            current = current->left;
        else
            current = current->right;
    }

    newNode->parent = parent;

    // если дерево пустое
    if (parent == nullptr)
    {
        root = newNode;
    }
    else if (value < parent->data)
    {
        parent->left = newNode;
    }
    else
    {
        parent->right = newNode;
    }

    fixInsert(root, newNode);
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

// вывод листьев
void printLeaves(Node* root)
{
    if (root == nullptr)
        return;

    if (root->left == nullptr && root->right == nullptr)
    {
        cout << root->data << " ";
    }

    printLeaves(root->left);
    printLeaves(root->right);
}

// поиск минимального элемента
Node* findMin(Node* node)
{
    while (node->left != nullptr)
    {
        node = node->left;
    }

    return node;
}

// удаление элемента
Node* deleteNode(Node* root, int value)
{
    if (root == nullptr)
        return root;

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
        // если нет детей
        if (root->left == nullptr && root->right == nullptr)
        {
            delete root;
            return nullptr;
        }

        // если один ребенок справа
        if (root->left == nullptr)
        {
            Node* temp = root->right;
            delete root;
            return temp;
        }

        // если один ребенок слева
        if (root->right == nullptr)
        {
            Node* temp = root->left;
            delete root;
            return temp;
        }

        // если два ребенка
        Node* temp = findMin(root->right);

        root->data = temp->data;

        root->right = deleteNode(root->right, temp->data);
    }

    return root;
}

int main()
{
    setlocale(LC_ALL, "russian");

    Node* root = nullptr;

    int n;

    cout << "введите количетсво элементов: ";
    cin >> n;

    cout << "ввдите элементы дерева: ";

    for (int i = 0; i < n; i++)
    {
        int value;
        cin >> value;

        insert(root, value);
    }

    cout << "симметричный обход дерева: ";
    inorder(root);
    cout << endl;

    cout << "листья дерева: ";
    printLeaves(root);
    cout << endl;

    int newValue;

    cout << "введите элемент для добавления: ";
    cin >> newValue;

    insert(root, newValue);

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

}