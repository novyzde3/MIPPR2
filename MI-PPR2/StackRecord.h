/* 
 * File:   StackRecord.h
 * Author: Radek
 *
 * Created on 29. září 2013, 11:48
 */

#ifndef STACKRECORD_H
#define	STACKRECORD_H

#include "Coin.h"
#include <vector>

using namespace std;

class StackRecord {
public:
    //vytvori se zaznam s prazdnym vectorem
    StackRecord(int iAmount, int iCoinId);
    //vytvori zaznam s novymi hodnotami + vector
    StackRecord(int iAmount, int iCoinId, int iCoin, const vector<int>& coins);
    StackRecord(const StackRecord& orig);
    virtual ~StackRecord();
        
    void Decrement();
    int GetCoinId() const;
    int GetAmount() const;
    int GetCoinCount() const;
    const vector<int>& GetCoins();
    bool IsFull() const;
    
private:
    //seznam pouzitych minci
    vector<int> m_coins;
    //poradi mince v m_CurCoins v Permutaci, ktera se nasledne pouzije
    int m_iCoinId;
    //aktualni hodnota k zaplaceni
    int m_iAmount;
};

#endif	/* STACKRECORD_H */

