#include "functions.h"

int main()
{
	Round* r1 = nullptr;
	int choice = 0;
	bool votesBegan = false, flag = false;
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
			while (r1 == nullptr)
			{
				try {
					r1 = SetRound();
				}
				catch (invalid_argument& msg)
				{
					cout << msg.what() << endl;
				}
				catch (...)
				{
					cout << "Unknown error!" << endl;
				}
			}
		}
		else
		{
			r1 = new Round();
			while (!flag)
			{
				try {
					flag = LoadElectionsFromFile(*r1, votesBegan);
				}
				catch (Load_error& msg)
				{
					cout << msg.what() << endl;
				}
				catch (...)
				{
					cout << "Problem with the file!" << endl;
				}
			}
		}

		PrintInterface();
		cin >> choice;
		while (choice != 10)
		{
			system("cls");
			if (choice >= 1 && choice <= 12)
			{
				try {
					if (r1 != nullptr)
						ActivateChoice(choice, *r1, votesBegan);
				}
				catch (Load_error& msg)
				{
					cout << msg.what() << endl;
				}
				catch (invalid_argument& msg)
				{
					cout << msg.what() << endl;
				}
				catch (...)
				{
					cout << "Unknown error!" << endl;
				}
			}
			else
				cout << "Not a valid choice! please try again." << endl;
			PrintInterface();
			cin >> choice;
		}
	}
}



