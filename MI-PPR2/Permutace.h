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
    vector<Coin> getNextPerm();
    void printCurCoins();
    void printMaxCoins();
    void printPayout();
    int getMaxCoinVal();
    
    
private:
    int nPayout; //n
    int mCoins; //m
    vector<int> Payout; // C
    vector<Coin> curCoins; // M
    vector<Coin> maxCoins;
    int maxCoinVal; // max(Payout) = Payout[nPayout-1] je maximalni hodnota mince, kterou muzeme pouzit  
    bool isCoinsInit;
    
    void initCurCoins();
    void initMaxCoins();
    void incrementCoins();
};

#endif	/* PERMUTACE_H */

