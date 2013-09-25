/* 
 * File:   Coin.h
 * Author: New
 *
 * Created on 24. září 2013, 12:02
 */

#ifndef COIN_H
#define	COIN_H

class Coin {
public:
    Coin();
    Coin(int);
    Coin(const Coin& orig);
    virtual ~Coin();
    
    int getHodnota();
    void setHodnota(int);
    int getPocet();
    void setPocet(int);
private:
    int hodnota;
    int pocet;
};

#endif	/* COIN_H */

