/* 
 * File:   Permutace.cpp
 * Author: New
 * 
 * Created on 24. září 2013, 11:39
 */

#include "Permutace.h"
#include "constants.h"
#include <math.h>

Permutace::Permutace(int n, int m, int* C) {
    cout << ((DEBUG) ? "D: Permutace construct start\n" : "");
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

Permutace Permutace::operator=(const Permutace &orig) {
    this->mCoins = orig.mCoins;
    this->maxCoinVal = orig.maxCoinVal;
    this->curCoins = orig.curCoins;
    this->maxCoins = orig.maxCoins;
    this->nPayout = orig.nPayout;
    this->Payout.resize(this->nPayout);
    for (int i = 0; i<this->nPayout; i++) {
        this->Payout[i] = orig.Payout[i];
    }
    return orig;
}

bool Permutace::operator==(const Permutace& sec) {
    if (this->mCoins != sec.mCoins) {
        cout << ((DEBUG) ? "D: Operator== objekty maji jiny pocet mCoins\n" : "");
        return false;
    }
    int a, b;
    Coin c;
    for (int i=0 ; i<mCoins ; i++) {
        a = this->curCoins[i].getHodnota();
        c = sec.curCoins[i];
        b = c.getHodnota();
        if (a != b) {
            cout << "Operator== nerovna se a=" << a << " b=" << b << " i=" << i << endl;
            return false;
        }
    }
    return true;
}

bool Permutace::isSameVectors(vector<Coin> u, vector<Coin> v) {
    if (u.size() != v.size()) {
        return false;
    }
    for (int i=0 ; i<(int)u.size() ; i++) {
        if (u[i].getHodnota() != v[i].getHodnota()) {
            return false;
        }
    }
    
    return true;
}

bool Permutace::isSmallerVector(vector<Coin> smaller, vector<Coin> bigger) {
    unsigned size; //delka kratsiho vectoru
    if (smaller.size() != bigger.size()) {
        cout << ((DEBUG) ? "D: isSmallerVector() Delky vectoru se nerovnaji\n" : "");
    }
    if (smaller.size() < bigger.size()) {
        size = smaller.size();
    }
    else {
        size = bigger.size();
    }
    
    for (int i=0 ; i<(int)size ; i++) {
        if (bigger[i].getHodnota() < smaller[i].getHodnota()) {
            return false;
        }
    }
    
    return true;
}

void Permutace::initCurCoins() {
    cout << ((DEBUG) ? "D: initCurCoins() start\n" : "");
    if (this->mCoins >= 1)
        this->curCoins[0].setHodnota(1);
    if (this->mCoins >= 2)
        this->curCoins[1].setHodnota(2);

    for (int i = 2; i<this->mCoins; i++) {
        this->curCoins[i] = (2 * this->curCoins[i - 1].getHodnota()) - 1;
    }
    this->isCoinsInit = true;
    cout << ((DEBUG) ? "D: initCurCoins() end\n" : "");
}

void Permutace::initMaxCoins(){
    for(int i = (int)maxCoins.size() - 1; i >= 0; i--){
        if(i == (int)maxCoins.size() - 1){
            maxCoins[i].setHodnota(this->maxCoinVal);
        }else{
            maxCoins[i].setHodnota((int)round(floor((maxCoins[i+1].getHodnota() + 1) / 2)));
        }
    }
}

vector<Coin> Permutace::getNextPerm() {
    // Prvni dotaz na permutaci -> vygeneruj nove permutace 1,2,3,5,9,17 podle vzorce
    if ( this->isCoinsInit == false ) {
        initCurCoins();
        initMaxCoins();
        return this->curCoins;
    }
    
    //Promenne pro definici konce
    // Pokud je curCoins mensi nebo rovna maxCoins pak se vrati predem definovany vector (nemozny)
    bool ret = true;
    vector<Coin> endVector;
    Coin coin(ENDVAL);
    endVector.push_back(coin);
    
    ret = this->incrementCurCoins();
    
    if (ret)
        return this->curCoins;
    else
        return endVector;
}

bool Permutace::incrementCurCoins() {
    int tmp, overflow = -1;
    
    if ((isSameVectors(curCoins, maxCoins)) || (isSmallerVector(maxCoins, curCoins))) {
        return false;
    }
    
    for (int i=0 ; i<mCoins ; i++) {
        tmp = curCoins[mCoins-1-i].getHodnota() + 1;
        if (tmp > maxCoins[mCoins-1-i].getHodnota()) {
            overflow = mCoins-1-i-1;
            continue;
        }
        else {
            curCoins[mCoins-1-i] = tmp;
            break;
        }
    }
    if (overflow != -1) {
        repairCurCoins(overflow);
        overflow = -1;
    }
    
    return true;
}

void Permutace::repairCurCoins(int from) {
    for (int i=from ; i<mCoins-1 ; i++) {
        curCoins[i+1].setHodnota(2 * curCoins[i].getHodnota() - 1);
    }
    if(from == 0 && DEBUG){
        cout << "Zvysuji " << curCoins[0].getHodnota() << endl; 
    }
}

void Permutace::printCurCoins() {
    cout << "Current Coins:" << endl;
    for (int i = 0; i<this->mCoins; i++) {
        cout << "\t" << this->curCoins[i].getHodnota() << "\t" << this->curCoins[i].getPocet() << endl;
    }
    int iSum = 0;
    for(vector<Coin>::iterator it = curCoins.begin(); it != curCoins.end(); it++)
        iSum += it->getPocet();
    cout << "Sum: " << iSum;

    cout << endl << endl;
}

void Permutace::printMaxCoins() {
    cout << "Max Coins:\n";
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

vector<Coin> Permutace::getCurCoins() {
    return this->curCoins;
}

vector<Coin> Permutace::getMaxCoins() {
    return this->maxCoins;
}

Permutace::~Permutace() { }

