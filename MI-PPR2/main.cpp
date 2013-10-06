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
    const int n = 7;
    int* C = new int[n];
    for (int i=0 ; i<n ; i++) {
        C[i] = i+1;
    }
    C[6] = 11;
    const int m = 4;
    
    vector<Coin> isEnd;
    isEnd.resize(1);
    
    Permutace* p = new Permutace(n, m, C);
    Calculation* calcPrim = new Calculation(n, m, C);
    Calculation* calcPrec = new Calculation(n, m, C);
    cout << "Maximalni hodnota mince: " << p->getMaxCoinVal() << endl;
    p->printPayout();
    
    while (isEnd[0].getHodnota() != ENDVAL) {
        isEnd = p->getNextPerm();
        calcPrim->trivEvaluateCurCoins(isEnd);
        calcPrec->evaluCurCoinsPrecise(isEnd);
        
        if (isEnd[0].getHodnota() != ENDVAL) //aby se posledni hodnota nevypisovala dvakrat (ale nevraci ji to 2x)
            p->printCurCoins();
        
        if (isEnd.size() < 1) {
            cerr << "main: Error function gerNextPerm return a vector with size 0" << endl;
            exit(-3);
        }
    }
    p->printMaxCoins();
    calcPrim->printBestCoins();
    calcPrec->printBestCoins();
    
    cout << endl << "Destrukce objektu." << endl;
    delete p;
    delete calcPrim;
    delete calcPrec;
    delete[] C;
    return 0;
}

