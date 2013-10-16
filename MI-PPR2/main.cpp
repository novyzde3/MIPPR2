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
    int n = 5;
    //cin >> n;
    int* C = new int[n];
    for (int i = 0; i < 6; i++) {
        C[i] = i * 7 - i*i;
    }

    int m = 4;
    /*for(int i = 0; i < n; i++) {
        cin >> C[i];
    }*/
    //cin >> m;

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

    //Jak rozdelit praci na zacatku vypoctu
    cout << "-------------------------------------------------------" << endl;
    int cpus = 50; //pocet procesoru
    int cnt = 0;
    
    /*tmpP2->getNextPerm(); //musi byt aby se inicializovala Permutace
    tmpP2->printMinCoins();
    tmpP2->printMaxCoins();*/

    Permutace* tmpP2;
    for (int i = 1; i <= p->getMCoins(); i++) {
        tmpP2 = new Permutace(n, i, C);
        isEnd[0].setHodnota(999);
        cout << "Cnt: ";
        while (isEnd[0].getHodnota() != ENDVAL) {
            isEnd = tmpP2->getNextPerm();
            cnt++;
            cout << cnt << " ";
            if (isEnd.size() < 1) {
                cerr << "main: Error function gerNextPerm return a vector with size 0" << endl;
                exit(-3);
            }
        }
        cout << endl;
        if (cnt >= cpus) {
            cout << "Rozdelit praci na " << cnt << " segmentu pro " << cpus << " procesoru (" << i << ") radu. Konec!" << endl;
            break;
        }
        cnt = 0;
        delete tmpP2;
    }

    delete p;
    delete calcPrim;
    delete[] C;
    return 0;
}

