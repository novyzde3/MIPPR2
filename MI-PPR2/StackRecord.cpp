/* 
 * File:   StackRecord.cpp
 * Author: Radek
 * 
 * Created on 29. září 2013, 11:48
 */

#include "StackRecord.h"

StackRecord::StackRecord(int iAmount, int iCoinId) {
    this->m_iCoinId = iCoinId;
    this->m_iAmount = iAmount;
}

StackRecord::StackRecord(int iAmount, int iCoinId, int iCoin, const vector<int>& coins){
    this->m_iCoinId = iCoinId;
    this->m_iAmount = iAmount;
    this->m_coins = coins;
    this->m_coins.push_back(iCoin);
}

StackRecord::StackRecord(const StackRecord& orig) {
    this->m_coins = orig.m_coins;
    this->m_iCoinId = orig.m_iCoinId;
    this->m_iAmount = orig.m_iAmount;
}

StackRecord::~StackRecord() {
}

void StackRecord::Decrement() {
    m_iCoinId--;
}

int StackRecord::GetCoinId() const {
    return m_iCoinId;
}

int StackRecord::GetAmount() const {
    return m_iAmount;
}

int StackRecord::GetCoinCount() const{
    return m_coins.size();
}

const vector<int>& StackRecord::GetCoins(){
    return m_coins;
}

bool StackRecord::IsFull() const{
    return m_iAmount == 0;
}

