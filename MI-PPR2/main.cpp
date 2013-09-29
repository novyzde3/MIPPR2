/* 
 * File:   main.cpp
 * Author: New
 *
 * Created on 24. září 2013, 11:37
 */

#include <cstdlib>
#include <iostream>

#include "Permutace.h"

using namespace std;


int main(int argc, char** argv) {
    const int n = 6;
    int* C = new int[n];
    for (int i=0 ; i<6 ; i++) {
        C[i] = 50-5*i;
    }
    const int m = 6;
    
    
    Permutace* p = new Permutace(n, m, C);
    //cout << "Maximalni hodnota mince: " << p->getMaxCoinVal() << endl;
    p->printPayout();
    while (p->nextPerm()) {
        //p->trivEvaluateCurCoins();
        p->evaluCurCoinsPrecise();
    }
    p->printBestCoins();
    
    delete p;
    
    
    p = new Permutace(n, m, C);
    //cout << "Maximalni hodnota mince: " << p->getMaxCoinVal() << endl;
    //p->printPayout();
    while (p->nextPerm()) {
        //p->trivEvaluateCurCoins();
        p->trivEvaluateCurCoins();
    }
    p->printBestCoins();
      
    
    //cout << endl << "Destrukce objektu." << endl;
    delete p;
    delete[] C;
    return 0;
}

