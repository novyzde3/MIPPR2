/* 
 * File:   Calculation.cpp
 * Author: Radek
 * 
 * Created on 6. říjen 2013, 10:15
 */

using namespace std;

#include <list>
#include "Calculation.h"
#include "StackRecord.h"
#include "Permutace.h"

Calculation::Calculation(int n, int m, int* C) {
    this->m_iCoinsSize = m;
    this->m_BestCoins.resize(m);
    this->m_iBestCoinCount = INT_MAX;
    this->m_iPayoutSize = n;
    this->m_Payout.resize(this->m_iPayoutSize);
    for (int i = 0; i<this->m_iPayoutSize; i++) {
        this->m_Payout[i] = C[i];
    }
}

Calculation::Calculation(const Calculation& orig) {
    this->m_iCoinsSize = orig.m_iCoinsSize;
    this->m_BestCoins = orig.m_BestCoins;
    this->m_iBestCoinCount = orig.m_iBestCoinCount;
    this->m_iPayoutSize = orig.m_iPayoutSize;
    this->m_Payout = orig.m_Payout;
}

Calculation::~Calculation() {
}

void Calculation::printBestCoins() {
    cout << "Best Coins:" << endl;
    for (int i = 0; i<this->m_iCoinsSize; i++) {
        cout << "\t" << this->m_BestCoins[i].getHodnota() << "\t" << this->m_BestCoins[i].getPocet() << endl;
    }
    int iSum = 0;
    for(vector<Coin>::iterator it = m_BestCoins.begin(); it != m_BestCoins.end(); it++)
        iSum += it->getPocet();
    cout << "Sum: " << iSum;

    cout << endl << endl;
}

vector<Coin> Calculation::getBestCoins() {
    return m_BestCoins;
}



int Calculation::trivEvaluateCurCoins(vector<Coin> permutation) {
    int iAmount, iCoinCount = 0, iCoinCountTmp, iCoinIndex;
    //bSkipped je true pokud se predcasne ukonci pocitani
    bool bSkipped = false;
    
    for (int i = 0; i < this->m_iPayoutSize; i++) {
        iAmount = this->m_Payout[i];
        
        iCoinCountTmp = 0;
        iCoinIndex = this->m_iCoinsSize - 1;
        while ((iCoinCount < this->m_iBestCoinCount) && (iAmount > 0) && (iCoinIndex >= 0)) {
            if ((iCoinCountTmp = iAmount / permutation[iCoinIndex].getHodnota())) {
                iCoinCount += iCoinCountTmp;
                iAmount -= (iCoinCountTmp*permutation[iCoinIndex].getHodnota());
                permutation[iCoinIndex].incPocet(iCoinCountTmp);
            }
            iCoinIndex--;

        }
        if(iAmount != 0){
            bSkipped = true;
            return -1;
        }
        if(iCoinCount >= this->m_iBestCoinCount){
            bSkipped = true;
            break;
        }
    }
    if(!bSkipped && iCoinCount < this->m_iBestCoinCount){
        this->m_iBestCoinCount = iCoinCount;
        this->m_BestCoins = permutation;
    }
    return iCoinCount;
}

void Calculation::evaluCurCoinsPrecise(vector<Coin> permutation) {
    list<StackRecord> myStack;
    vector<Coin> tmpBestCoins = permutation;
    
    //iAmount - castka k vyplaceni, iCoinId - pozice mince, ktera se zkusi v vyplaceni
    //iCoinCount - pocet minci potrebnych k vyplaceni m_Payout
    //iFirstCoin - iCoinId pro zacatek posloupnosti
    int iAmount, iCoinId, iCoinCount = 0, iFirstCoin;
    bool bCounted;
    
    for(int i = 0; i < this->m_iPayoutSize; i++) {
        bCounted = false;
        iFirstCoin = this->m_iCoinsSize;        
        myStack.clear();
        myStack.push_back(StackRecord(this->m_Payout[i], iFirstCoin--));
        
        while (!myStack.empty()) {
            iAmount = myStack.back().GetAmount();
            iCoinId = myStack.back().GetCoinId() - 1;
            
            if(iCoinId >= 0){
                if((iAmount / permutation[iCoinId].getHodnota()) >= 1){
                    myStack.push_back(StackRecord(iAmount - permutation[iCoinId].getHodnota(), 
                            iCoinId + 1, permutation[iCoinId].getHodnota(), myStack.back().GetCoins()));
                }else{
                    myStack.back().Decrement();
                }
            }else{
                myStack.pop_back();
                if(!myStack.empty())
                    myStack.back().Decrement();
            }
                        
            if(!myStack.empty() && myStack.back().IsFull()){
                bCounted = true;
                iCoinCount += myStack.back().GetCoinCount();
                for(vector<Coin>::iterator it = permutation.begin(); it != permutation.end(); it++){
                    it->setPocet(0);
                }
                vector<int> coins = myStack.back().GetCoins();
                for(vector<int>::iterator it = coins.begin(); it != coins.end(); it++){
                    for(int i = 0; i < this->m_iCoinsSize; i++){
                        if(permutation[i].getHodnota() == *it){
                            permutation[i].incPocet(1);
                        }
                    }
                }
                break;
            }
        }
        if(!bCounted)
            return;
        for(int i = 0; i < this->m_iCoinsSize; i++){
            tmpBestCoins[i].incPocet(permutation[i].getPocet());
        }
    }
    
    if(iCoinCount < this->m_iBestCoinCount){
        this->m_iBestCoinCount = iCoinCount;
        this->m_BestCoins = tmpBestCoins;
    }
}

