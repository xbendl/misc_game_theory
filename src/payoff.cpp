// ipd.cpp

#include "payoff.h"

using namespace std;

Payoff::Payoff(int payoff_cc, int payoff_cd, int payoff_dc, int payoff_dd)
{
    this->payoff_cc = payoff_cc;
    this->payoff_cd = payoff_cd;
    this->payoff_dc = payoff_dc;
    this->payoff_dd = payoff_dd;
}

void Payoff::GetPayoffMatrix(std::vector<Strategy *> &popPlayer, std::vector<Strategy *> &popOpponent, PayoffMatrix &payoffMatrix, int matchLength)
{
    this->matchLength = matchLength;

    // Inicializace vyplatni matice
    PayoffMatrixRow payoffMatrixRow;
    payoffMatrixRow.assign(popPlayer.size(), 0);
    payoffMatrix.assign(popPlayer.size(), payoffMatrixRow);

    // Vyplneni vyplatni matice
    for(int i = 0; i < (int)popPlayer.size(); i++)
    {
        payoffMatrixRow.clear();
        for(int j = 0; j <= i; j++)
        {
            popPlayer[i]->Reset();                  // reset nastaveni strategii vybranych hracu
            popOpponent[j]->Reset();

            Match(popPlayer[i], popOpponent[j]);    // realizace souboje dvou hracu - strategii

            payoffMatrix[i][j] = playerScore;       // nastaveni vyplaty hrace

            if(i != j)
                payoffMatrix[j][i] = opponentScore; // nastaveni vyplaty oponenta
        }
    }
}

void Payoff::Match(Strategy *playerStrategy, Strategy *opponentStrategy)
{
    std::vector<E_DECISION> gameRecordPlayer;    // zaznam tahu hrace.
    std::vector<E_DECISION> gameRecordOpponent;  // zaznam tahu protihrace.

    E_DECISION playerDecision, opponentDecision;

    playerScore = 0;
    opponentScore = 0;

    // Realizace souboje (pocet tahu kazdeho hrace v ramci souboje = matchLength)
    for(int i = 0; i < matchLength; i++)
    {
        // Ziskani tahu hrace i oponenta
        playerDecision = playerStrategy->GetNextDecision(i);
        opponentDecision = opponentStrategy->GetNextDecision(i);

        // Zaznamenani tahu hrace i oponenta
        gameRecordPlayer.push_back(playerDecision);
        gameRecordOpponent.push_back(opponentDecision);

        // Prubezne ukladani score zapasu
        playerScore += GetPayoffRound(playerDecision, opponentDecision);
        opponentScore += GetPayoffRound(opponentDecision, playerDecision);

        // Aktualizace strategie dle soucasnych tahu (ma vyznam pouze u nekterych strategii)
        playerStrategy->UpdateGameInformation(playerDecision, opponentDecision);
        opponentStrategy->UpdateGameInformation(opponentDecision, playerDecision);
    }
}

int Payoff::GetPayoffRound(E_DECISION playerDecision, E_DECISION opponentDecision)
{
    if((playerDecision == D_COOPERATE) && (opponentDecision == D_COOPERATE))     // spoluprace x spoluprace
        return payoff_cc;
    else if((playerDecision == D_COOPERATE) && (opponentDecision == D_DEFECT))   // spoluprace x zrada
        return payoff_cd;
    else if((playerDecision == D_DEFECT) && (opponentDecision == D_COOPERATE))   // zrada x spoluprace
        return payoff_dc;
    else                                                                         // zrada x zrada
        return payoff_dd;
}
