#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include <fstream>
#include <vector>

using namespace std;

template <typename T>
class AVLTree{
public:
    // Узел
    class node{
    public:
        T key;
        int height;
        node* left;
        node* right;

        node(T k){
            height = 1;
            key = k;
            left = nullptr;
            right = nullptr;
        }
    };

    // Конструктор по-умолчанию
    AVLTree(){}

    // Деструктор
    ~AVLTree(){
        // Удаляем корень, пока корень не станет пустым
        while(this->root){
            remove(this->root->key);
        }
    }

    // Метод подсчета элементов
    int count(){
        return amount;
    }

    // Метод добавления узла
    void insert(T x){
        root = insertPrivate(root, x);
    }

    // Метод удаления узла
    void remove(T x){
        root = removePrivate(root, x);
    }

    // Метод поиска в дереве
    node* find(T x){
        return findPrivate(root, x);
    }

    // Метод вывода дерева в консоль по порядку
    void inorder(){
        inorderPrivate(root);
        cout << endl;
    }

    // Метод получения узла-корня
    node* getRoot(){
        return root;
    }

private:
    node* root = nullptr;  // Главный узел - корень
    int amount = 0;  // Кол-во элементов дерева

    // Метод, возвращающий высоту узла
    int height(node* curr_node){
        if (curr_node == nullptr)
            return 0;
        return curr_node->height;
    }

    // Правый поворот
    node* rightRotation(node* curr_node){
        node* new_curr_node = curr_node->left;
        curr_node->left = new_curr_node->right;
        new_curr_node->right = curr_node;
        curr_node->height = 1 + max(height(curr_node->left), height(curr_node->right));
        new_curr_node->height = 1 + max(height(new_curr_node->left), height(new_curr_node->right));
        return new_curr_node;
    }

    // Левый поворот
    node* leftRotation(node* curr_node){
        node* new_curr_node = curr_node->right;
        curr_node->right = new_curr_node->left;
        new_curr_node->left = curr_node;
        curr_node->height = 1 + max(height(curr_node->left), height(curr_node->right));
        new_curr_node->height = 1 + max(height(new_curr_node->left), height(new_curr_node->right));
        return new_curr_node;
    }

    // Внутрений метод вывода в консоль элементов (рекурсивный)
    void inorderPrivate(node* curr_node){
        if (curr_node == nullptr)
            return;
        inorderPrivate(curr_node->left);
        cout << curr_node->key << " ";
        inorderPrivate(curr_node->right);
    }

    // Внутрений метод вставки (рекурсивный)
    node* insertPrivate(node* curr_node, T x){

        // Проверка на то, дошли ли до места для вставки узла:
        if (curr_node == nullptr){
            node* temp = new node(x);
            amount++;
            return temp;
        }
        if (x < curr_node->key)
            curr_node->left = insertPrivate(curr_node->left, x);
        else if (x > curr_node->key)
            curr_node->right = insertPrivate(curr_node->right, x);

        // После вставки идет балансировка:
        curr_node->height = 1 + max(height(curr_node->left), height(curr_node->right));
        int bal = height(curr_node->left) - height(curr_node->right);
        
        if (bal > 1){
            if (x < curr_node->left->key){
                return rightRotation(curr_node);
            }
            else{
                curr_node->left = leftRotation(curr_node->left);
                return rightRotation(curr_node);
            }
        }
        else if (bal < -1){
            if (x > curr_node->right->key){
                return leftRotation(curr_node);
            }
            else{
                curr_node->right = rightRotation(curr_node->right);
                return leftRotation(curr_node);
            }
        }
        return curr_node;
    }

    // Внутрений метод удаления узла (рекурсивный)
    node* removePrivate(node* curr_node, T x){

        // Проверяем не уткнулись ли в пустой узел
        if (curr_node == nullptr)
            return nullptr;

        if (x < curr_node->key){
            curr_node->left = removePrivate(curr_node->left, x);
        }
        else if (x > curr_node->key){
            curr_node->right = removePrivate(curr_node->right, x);
        }
        else{

            // Если нашли нужный узел, то удаляем значения из него:
            node* r = curr_node->right;

            if (curr_node->right == nullptr){
                node* l = curr_node->left;
                delete (curr_node);
                curr_node = l;
                amount--;
            }
            else if (curr_node->left == nullptr){
                delete (curr_node);
                curr_node = r;
                amount--;
            }
            else{
                while (r->left != nullptr)
                    r = r->left;
                curr_node->key = r->key;
                curr_node->right = removePrivate(curr_node->right, r->key);
            }
        }

        if (curr_node == nullptr){
            return curr_node;
        }

        // Балансировка после удаления:
        curr_node->height = 1 + max(height(curr_node->left), height(curr_node->right));
        int bal = height(curr_node->left) - height(curr_node->right);
        
        if (bal > 1){
            if (height(curr_node->left) >= height(curr_node->right)){
                return rightRotation(curr_node);
            }
            else{
                curr_node->left = leftRotation(curr_node->left);
                return rightRotation(curr_node);
            }
        }
        else if (bal < -1){
            if (height(curr_node->right) >= height(curr_node->left)){
                return leftRotation(curr_node);
            }
            else{
                curr_node->right = rightRotation(curr_node->right);
                return leftRotation(curr_node);
            }
        }

        return curr_node;
    }

    // Внутрений метод поиска узла (рекурсивный)
    node* findPrivate(node* curr_node, T x){
        if (curr_node == nullptr)
            return nullptr;
            
        T k = curr_node->key;
        if (k == x)
            return curr_node;
        if (k > x)
            return findPrivate(curr_node->left, x);
        if (k < x)
            return findPrivate(curr_node->right, x);
    }
};

int main(){
    const int TEST_AMOUNT = 10;
    const int RETRIES = 20;
    const int SORTED_TRIES = 5;
    const int START_N = 50000;
    const int STEP_N = 50000;

    mt19937 engine(time(0));

    ofstream res_file;
    res_file.open("res.txt", ios::out);

    int N = START_N;

    for(int test_num = 0; test_num < TEST_AMOUNT; test_num++){

        res_file << "----------------------" << endl;
        res_file << "Test " << test_num + 1 << " (N = " << N << ")" << ":" << endl << endl;

        for(int try_num = 0; try_num < RETRIES; try_num++){

            res_file << "Try " << try_num + 1;

            vector<double> arr(N);
            AVLTree<double> tree;

            // Заполняем вектор/массив:
            uniform_real_distribution<double> gen(-1.0, 1.0);
            for(auto& el: arr)
                el = gen(engine);

            // Если это первые 5 прогонов, то сортируем массив:
            if(try_num < SORTED_TRIES){
                sort(arr.begin(), arr.end());
                res_file << " (sorted):" << endl;
            }else{
                res_file << ":" << endl;
            }

            res_file << "Search: " << endl;

            // Заполняем дерево:
            for(auto el: arr)
                tree.insert(el);
            
            // Засекаем время:
            chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();

            // Поиск:
            for(int i = 0; i < 100; i++){
                tree.find(gen(engine));
            }
            
            // Вычисляем время:
            chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> milli_diff = end - start;

            // Выводим информацию в файл:
            res_file << milli_diff.count() << endl;

            res_file << "Insert: " << endl;

            // Засекаем время:
            start = chrono::high_resolution_clock::now();

            // Вставка:
            for(int i = 0; i < 100; i++){
                tree.insert(gen(engine));
            }
            
            // Вычисляем время:
            end = chrono::high_resolution_clock::now();
            milli_diff = end - start;

            // Выводим информацию в файл:
            res_file << milli_diff.count() << endl;

            arr.clear();
            tree.~AVLTree();
        }

        N += STEP_N;
        res_file << "----------------------" << endl << endl;
    }

    res_file.close();
}
