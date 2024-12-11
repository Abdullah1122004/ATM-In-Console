#include<iostream>
#include<vector>
#include<string>
#include<fstream>

using namespace std;

const string ClientsFileName = "Clients.txt";

void Login();
void AtmMainMenuScreen();
void QuickWithdrawScreen();
void NormalWithdrawScreen();

void LoginScreen()
{
	cout << "\n_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _  \n";
	cout << "\n\tLogin Screen";
	cout << "\n_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _  \n";
}

struct stClient
{
	string AccountNumber = "";
	string PinCode = "";
	string Name = "";
	string Phone = "";
	double AccountBalance = 0;
	bool MarkForDelet = false;
};

 stClient CurrentClient;

vector <string> SplitString(string Line, string Delim)
{
	vector <string> vClient;

	short pos = 0;
	string word = "";

	while ((pos = Line.find(Delim)) != std::string::npos)
	{
		word = Line.substr(0, pos);

		if (word != "")
		{
			vClient.push_back(word);
		}

		Line.erase(0, pos + Delim.length());
	}

	if (Line != "")
	{
		vClient.push_back(Line);
	}

	return vClient;
}

stClient ConvertLineToRecord(string Line, string Seprator = "#//#")
{
	vector <string> vClient = SplitString(Line, Seprator);

	stClient Client;

	Client.AccountNumber = vClient[0];
	Client.PinCode = vClient[1];
	Client.Name = vClient[2];
	Client.Phone = vClient[3];
	Client.AccountBalance = stod(vClient[4]);

	return Client;
}

vector <stClient> LoadClientsDataFromFile(string ClientsFileName)
{
	vector <stClient> vClients;

	fstream ClientsFile;

	ClientsFile.open(ClientsFileName, ios::in);

	if (ClientsFile.is_open())
	{
		string Line;
		stClient Client;

		while (getline(ClientsFile, Line))
		{
			Client = ConvertLineToRecord(Line);

			vClients.push_back(Client);
		}

		ClientsFile.close();
	}

	return vClients;
}

bool ClientExistsByAccountNubmerAndPinCode(string AccountNumber, string
	PinCode, stClient& CurrentClient)
{
	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);

	for (stClient C : vClients)
	{
		if (C.AccountNumber == AccountNumber && C.PinCode == PinCode)
		{
			CurrentClient = C;
			return true;
		}
	}

	return false;
}

enum enAtmMenuOptions {QuickWithdraw = 1, NormalWithdraw = 2, 
Deposit = 3, CheckBalance = 4, Logout = 5};

short ReadAtmMenuOption()
{
	short Number = 0;

	cout << "Choose what do you want to do? [1 to 5]? ";
	cin >> Number;

	return Number;
}

void ShowCheckBalanceScreen()
{
	cout << "\n====================================\n";
	cout << "\tCheck Balance Screen";
	cout << "\n====================================\n";

	cout << "\n Your Balance Is " << CurrentClient.AccountBalance << endl;
}

void GoBakeToAtmMenuScreen()
{
	cout << "\nPress any key to go back to main menu...";
	system("pause > 0");

	AtmMainMenuScreen();
}

string ConvertRecordToLine(stClient Client, string Delim = "#//#")
{
	string line = "";

	line += Client.AccountNumber + Delim;
	line += Client.PinCode + Delim;
	line += Client.Name + Delim;
	line += Client.Phone + Delim;
	line += to_string(Client.AccountBalance);

	return line;
}

vector <stClient> LoadClientsDataToFile(string ClientsFileName,
	vector <stClient>& vClients)
{
	fstream ClientsFile;

	ClientsFile.open(ClientsFileName, ios::out);

	if (ClientsFile.is_open())
	{
		string Line;

		for (stClient C : vClients)
		{
			if (C.MarkForDelet == false)
			{
				Line = ConvertRecordToLine(C);

				ClientsFile << Line << endl;
			}
		}

		ClientsFile.close();
	}

	return vClients;

}

short ReadQuickWithdrawOption()
{
	short Choice = 0;

	while (Choice < 1 || Choice > 9)
	{
		cout << "\nChoose what to do from [1] to [9]? ";
		cin >> Choice;
	}

	return Choice;
}

short GetQuickWithDrawAmount(short QuickWithDrawOption)
{
	switch (QuickWithDrawOption)
	{
	case 1:
		return 20;
	case 2:
		return 50;
	case 3:
		return 100;
	case 4:
		return 200;
	case 5:
		return 400;
	case 6:
		return 600;
	case 7:
		return 800;
	case 8:
		return 1000;
	default:
		return 0;
	}
}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double
	Amount, vector <stClient>& vClients)
{
	char Answer = 'n';

	cout << "\n\nAre ou sure you want perfrom this transaction? y/n? ";
	cin >> Answer;

	if (Answer == 'y' || Answer == 'Y')
	{
		for (stClient& C : vClients)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C.AccountBalance += Amount;
				LoadClientsDataToFile(ClientsFileName, vClients);
				cout << "\n\nDpne Successfully. New balance is: " <<
					C.AccountBalance;

				return true;
			}
		}

		return false;
	}
}

void PerformQuickWithdrawOption(short QuickWithDrawOption)
{
	if (QuickWithDrawOption == 9)//Exist
		return;

	short WithDrawBalance = GetQuickWithDrawAmount(QuickWithDrawOption);

	if (WithDrawBalance > CurrentClient.AccountBalance)
	{
		
		cout << "\nThe amount exceeds your balance, make another choice.\n";
		cout << "Prees Anykey to continue...";
		system("pause > 0");
		system("cls");
		QuickWithdrawScreen();
		return;
	}

	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);

	DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber,
		WithDrawBalance * -1, vClients);

	CurrentClient.AccountBalance -= WithDrawBalance;
}

void QuickWithdrawScreen()
{
	cout << "\n= = = = = = = = = = = = = = = = = = \n";
	cout << "\tQick Withdraw";
	cout << "\n= = = = = = = = = = = = = = = = = = \n";

	cout << "\t[1] 20\t[2] 50\n";
	cout << "\t[3] 100\t[4] 200\n";
	cout << "\t[5] 400\t[6] 600\n";
	cout << "\t[7] 800\t[8] 1000\n";
	cout << "\t[9] Exist";

	cout << "\n= = = = = = = = = = = = = = = = = = \n";

	cout << "\nYour Balance Is " << CurrentClient.AccountBalance << endl;

	PerformQuickWithdrawOption(ReadQuickWithdrawOption());
}

int ReadNormalWithDrawAmount()
{
	int Amount = 0;

	cout << "\nEnter an amount mulitibl of 5's?	";
	cin >> Amount;

	if (Amount % 5 != 0)
	{
		cout << "\nEnter an amount mulitibl of 5's?	";
		cin >> Amount;
	}

	return Amount;
}

void PerformNormalWithdrawOption()
{
	int NormalWithdrawAmount = ReadNormalWithDrawAmount();


	if (NormalWithdrawAmount > CurrentClient.AccountBalance)
	{
		cout << "\nThe amount exceeds your balance, make another choice.\n";
		cout << "Prees Anykey to continue...";
		system("pause > 0");
		NormalWithdrawScreen();
		return;
	}

	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);

	DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber,
		NormalWithdrawAmount * -1, vClients);

	CurrentClient.AccountBalance -= NormalWithdrawAmount;
}

void NormalWithdrawScreen()
{

	cout << "\n= = = = = = = = = = = = = = = = = = \n";
	cout << "\tNormal Withdraw";
	cout << "\n= = = = = = = = = = = = = = = = = = \n";

	PerformNormalWithdrawOption();
}

int ReadDepositAmount()
{
	int DepositAmount = 0;

	cout << "\nEnter a Positive Deposit Amount? ";
	cin >> DepositAmount;

	if (DepositAmount < 0)
	{
		cout << "\nEnter a Positive Deposit Amount? ";
		cin >> DepositAmount;
	}

	return DepositAmount;
}

void PerformDepositOption(int DepositAmount)
{
	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);

	DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber,
		DepositAmount, vClients);

	CurrentClient.AccountBalance += DepositAmount;
}

void DepositScreen()
{

	cout << "\n= = = = = = = = = = = = = = = = = = \n";
	cout << "\tDeposit Screen";
	cout << "\n= = = = = = = = = = = = = = = = = = \n";

	PerformDepositOption(ReadDepositAmount());
}

void PerformMainMenuOptions(enAtmMenuOptions AtmMenuOption)
{
	switch (AtmMenuOption)
	{
	case enAtmMenuOptions::QuickWithdraw:
	{
		system("cls");
		QuickWithdrawScreen();
		GoBakeToAtmMenuScreen();
		break;
	} 
	case enAtmMenuOptions::NormalWithdraw:
	{
		system("cls");
		NormalWithdrawScreen();
		GoBakeToAtmMenuScreen();
		break;
	}
	case enAtmMenuOptions::Deposit:
	{
		system("cls");
		DepositScreen();
		GoBakeToAtmMenuScreen();
		break;
	}
	case enAtmMenuOptions::CheckBalance:
	{
		system("cls");
		ShowCheckBalanceScreen();
		GoBakeToAtmMenuScreen();
		break;
	}
	case enAtmMenuOptions::Logout:
	{
		system("cls");
		Login();
	}
	}
}

void AtmMainMenuScreen()
{
	system("cls");

	cout << "\n= = = = = = = = = = = = = = = = = = \n";
	cout << "\tATM Main Menu Screen";
	cout << "\n= = = = = = = = = = = = = = = = = = \n";

	cout << "\t[1] Quick withdraw.\n";
	cout << "\t[2] Normal Withdraw.\n";
	cout << "\t[3] Deposit.\n";
	cout << "\t[4] Check Balance.\n";
	cout << "\t[5] Logout.\n";
	cout << "= = = = = = = = = = = = = = = = = =\n";

	PerformMainMenuOptions(enAtmMenuOptions(ReadAtmMenuOption()));
}

void Login()
{

	bool LoginFaild = false;
	string AccountNumber = "", PinCode = "";


	//do
	//{
	//	system("cls");

	//	LoginScreen();

	//	if (LoginFaild)
	//	{
	//		cout << "\nSorry AccountNumber OR PinCode wrong.!\n";
	//	}

	//	cout << "\nEnter AccountNumber? ";
	//	cin >> AccountNumber;

	//	cout << "\nEnter PinCode? ";
	//	cin >> PinCode;

	//	LoginFaild = !ClientExistsByAccountNubmerAndPinCode(AccountNumber,
	//		PinCode, CurrentClient);

	//} while (LoginFaild);

	AtmMainMenuScreen();
}

int main()
{
	Login();

	system("pause > 0");

	return 0;
}