/* 
 * File:   Permutace.cpp
 * Author: New
 * 
 * Created on 24. září 2013, 11:39
 */

#include "Permutace.h"
#include <math.h>

Permutace::Permutace(int n, int m, int* C) {
    this->mCoins = m;
    this->curCoins.resize(m);
    this->maxCoins.resize(m);
    this->nPayout = n;
    this->Payout.resize(this->nPayout);
    for (int i = 0; i<this->nPayout; i++) {
        this->Payout[i] = C[i];
    }
    this->maxCoinVal = this->Payout[this->nPayout - 1];
    this->isCoinsInit = false;
}

Permutace::Permutace(const Permutace& orig) {
    this->mCoins = orig.mCoins;
    this->maxCoinVal = orig.maxCoinVal;
    this->curCoins = orig.curCoins;
    this->maxCoins = orig.maxCoins;
    this->nPayout = orig.nPayout;
    this->Payout.resize(this->nPayout);
    for (int i = 0; i<this->nPayout; i++) {
        this->Payout[i] = orig.Payout[i];
    }
}

void Permutace::initCurCoins() {
    if (this->mCoins >= 1)
        this->curCoins[0].setHodnota(1);
    if (this->mCoins >= 2)
        this->curCoins[1].setHodnota(2);

    for (int i = 2; i<this->mCoins; i++) {
        this->curCoins[i] = (2 * this->curCoins[i - 1].getHodnota()) - 1;
    }
    this->isCoinsInit = true;
}

void Permutace::initMaxCoins(){
    for(int i = maxCoins.size() - 1; i >= 0; i--){
        if(i == maxCoins.size() - 1){
            maxCoins[i].setHodnota(this->maxCoinVal);
        }else{
            maxCoins[i].setHodnota((int)round(floor((maxCoins[i+1].getHodnota() + 1) / 2)));
        }
    }
}

vector<Coin> Permutace::getNextPerm() {
    if ( this->isCoinsInit == false ) {
        initCurCoins();
        initMaxCoins();
        return this->curCoins;
    }
    cout << "Incrementuji" << endl;
    this->incrementCoins();
    return this->curCoins;
}

void Permutace::incrementCoins() {
    // Zvysi posledni cislo o 1
    int lastValue = this->curCoins[this->mCoins-1].getHodnota();
    this->curCoins[this->mCoins-1].setHodnota(lastValue+1);
}

void Permutace::printCurCoins() {
    cout << "Current Coins:";
    for (int i = 0; i<this->mCoins; i++) {
        cout << "\t" << this->curCoins[i].getHodnota() << "\t" << this->curCoins[i].getPocet() << endl;
    }
    cout << endl << endl;
}

void Permutace::printMaxCoins() {
    cout << "Max Coins:";
    for (int i = 0; i<this->mCoins; i++) {
        cout << "\t" << this->maxCoins[i].getHodnota() << endl;
    }
    cout << endl << endl;
}

void Permutace::printPayout() {
    cout << "Payout (vyplacene castky):" << endl << "\t{ ";
    for (int i = 0; i<this->nPayout; i++) {
        cout << this->Payout[i] << ", ";
    }
    cout << "}" << endl << endl;
}

int Permutace::getMaxCoinVal() {
    return this->maxCoinVal;
}

Permutace::~Permutace() { }

