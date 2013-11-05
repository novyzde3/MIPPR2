/* 
 * File:   main.cpp
 * Author: New
 *
 * Created on 24. září 2013, 11:37
 */

#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <vector>
#include "Permutace.h"
#include "Calculation.h"
#include "constants.h"
//#include <mpi.h>
#include <fstream>
#include <time.h>
#include "../../../../../../../Program Files (x86)/MPICH2/include/mpi.h"

using namespace std;

void printTime(time_t begin, time_t end){
    int seconds = difftime(end, begin);
    int minutes = seconds / 60;
    cout << "Spent time: " << minutes << " m :" << seconds % 60 << " s" << endl;
}

bool rozdelitNaKolikaRadech(Permutace *perm, int cpus, int &radu, int &cnt) {
    vector<Coin> isEnd;
    isEnd.resize(1);
    
    //Jak rozdelit praci

    Permutace* tmpP2;
    for (int i = 1; i <= perm->getMCoins() - 1; i++) {
        tmpP2 = perm->copyAndCut(i);
        isEnd[0].setHodnota(STARTVAL);
        while (isEnd[0].getHodnota() != ENDVAL) {
            isEnd = tmpP2->getNextPerm();
            cnt++;
            if (isEnd.size() < 1) {
                cerr << "rozdelitNaKolikaRadech(): Error function gerNextPerm return a vector with size 0" << endl;
                exit(-3);
            }
        }
        
        delete tmpP2;
        if (cnt >= cpus) {
            radu = i;
            return true;
        }
        cnt = 0;
    }
    if(cpus > 2){
        cerr << "rozdelitNaKolikaRadech: Error, permutace ma mene prvku nez je procesoru." << endl;
        exit (-4);
    }
    return false;
    //Pokud ma cela permutace mene prvku nez je procesoru
    //cerr << "rozdelitNaKolikaRadech: Error, permutace ma mene prvku nez je procesoru." << endl;
    //exit (-4);
}

int rozdelPraci(Permutace *perm, int m, int cpus, int source, int receiver = -1) {
    int radu = 0, segmentu = 0, oblast;
    int message[m*2];
    if(!rozdelitNaKolikaRadech(perm, cpus, radu, segmentu))
        return false;
    oblast = segmentu / cpus;
    //cout << "Rozdelit praci na " << segmentu << " segmentu pro " << cpus << " procesoru (" << radu << ") radu. Konec!" << endl;
    
    Permutace* tmpPerm = perm->copyAndCut(radu);
    vector<Coin> startCoins, endCoins;
    int coinId;
    for(int i = 1; i < cpus; i++){
        coinId = 0;
        //aby se nepocitalo vicekrat to same - tmpPerm->getCurCoins() vraci nejnizsi permutaci pro dany zacatek
        if(i == 1)
            startCoins = perm->getCurCoins();
        else
            startCoins = tmpPerm->getCurCoins();
        for(vector<Coin>::iterator it = startCoins.begin(); it != startCoins.end(); it++)
            message[coinId++] = it->getHodnota();
        while(coinId < m){
            message[coinId++] = -1;
        }
        for(int s = 0; s < oblast; s++)
            tmpPerm->getNextPerm();
        
        endCoins = tmpPerm->getCurCoins();
        for(vector<Coin>::iterator it = endCoins.begin(); it != endCoins.end(); it++)
            message[coinId++] = it->getHodnota();
        while(coinId < 2*m){
            message[coinId++] = -1;
        }
        if(DEBUG){
            cout << "---------------------" << source << "POSILA PRACI " << (receiver == -1 ? i : receiver) << " ------------------------" << endl;
            for(int i = 0; i < m*2; i++)
                cout << message[i] << " ";
            cout << endl;
            cout << "--------------------------------------------------------------" << endl;
        }
        MPI_Send (message, m*2, MPI_INT, (receiver == -1 ? i : receiver), FLAG_NEW_DATA, MPI_COMM_WORLD);
    }
    if(DEBUG){
        cout << "Orig coins" << endl;
        Permutace::printCoinsSimple(perm->getCurCoins());
    }
    perm->setCurCoins(tmpPerm->getCurCoins(), radu);
    if(DEBUG){
        cout << "New Start" << endl;
        Permutace::printCoinsSimple(perm->getCurCoins());
    }
    return true;
}

void processReceivedWork(Permutace* perm, int message[], int messageLength, int my_rank){
    vector<Coin> tmpVector(messageLength/2);
    int length = 0;
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
    if(DEBUG){
        cout << "PROCES " << my_rank << " DOSTAL PRACI" << endl;
        cout << "Start: " << endl;
        Permutace::printCoinsSimple(perm->getCurCoins());
        cout << "End: " << endl;
        Permutace::printCoinsSimple(perm->getMaxCoins());
        
    }
}

void processResults(int message[], int messageLength, int& bestCoinCount, vector<Coin>& bestCoins){
    int sum = 0;
 
    for(int i = 1; i < messageLength; i += 2){
        sum += message[i];
    }
    if(sum < bestCoinCount && sum > 0){
        bestCoinCount = sum;
        for(int j = 0; j < messageLength; j += 2){
            bestCoins[j/2].setHodnota(message[j]);
            bestCoins[j/2].setPocet(message[j+1]);
        }
    }
}

void receiveMessage(Permutace* perm, int message[], int messageLength, int my_rank, bool& bHasWork, bool& bForceEnd, MPI_Status& status, bool& bWhiteToken, 
        int tag = -1, int from = -1, bool blocking = false){
    int flag;
    from = (from == -1) ? MPI_ANY_SOURCE : from;
    tag = (tag == -1) ? MPI_ANY_TAG : tag;
    if(!blocking){
        MPI_Iprobe(from, tag, MPI_COMM_WORLD, &flag, &status);
        if(!flag)
            return;
    }
    //cout << "PROCES " << my_rank << " CEKA NA ZPRAVU OD " << from << " S TAGEM " << tag << endl;
    MPI_Recv(message, messageLength, MPI_INT, from, tag, MPI_COMM_WORLD, &status);
    //cout << "PROCES " << my_rank << " DOSTAL ZPRAVU OD " << status.MPI_SOURCE << " TYPU " << status.MPI_TAG << endl; 
    switch(status.MPI_TAG){
        case FLAG_HAS_WORK:
            if(bHasWork){
                if(!rozdelPraci(perm, (messageLength/2), 2, my_rank, status.MPI_SOURCE)){
                    bHasWork = false;
                    MPI_Send(message, messageLength, MPI_INT, status.MPI_SOURCE, FLAG_NO_WORK, MPI_COMM_WORLD);
                    return;
                }
                if(my_rank > status.MPI_SOURCE)
                    bWhiteToken = true;
            }
            else{
                MPI_Send(message, messageLength, MPI_INT, status.MPI_SOURCE, FLAG_NO_WORK, MPI_COMM_WORLD);
            }
            return;
        case FLAG_NEW_DATA:
            processReceivedWork(perm, message, messageLength, my_rank);
            return;
        case FLAG_FORCE_END:
            if(DEBUG)
                cout << "PROCES " << my_rank << " DOSTAL FORCE END!!!!" << endl;
            bForceEnd = true;
            return;
        case FLAG_IS_FINISHED:
            if(DEBUG)
                cout << "PROCES " << my_rank << " DOSTAL ZPRAVU OD " << status.MPI_SOURCE << " TYPU " << status.MPI_TAG << endl;
            MPI_Send(message, messageLength, MPI_INT, status.MPI_SOURCE, ((bWhiteToken || bHasWork) == true ? FLAG_IS_FINISHED_NO : FLAG_IS_FINISHED_YES), MPI_COMM_WORLD);
            if(DEBUG)
                cout << "PROCES " << my_rank << " ODESLAL ZPRAVU " << status.MPI_SOURCE << " TYPU " << ((bWhiteToken || bHasWork) == true ? FLAG_IS_FINISHED_NO : FLAG_IS_FINISHED_YES) << endl;
            bWhiteToken = false;
            return;
    }
}

bool tryToGetWork(Permutace* perm, int cpuCount, int my_rank, int message[], int messageLength, bool& bForceEnd){
    vector<int> cpuList(cpuCount-2);
    int index = 0;
    MPI_Status status;
    bool dummyToken = false;
    int cpu;
    for(int i = 1; i < cpuCount; i++){
        if(i == my_rank)
            continue;
        cpuList[index++] = i;
    }
    random_shuffle(cpuList.begin(), cpuList.end());
    
    MPI_Send(message, messageLength, MPI_INT, 0, FLAG_HAS_WORK, MPI_COMM_WORLD);
    while(true){
        if(bForceEnd)
            return false;
        receiveMessage(perm, message, messageLength, my_rank, dummyToken, bForceEnd, status, dummyToken, -1, -1, true);
        if(status.MPI_SOURCE == 0 && (status.MPI_TAG == FLAG_NEW_DATA || status.MPI_TAG == FLAG_NO_WORK || status.MPI_TAG == FLAG_FORCE_END))
            break;
    }
    if(status.MPI_TAG == FLAG_NEW_DATA){
        return true;
    }else if(status.MPI_TAG == FLAG_NO_WORK){
        for(int i = 0; i < cpuList.size(); i++){
            cpu = cpuList[i];
            MPI_Send(message, messageLength, MPI_INT, cpu, FLAG_HAS_WORK, MPI_COMM_WORLD);
            while(true){
                if(bForceEnd)
                    return false;
                receiveMessage(perm, message, messageLength, my_rank, dummyToken, bForceEnd, status, dummyToken, -1, -1, true);
                if(status.MPI_SOURCE == cpu && status.MPI_TAG == FLAG_NEW_DATA){
                    return true;
                }else if(status.MPI_SOURCE == cpu && status.MPI_TAG == FLAG_NO_WORK){
                    break;
                }else if(status.MPI_SOURCE == cpu && status.MPI_TAG == FLAG_FORCE_END){
                    return false;
                }
            }
        }
    }
    return false;
}

bool startFinishing(int cpuCount, int message[], int messageLength, bool& bForceEnd){
    MPI_Status status;
    int flag;
    bool bDummy = false;
    for(int i = 1; i < cpuCount; i++){
        if(DEBUG)
            cout << "SKONCIL PROCES " << i << " ?" << endl;
        MPI_Send(message, messageLength, MPI_INT, i, FLAG_IS_FINISHED, MPI_COMM_WORLD);
        while(true){
            MPI_Iprobe(i, FLAG_IS_FINISHED_YES, MPI_COMM_WORLD, &flag, &status);
            if(flag){
                if(DEBUG)
                    cout << "PROCES " << i << " SKONCIL" << endl;
                MPI_Recv(message, messageLength, MPI_INT, i, FLAG_IS_FINISHED_YES, MPI_COMM_WORLD, &status);
                break;
            }
            MPI_Iprobe(i, FLAG_IS_FINISHED_NO, MPI_COMM_WORLD, &flag, &status);
            if(flag){
                if(DEBUG)
                    cout << "PROCES " << i << " NESKONCIL" << endl;
                MPI_Recv(message, messageLength, MPI_INT, i, FLAG_IS_FINISHED_NO, MPI_COMM_WORLD, &status);
                return false;
            }
            receiveMessage(NULL, message, messageLength, 0, bDummy, bForceEnd, status, bDummy, -1, -1, true);
            if(status.MPI_SOURCE == i && status.MPI_TAG == FLAG_IS_FINISHED_YES){
                if(DEBUG)
                    cout << "PROCES " << i << " SKONCIL" << endl;
                break;
            }else if(status.MPI_SOURCE == i && status.MPI_TAG == FLAG_IS_FINISHED_NO){
                if(DEBUG)
                    cout << "PROCES " << i << " NESKONCIL" << endl;
                return false;
            }
        }
    }
    bForceEnd = true;
    return true;
}

void forceEnd(int cpuCount, int message[], int messageLength){
    for(int i = 1; i < cpuCount; i++){
        MPI_Send(message, messageLength, MPI_INT, i, FLAG_FORCE_END, MPI_COMM_WORLD);
        if(DEBUG)
            cout << "FORCE END FOR " << i << endl;
    }
}

int main(int argc, char** argv) {
    time_t begin = time(NULL);
    int m, n, *C;
    ifstream fin(argv[1]);
    
    fin >> n;
    C = new int[n];
    for (int i = 0; i < n; i++) {
        fin >> C[i];
    }
    fin >> m;
    fin.close();
    
    
    //atributy slouzici ke komunikaci mezi procesy, pripadne k zajisteni prace
    int my_rank;
    int cpuCount;
    int message[m*2];
    int messageLength = m*2;
    bool bHasWork = false;
    bool bForceEnd = false;
    MPI_Status status;
    bool bWhiteToken = false;
    
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
//    if(DEBUG)
//        perm->printCurCoins();
    //cout << "n = " << n << " m = " << m << endl;
    if(my_rank == 0){
        rozdelPraci(perm, m, cpuCount, 0);
    }else{
        MPI_Recv(&message, messageLength, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        processReceivedWork(perm, message, messageLength, my_rank);
    }
    if(DEBUG)
    {
        cout << "---------------------PROCES " << my_rank << "------------------------" << endl;
        perm->printCurCoins();
        perm->printMaxCoins();
        cout << "---------------------PROCES " << my_rank << "------------------------" << endl;
    }
    bHasWork = true;
    Calculation* calcPrim = new Calculation(n, m, C);
    //cout << "n = " << n << " m = " << m << endl;
    if(DEBUG)
        perm->printPayout();
    isEnd = perm->getCurCoins();
    while (true) {
        if(bForceEnd)
            break;
        if (isEnd.size() < 1) {
            cerr << "main: Error function gerNextPerm return a vector with size 0" << endl;
            exit(-3);
        }
        if(isEnd[0].getHodnota() == ENDVAL){
            bHasWork = false;
            if(my_rank == 0){
                if(DEBUG){
                    cout << "-----------------------ZACINA UKONCOVANI-----------------------" << endl;
                }
                while(true){
                    if(startFinishing(cpuCount, message, messageLength, bForceEnd))
                        break;
                }
                forceEnd(cpuCount, message, messageLength);
            }else{
                if(DEBUG){
                    cout << "-----------------PROCES " << my_rank << " NEMA PRACI----------------------" << endl;
                }
                bool bRet;
                while(true){
                    bRet = tryToGetWork(perm, cpuCount, my_rank, message, messageLength, bForceEnd);
                    if(bRet || bForceEnd)
                        break;
                }
                if(bRet){
                    bHasWork = true;
                    isEnd = perm->getCurCoins();
                }
            }
            continue;
        }
//        if(DEBUG)
//            Permutace::printCoins(isEnd);
        int tmpRes = calcPrim->trivEvaluateCurCoins(isEnd);
        if(DEBUG){
            cout << "-------------VYPOCTENO-----------------" << endl; 
            cout << "Vysledek: " << tmpRes << endl;
            Permutace::printCoinsSimple(isEnd);
        }
        //calcPrim->evaluCurCoinsPrecise(isEnd);
        isEnd = perm->getNextPerm();
        receiveMessage(perm, message, messageLength, my_rank, bHasWork, bForceEnd, status, bWhiteToken);
    }
    
    if(my_rank == 0){
        bestCoins = calcPrim->getBestCoins();
        int counter = 0;
        while(counter < cpuCount-1){
            MPI_Recv(&message, messageLength, MPI_INT, MPI_ANY_SOURCE, FLAG_RESULTS, MPI_COMM_WORLD, &status);
            counter++;
            processResults(message, messageLength, bestCoinCount, bestCoins);
            if(DEBUG)
                cout << "---------------------PROCES DOSTAL VYSLEDEK OD "<< status.MPI_SOURCE<< " ------------------------" << endl;
        }
        cout << "-----------------------------RESULT------------------------------" << endl;
        Permutace::printCoins(bestCoins);
    }else{
        vector<Coin> tmp = calcPrim->getBestCoins();
        int index = 0;
        for(vector<Coin>::iterator it = tmp.begin(); it != tmp.end(); it++){
            message[index++] = it->getHodnota();
            message[index++] = it->getPocet();
        }
        if(DEBUG){
            cout << "---------------------PROCES " << my_rank << " POSLAL VYSLEDEK------------------------" << endl;
            Permutace::printCoins(tmp);
        }
        MPI_Send(message, messageLength, MPI_INT, 0, FLAG_RESULTS, MPI_COMM_WORLD);
        
            
        
    }
    //calcPrim->printBestCoins();
    
    
    if(my_rank == 0)
    {
        time_t end = time(NULL);
        printTime(begin, end);
    }
    MPI_Finalize();
    delete perm;
    delete calcPrim;
    delete [] C;
    return 0;
}

