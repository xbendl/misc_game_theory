/**
* \file manager.h
* Modul pro rizeni reseni problemu opakovaneho veznova dilematu
* \author Bendl Jaroslav (xbendl00)
*/

#ifndef MANAGER_H_INCLUDED
#define MANAGER_H_INCLUDED

#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include "strategy.h"
#include "payoff.h"
#include "evolution.h"

class Manager
{
    /// Parametry a nastaveni evoluce.
    typedef struct
    {
        int matchLength;      //< Delka souboje dvou strategii.
        int generationCount;  //< Delka evoluce (pocet generaci).
        int payoff_cc;        //< Vyplata v pripade scenare: spoluprace x spoluprace.
        int payoff_cd;        //< Vyplata v pripade scenare: spoluprace x zrada.
        int payoff_dc;        //< Vyplata v pripade scenare: zrada x spoluprace.
        int payoff_dd;        //< Vyplata v pripade scenare: zrada x zrada.
        double endTreshhold;  //< Prah pro ukonceni evolucniho vypoctu (nemeni-li se stavy populaci o vice nez dany prah).
    } tParameters;

    std::vector<Strategy *> popPlayer;    //< Populace uvazovanych strategii.
    std::vector<Strategy *> popOpponent;  //< Populace uvazovanych strategii protihrace.
    std::string outputFile;               //< Cesta k vystupnimu souboru.
    tParameters parameters;               //< Zadani ulohy.
    Payoff::PayoffMatrix payoffMatrix;             //< Vyplatni matice.
    Evolution::EvolutionProcess evolutionProcess;  //< Zaznam evolucniho behu.

    /**
    * Nacte ze vstupniho souboru data o evolvovanych strategiich.
    * @param parameters Struktura pro ulozeni ziskanych parametru ulohy.
    * @param inputFile Cesta ke vstupnimu souboru s parametry ulohy.
    * @return Udava, zda-li se podarilo nacist parametry ulohy.
    */
    bool LoadData(tParameters &parameters, std::string inputFile);

    /**
    * Ulozi do vystupniho souboru obecne vysledky evolucniho behu.
    */
    bool SaveResult();

    /**
    * Ulozi do vystupniho souboru vysledky vhodne pro zobrazeni pomoci programu gnuplot.
    */
    bool SaveData();

    public:
    /**
    * Konstruktor
    * @param inputFile Cesta ke vstupnimu souboru.
    * @param outputFile Cesta k vystupnimu souboru.
    */
    Manager(std::string inputFile, std::string outputFile);

    /**
    * Destruktor.
    */
    ~Manager();

    /**
    * Pripravi vyplatni matici a spusti evoluci strategii.
    */
    void StartEvolution();
};

#endif // MANAGER_H_INCLUDED
