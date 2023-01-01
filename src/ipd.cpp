// ipd.cpp

#include "ipd.h"

using namespace std;

void printHelp()
{
    cout << "*** NAPOVEDA K PROGRAMU SIMULATORU OPAKOVANEHO VEZNOVA DILEMATU***" << endl;
    cout << "    Synopsis: ipd -i inputFile -o outputFile" << endl;
    cout << "     -i inputFile Cesta ke vstupnimu souboru." << endl;
    cout << "     -o outputFile Cesta k vystupnimu souboru." << endl;
}

int main(int argc, char *argv[])
{
    srand(time(0));

    // Zpracovani parametru prikazoveho radku
    bool inputFileFlag, outputFileFlag;
    string inputFile = "", outputFile = "";
    char c;

    while((c = getopt(argc, argv, "i:o:h")) != -1)
    {
        switch(c)
        {
            case 'i':  // Jmeno vstupniho souboru
                inputFile = string(optarg);
                inputFileFlag = true;
                break;
            case 'o':  // Jmeno vstupniho souboru
                outputFile = string(optarg);
                outputFileFlag = true;
                break;
            case 'h':  // Napoveda
                printHelp();
                return true;
            case '?':  // Neznamy parametr
                cerr << "CHYBA: Nespravne parametry prikazoveho radku. Napovedu vypisete pomoci -h" << endl;
                return -1;
            case ':':  // Nezadany povinny parametr
                cerr << "CHYBA: Nespravne parametry prikazoveho radku. Napovedu vypisete pomoci -h" << endl;
                break;
        }
    }
    if(!inputFileFlag || !outputFileFlag)
    {
        cerr << "CHYBA: Nespravne parametry prikazoveho radku. Napovedu vypisete pomoci -h" << endl;
        return false;
    }

    Manager manager(inputFile, outputFile);
    manager.StartEvolution();

    return true;
}
