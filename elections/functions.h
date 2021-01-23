#pragma once
#include "globalFunctions.h"
#include "citizen.h"
#include "mahoz.h"
#include "dividedMahoz.h"
#include "miflaga.h"
#include "round.h"

//function that start a new round of elections
Round* SetRound();
//this function prints the main manu
void PrintMainMenu();
//function that print the interface to the user
void PrintInterface();
//a function that check user choice
void ActivateChoice(int _choice, Round& _round,bool& electionStart);
///a function that activate main interface (first one)
void activateMainInterface(Round* _round, bool& electionStart);

//a function that add a new citizen
void addNewCitizen(Round& _round);
//a function that add a new mahoz
void addNewmahoz(Round& _round);
//a function that add a new miflaga
void addNewMiflaga(Round& _round);
//a function that add a new rep to mifalga
void addNewRepToMiflaga(Round& _round);
//a function that add a new vote to votes schedule in round class
void addNewVote(Round& _round, bool& electionStart);
//calculates the results
void CalculateResults(Round& _round);
//saves the round elections to file
void SaveElectionsToFile(Round& _round, bool electionStart);
//load the round elections from file
bool LoadElectionsFromFile(Round& _round, bool& electionStart);
