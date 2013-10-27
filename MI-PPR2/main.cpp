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
#include "../../../../../../../Program Files (x86)/MPICH2/include/mpi.h"

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
        //cout << "Cnt: ";
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
            radu = i;
            return;
        }
        cnt = 0;
    }
    //Pokud ma cela permutace mene prvku nez je procesoru
    cerr << "rozdelitNaKolikaRadech: Error, permutace ma mene prvku nez je procesoru." << endl;
    //exit (-4);
}

int rozdelPraci(Permutace *perm, int n, int *C, int m, int cpus) {
    int radu = 0, segmentu = 0;
    int message[m*2];
    rozdelitNaKolikaRadech(perm, n, C, cpus, radu, segmentu);
    //cout << "Rozdelit praci na " << segmentu << " segmentu pro " << cpus << " procesoru (" << radu << ") radu. Konec!" << endl;
    
    Permutace* tmpPerm = perm->copyAndCut(radu);
    vector<Coin> startCoins, endCoins;
    int coinId;
    for(int i = 1; i < cpus; i++){
        coinId = 0;
        startCoins = tmpPerm->getCurCoins();
        for(vector<Coin>::iterator it = startCoins.begin(); it != startCoins.end(); it++)
            message[coinId++] = it->getHodnota();
        while(coinId < m){
            message[coinId++] = -1;
        }
        endCoins = tmpPerm->getNextPerm();
        for(vector<Coin>::iterator it = endCoins.begin(); it != endCoins.end(); it++)
            message[coinId++] = it->getHodnota();
        while(coinId < 2*m){
            message[coinId++] = -1;
        }
        MPI_Send (message, m*2, MPI_INT, i, FLAG_NEW_DATA, MPI_COMM_WORLD);
    }
    perm->setCurCoins(tmpPerm->getCurCoins(), radu);
    return 0;
}

void zpracujPrvniData(Permutace* perm, int message[], int messageLength){
    vector<Coin> tmpVector(messageLength/2);
    int length = 0;
    if(DEBUG){
        cout << "---------------------PRISLA ZPRAVA------------------------" << endl;
        for(int i = 0; i < messageLength; i++)
            cout << message[i] << " ";
        cout << endl;
        cout << "----------------------------------------------------------" << endl;
    }
    for(int i = 0; i < messageLength/2; i++){
        tmpVector[i] = Coin(message[i]);
        if(message[i] != -1)
            length++;
    }
    perm->setCurCoins(tmpVector, length);
    length = 0;
    for(int i = 0; i < messageLength/2; i++){
        tmpVector[i] = Coin(message[(messageLength/2) + i]);
        if(message[(messageLength/2) + i] != -1)
            length++;
    }
    perm->setMaxCoinsExceptLast(tmpVector, length);
}

void zpracujVysledek(int message[], int messageLength, int& bestCoinCount, vector<Coin>& bestCoins){
    int sum = 0;
 
    for(int i = 1; i < messageLength; i += 2){
        sum += message[i];
    }
    if(sum < bestCoinCount){
        bestCoinCount = sum;
        for(int j = 0; j < messageLength; j += 2){
            bestCoins[j/2].setHodnota(message[j]);
            bestCoins[j/2].setPocet(message[j+1]);
        }
    }
}

int main(int argc, char** argv) {
    int n = 5;
    int* C = new int[n];
    for (int i = 0; i < n; i++) {
        C[i] = i * 7 - i*i;
    }
    int m = 4;
    
    //atributy slouzici ke komunikaci mezi procesy, pripadne k zajisteni prace
    int my_rank;
    int cpuCount;
    int message[m*2];
    int messageLength = m*2;
    bool bHasWork = false;
    bool bEverWorked = false;
    MPI_Status status;
    
    int bestCoinCount = INT_MAX;
    vector<Coin> bestCoins(m);
    
    MPI_Init( &argc, &argv );
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &cpuCount);
    
    vector<Coin> isEnd;
    isEnd.resize(1);
    isEnd[0].setHodnota(STARTVAL);
    
    //uvodni rozdeleni prace
    Permutace* perm = new Permutace(n, m, C);
    perm->getNextPerm();
    if(my_rank == 0){
        rozdelPraci(perm, n, C, m, cpuCount);
    }else{
        MPI_Recv(&message, messageLength, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        zpracujPrvniData(perm, message, messageLength);
    }
    if(DEBUG)
    {
        cout << "---------------------PROCES " << my_rank << "------------------------" << endl;
        perm->printCurCoins();
        perm->printMaxCoins();
        cout << "---------------------PROCES " << my_rank << "------------------------" << endl;
    }
    Calculation* calcPrim = new Calculation(n, m, C);
    //cout << "n = " << n << " m = " << m << endl;
    perm->printPayout();
    isEnd = perm->getCurCoins();
    while (true) {
        if (isEnd.size() < 1) {
            cerr << "main: Error function gerNextPerm return a vector with size 0" << endl;
            exit(-3);
        }
        if(isEnd[0].getHodnota() == ENDVAL)
            break;
        if(DEBUG)
            Permutace::printCoins(isEnd);
        calcPrim->trivEvaluateCurCoins(isEnd);
        //calcPrim->evaluCurCoinsPrecise(isEnd);
        isEnd = perm->getNextPerm();
    }
    
    if(my_rank == 0){
        bestCoins = calcPrim->getBestCoins();
        int counter = 0;
        while(counter < cpuCount-1){
            MPI_Recv(&message, messageLength, MPI_INT, MPI_ANY_SOURCE, FLAG_RESULTS, MPI_COMM_WORLD, &status);
            counter++;
            zpracujVysledek(message, messageLength, bestCoinCount, bestCoins);
            if(DEBUG)
                cout << "---------------------PROCES DOSTAL VYSLEDEK------------------------" << endl;
        }
        Permutace::printCoins(bestCoins);
    }else{
        vector<Coin> tmp = calcPrim->getBestCoins();
        int index = 0;
        for(vector<Coin>::iterator it = tmp.begin(); it != tmp.end(); it++){
            message[index++] = it->getHodnota();
            message[index++] = it->getPocet();
        }
        MPI_Send(message, messageLength, MPI_INT, 0, FLAG_RESULTS, MPI_COMM_WORLD);
        if(DEBUG)
            cout << "---------------------PROCES " << my_rank << " POSLAL VYSLEDEK------------------------" << endl;
    }
    //calcPrim->printBestCoins();
    
    
    
    MPI_Finalize();
    delete perm;
    delete calcPrim;
    delete [] C;
    return 0;
}

