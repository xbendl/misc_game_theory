/**f
* \file evolution.h
* Modul pro evoluci populace agentu hrajici ruzne strategie s vyuzitim znalosti vyplatni matice.
* \author Bendl Jaroslav (xbendl00)
*/

#ifndef OPTIMALIZATOR_H_INCLUDED
#define OPTIMALIZATOR_H_INCLUDED

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include "payoff.h"

//< Trida pro evoluci populace agentu.
class Evolution
{
    int generationCount;   //< Delka evoluce (pocet generaci).

    public:
    /// Zaznam o uspesnosti strategie v prubehu evoluce.
    typedef struct
    {
        std::string strategyName;       //< Jmeno strategie.
        std::vector<double> strategyCount; //< Pocet vyskytu jedincu s danou strategii v jednotlivych cyklech evolucniho behu.
    } tEvolutionResult;

    /// Typ pro ulozeni vysledku evolucniho procesu pro mnozinu strategii.
    typedef std::vector<tEvolutionResult> EvolutionProcess;

    public:
    /**
    * Konstruktor.
    * @param generationCount Delka evoluce (pocet generaci).
    */
    Evolution(int generationCount);

    /**
    * Provede evoluci populace agentu.
    * @param evolutionProcess Zaznamy o vyvoji poctu hracu jednotlivych strategii behem evoluce.
    * @param payoffMatrix Vyplatni matice.
    * @param endTreshhold Prah pro ukonceni vypoctu (nemeni-li se vysledky evolucniho behu o vice nez tuto hodnotu).
    */
    void StartEvolution(EvolutionProcess &evolutionProcess, Payoff::PayoffMatrix &payoffMatrix, double endTreshhol);
};

#endif //< OPTIMALIZATOR_H_INCLUDED
