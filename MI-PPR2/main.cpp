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


void rozdelitNaKolikaRadech(Permutace *p, int n, int *C, int cpus, int &radu, int &cnt) {
    vector<Coin> isEnd;
    isEnd.resize(1);
    
    //Jak rozdelit praci na zacatku vypoctu
    cout << "-------------------Rozdeleni prace-------------------" << endl;
    
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
    exit (-4);
}

int rozdelPraci(Permutace *p, int n, int *C, int cpus) {
    int radu, segmentu;
    rozdelitNaKolikaRadech(p, n, C, cpus, radu, segmentu);
    cout << "Rozdelit praci na " << segmentu << " segmentu pro " << cpus << " procesoru (" << radu << ") radu. Konec!" << endl;
    
    return 0;
}

int main(int argc, char** argv) {
    int cpus = 43; //pocet procesoru
    
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
    
    // rozdeleni prace na zacatku (dela procesor 1)
    rozdelPraci(p, n, C, cpus);
    

    delete p;
    delete calcPrim;
    delete[] C;
    return 0;
}

