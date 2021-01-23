#include "functions.h"

int main()
{
	Round* r1;
	int choice = 0;
	bool votesBegan = false;
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
			try {
				r1 = SetRound();
			}
			catch (const char* msg)
			{
				cout << msg << endl;
				r1 = SetRound();
			}
		}
		else
		{
			r1 = new Round();
			try {
				LoadElectionsFromFile(*r1, votesBegan);
			}
			catch (Load_error& msg)
			{
				cout << msg.what() << endl;
				activateMainInterface(r1, votesBegan);
			}
			catch (...)
			{
				cout << "Problem with the file!" << endl;
				activateMainInterface(r1, votesBegan);
			}
		}

		PrintInterface();
		cin >> choice;
		while (choice != 10)
		{
			system("cls");
			if (choice >= 1 && choice <= 12)
			{
				if (r1 != nullptr)
					ActivateChoice(choice, *r1, votesBegan);
			}
			else
				cout << "Not a valid choice! please try again." << endl;
			PrintInterface();
			cin >> choice;
		}
	}
}



