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
    const int n = 7;
    int* C = new int[n];
    for (int i=0 ; i<n ; i++) {
        C[i] = i+1;
    }
    C[6] = 9;
    const int m = 5;//s 4 funguje blbe
    
    
    Permutace* p = new Permutace(n, m, C);
    cout << "Maximalni hodnota mince: " << p->getMaxCoinVal() << endl;
    p->printPayout();
    
    bool isEnd = false;
    while (!isEnd) {
        p->getNextPerm();
        p->printCurCoins();
        isEnd = p->isSameVectors(p->getCurCoins(), p->getMaxCoins());
    }
    p->printMaxCoins();
    
    cout << endl << "Destrukce objektu." << endl;
    delete p;
    delete[] C;
    return 0;
}

