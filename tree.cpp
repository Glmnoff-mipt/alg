#include <random>
#include <iostream>


template <typename T>
class Treap 
{
private:
    struct Node 
    {
        T value;              
        int priority;         
        int count;            
        Node* left;         
        Node* right;          
        Node(T val, int prio) : value(val), priority(prio), count(1), left(nullptr), right(nullptr) {}
    };

    Node* root;              
    std::mt19937 rng;         // Генератор случайных чисел

    int getCount(Node* node) 
    {
        return node ? node->count : 0;
    }

    void updateCount(Node* node) 
    {
        if (node) 
        {
            node->count = 1 + getCount(node->left) + getCount(node->right);
        }
    }

    void split(Node* current, T key, Node*& left, Node*& right) 
    {
        if (!current) 
        {
            left = right = nullptr;
            return;
        }
        if (current->value < key) 
        {
            split(current->right, key, current->right, right);
            left = current;
        } 
        else 
        {
            split(current->left, key, left, current->left);
            right = current;
        }
        updateCount(current);
    }

    Node* merge(Node* left, Node* right) 
    {
        if (!left || !right) 
        {
            return left ? left : right;
        }
        if (left->priority > right->priority) 
        {
            left->right = merge(left->right, right);
            updateCount(left);
            return left;
        } 
        else 
        {
            right->left = merge(left, right->left);
            updateCount(right);
            return right;
        }
    }

    // Вспомогательная функция для удаления узла
    Node* eraseHelper(Node* current, T key) 
    {
        if (!current) 
        {
            return nullptr;
        }
        if (current->value == key) 
        {
            Node* temp = merge(current->left, current->right);
            delete current;
            return temp;
        }
        if (key < current->value) 
        {
            current->left = eraseHelper(current->left, key);
        } 
        else 
        {
            current->right = eraseHelper(current->right, key);
        }
        updateCount(current);
        return current;
    }

public:
    Treap() : root(nullptr), rng(std::random_device{}()) {}

    ~Treap() 
    {
        while (root) 
        {
            erase(root->value);
        }
    }

    void insert(T value) 
    {
        std::uniform_int_distribution<int> dist(0, 1'000'000'000); // Диапазон приоритетов
        Node* less;
        Node* greater;
        split(root, value, less, greater);
        Node* newNode = new Node(value, dist(rng)); 
        root = merge(merge(less, newNode), greater);
    }

    void erase(T value) 
    {
        root = eraseHelper(root, value);
    }

    int size() 
    {
        return getCount(root);
    }
};


int main() 
{
    Treap<int> treap;
    treap.insert(5);
    treap.insert(3);
    treap.insert(7);
    treap.insert(3); 
    std::cout << "Size: " << treap.size() << std::endl; // Вывод: 4
    treap.erase(3);
    std::cout << "Size after erase: " << treap.size() << std::endl; // Вывод: 2
    return 0;
}