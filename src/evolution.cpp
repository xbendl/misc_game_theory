// evolution.cpp

#include "evolution.h"

using namespace std;

Evolution::Evolution(int generationCount)
{
    this->generationCount = generationCount;
}

void Evolution::StartEvolution(EvolutionProcess &evolutionProcess, Payoff::PayoffMatrix &payoffMatrix, double endTreshhold)
{
    vector<int> helpVector;
    int helpVar = 0;
    double playerCount = 0;     // celkovy pocet hracu
    double newPlayerCount = 0;  // pomocna promenna pro novy vypocet noveho poctu hracu dane strategie
    double divisionCoeff = 0;   // pomocny koeficient pro ziskani noveho poctu hracu dane strategie

    // Zjisteni celkoveho poctu hracu
    for(int i = 0; i < (int)evolutionProcess.size(); i++)
        playerCount += (double)evolutionProcess[i].strategyCount[0];

    // Evolucni cyklus
    for(int i = 1; i < generationCount; i++)
    {
        helpVector.clear();
        for(int j = 0; j < (int)evolutionProcess.size(); j++)
        {
            helpVar = 0;
            for(int k = 0; k < (int)evolutionProcess.size(); k++)
            {
                if(j != k)
                    helpVar += evolutionProcess[k].strategyCount[i-1] * payoffMatrix[j][k];
                else
                    helpVar += (evolutionProcess[k].strategyCount[i-1] - 1) * payoffMatrix[j][k];
            }
            helpVector.push_back(helpVar);
        }

        // Vypocet koeficientu pro prepocet poctu hracu
        helpVar = 0;
        for(int j = 0; j < (int)evolutionProcess.size(); j++)
            helpVar += (evolutionProcess[j].strategyCount[i-1] * helpVector[j]);
        divisionCoeff = ((double)helpVar) / ((double) playerCount);

        // Prepocitani poctu hracu s danou strategii
        for(int j = 0; j < (int)evolutionProcess.size(); j++)
        {
            newPlayerCount = (helpVector[j] * evolutionProcess[j].strategyCount[i-1] / divisionCoeff);
            (evolutionProcess[j].strategyCount).push_back(newPlayerCount);
            //cout << i << " " << evolutionProcess[j].strategyName << " = " << newPlayerCount << endl;
        }

        // Ukoncovaci test
        bool isOk = true;
        for(int j = 0; j < (int)evolutionProcess.size(); j++)
        {   // Rozdil mezi zastoupenim jedincu v populaci je kontrolovan pro kazdou strategii
            if(abs(evolutionProcess[j].strategyCount[evolutionProcess[j].strategyCount.size()-1] - evolutionProcess[j].strategyCount[evolutionProcess[j].strategyCount.size()-2]) > endTreshhold)
            {   // Rozdil mezi zastoupenim jedincu strategie v populaci nesmi prekrocit zadany prah
                isOk = false;
                break;
            }
        }
        if(isOk)
            break;
    }
}
