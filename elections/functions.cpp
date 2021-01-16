#include "functions.h"
#define rcastcc reinterpret_cast<const char*>
#define rcastc reinterpret_cast<char*>

void PrintMainMenu()
{
	cout << "Please enter the number of your choice:" << endl;
	cout << "1. Start a new round" << endl;
	cout << "2. Load existed round" << endl;
	cout << "3. QUIT" << endl;
}

Round* SetRound()
{
	bool stop = false;
	Round* res;
	int _day, _month, _year, _isSimpleRound;
	do 
	{
		cout << "Please choose the round type: (1) for regular (2) for simple" << endl;
		cin >> _isSimpleRound;
		cout << "Please enter the elections date:" << endl;
		cout << "day: ";
		cin >> _day;
		cout << "month: ";
		cin >> _month;
		cout << "year: ";
		cin >> _year;
		system("cls");
		if (_day > 32 || _day <= 0 || _month > 12 || _month <= 0 || _year < 2020 || _year>2100 || _isSimpleRound > 2 || _isSimpleRound < 1)
		{
			if (_isSimpleRound > 2 || _isSimpleRound < 1)
				cout << "Round type is invalid,Please try again" << endl;
			else
				cout << "Date is invalid,Please try again" << endl;
		}
		else
			stop = true;
		
	} while (!stop);
	res = new Round(_day, _month, _year, _isSimpleRound - 1);
	if (_isSimpleRound - 1)
	{
		addNewmahoz(*res);
	}

	system("cls");
	return res;
}

void PrintInterface()
{
	cout << "Please enter the number of your choice:" << endl;
	cout << "1. Add mahoz" << endl;
	cout << "2. Add citizen" << endl;
	cout << "3. Add miflaga" << endl;
	cout << "4. Add citizen to miflaga" << endl;
	cout << "5. Print all mahoz" << endl;
	cout << "6. Print all citizens" << endl;
	cout << "7. Print all miflaga" << endl;
	cout << "8. Vote" << endl;
	cout << "9. Elections results" << endl;
	cout << "10. QUIT" << endl << endl << endl;
	cout << "11. Save elections to File" << endl;
	cout << "12. Load elections From File" << endl;
}

void activateMainInterface(Round* _round,bool& electionStart)
{

	int choice = 0;
	PrintMainMenu();
	cin >> choice;
	while (choice < 1 || choice>3)
	{
		system("cls");
		cout << "Not a valid choice! please try again." << endl;
		PrintMainMenu();
		cin >> choice;
	}
	if (choice != 3)
	{
		system("cls");
		if (choice == 1)
		{
			delete _round;
			_round = SetRound();
		}
		else
		{
			LoadElectionsFromFile(*_round, electionStart, 1);
		}
	}



}
void ActivateChoice(int _choice, Round& _round,bool& electionStart)
{
	switch (_choice)
	{
	case 1: //add mahoz
		if (electionStart)
			cout << "Elections already started, CAN'T ADD MAHOZ" << endl;
		else if(_round.getIsSimpleRound())
			cout << "These are simple elections, CAN'T ADD MAHOZ" << endl;
		else
			addNewmahoz(_round);
		break;

	case 2: //add citizen
		addNewCitizen(_round);
		break;

	case 3: //add miflaga
		if (electionStart)
			cout << "Elections already started,CANT ADD MIFLAGA" << endl;
		else
			addNewMiflaga(_round);

		break;

	case 4: //add citizen to miflaga
		addNewRepToMiflaga(_round);
		break;

	case 5: //print all mahoz
		_round.getAllMahoz().printAll();
		break; 

	case 6: //print all citizen
		_round.getAllCitizen().printAll();
		break;

	case 7: //print all miflaga
		_round.getAllMiflaga().printAll();
		break;

	case 8: //vote
		int numOfMiflaga, numOfMahoz;
		numOfMiflaga = _round.getAllMiflaga().getlenght();
		numOfMahoz = _round.getAllMahoz().getlenght();
		if (numOfMahoz == 0 || numOfMiflaga==0)
			cout << "NO DETAILS ENTERED YET,CANT START votes!!" << endl;
		else
			addNewVote(_round, electionStart);
		break;

	case 9: //elections results
		CalculateResults(_round);
		break;

	case 11: //save round
		SaveElectionsToFile(_round, electionStart);
		break;

	case 12: //load round
		LoadElectionsFromFile(_round, electionStart,2);
		break;
	}
}

void addNewCitizen(Round& _round)
{
	string name;
	mahoz* mahozptr;
	int id, year, mahozNum;
	citizen* cit;
	cout << "Citizen name: ";
	cin >> name;
	cout << "ID: ";
	cin >> id;
	cout << "Year of birth: ";
	cin >> year;
	cout << "Mahoz number: ";
	cin >> mahozNum;
	if (_round.getIsSimpleRound())
		mahozNum = 1;
	mahozptr = _round.getAllMahoz().getObjectPtr(mahozNum);
	cit = _round.getAllCitizen().getObjectPtr(id);
	if (cit == nullptr && mahozptr != nullptr) ///citizen already exist
	{
		_round.AddCitizenToArray(name, id, year, mahozNum);//added the citizen the all citizens array

		//adding the citizen to citizens that can vote in specific mahoz
		mahozptr->getCanVoteCit().addptr(new citizen(name, id, year, mahozptr));
		mahozptr->addCitizenNum();
		cout << "Citizen been added SUCCESSFULLY" << endl;
	}
	else if (mahozptr != nullptr)
		cout << "could not add the citizen,ALREADY EXIST" << endl;
	else
		cout << "could not add the citizen,MAHOZ DOESNT EXIST" << endl;
}

void addNewmahoz(Round& _round)
{

	int dividedChoice;
	bool dividedFlag = false;
	string name;
	int numOfRep;
	if (_round.getIsSimpleRound())
	{
		dividedChoice = 1;
		dividedFlag = true;
		name = "Elections State";
	}
	else
	{
		cout << "choose mahoz type (1)divided mahoz (2)Standard mahoz: ";
		cin >> dividedChoice;
		cout << "Mahoz name: ";
		cin >> name;
		if (dividedChoice == 1)
			dividedFlag = true;
	}
	cout << "Number of representives: ";
	cin >> numOfRep;

	if (dividedChoice == 1 || dividedChoice == 2)
	{
		if (numOfRep <= 0)
			cout << "INVALID REPRESENTIVES NUMBER!" << endl;
		else
		{
			_round.AddMahozToArray(name, numOfRep,dividedFlag);
			cout << "mahoz been added SUCCESSFULY" << endl;
		}
	}
	else
		cout << "INVALID MAHOZ TYPE!" << endl;
}
void addNewMiflaga(Round& _round)
{
	string name;
	mahoz* mahozptr;
	citizen* cit;
	int id;
	cout << "Miflaga name: ";
	cin >> name;
	cout << "Head of miflaga id: ";
	cin >> id;
	cit = _round.getAllCitizen().getObjectPtr(id);
	if (cit != nullptr) ///head of miflaga exist
	{
		_round.AddMiflagaToArray(name, id);
		cout << "Miflaga been added SUCCESSFULY" << endl;
	}
	else
		cout << "COULD NOT add miflaga, WRONG ID" << endl;
}

void addNewRepToMiflaga(Round& _round)
{
	int id, miflagaSerial, mahozNum;
	miflaga* mif;
	citizen* cit;
	mahoz* mahozptr;
	cout << "representive ID: ";
	cin >> id;
	cout << "miflaga Serial Number: ";
	cin >> miflagaSerial;
	cout << "mahoz SerialNumber: ";
	cin >> mahozNum;
	if (_round.getIsSimpleRound())
		mahozNum = 1;
	cit = _round.getAllCitizen().getObjectPtr(id);
	if (cit == nullptr)
		cout << "Could not add citizen,CITIZEN DOES NOT EXIST" << endl;
	else
	{
		mif = _round.getAllMiflaga().getObjectPtr(miflagaSerial);
		if (mif == nullptr)
			cout << "Could not add citizen,MIFLAGA DOES NOT EXIST" << endl;
		else
		{
			mahozptr = _round.getAllMahoz().getObjectPtr(mahozNum);
			if (mahozptr != nullptr)
			{
				mif->getList().addRepToMahoz(cit, mahozNum);
				cout << "representative been added SUCCESSFULY" << endl;
			}
			else
				cout << "COULD NOT ADD representative,MAHOZ DOES NOT EXIST" << endl;

		}
	}
}
void addNewVote(Round& _round, bool& electionStart)
{
	int id, miflagaSerial;
	citizen* citglobal, * cit;
	miflaga* mif;
	mahoz* mahozptr;

	cout << "citizen ID: ";
	cin >> id;
	cout << "miflaga Serial Number: ";
	cin >> miflagaSerial;
	citglobal = _round.getAllCitizen().getObjectPtr(id);
	mif = _round.getAllMiflaga().getObjectPtr(miflagaSerial);
	if (citglobal == nullptr)
		cout << "Citizen ID IS INVALID" << endl;
	else if (mif == nullptr)
		cout << "Miflaga DOESNT EXIST" << endl;
	else
	{
		mahozptr = const_cast<mahoz*>(citglobal->getMahoz());
		cit = mahozptr->getCanVoteCit().getObjectPtr(id);
		if (cit != nullptr)//citizen can vote
		{
			if (electionStart == false)///checking if this is the first vote
			{///election started
				_round.StartNewVotesSchedule();
				electionStart = true;///to block adding more miflaga/mahoz add
			}
			_round.vote(mahozptr->getID(), miflagaSerial);
			mahozptr->addVotersNum();
			mahozptr->getCanVoteCit().removeptr(cit);
			cout << "Citizen vote has been SAVED" << endl;
		}
		else ///citizen already voted
			cout << "citizen ALREADY VOTED" << endl;
	}
}

void CalculateResults(Round& _round)
{
	mahoz* mahozptr;
	float votersPrecent;
	int numOfMiflaga = _round.getAllMiflaga().getlenght();
	int numOfMahoz = _round.getAllMahoz().getlenght();
	_round.InitResults();
	_round.setRepList();
	_round.setResults();
	cout << "Elections date: " << _round.getDay() << "/" << _round.getMonth() << "/" << _round.getYear() << endl;
	for (int i = 0; i < numOfMahoz; i++)
	{
		mahozptr = _round.getAllMahoz().getObjectPtr(i + 1);
		cout << endl << *(mahozptr) << endl;
		if (typeid(*mahozptr) == typeid(mahoz))
		{
			cout << "winner in this mahoz: " << endl << *(_round.getAllMiflaga().getObjectPtr(mahozptr->getWinnerIndx() + 1)->getHeadOfMiflaga()) << endl;
		}
		else
		{
			dividedMahoz* dMahoz = static_cast<dividedMahoz*>(mahozptr);//casting to divided mahoz
			_round.PrintDevidedResults(dMahoz);
		}
		cout << "The representives from all parties of the mahoz:" << endl;
		mahozptr->getElectedCit().printAll();

		cout << "all votes: " << endl;
		_round.PrintVotesSchedule(i);

		votersPrecent = static_cast<float>(mahozptr->getNumOfVoters()) / static_cast<float>(mahozptr->getNumOfCitizen()) * 100;
		cout << "% voters in this mahoz: " << votersPrecent << endl;
	}
	cout << endl << "Heads of miflagot by rank: " << endl;
	_round.PrintResults();
	cout << endl;
}

void SaveElectionsToFile(Round& _round, bool electionStart)
{
	string fileName;
	cout << "Enter File Name: ";
	cin >> fileName;
	ofstream outFile(fileName, ios::binary|ios::trunc);
	if (!outFile)
		cout << "Could not Open the FILE" << endl;
	else
	{
		outFile.write(rcastcc(&electionStart), sizeof(electionStart));
		_round.Save(outFile);
		cout << "Election been saved SUCCESSFULY" << endl;
	}
}

void LoadElectionsFromFile(Round& _round, bool& electionStart,int whoCalled)
{
	string fileName;
	cout << "Enter File Name: ";
	cin >> fileName;
	ifstream inFile(fileName, ios::binary);
	if (!inFile)
	{
		cout << "Could not Open the FILE" << endl;
		if (whoCalled == 1)
			activateMainInterface(&_round,electionStart);
	}
	else
	{
		inFile.read(rcastc(&electionStart), sizeof(electionStart));
		_round.Load(inFile);
		cout << "Elections been Loaded SUCCESSFULY" << endl;
	}
}