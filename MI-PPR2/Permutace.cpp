/* 
 * File:   Permutace.cpp
 * Author: New
 * 
 * Created on 24. září 2013, 11:39
 */

#include "Permutace.h"

Permutace::Permutace(int n, int m, int* C) {
    this->mCoins = m;
    this->Coins = new Coin[this->mCoins]();
    this->nPayout = n;
    this->Payout = new int[this->nPayout];
    for (int i=0 ; i<this->nPayout ; i++) {
        this->Payout[i] = C[i];
    }
    this->maxCoinVal = this->Payout[this->nPayout-1];
}

Permutace::Permutace(const Permutace& orig) {
    this->mCoins = orig.mCoins;
    this->maxCoinVal = orig.maxCoinVal;
    this->Coins = new Coin[this->mCoins]();
    this->nPayout = orig.nPayout;
    this->Payout = new int[this->nPayout];
    for (int i=0 ; i<this->nPayout ; i++) {
        this->Payout[i] = orig.Payout[i];
    }
}

void Permutace::initPermutace() {
    if (this->mCoins >= 1)
        this->Coins[0].setHodnota(1);
    if (this->mCoins >= 2)
        this->Coins[1].setHodnota(2);
    
    for (int i=2 ; i<this->mCoins ; i++) {
        this->Coins[i] = (2 * this->Coins[i-1].getHodnota()) - 1;
    }
}

void Permutace::generuj() {
    initPermutace();
}

void Permutace::printCoins() {
    cout << "Coins:";
    for (int i=0 ; i<this->mCoins ; i++) {
        cout << "\t" << this->Coins[i].getHodnota() << "\t" << this->Coins[i].getPocet() << endl;
    }
    cout << endl << endl;
}

void Permutace::printPayout() {
    cout << "Payout (vyplacene castky):" << endl << "\t{ ";
    for (int i=0 ; i<this->nPayout ; i++) {
        cout << this->Payout[i] << ", ";
    }
    cout << "}" << endl << endl;
}

int Permutace::getMaxCoinVal() {
    return this->maxCoinVal;
}

Permutace::~Permutace() {
    delete[] this->Coins;
    delete[] this->Payout;
}

