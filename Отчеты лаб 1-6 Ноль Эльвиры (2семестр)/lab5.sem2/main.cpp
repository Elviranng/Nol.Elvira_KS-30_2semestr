#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Структура для хранения узлов
struct GraphNode
{
    int val, cost;
    GraphNode* next;
};
 
// Структура для хранения ребра графа (взвешенного)
struct EdgeW {
    int start, finish, cost;
};

// Структура для хранения ребра графа (не взвешенного)
struct EdgeNW {
    int start, finish;
};

class MainGraph
{
    // Метод для выделения нового узла списка смежности (взвешенного)
    GraphNode* adjListNewNode(int value, int cost, GraphNode* head)
    {
        GraphNode* newNode = new GraphNode;
        newNode->val = value;
        newNode->cost = cost;

        // Новый узел должен указывать на текущее направление
        newNode->next = head;
 
        return newNode;
    }
    
    // Метод для выделения нового узла списка смежности (не взвешенного)
    GraphNode* adjListNewNode(int value, GraphNode* head)
    {
        GraphNode* newNode = new GraphNode;
        newNode->val = value;
        newNode->cost = -1;

        // Новый узел должен указывать на текущее направление
        newNode->next = head;
 
        return newNode;
    }
    
    int N;  // Общее кол-во узлов в графе
    int N_EDGES = 0;  // Общее кол-во ребер
 
public:
    
    bool is_directed;  // Направленный ли граф
    bool is_wieghted;  // Взвешенный ли граф
    GraphNode **head;  // Массив указателей на узлы, для представления списка смежности
    
    EdgeNW* edges_not_weighted = nullptr;  // Указатель на массив не взвешенных ребер
    EdgeW* edges_weighted = nullptr;  // Указатель на массив взвешенных ребер
    
    // Конструктор (для взвешенного)
    MainGraph(vector<EdgeW> edges, int n, int N, bool is_directed)
    {   
        edges_weighted = new EdgeW[n];
        for(int i = 0; i < n; i++){
            edges_weighted[i] = edges[i];
        }
        // Выставляем то, что граф взвешен
        this->is_wieghted = true;

        // Выставляем тип графа
        this->is_directed = is_directed;

        // Выделяем память
        head = new GraphNode*[N]();
        this->N = N;
        
        // Инициализируем указатели на направления для всех вершин
        for (int i = 0; i < N; i++) {
            head[i] = nullptr;
        }
 
        // Добавляем ребра направленному графу
        for (unsigned i = 0; i < n; i++)
        {
            int start = edges[i].start;
            int finish = edges[i].finish;
            int cost = edges[i].cost;
 
            // Вставка в начало
            GraphNode* newNode = adjListNewNode(finish, cost, head[start]);

            // Указатели направления должны указывать на новый узел
            head[start] = newNode;
            this->N_EDGES++;

            // Если граф не направленный, то каждому направлению создаем ещё и обратное
            if(!this->is_directed){

                newNode = adjListNewNode(start, cost, head[finish]);

                // Меняем направление, чтобы оно вело на новый узел
                head[finish] = newNode;
            }
        }
        if(!this->is_directed){
            duplicateEdgs();
        }
    }
    
    // Конструктор (для не взвешенного)
    MainGraph(vector<EdgeNW> edges, int n, int N, bool is_directed)
    {
        edges_not_weighted = new EdgeNW[n];
        for(int i = 0; i < n; i++){
            edges_not_weighted[i] = edges[i];
        }

        // Выставляем то, что граф не взвешен
        this->is_wieghted = false;

        // Выставляем тип графа
        this->is_directed = is_directed;

        // Выделяем память
        head = new GraphNode*[N]();
        this->N = N;

        // Инициализируем указатели на направления для всех вершин
        for (int i = 0; i < N; i++) {
            head[i] = nullptr;
        }

        
        // Добавляем ребра направленному графу
        for (unsigned i = 0; i < n; i++)
        {
            int start = edges[i].start;
            int finish = edges[i].finish;
 
            // Вставка в начало
            GraphNode* newNode = adjListNewNode(finish, head[start]);

            // Указатели направления должны указывать на новый узел
            head[start] = newNode;

            this->N_EDGES++;

            // Если граф не направленный, то каждому направлению создаем ещё и обратное
            if(!this->is_directed){

                newNode = adjListNewNode(start, head[finish]);

                // Меняем направление, чтобы оно вело на новый узел
                head[finish] = newNode;
            }
        }
        if(!this->is_directed){
            duplicateEdgs();
        }
    }

    // Деструктор
    ~MainGraph() {
        for (int i = 0; i < N; i++) {
            delete[] head[i];
        }
        delete[] edges_weighted;
        delete[] edges_not_weighted;
        delete[] head;
    }

    // Метод для создания обратных направлений неориентированному графу
    void duplicateEdgs(){
        if(this->is_wieghted){
            vector<EdgeW> tmp_edges(this->N_EDGES);
            for(int i = 0; i < this->N_EDGES; i++){
                tmp_edges[i] = this->edges_weighted[i];
            }

            // Ищем
            for(int i = 0; i < this->N_EDGES; i++){
                EdgeW tmp_el = tmp_edges[i];
                for( auto iter = tmp_edges.begin(); iter != tmp_edges.end() - 1; iter++ ){
                    if(( iter->start == tmp_el.finish) && (iter->finish == tmp_el.start)){
                        tmp_edges.erase(iter);
                        break;
                    }
                        
                }
                cout << i;
            }
            
            int tmp_size = tmp_edges.size();
            for(int i = 0; i < tmp_size; i++){
                tmp_edges.push_back({tmp_edges[i].finish, tmp_edges[i].start, tmp_edges[i].cost});
            }

            delete[] edges_weighted;
            edges_weighted = new EdgeW[tmp_edges.size()];
            for(int i = 0; i < tmp_edges.size(); i++){
                edges_weighted[i] = tmp_edges[i];
            }
            this->N_EDGES = tmp_edges.size();

        }else{
            vector<EdgeNW> tmp_edges(this->N_EDGES);
            for(int i = 0; i < this->N_EDGES; i++){
                tmp_edges[i] = this->edges_not_weighted[i];
            }

            // Ищем
            for(int i = 0; i < this->N_EDGES; i++){
                EdgeNW tmp_el = tmp_edges[i];
                for( auto iter = tmp_edges.begin(); iter != tmp_edges.end() - 1; iter++ ){
                    if(( iter->start == tmp_el.finish) && (iter->finish == tmp_el.start)){
                        tmp_edges.erase(iter);
                        break;
                    }
                        
                }
            }

            int tmp_size = tmp_edges.size();
            for(int i = 0; i < tmp_size; i++){
                tmp_edges.push_back({tmp_edges[i].finish, tmp_edges[i].start});
            }

            delete[] edges_not_weighted;
            edges_not_weighted = new EdgeNW[tmp_edges.size()];
            for(int i = 0; i < tmp_edges.size(); i++){
                edges_not_weighted[i] = tmp_edges[i];
            }
            this->N_EDGES = tmp_edges.size();
        }
    }

    // Метод выдачи матрицы смежности
    vector<vector<int>> adjMat(){

        // Создаем двумерный вектор размера NxN (кол-во узлов)
        vector<vector<int>> main_matrix (this->N, vector<int>(this->N, 0));

        // Заполняем матрицу смежности
        if(this->is_wieghted){
            for(int i = 0; i < N; i++){
                GraphNode* current_node = head[i];
                while (current_node != nullptr){
                    main_matrix[i][current_node->val] = current_node->cost;
                    current_node = current_node->next;
                }
            }
        }else{
            for(int i = 0; i < N; i++){
                GraphNode* current_node = head[i];
                while (current_node != nullptr){
                    main_matrix[i][current_node->val] = 1;
                    current_node = current_node->next;
                }
            }
        }

        return main_matrix;
    }

    vector<vector<int>> incMat(){
        // Создаем двумерный вектор размера NxM (кол-во узлов x кол-во ребер)
        vector<vector<int>> main_matrix (this->N, vector<int>(this->N_EDGES, 0));

        if(this->is_wieghted){
            for(int i = 0; i < this->N; i++){
                for(int j = 0; j < this->N_EDGES; j++){
                    if(i == edges_weighted[j].start){
                        main_matrix[i][j] = 1;
                    }
                    if(i == edges_weighted[j].finish){
                        main_matrix[i][j] = -1;
                    }
                }
            }
        }else{
            for(int i = 0; i < this->N; i++){
                for(int j = 0; j < this->N_EDGES; j++){
                    if(i == edges_not_weighted[j].start){
                        main_matrix[i][j] = 1;
                    }
                    if(i == edges_not_weighted[j].finish){
                        main_matrix[i][j] = -1;
                    }
                }
            }
        }
        

        return main_matrix;
    }

    // Метод выдачи списка смежности
    vector<vector<int>> adjList(){
        vector<vector<int>> main_matrix (this->N, vector<int>(0));

        if(is_wieghted){
            for(int i = 0; i < N; i++){
                for(int j = 0; j < N_EDGES; j++){
                    if(edges_weighted[j].start == i){
                        main_matrix[i].push_back(edges_weighted[j].finish);
                    }else if(edges_weighted[j].finish == i){
                        main_matrix[i].push_back(edges_weighted[j].start);
                    }
                }
                sort(main_matrix[i].begin(), main_matrix[i].end());
                main_matrix[i].erase(unique(main_matrix[i].begin(), main_matrix[i].end()), main_matrix[i].end());
            }
        }else{
            for(int i = 0; i < N; i++){
                for(int j = 0; j < N_EDGES; j++){
                    if(edges_not_weighted[j].start == i){
                        main_matrix[i].push_back(edges_not_weighted[j].finish);
                    }else if(edges_not_weighted[j].finish == i){
                        main_matrix[i].push_back(edges_not_weighted[j].start);
                    }
                }
                sort(main_matrix[i].begin(), main_matrix[i].end());
                main_matrix[i].erase(unique(main_matrix[i].begin(), main_matrix[i].end()), main_matrix[i].end());
            }
        }
        
        return main_matrix;
    }

    // Метод выдачи списка ребер
    vector<vector<int>> edgsList(){

        int edges_amount = N_EDGES;
        if(!is_directed){
            edges_amount = N_EDGES / 2;
        }
        vector<vector<int>> main_matrix (edges_amount, vector<int>(2));

        if(is_wieghted){
            for(int i = 0; i < edges_amount; i++){
                main_matrix[i][0] = edges_weighted[i].start;
                main_matrix[i][1] = edges_weighted[i].finish;
            }
        }else{
            for(int i = 0; i < edges_amount; i++){
                main_matrix[i][0] = edges_not_weighted[i].start;
                main_matrix[i][1] = edges_not_weighted[i].finish;
            }
        }

        return main_matrix;
        
    }

    // Метод поиска длиннейшего пути
    vector<EdgeW> longestRootFromAToB(int start_node, int end_node){
        vector<vector<int>> adj_matrix = adjMat();
        int n = this->N;
        vector<EdgeW> result;
        int tmp_max_dist = 0;
        int next_node = start_node;
        int tmp_node;
        int beg_node;
        while(next_node != end_node){
            beg_node = next_node;
            for(int i = 0; i < n; i++){
                if(adj_matrix[next_node][i] > tmp_max_dist){
                    tmp_max_dist = adj_matrix[next_node][i];
                    tmp_node = i;
                }
            }
            next_node = tmp_node;
            result.push_back({beg_node, next_node, tmp_max_dist});
            tmp_max_dist = 0;
        }
        
        return result;
    }
};

class MyGraphGenerator{
    private:
        int max_vertexes = 10;
        int min_vertexes = 5;

        int max_edges = 9;
        int min_edges = 3;

        int max_edges_for_vertex = 3;

        bool is_directed = true;

        int max_incoming_edges = 5;
        int max_outcoming_edges = 5;

        bool is_weighted = true;

        int max_weight = 10;
        int min_weight = 1;

        bool is_legal_islands = false;

        MainGraph* result_graph = nullptr;

    public:
        MyGraphGenerator(){};

        void setMaxVertexes(int val){
            this->max_vertexes = val;
        }

        void setMinVertexes(int val){
            this->min_vertexes = val;
        }

        void setMaxEdges(int val){
            this->max_edges = val;
        }

        void setMinEdges(int val){
            this->min_edges = val;
        }

        void setMaxEdgesForVertex(int val){
            this->max_edges_for_vertex = val;
        }

        void setIsDirected(bool val){
            this->is_directed = val;
        }

        void setMaxIncomingEdges(int val){
            this->max_incoming_edges = val;
        }

        void setMaxOutcomingEdges(int val){
            this->max_outcoming_edges = val;
        }

        void setIsWeighted(bool val){
            this->is_weighted = val;
        }

        void setMaxWeight(int val){
            this->max_weight = val;
        }

        void SetMinWeight(int val){
            this->min_weight = val;
        }

        void setIsLegalIslands(bool val){
            this->is_legal_islands = val;
        }

        MainGraph* generateRandomGraph(){
            
            int vertexes_amount = this->min_vertexes + rand() % (this->max_vertexes - this->min_vertexes + 1);
            int edges_amount = this->min_edges + rand() % (this->max_edges - this->min_edges + 1);

            int* vertex_edges = new int[vertexes_amount];
            int* vertex_outcoming_edges = new int[vertexes_amount];
            int* vertex_incoming_edges = new int[vertexes_amount];
            bool* vertex_empty = new bool[vertexes_amount];

            for(int i = 0; i < vertexes_amount; i++){
                vertex_edges[i] = 0;
                vertex_outcoming_edges[i] = 0;
                vertex_incoming_edges[i] = 0;
                vertex_empty[i] = 1;
            }

            vector<EdgeW> edges_weighted(edges_amount);
            vector<EdgeNW> edges_not_weighted(edges_amount);

            if(this->is_weighted){
                for(int i = 0; i < edges_amount; i++){
                    edges_weighted[i] = {rand() % (edges_amount + 1), rand() % (edges_amount + 1), this->min_weight + rand() % (this->max_weight - this->min_weight + 1)};

                    while((edges_weighted[i].finish == edges_weighted[i].start) || (vertex_edges[edges_weighted[i].finish] > this->max_edges_for_vertex) || (vertex_edges[edges_weighted[i].start] > this->max_edges_for_vertex)){
                        edges_weighted[i].finish = rand() % (edges_amount + 1);
                        if(vertex_edges[edges_weighted[i].start] > this->max_edges_for_vertex){
                            edges_weighted[i].start = rand() % (edges_amount + 1);
                        }
                    }

                    while((vertex_outcoming_edges[edges_weighted[i].start] >= this->max_outcoming_edges) || (vertex_incoming_edges[edges_weighted[i].finish] >= this->max_incoming_edges)){
                        if(vertex_edges[edges_weighted[i].start] >= this->max_edges_for_vertex){
                            edges_weighted[i].start = rand() % (edges_amount + 1);
                        }else{
                            edges_weighted[i].finish = rand() % (edges_amount + 1);
                        }
                    }

                    vertex_empty[edges_weighted[i].start] = 0;
                    vertex_empty[edges_weighted[i].finish] = 0;
                    vertex_edges[edges_weighted[i].start]++;
                    vertex_edges[edges_weighted[i].finish]++;
                    vertex_incoming_edges[edges_weighted[i].finish]++;
                    vertex_outcoming_edges[edges_weighted[i].start]++;
                }

                if(!this->is_legal_islands){
                    int vertexes_empty_num = 0;
                    for(int i = 0; i < vertexes_amount; i++){
                        if(vertex_empty[i]){
                            vertexes_empty_num++;
                            if(i != 0){
                                for(int j = 0; j< edges_amount; j++){
                                    if(edges_weighted[j].finish == i){
                                        edges_weighted[j].finish--;
                                    }else if(edges_weighted[j].start == i){
                                        edges_weighted[j].start--;
                                    }
                                }
                            }else{
                                for(int j = 0; j< edges_amount; j++){
                                    if(edges_weighted[j].finish == i){
                                        edges_weighted[j].finish++;
                                    }else if(edges_weighted[j].start == i){
                                        edges_weighted[j].start++;
                                    }
                                }
                            }

                        }
                    }
                    vertexes_amount -= vertexes_empty_num;
                }

                delete[] vertex_edges;
                delete[] vertex_outcoming_edges;
                delete[] vertex_incoming_edges;

                this->result_graph = new MainGraph(edges_weighted, edges_amount, vertexes_amount, this->is_directed);
                
                return result_graph;

            }else{
                for(int i = 0; i < edges_amount; i++){
                    edges_not_weighted[i] = {rand() % (edges_amount + 1), rand() % (edges_amount + 1)};

                    while((edges_not_weighted[i].finish == edges_not_weighted[i].start) || (vertex_edges[edges_not_weighted[i].finish] > this->max_edges_for_vertex) || (vertex_edges[edges_not_weighted[i].start] > this->max_edges_for_vertex)){
                        edges_not_weighted[i].finish = rand() % (edges_amount + 1);
                        if(vertex_edges[edges_not_weighted[i].start] > this->max_edges_for_vertex){
                            edges_not_weighted[i].start = rand() % (edges_amount + 1);
                        }
                    }

                    while((vertex_outcoming_edges[edges_not_weighted[i].start] >= this->max_outcoming_edges) || (vertex_incoming_edges[edges_not_weighted[i].finish] >= this->max_incoming_edges)){
                        if(vertex_edges[edges_not_weighted[i].start] >= this->max_edges_for_vertex){
                            edges_not_weighted[i].start = rand() % (edges_amount + 1);
                        }else{
                            edges_not_weighted[i].finish = rand() % (edges_amount + 1);
                        }
                    }

                    vertex_empty[edges_not_weighted[i].start] = 0;
                    vertex_empty[edges_not_weighted[i].finish] = 0;
                    vertex_edges[edges_not_weighted[i].start]++;
                    vertex_edges[edges_not_weighted[i].finish]++;
                    vertex_incoming_edges[edges_not_weighted[i].finish]++;
                    vertex_outcoming_edges[edges_not_weighted[i].start]++;
                }

                if(!this->is_legal_islands){
                    int vertexes_empty_num = 0;
                    for(int i = 0; i < vertexes_amount; i++){
                        if(vertex_empty[i]){
                            vertexes_empty_num++;
                            if(i != 0){
                                for(int j = 0; j< edges_amount; j++){
                                    if(edges_not_weighted[j].finish == i){
                                        edges_not_weighted[j].finish--;
                                    }else if(edges_not_weighted[j].start == i){
                                        edges_not_weighted[j].start--;
                                    }
                                }
                            }else{
                                for(int j = 0; j< edges_amount; j++){
                                    if(edges_not_weighted[j].finish == i){
                                        edges_not_weighted[j].finish++;
                                    }else if(edges_not_weighted[j].start == i){
                                        edges_not_weighted[j].start++;
                                    }
                                }
                            }

                        }
                    }
                    vertexes_amount -= vertexes_empty_num;
                }

                delete[] vertex_edges;
                delete[] vertex_outcoming_edges;
                delete[] vertex_incoming_edges;

                this->result_graph = new MainGraph(edges_not_weighted, edges_amount, vertexes_amount, this->is_directed);
                return result_graph;
            }


            
        }

        ~MyGraphGenerator(){
            delete result_graph;


        }

};

int main()
{
    vector<EdgeW> edges = {
        // `(x, y, w)` —> ребро из `x` в `y` с весом `w`
        { 0, 1, 6 }, { 1, 2, 7 }, { 2, 0, 5 }, { 2, 1, 4 },
        { 3, 2, 10 }, { 4, 5, 1 }, { 5, 4, 3 }
    };

    // vector<EdgeNW> edges[] =
    // {
    //     // `(x, y, w)` —> ребро из `x` в `y` с весом `w`
    //     { 0, 1 }, { 1, 2 }, { 2, 0 }, { 2, 1 },
    //     { 3, 2 }, { 4, 5 }, { 5, 4 }
    // };

    // кол-во узлов
    int N = 6;
 
    // кол-во ребер
    int n = edges.size();
 
    // создаем граф
    MainGraph graph(edges, n, N, 1);

    cout << "Матрица смежности" << endl;
    vector<vector<int>> test = graph.adjMat();
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            cout << test[i][j] << "\t";
        }
        cout << endl;
    }
    
    cout << "Матрица инциндентности" << endl;
    vector<vector<int>> test_1 = graph.incMat();
    for(int i = 0; i < N; i++){
        for(int j = 0; j < test_1[i].size(); j++){
            cout << test_1[i][j] << "\t";
        }
        cout << endl;
    }

    cout << "Список смежности" << endl;
    vector<vector<int>> test_2 = graph.adjList();
    for(int i = 0; i < N; i++){
        cout << i << ": ";
        for(int j = 0; j < test_2[i].size(); j++){
            cout << test_2[i][j] << "\t";
        }
        cout << endl;
    }

    cout << "Список ребер" << endl;
    vector<vector<int>> test_3 = graph.edgsList();
    for(int i = 0; i < test_3.size(); i++){
        cout << i << ": ";
        for(int j = 0; j < test_3[i].size(); j++){
            cout << test_3[i][j] << "\t";
        }
        cout << endl;
    }

    cout << "Поиск длиннейшего маршрута из A в B" << endl;
    vector<EdgeW> test_4 = graph.longestRootFromAToB(3, 0);
    for(int i = 0; i < test_4.size(); i++){
        cout << test_4[i].start << " - " << test_4[i].finish << " : " << test_4[i].cost;
        cout << endl;
    }

    MyGraphGenerator new_graph;
    MainGraph* res = new_graph.generateRandomGraph();
    
    cout << "Матрица смежности" << endl;
    vector<vector<int>> res_test = res->adjMat();
    for(int i = 0; i < res_test.size(); i++){
        for(int j = 0; j < res_test[i].size(); j++){
            cout << res_test[i][j] << "\t";
        }
        cout << endl;
    }
    cout << "Матрица инциндентности" << endl;
    vector<vector<int>> res_test_1 = res->incMat();
    for(int i = 0; i < res_test_1.size(); i++){
        for(int j = 0; j < res_test_1[i].size(); j++){
            cout << res_test_1[i][j] << "\t";
        }
        cout << endl;
    }
    cout << "Список смежности" << endl;
    vector<vector<int>> res_test_2 = res->adjList();
    for(int i = 0; i < res_test_2.size(); i++){
        cout << i << ": ";
        for(int j = 0; j < res_test_2[i].size(); j++){
            cout << res_test_2[i][j] << "\t";
        }
        cout << endl;
    }
    cout << "Список ребер" << endl;
    vector<vector<int>> res_test_3 = res->edgsList();
    for(int i = 0; i < res_test_3.size(); i++){
        cout << i << ": ";
        for(int j = 0; j < res_test_3[i].size(); j++){
            cout << res_test_3[i][j] << "\t";
        }
        cout << endl;
    }
}