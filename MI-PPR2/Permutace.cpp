/* 
 * File:   Permutace.cpp
 * Author: New
 * 
 * Created on 24. září 2013, 11:39
 */

#include "Permutace.h"
#include <math.h>

Permutace::Permutace(int n, int m, int* C) {
    this->m_iCoinsSize = m;
    this->m_CurCoins.resize(m);
    this->m_MaxCoins.resize(m);
    this->m_BestCoins.resize(m);
    this->m_iBestCoinCount = INT_MAX;
    this->m_iPayoutSize = n;
    this->m_Payout.resize(this->m_iPayoutSize);
    for (int i = 0; i<this->m_iPayoutSize; i++) {
        this->m_Payout[i] = C[i];
    }
    this->m_iMaxCoinVal = this->m_Payout[this->m_iPayoutSize - 1];
    this->m_bIsCoinsInit = false;
}

Permutace::Permutace(const Permutace& orig) {
    this->m_iCoinsSize = orig.m_iCoinsSize;
    this->m_iMaxCoinVal = orig.m_iMaxCoinVal;
    this->m_CurCoins = orig.m_CurCoins;
    this->m_MaxCoins = orig.m_MaxCoins;
    this->m_BestCoins = orig.m_BestCoins;
    this->m_iBestCoinCount = orig.m_iBestCoinCount;
    this->m_iPayoutSize = orig.m_iPayoutSize;
    this->m_Payout = orig.m_Payout;
    this->m_bIsCoinsInit = orig.m_bIsCoinsInit;
}

Permutace::~Permutace() {
}

void Permutace::initCurCoins() {
    if (this->m_iCoinsSize >= 1)
        this->m_CurCoins[0].setHodnota(1);
    if (this->m_iCoinsSize >= 2)
        this->m_CurCoins[1].setHodnota(2);

    for (int i = 2; i<this->m_iCoinsSize; i++) {
        this->m_CurCoins[i] = (2 * this->m_CurCoins[i - 1].getHodnota()) - 1;
    }
    for(vector<Coin>::iterator it = m_CurCoins.begin(); it != m_CurCoins.end(); it++)
        it->setPocet(0);
    
    this->m_bIsCoinsInit = true;
}

void Permutace::initMaxCoins() {
    for (int i = m_MaxCoins.size() - 1; i >= 0; i--) {
        if (i == m_MaxCoins.size() - 1) {
            m_MaxCoins[i].setHodnota(this->m_iMaxCoinVal);
        } else {
            m_MaxCoins[i].setHodnota((int) round(floor((m_MaxCoins[i + 1].getHodnota() + 1) / 2)));
        }
    }
}

void Permutace::finishCurCoins(int iFrom) {   
    for (int i = iFrom + 1; i<this->m_iCoinsSize; i++) {
        this->m_CurCoins[i] = (2 * this->m_CurCoins[i - 1].getHodnota()) - 1;
    }
}

bool Permutace::nextPerm() {
    if (this->m_bIsCoinsInit == false) {
        initCurCoins();
        initMaxCoins();
        return true;
    }
    //cout << "Incrementuji" << endl;
    return this->incrementCoins();
}

bool Permutace::incrementCoins() {
    int coinId = this->m_iCoinsSize - 1;
    int iNo;
    
    for(vector<Coin>::iterator it = m_CurCoins.begin(); it != m_CurCoins.end(); it++)
        it->setPocet(0);
    
    while(coinId >= 0){
        iNo = this->m_CurCoins[coinId].getHodnota();
        if(iNo < this->m_MaxCoins[coinId].getHodnota()){
            this->m_CurCoins[coinId].setHodnota(iNo + 1);
            this->finishCurCoins(coinId);
            break;
        }else{
            coinId--;
        }
    }
    return coinId >= 0;
}

void Permutace::printCurCoins() {
    cout << "Current Coins:";
    for (int i = 0; i<this->m_iCoinsSize; i++) {
        cout << "\t" << this->m_CurCoins[i].getHodnota() << "\t" << this->m_CurCoins[i].getPocet() << endl;
    }
    cout << endl << endl;
}

void Permutace::printCurCoinsOnlyPerm() {
    cout << "Current Coins:";
    for (int i = 0; i<this->m_iCoinsSize; i++) {
        cout << " " << this->m_CurCoins[i].getHodnota();
    }
    cout << endl << endl;
}

void Permutace::printMaxCoins() {
    cout << "Max Coins:";
    for (int i = 0; i<this->m_iCoinsSize; i++) {
        cout << "\t" << this->m_MaxCoins[i].getHodnota() << endl;
    }
    cout << endl << endl;
}

void Permutace::printMaxCoinsOnlyPerm() {
    cout << "Max Coins:";
    for (int i = 0; i<this->m_iCoinsSize; i++) {
        cout << " " << this->m_MaxCoins[i].getHodnota();
    }
    cout << endl << endl;
}

void Permutace::printBestCoins() {
    cout << "Best Coins:";
    for (int i = 0; i<this->m_iCoinsSize; i++) {
        cout << "\t" << this->m_BestCoins[i].getHodnota() << "\t" << this->m_BestCoins[i].getPocet() << endl;
    }
    cout << endl << endl;
}

void Permutace::printPayout() {
    cout << "Payout (vyplacene castky):" << endl << "\t{ ";
    for (int i = 0; i<this->m_iPayoutSize; i++) {
        cout << this->m_Payout[i] << ", ";
    }
    cout << "}" << endl << endl;
}

int Permutace::getMaxCoinVal() {
    return this->m_iMaxCoinVal;
}

void Permutace::evaluateCurCoins() {
    int iAmount, iCoinCount, iCoinCountTmp, iCoinIndex;

    for (int i = 0; i < this->m_iPayoutSize; i++) {
        iAmount = this->m_Payout[i];

        iCoinCount = 0;
        iCoinCountTmp = 0;
        iCoinIndex = this->m_iCoinsSize - 1;
        while (iCoinCount < this->m_iBestCoinCount && iAmount != 0 && iCoinIndex >= 0) {
            if ((iCoinCountTmp = iAmount / this->m_CurCoins[iCoinIndex].getHodnota())) {
                iCoinCount += iCoinCountTmp;
                iAmount -= (iCoinCountTmp*this->m_CurCoins[iCoinIndex].getHodnota());
                this->m_CurCoins[iCoinIndex].setPocet(iCoinCountTmp);
            }
            iCoinIndex--;

        }
        if(iAmount == 0){
            if(iCoinCount < this->m_iBestCoinCount){
                this->m_iBestCoinCount = iCoinCount;
                this->m_BestCoins = this->m_CurCoins;
            }
        }
    }
}

