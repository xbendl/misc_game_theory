/**
* \file payoff.h
* Modul pro vypocet vyplatni matice uvazovanych strategii reseni problemu opakovaneho veznova dilematu.
* \author Bendl Jaroslav (xbendl00)
*/

#ifndef PAYOFF_H_INCLUDED
#define PAYOFF_H_INCLUDED

#include <iostream>
#include <vector>
#include "strategy.h"

class Payoff
{
    int matchLength;                      //< Delka souboje.
    int playerScore;                      //< Pomocna promenna pro uchovani score hrace z jednoho souboje.
    int opponentScore;                    //< Pomocna promenna pro uchovani score protihrace z jednoho souboje.
    int payoff_cc;                        //< Vyplata v pripade scenare: spoluprace x spoluprace
    int payoff_cd;                        //< Vyplata v pripade scenare: spoluprace x zrada
    int payoff_dc;                        //< Vyplata v pripade scenare: zrada x spoluprace
    int payoff_dd;                        //< Vyplata v pripade scenare: zrada x zrada

    public:
    /// Typ pro ulozeni radku vyplatni matice.
    typedef std::vector<int> PayoffMatrixRow;

    /// Typ pro ulozeni mrizky vyplatni matice.
    typedef std::vector<PayoffMatrixRow> PayoffMatrix;

    /**
    * Konstruktor
    * @param payoff_cc Vyplata v pripade scenare: spoluprace x spoluprace
    * @param payoff_cd Vyplata v pripade scenare: spoluprace x zrada
    * @param payoff_dc Vyplata v pripade scenare: zrada x spoluprace
    * @param payoff_dd Vyplata v pripade scenare: zrada x zrada
    */
    Payoff(int payoff_cc, int payoff_cd, int payoff_dc, int payoff_dd);

    /**
    * Vypocita vyplatni matici.
    * @param popPlayer Uvazovane strategie ve vyplatni matici.
    * @param popOpponent Uvazovane strategie protihrace ve vyplatni matici.
    * @param payoffMatrix Vypocitavana vyplatni matice.
    * @param matchLength Uvazovana delka souboje pro urceni score ve vyplatni matici.
    */
    void GetPayoffMatrix(std::vector<Strategy *> &popPlayer, std::vector<Strategy *> &popOpponent, PayoffMatrix &payoffMatrix, int matchLength);

    /**
    * Vrati hodnotu z vyplatni matice pro dany tah hrace a jeho protihrace.
    * @param playerDecision Aktualni tah hrace.
    * @param opponentDecision Aktualni tah protihrace.
    * @return Hodnota z vyplatni matice pro dany tah hrace a jeho protihrace.
    */
    int GetPayoffRound(E_DECISION playerDecision, E_DECISION opponentDecision);

    /**
    * Simuluje souboj dvou hracu.
    * @param playerStrategy Prvni hrac.
    * @param opponentStrategy Druhy hrac.
    */
    void Match(Strategy *playerStrategy, Strategy *opponentStrategy);
};

#endif // PAYOFF_H_INCLUDED
