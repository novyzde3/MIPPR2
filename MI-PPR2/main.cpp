/* 
 * File:   main.cpp
 * Author: New
 *
 * Created on 24. září 2013, 11:37
 */

#include <cstdlib>
#include <iostream>
#include <vector>
#include "Permutace.h"
#include "Calculation.h"
#include "constants.h"

using namespace std;


int main(int argc, char** argv) {  
    int n, m;
    
    cin >> n;
    int* C = new int[n];
    for(int i = 0; i < n; i++)
        cin >> C[i];
    cin >> m;
    
    vector<Coin> isEnd;
    isEnd.resize(1);
    
    Permutace* p = new Permutace(n, m, C);
    Calculation* calcPrim = new Calculation(n, m, C);
    //cout << "Maximalni hodnota mince: " << p->getMaxCoinVal() << endl;
    cout << "n = " << n << " m = " << m << endl;
    p->printPayout();
    
    while (isEnd[0].getHodnota() != ENDVAL) {
        isEnd = p->getNextPerm();
        //calcPrim->trivEvaluateCurCoins(isEnd);
        calcPrim->evaluCurCoinsPrecise(isEnd);
        
        if (isEnd.size() < 1) {
            cerr << "main: Error function gerNextPerm return a vector with size 0" << endl;
            exit(-3);
        }
    }
//    p->printMaxCoins();
    calcPrim->printBestCoins();
    
    delete p;
    delete calcPrim;
    delete[] C;
    return 0;
}

