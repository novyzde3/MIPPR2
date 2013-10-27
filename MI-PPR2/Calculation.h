/* 
 * File:   Calculation.h
 * Author: Radek
 *
 * Created on 6. říjen 2013, 10:15
 */

#ifndef CALCULATION_H
#define	CALCULATION_H

#include <vector>
#include <cstdlib>
#include <limits.h>
#include <iostream>
#include "Coin.h"

using namespace std;

class Calculation {
public:
    Calculation(int n, int m, int* C);
    Calculation(const Calculation& orig);
    virtual ~Calculation();
    
    void printBestCoins();
    vector<Coin> getBestCoins();
    //trivialni metoda na zjisteni, jak zaplatit
    void trivEvaluateCurCoins(vector<Coin> permutation);
    //netrivialni metoda na zjisteni, jak zaplatit - pouziva zasobnik a zkousi vsechny moznosti
    void evaluCurCoinsPrecise(vector<Coin> permutation);
private:
    int m_iPayoutSize; //n
    int m_iCoinsSize; //m
    vector<int> m_Payout; // C
    vector<Coin> m_BestCoins; //aktualne nejlepsi posloupnost
    int m_iBestCoinCount;  //nejlepsi pocet minci, kterymi se bude platit
};

#endif	/* CALCULATION_H */

