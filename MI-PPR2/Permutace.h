/* 
 * File:   Permutace.h
 * Author: New
 *
 * Created on 24. září 2013, 11:39
 */

#ifndef PERMUTACE_H
#define	PERMUTACE_H

#include "Coin.h"
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

class Permutace {
public:
    Permutace(int n, int m, int* C);
    Permutace(const Permutace& orig);
    virtual ~Permutace();
    bool nextPerm();
    void printCurCoins();
    void printMaxCoins();
    void printBestCoins();
    void printPayout();
    int getMaxCoinVal();
    bool evaluateCurCoins();
    
    
private:
    int m_iPayoutSize; //n
    int m_iCoinsSize; //m
    vector<int> m_Payout; // C
    vector<Coin> m_CurCoins; // M
    vector<Coin> m_MaxCoins;
    vector<Coin> m_BestCoins; //aktualne nejlepsi posloupnost
    int m_iBestCoinCount;  //nejlepsi pocet minci, kterymi se bude platit
    int m_iMaxCoinVal; // max(Payout) = Payout[nPayout-1] je maximalni hodnota mince, kterou muzeme pouzit  
    bool m_bIsCoinsInit;
    
    void initCurCoins();
    void initMaxCoins();
    bool incrementCoins();
};

#endif	/* PERMUTACE_H */

