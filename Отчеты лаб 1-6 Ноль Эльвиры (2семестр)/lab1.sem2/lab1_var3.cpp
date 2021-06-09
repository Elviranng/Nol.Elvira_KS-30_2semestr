#include <iostream>
#include "algorithm"
#include "time.h"
#include "stdlib.h"
#include <vector>
#include <chrono>

using namespace std;
const int n = 8;
void func(double list[], vector<double>& test,int i) {
	double a, b; a = -1; b = 1;
	for (int j = 0; j != list[i]; j++) {
		test.push_back((double)rand() / (double)RAND_MAX * (b - a) + a);
	}
}
int main()
{
	int crutch=0;
	double list[n] = { 1000,2000,4000,8000,16000,32000,64000,128000 };
	vector<double> test;
	int i = 0;
	for (i = 0; i != n-1; i++) {
		for (int z = 0; z != 20; z++) {
			srand(time(NULL) + z);
			func(list, test, i);
			chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();
			if (test[1] < test[0]) { swap(test[1], test[0]); }
			for (int j = 0; j != list[i]-1; j++) {
				if (test[j + 1] > test[j]) { continue; }
				j++; crutch = j;
				while (j!=0&&test[j]<test[j-1]) {
					swap(test[j], test[j - 1]);
					j--;
				}
				j= crutch; j--;

			}
			chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();
			chrono::duration<double, nano> nano_diff = end - start;
			chrono::duration<double, micro> micro_diff = end - start;
			chrono::duration<double, milli> milli_diff = end - start;
			chrono::duration<double> sec_diff = end - start;
			cout << endl;
			cout << "Time: " << nano_diff.count() << " nano sec." << endl;
			cout << "Time: " << micro_diff.count() << " micro sec." << endl;
			cout << "Time: " << milli_diff.count() << " milli sec." << endl;
			cout << "Time: " << sec_diff.count() << " sec." << endl;
//			for (int q = 0; q != list[i]; q++) { cout << test[q] << " "; } //вывод результата сортировки
			test.clear(); cout << endl;
		}
	}

}
