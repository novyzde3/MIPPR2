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
    
    int getHodnota() const;
    void setHodnota(int);
    int getPocet() const;
    void setPocet(int);
    void incPocet(int iPlus);
private:
    int hodnota;
    int pocet;
};

#endif	/* COIN_H */

