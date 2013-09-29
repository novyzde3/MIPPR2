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
    Permutace operator=(const Permutace&);
    bool operator==(const Permutace&);
    bool isSameVectors(vector<Coin>, vector<Coin>);
    bool isSmallerVector(vector<Coin>, vector<Coin>);
    virtual ~Permutace();
    vector<Coin> getNextPerm();
    void printCurCoins();
    void printMaxCoins();
    void printPayout();
    int getMaxCoinVal();
    vector<Coin> getCurCoins();
    vector<Coin> getMaxCoins();
    
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
    bool incrementCurCoins();
    void repairCurCoins(int);
};

#endif	/* PERMUTACE_H */

