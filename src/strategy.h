/**f
* \file strategy.h
* Modul pro definice prace strategii reseni problemu opakovaneho veznova diplematu
* \author Bendl Jaroslav (xbendl00)
*/

#ifndef STRATEGY_H_INCLUDED
#define STRATEGY_H_INCLUDED

#include <iostream>
#include <stdlib.h>
#include <vector>

/// Vycet moznych rozhodnuti hrace.
enum E_DECISION
{
    D_COOPERATE = 0,  //< Rozhodnuti spolupracovat.
    D_DEFECT = 1,     //< Rozhodnuti zradit.
    D_RANDOM = 2      //< Nahodne rozhodnuti.
};

/// Bazova trida pro jednotlive typy hernich strategii.
class Strategy
{
    public:
    std::string strategyName;  //< Jmeno strategie.
    int strategyCount;         //< Pocatecni pocet vystytu hracu s danou strategii.

    /**
    * Ziska nasledujici tah hrace.
    * @param iteration Aktualni iterace hry.
    * @return Nasledujici tah hrace ('C' => spoluprace, 'D' => zrada).
    */
    virtual E_DECISION GetNextDecision(int iteration) = 0;

    /**
    * Aktualizuje informace o probihajici hre.
    * @param iteration Aktualni iterace hry.
    * @param lastPlayerDecision Posledni tah hrace.
    * @param lastOpponentDecision Posledni tah protihrace.
    */
    virtual void UpdateGameInformation(E_DECISION lastPlayerDecision, E_DECISION lastOpponentDecision) = 0;

    /**
    * Reset nastaveni strategie.
    */
    virtual void Reset() = 0;
};

/// Trida hernich strategii zalozene na cyklickem opakovani sekvence rozhodnuti.
class PeriodicalStrategy : public Strategy
{
    std::vector<E_DECISION> decisionSequence;    //< Sekvence cyklicky opakovanych rozhodnuti hrace.

    public:
    /**
    * Konstruktor
    * @param strategyName Jmeno strategie.
    * @param strategyCount Pocatecni pocet vyskytu strategie.
    * @param decisionSequnce Sekvence cyklicky opakovanych rozhodnuti hrace.
    */
    PeriodicalStrategy(std::string strategyName, int strategyCount, std::string decisionSequenceString);

    E_DECISION GetNextDecision(int iteration);

    void UpdateGameInformation(E_DECISION lastPlayerDecision, E_DECISION lastOpponentDecision);

    void Reset();
};

/// Trida hernich strategii zalozenych na analyze predchozich tazu protihrace.
class BufferStrategy : public Strategy
{
    std::vector<E_DECISION> decisionSequence; //< Sekvence cyklicky opakovanych rozhodnuti hrace.
    int playerBufferLength;                   //< Pocet minulych tahu hrace nutnych k rozhodnuti o nasledujim tahu.
    int opponentBufferLength;                 //< Pocet minulych tahu protihrace nutnych k rozhodnuti o nasledujim tahu.
    std::vector<E_DECISION> playerBuffer;     //< Buffer tahu hrace.
    std::vector<E_DECISION> opponentBuffer;   //< Buffer tahu protihrace.

    public:
    /**
    * Konstruktor
    * @param decisionSequnce Sekvence cyklicky opakovanych rozhodnuti hrace.
    * @param strategyName Jmeno strategie.
    * @param playerBufferLenth Pocet minulych tahu hrace nutnych k rozhodnuti o nasledujim tahu.
    * @param opponentBufferLenth Pocet minulych tahu protihrace nutnych k rozhodnuti o nasledujim tahu.
    * @param decisionSequenceString Definicni predpis tahu hrace.
    */
    BufferStrategy(std::string strategyName, int strategyCount, int playerBufferLength, int opponentBufferLength, std::string decisionSequenceString);

    E_DECISION GetNextDecision(int iteration);

    void UpdateGameInformation(E_DECISION lastPlayerDecision, E_DECISION lastOpponentDecision);

    void Reset();
};

/// Trida hernich strategii zalozenych na analyze predchozich tazu protihrace.
class ProbabilisticStrategy : public Strategy
{
    E_DECISION tieDecision;   //< Rozhodnuti v pripade vyrovnaneho pomeru tahu "spoluprace x remiza" u protihrace (aplikovano i na prvni tah).
    int cooperateCount;       //< Celkovy pocet tahu "spoluprace" u protihrace.
    int defectCount;          //< Celkovy pocet tahu "zrada" u spoluhrace.

    public:
    /**
    * Konstruktor
    * @param decisionSequnce Sekvence cyklicky opakovanych rozhodnuti hrace.
    * @param strategyName Jmeno strategie.
    * @param tieDecision Rozhodnuti v pripade vyrovnaneho pomeru spoluprace x zrada u protihrace (rozhodnuti aplikovano i na prvni tah).
    */
    ProbabilisticStrategy(std::string strategyName, int strategyCount, char tieDecision);

    E_DECISION GetNextDecision(int iteration);

    void UpdateGameInformation(E_DECISION lastPlayerDecision, E_DECISION lastOpponentDecision);

    void Reset();
};

#endif // STRATEGY_H_INCLUDED
