/* 
 * File:   Coin.cpp
 * Author: New
 * 
 * Created on 24. září 2013, 12:02
 */

#include "Coin.h"
#include <stdlib.h>
#include <iostream>

using namespace std;

#include "Coin.h"

Coin::Coin() {
    this->hodnota = -1;
    this->pocet = 0;
}

Coin::Coin(int hodnota) {
    this->hodnota = hodnota;
    this->pocet = 0;
}

Coin::Coin(const Coin& orig) {
    this->hodnota = orig.hodnota;
    this->pocet = orig.pocet;
}

Coin::~Coin() {}

int Coin::getHodnota() {
    return this->hodnota;
}
void Coin::setHodnota(int hodnota) {
    if (hodnota <= 0) {
        cout << "Error: Hodnota je mensi nez 0 (" << hodnota << ")." << endl;
        exit(-200);
    }
    this->hodnota = hodnota;
}

int Coin::getPocet() {
    return this->pocet;
}

void Coin::setPocet(int pocet) {
    if (pocet < 0) {
        cout << "Error: Pocet je mensi nez 0 (" << pocet << ")." << endl;
        exit(-201);
    }
    this->pocet = pocet;
}
