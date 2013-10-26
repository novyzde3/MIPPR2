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


void rozdelitNaKolikaRadech(Permutace *perm, int n, int *C, int cpus, int &radu, int &cnt) {
    vector<Coin> isEnd;
    isEnd.resize(1);
    
    //Jak rozdelit praci na zacatku vypoctu
    cout << "-------------------Rozdeleni prace-------------------" << endl;

    Permutace* tmpP2;
    for (int i = 1; i <= perm->getMCoins(); i++) {
        tmpP2 = perm->copyAndCut(i);
        isEnd[0].setHodnota(STARTVAL);
        cout << "Cnt: ";
        while (isEnd[0].getHodnota() != ENDVAL) {
            isEnd = tmpP2->getNextPerm();
            cnt++;
            //cout << cnt << " ";
            if (isEnd.size() < 1) {
                cerr << "rozdelitNaKolikaRadech(): Error function gerNextPerm return a vector with size 0" << endl;
                exit(-3);
            }
        }
        
        delete tmpP2;
        cout << endl;
        if (cnt >= cpus) {
            cout << "Rozdelit praci na " << cnt << " segmentu pro " << cpus << " procesoru (" << i << ") radu. Konec!" << endl;
            radu = i;
            return;
        }
        cnt = 0;
    }
    //Pokud ma cela permutace mene prvku nez je procesoru
    cerr << "rozdelitNaKolikaRadech: Error, permutace ma mene prvku nez je procesoru." << endl;
    //exit (-4);
}

int rozdelPraci(Permutace *perm, int n, int *C, int cpus) {
    int radu = 0, segmentu = 0;
    rozdelitNaKolikaRadech(perm, n, C, cpus, radu, segmentu);
    cout << "Rozdelit praci na " << segmentu << " segmentu pro " << cpus << " procesoru (" << radu << ") radu. Konec!" << endl;
    
    return 0;
}

int main(int argc, char** argv) {
    int cpus = 43; //pocet procesoru
    
    int n = 5;
    int* C = new int[n];
    for (int i = 0; i < n; i++) {
        C[i] = i * 7 - i*i;
    }
    int m = 4;
    
    vector<Coin> isEnd;
    isEnd.resize(1);
    isEnd[0].setHodnota(STARTVAL);
    
    Permutace* perm = new Permutace(n, m, C);
    Calculation* calcPrim = new Calculation(n, m, C);
    cout << "n = " << n << " m = " << m << endl;
    perm->printPayout();

    while (true) {
        isEnd = perm->getNextPerm();
        if(isEnd[0].getHodnota() == ENDVAL)
            break;

        calcPrim->trivEvaluateCurCoins(isEnd);
        //calcPrim->evaluCurCoinsPrecise(isEnd);

        if (isEnd.size() < 1) {
            cerr << "main: Error function gerNextPerm return a vector with size 0" << endl;
            exit(-3);
        }
    }
    //    p->printMaxCoins();
    calcPrim->printBestCoins();
    
    // rozdeleni prace na zacatku (dela procesor 1)
    Permutace* permDivide = new Permutace(n, m, C);
    permDivide->getNextPerm();
    rozdelPraci(permDivide, n, C, cpus);
    

    delete perm;
    delete permDivide;
    delete calcPrim;
    delete [] C;
    return 0;
}

