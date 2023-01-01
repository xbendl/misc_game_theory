// strategy.cpp

#include "strategy.h"

using namespace std;

PeriodicalStrategy::PeriodicalStrategy(std::string strategyName, int strategyCount, std::string decisionSequenceString)
{
    this->strategyName = strategyName;
    this->strategyCount = strategyCount;

    for(int i = 0; i < (int)decisionSequenceString.size(); i++)
    {
        if(decisionSequenceString[i] == 'C')
            decisionSequence.push_back(D_COOPERATE);
        else if(decisionSequenceString[i] == 'R')
            decisionSequence.push_back(D_RANDOM);
        else
            decisionSequence.push_back(D_DEFECT);
    }
}

E_DECISION PeriodicalStrategy::GetNextDecision(int iteration)
{
    if(decisionSequence[iteration % decisionSequence.size()] == D_RANDOM)
        return ((rand() % 2) == 0) ? D_COOPERATE : D_DEFECT;
    return decisionSequence[iteration % decisionSequence.size()];
}

void PeriodicalStrategy::UpdateGameInformation(E_DECISION lastPlayerDecision, E_DECISION lastOpponentDecision)
{
    // neni potreba implementovat
}

void PeriodicalStrategy::Reset()
{
    // neni potreba implementovat
}

///////////////////////////////////////////////////////////////////////////////

BufferStrategy::BufferStrategy(std::string strategyName, int strategyCount, int playerBufferLength, int opponentBufferLength, std::string decisionSequenceString)
{
    playerBuffer.assign(playerBufferLength, D_COOPERATE);
    opponentBuffer.assign(opponentBufferLength, D_COOPERATE);

    this->strategyName = strategyName;
    this->strategyCount = strategyCount;
    this->playerBufferLength = playerBufferLength;
    this->opponentBufferLength = opponentBufferLength;

    for(int i = 0; i < (int)decisionSequenceString.size(); i++)
    {
        if(decisionSequenceString[i] == 'C')
            decisionSequence.push_back(D_COOPERATE);
        else if(decisionSequenceString[i] == 'R')
            decisionSequence.push_back(D_RANDOM);
        else
            decisionSequence.push_back(D_DEFECT);
    }
}

E_DECISION BufferStrategy::GetNextDecision(int iteration)
{
    if((iteration < playerBufferLength) || (iteration < opponentBufferLength))
    {   // prvni kroky jsou pevne urceny zvolenou strategii
        return decisionSequence[iteration];
    }
    else
    {   // dalsi kroky jsou urceny podle analyzy tahu protihrace
        int index = 0;
        for(int i = 0; i < playerBufferLength; i++)
        {
            index = index << 1;
            if(playerBuffer[i] == D_DEFECT)
                index = index | 1;
        }
        for(int i = 0; i < opponentBufferLength; i++)
        {
            index = index << 1;
            if(opponentBuffer[i] == D_DEFECT)
                index = index | 1;
        }
        index += (playerBufferLength > opponentBufferLength) ? playerBufferLength : opponentBufferLength;
        return decisionSequence[index];
    }
}

void BufferStrategy::UpdateGameInformation(E_DECISION lastPlayerDecision, E_DECISION lastOpponentDecision)
{
    if(playerBufferLength > 0)
    {   // Aktualizace bufferu poslednich tahu hrace
        playerBuffer.erase(playerBuffer.begin());    // Vyjmuti nejstarsiho zaznamenaneho tahu hrace
        playerBuffer.push_back(lastPlayerDecision);  // Vlozeni nejnovejsiho tahu hrace
    }
    if(opponentBufferLength > 0)
    {   // Aktualizace bufferu poslednich tahu protihrace
        opponentBuffer.erase(opponentBuffer.begin());    // Vyjmuti nejstarsiho zaznamenaneho tahu protihrace
        opponentBuffer.push_back(lastOpponentDecision);  // Vlozeni nejnovejsiho tahu protihrace
    }
}

void BufferStrategy::Reset()
{
    playerBuffer.assign(playerBufferLength, D_COOPERATE);
    opponentBuffer.assign(opponentBufferLength, D_COOPERATE);
}

///////////////////////////////////////////////////////////////////////////////

ProbabilisticStrategy::ProbabilisticStrategy(std::string strategyName, int strategyCount, char tieDecision)
{
    this->strategyName = strategyName;
    this->strategyCount = strategyCount;
    this->tieDecision = (tieDecision == 'C') ? D_COOPERATE : D_DEFECT;

    cooperateCount = 0;
    defectCount = 0;
}

E_DECISION ProbabilisticStrategy::GetNextDecision(int iteration)
{
    if(cooperateCount > defectCount)
    {
        return D_COOPERATE;
    }
    else if(cooperateCount < defectCount)
    {
        return D_DEFECT;
    }
    else
        return tieDecision;
}

void ProbabilisticStrategy::UpdateGameInformation(E_DECISION lastPlayerDecision, E_DECISION lastOpponentDecision)
{
    if(lastOpponentDecision == D_COOPERATE)
        cooperateCount++;
    else
        defectCount++;
}

void ProbabilisticStrategy::Reset()
{
    cooperateCount = 0;
    defectCount = 0;
}
