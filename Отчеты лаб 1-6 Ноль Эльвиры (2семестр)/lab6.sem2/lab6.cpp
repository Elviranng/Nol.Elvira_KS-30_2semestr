#include <iostream>
#include <string>
#include <ctime>
 
using namespace std;

void game(string player1, string player2, double &win1, double &win2){
    win1 = win2 = 0;
    int N = 10000;
    string res;
    for(int i = 0; i < N; i++){
        res = "";
        res += to_string(rand() % 2) + to_string(rand() % 2);
        while((player1 != res) && (player2 != res)){
            res += to_string(rand() % 2);
            res = res.substr(res.length() - 3, 3);
        }
        if(player1 == res){
            win1++;
        }else if(player2 == res){
            win2++;
        }        
    }
    win1 = win1 / N;
    win2 = win2 / N;
}

int main(){
    srand(time(0));
    string player1, player2;
    int count = 0;
    double sum_chance1 = 0, sum_chance2 = 0;
    double chance1, chance2;
    string res_options[8] = {"000", "001", "010", "011", "100", "101", "110", "111"};
    cout << "p2" << R"(\)" << "p1\t";
    for(int i = 0; i < 8; i++){
        cout << res_options[i] << "\t";
    }
    cout << endl;
    for(int i = 0; i < 8; i++){
        player2 = res_options[i];
        cout << player2 << "\t";
        for(int j = 0; j < 8; j++){
            player1 = res_options[j];            
            if(player1 == player2){
                cout << "---\t";
            }else{
                count++;
                game(player1, player2, chance1, chance2);
                sum_chance1 += chance1;
                sum_chance2 += chance2;
                cout << fixed;
                cout.precision(2);
                cout << chance1 << "\t";
            }
            if(j == 7){
                cout << endl;
            }
        }
    }
    cout << "Average probability of winning player1: " << sum_chance1 / count << endl;
    cout << "Average probability of winning player2: " << sum_chance2 / count << endl;
    return 0;
}