// manager.cpp

#include "manager.h"

using namespace std;

Manager::Manager(std::string inputFile, std::string outputFile)
{
    // Nacteni parametru ze vstupniho souboru
    if(!LoadData(parameters, inputFile))
    {
        cerr << "Nepodarilo se otevrit vstupni soubor" << endl;
        exit(EXIT_FAILURE);
    }

    this->outputFile = outputFile;
}

Manager::~Manager()
{
    for(int i = 0; i < (int)popPlayer.size(); i++)
        delete popPlayer[i];
    for(int i = 0; i < (int)popOpponent.size(); i++)
        delete popOpponent[i];
}

void Manager::StartEvolution()
{
    // Vypocet vyplatni matice
    Payoff payoff(parameters.payoff_cc, parameters.payoff_cd, parameters.payoff_dc, parameters.payoff_dd);
    payoff.GetPayoffMatrix(popPlayer, popOpponent, payoffMatrix, parameters.matchLength);

    /*// Vypis vyplatni matice
    for(int i = 0; i < (int)payoffMatrix.size(); i++)
    {
        for(int j = 0; j < (int)payoffMatrix[i].size(); j++)
            cout << payoffMatrix[i][j] << " ";
        cout << endl;
    }*/

    // Priprava vektoru (prirazeni strategii a pocatecniho poctu vyskytu hracu strategie) pro prvni krok evolucniho vypoctu
    Evolution::tEvolutionResult evolutionResult;
    for(int i = 0; i < (int)popPlayer.size(); i++)
    {
        evolutionResult.strategyName = popPlayer[i]->strategyName;
        (evolutionResult.strategyCount).push_back((double)popPlayer[i]->strategyCount);
        evolutionProcess.push_back(evolutionResult);
        evolutionResult.strategyCount.clear();
    }

    // Provedeni evoluce
    Evolution evolution(parameters.generationCount);
    evolution.StartEvolution(evolutionProcess, payoffMatrix, parameters.endTreshhold);

    // Ulozeni vysledku
    if(!SaveResult()  || !SaveData())
    {
        cerr << "CHYBA: Nepodarilo se ulozit vysledky do vystupniho souboru." << endl;
        exit(EXIT_FAILURE);
    }
}

bool Manager::SaveResult()
{
    ofstream file;
    file.open(outputFile.c_str());

    if(!file.good())
        return false;

    file << "***** SOLUTION --- ITERATED PRISONER'S DILEMMA ****\n";
    file << "\n[PARAMERY]\n";
    file << "MATCH_LENGTH: \t" << parameters.matchLength << "\n";
    file << "GENERATION_COUNT: \t" << parameters.generationCount << "\n";
    file << "END_TRESSHOLD: \t" << parameters.endTreshhold << "\n";
    file << "PAYOFF - CC: \t" << parameters.payoff_cc << "\n";
    file << "PAYOFF - CD: \t" << parameters.payoff_cd << "\n";
    file << "PAYOFF - DC: \t" << parameters.payoff_dc << "\n";
    file << "PAYOFF - DD: \t" << parameters.payoff_dd << "\n";

    file << "\n[POPULACE - ZACATEK EVOLUCE]\n";
    for(int i = 0; i < (int)evolutionProcess.size(); i++)
        file << evolutionProcess[i].strategyName << ":\t" << (int)(evolutionProcess[i].strategyCount[0] + 0.5) <<"\n";

    file << "\n[POPULACE - KONEC EVOLUCE]\n";
    for(int i = 0; i < (int)evolutionProcess.size(); i++)
        file << evolutionProcess[i].strategyName << ":\t" << (int)(evolutionProcess[i].strategyCount[evolutionProcess[i].strategyCount.size()-1] + 0.5) <<"\n";

    file << "\n[VYPLATNI MATICE]\n";
    string whiteSpaceString;
    int helpVar;
    int helpVar2;
    whiteSpaceString.assign(10, ' ');
    file << whiteSpaceString;
    for(int i = 0; i < (int)evolutionProcess.size(); i++)
    {
        file << evolutionProcess[i].strategyName;
        whiteSpaceString.assign((10 - evolutionProcess[i].strategyName.size()), ' ');
        file << whiteSpaceString;
    }
    file << "\n";
    for(int i = 0; i < (int)payoffMatrix.size(); i++)
    {
        file << evolutionProcess[i].strategyName;
        whiteSpaceString.assign((10 - evolutionProcess[i].strategyName.size()), ' ');
        file << whiteSpaceString;

        for(int j = 0; j < (int)payoffMatrix[i].size(); j++)
        {
            // Dopocitani bilych znaku
            helpVar = (payoffMatrix[i][j] == 0) ? (payoffMatrix[i][j] + 1) : payoffMatrix[i][j];
            helpVar2 = 0;
            while((helpVar /= 10) > 0)
                helpVar2++;

            file << payoffMatrix[i][j];
            whiteSpaceString.assign((10 - helpVar2), ' ');
            file << whiteSpaceString;
        }
        file << "\n";
    }

    file.close();

    return true;
}

bool Manager::SaveData()
{
    string outputGnuplotDataFile = outputFile;  // cesta k souboru s daty pro gnuplot
    string outputGnuplotCmdFile = outputFile;   // cesta k souboru k prikazy pro zpracovani dat pro gnuplot
    string outputPostscriptFile = outputFile;   // cesta k postscript souboru s vygenerovanym grafem

    // Urceni nazvy pro datovy gnuplot soubor a ridici gnuplot soubor
    if(outputFile.find_last_of('.') != string::npos)
    {
        outputGnuplotDataFile = outputFile.substr(0, outputFile.find_last_of('.'));
        outputGnuplotCmdFile = outputFile.substr(0, outputFile.find_last_of('.'));
        outputPostscriptFile = outputFile.substr(0, outputFile.find_last_of('.'));
    }
    outputGnuplotDataFile += ".dat";
    outputGnuplotCmdFile += ".cmd";
    outputPostscriptFile += ".ps";

    // Gnuplot data soubor
    ofstream file;
    file.open(outputGnuplotDataFile.c_str());

    if(!file.good())
        return false;

    for(int i = 0; i < (int)evolutionProcess.size(); i++)
    {
        file << evolutionProcess[i].strategyName << " ";
    }
    file << "\n";
    for(int i = 0; i < (int)evolutionProcess[0].strategyCount.size(); i++)
    {
        file << (i + 1) << " ";

        for(int j = 0; j < (int)evolutionProcess.size(); j++)
            file << evolutionProcess[j].strategyCount[i] << " ";
        file << "\n";
    }
    file.close();

    // Gnuplot ridici soubor
    file.open(outputGnuplotCmdFile.c_str());

    if(!file.good())
        return false;

    file << "set grid" << "\n";
    file << "set key right top" << "\n";
    //file << "set linestyle linewidth 3" << "\n";
    file << "set xlabel \"Generace\"\n";
    file << "set ylabel \"Pocet jedincu\"\n";
    file << "set data style lines " << "\n";
    file << "plot ";
    for(int i = 0; i < (int)evolutionProcess.size(); i++)
    {
        file << "\t\'" << outputGnuplotDataFile << "\' using " << "1:" << (i + 2) << " title \"" << evolutionProcess[i].strategyName <<  "\"";
        if(i != ((int)evolutionProcess.size() - 1))
            file << " with lines lw 2, \\\n";
        else
            file << " with lines lw 2\n";
    }
    file << "pause -1 \"Pro ukonceni stisknete >enter<\"\n";
    //file << "set terminal postscript eps monochrome\n";
    file << "set terminal postscript eps color solid\n";
    file << "set output \"" << outputPostscriptFile << "\"\n";
    file << "replot";

    file.close();

    // Vykresleni grafu v gnuplotu
    string command = "gnuplot " + outputGnuplotCmdFile;
    system(command.c_str());

    cout << "> Vysledky byly ulozeny do souboru: " << outputFile << endl;
    cout << "> Graf byl ulozen do souboru: " << outputPostscriptFile << endl;

    return true;
}

bool Manager::LoadData(tParameters &parameters, std::string inputFile)
{
    // Otevreni souboru
    ifstream inFile;
    inFile.open(inputFile.c_str(), std::ifstream::in);
    if(inFile.fail())
    {   // Konfiguracni soubor se nepodarilo otevrit
        return false;
    }

    // Ziskani promennych z konfiguracniho souboru
    string line, variableName, strategyName;
    int strategyCount;
    //E_STRATEGY  strategy;

    while(getline(inFile, line))
    {   // prochazeni vstupniho souboru
        if((line.size() > 0) && (isalpha(line[0])) && (line.find('=') != string::npos))
        {   // filtrovani radku s parametry
            variableName = line.substr(0, line.find('='));
            variableName.erase(variableName.find_last_not_of(' ') + 1);
            variableName.erase(0, variableName.find_first_not_of(' '));

            if((variableName == "MATCH_LENGTH") && (line.find('=') != string::npos))
                parameters.matchLength = atoi(line.substr(line.find('=') + 1).c_str());
            if((variableName == "GENERATION_COUNT") && (line.find('=') != string::npos))
                parameters.generationCount = atoi(line.substr(line.find('=') + 1).c_str());
            if((variableName == "PAYOFF_CC") && (line.find('=') != string::npos))
                parameters.payoff_cc = atoi(line.substr(line.find('=') + 1).c_str());
            if((variableName == "PAYOFF_CD") && (line.find('=') != string::npos))
                parameters.payoff_cd = atoi(line.substr(line.find('=') + 1).c_str());
            if((variableName == "PAYOFF_DC") && (line.find('=') != string::npos))
                parameters.payoff_dc = atoi(line.substr(line.find('=') + 1).c_str());
            if((variableName == "PAYOFF_DD") && (line.find('=') != string::npos))
                parameters.payoff_dd = atoi(line.substr(line.find('=') + 1).c_str());
            if((variableName == "END_TRESHHOLD") && (line.find('=') != string::npos))
                parameters.endTreshhold = atof(line.substr(line.find('=') + 1).c_str());
        }
        else if(line.find(';') != string::npos)
        {   // filtrovani radku se strategiemi a jejich cetnosti
            strategyName = line.substr(0, line.find(';'));                  // ziskani nazvu strategie
            strategyCount = atoi(line.substr(line.find(';') + 1).c_str());  // ziskani poctu hracu s danou strategii

            // Periodicke strategie
            if(strategyName == "ALL_C")
            {
                popPlayer.push_back((Strategy *) new PeriodicalStrategy(strategyName, strategyCount, "C"));
                popOpponent.push_back((Strategy *) new PeriodicalStrategy(strategyName, strategyCount, "C"));
            }
            else if(strategyName == "ALL_D")
            {
                popPlayer.push_back((Strategy *) new PeriodicalStrategy(strategyName, strategyCount, "D"));
                popOpponent.push_back((Strategy *) new PeriodicalStrategy(strategyName, strategyCount, "D"));
            }
            else if(strategyName == "PER_CD")
            {
                popPlayer.push_back((Strategy *) new PeriodicalStrategy(strategyName, strategyCount, "CD"));
                popOpponent.push_back((Strategy *) new PeriodicalStrategy(strategyName, strategyCount, "CD"));
            }
            else if(strategyName == "PER_CCD")
            {
                popPlayer.push_back((Strategy *) new PeriodicalStrategy(strategyName, strategyCount, "CCD"));
                popOpponent.push_back((Strategy *) new PeriodicalStrategy(strategyName, strategyCount, "CCD"));
            }
            else if(strategyName == "PER_DDC")
            {
                popPlayer.push_back((Strategy *) new PeriodicalStrategy(strategyName, strategyCount, "DDC"));
                popOpponent.push_back((Strategy *) new PeriodicalStrategy(strategyName, strategyCount, "DDC"));
            }
            else if(strategyName == "RANDOM")
            {
                popPlayer.push_back((Strategy *) new PeriodicalStrategy(strategyName, strategyCount, "R"));
                popOpponent.push_back((Strategy *) new PeriodicalStrategy(strategyName, strategyCount, "R"));
            }

            // Historicke strategie
            else if(strategyName == "TFT")
            {
                popPlayer.push_back((Strategy *) new BufferStrategy(strategyName, strategyCount, 1, 1, "CCDCD"));
                popOpponent.push_back((Strategy *) new BufferStrategy(strategyName, strategyCount, 1, 1, "CCDCD"));
            }
            else if(strategyName == "TF2T")
            {
                popPlayer.push_back((Strategy *) new BufferStrategy(strategyName, strategyCount, 1, 2, "CCCCCDCCCD"));
                popOpponent.push_back((Strategy *) new BufferStrategy(strategyName, strategyCount, 1, 2, "CCCCCDCCCD"));
            }
            else if(strategyName == "SPITEFUL")
            {
                popPlayer.push_back((Strategy *) new BufferStrategy(strategyName, strategyCount, 1, 1, "CCDDD"));
                popOpponent.push_back((Strategy *) new BufferStrategy(strategyName, strategyCount, 1, 1, "CCDDD"));
            }
            else if(strategyName == "MISTRUST")
            {
                popPlayer.push_back((Strategy *) new BufferStrategy(strategyName, strategyCount, 1, 1, "DCDCD"));
                popOpponent.push_back((Strategy *) new BufferStrategy(strategyName, strategyCount, 1, 1, "DCDCD"));
            }
            else if(strategyName == "PAVLOV")
            {
                popPlayer.push_back((Strategy *) new BufferStrategy(strategyName, strategyCount, 1, 1, "CCDDC"));
                popOpponent.push_back((Strategy *) new BufferStrategy(strategyName, strategyCount, 1, 1, "CCDDC"));
            }
            else if(strategyName == "SLOW_TFT")
            {
                popPlayer.push_back((Strategy *) new BufferStrategy(strategyName, strategyCount, 1, 2, "CCCCCDCDDD"));
                popOpponent.push_back((Strategy *) new BufferStrategy(strategyName, strategyCount, 1, 2, "CCCCCDCDDD"));
            }
            else if(strategyName == "HARD_TFT")
            {
                popPlayer.push_back((Strategy *) new BufferStrategy(strategyName, strategyCount, 1, 2, "CCCDDDCDDD"));
                popOpponent.push_back((Strategy *) new BufferStrategy(strategyName, strategyCount, 1, 2, "CCCDDDCDDD"));
            }

            // Pravdepodobnostni strategie
            else if(strategyName == "SOFT_MAJO")
            {
                popPlayer.push_back((Strategy *) new ProbabilisticStrategy(strategyName, strategyCount, 'C'));
                popOpponent.push_back((Strategy *) new ProbabilisticStrategy(strategyName, strategyCount, 'C'));
            }
            else if(strategyName == "HARD_MAJO")
            {
                popPlayer.push_back((Strategy *) new ProbabilisticStrategy(strategyName, strategyCount, 'D'));
                popOpponent.push_back((Strategy *) new ProbabilisticStrategy(strategyName, strategyCount, 'D'));
            }
        }
    }
    return true;
}
