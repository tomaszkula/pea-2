#include "Menu.h"

Menu::Menu(int optToQuit)
{
}

Menu::~Menu()
{
	if (g) delete g;
}

void Menu::Display()
{
	std::cout << "[PEA 2] [MENU]\n";
	std::cout << "[1] Wczytaj z pliku\n";
	std::cout << "[2] Wygeneruj losowo\n";
	std::cout << "[3] Wyswietl dane\n";
	std::cout << "[4] Pomiary do sprawozdania\n";
	std::cout << "[5] Wprowadzenie kryterium stopu(aktualnie : " << Graph::stopCondition << "s)\n";
	std::cout << "[6] Poszukiwanie z zakazami(Tabu Search)\n";
	std::cout << "[0] Wyjdz z programu\n";
}

void Menu::Choose()
{
	std::cout << "Wybierz opcje: ";
	std::cin >> opt;
}

int Menu::GetOption()
{
	return opt;
}

void Menu::DoTask()
{
	std::cout << "\n";

	switch (opt)
	{
	case 0:
		return;

	case 1:
		ReadFromFile();
		break;

	case 2:
		GenerateRandomly();
		break;

	case 3:
		DisplayData();
		break;

	case 4:
		TimeMeasure();
		break;

	case 5:
		SetStopCondition();
		break;

	case 6:
		DisplayTabuSearch();
		break;
	}

	std::cout << "\n";
}

void Menu::ReadFromFile()
{
	std::string fileName = "";
	std::cout << "Podaj nazwe pliku: ";
	std::cin >> fileName;

	std::fstream file;
	file.open(fileName, std::fstream::in);
	if (!file.is_open())
	{
		std::cout << "Podany plik nie istnieje lub zostal zle otworzony!\n";
		return;
	}
	
	std::string line;
	if (fileName.find(".txt") != std::string::npos)
	{
		int linesCount = 0, nodesCount = 0;
		while (std::getline(file, line))
		{
			if (++linesCount == 1)
			{
				nodesCount = atoi(line.c_str());

				if (g) delete g;
				g = new Graph(nodesCount);
			}
			else
			{
				int *row = new int[nodesCount];
				std::istringstream iss(line);
				for (int i = 0; i < nodesCount; i++)
				{
					iss >> row[i];
				}

				g->CompleteDistancesRow(linesCount - 2, row);
			}
		}

		std::cout << "Poprawnie zaladowano dane z pliku TXT.\n";
		std::cout << "Ilosc wierzcholkow: " << nodesCount << "\n";
	}
	else if (fileName.find(".atsp") != std::string::npos)
	{
		int nodesCount = 0;
		bool isNodesCount = false, isDataStart = false;
		int *row = new int[nodesCount], rowId = 0, nr = 0;
		while (std::getline(file, line))
		{
			std::smatch match;
			if (!isNodesCount)
			{
				std::regex reNodesCount("(DIMENSION: )(\\d+)");
				if (std::regex_search(line, match, reNodesCount))
				{
					nodesCount = atoi(match[2].str().c_str());
					isNodesCount = true;

					if (g) delete g;
					g = new Graph(nodesCount);

					if (row) delete[] row;
					row = new int[nodesCount];
				}
			}
			else if (!isDataStart)
			{
				std::regex reDataStart("EDGE_WEIGHT_SECTION");
				if (std::regex_search(line, match, reDataStart))
				{
					isDataStart = true;
				}
			}
			else if (isDataStart)
			{
				std::regex reEOF("EOF");
				if (std::regex_search(line, match, reEOF))
				{
					std::cout << "Poprawnie zaladowano dane z pliku ATSP.\n";
					std::cout << "Ilosc wierzcholkow: " << nodesCount << "\n";
					break;
				}

				std::istringstream iss(line);
				while (iss >> row[nr])
				{
					nr++;

					if (nr >= nodesCount)
					{
						g->CompleteDistancesRow(rowId, row);
						rowId++;

						row = new int[nodesCount];
						nr = 0;
					}
				}
			}
		}
	}
	else
	{
		std::cout << "Zly format pliku!\n";
	}

	file.close();
}

void Menu::GenerateRandomly()
{
	int n;
	std::cout << "Podaj ilosc miast(wierzcholkow): ";
	std::cin >> n;

	int a = 0, b = 50;
	std::cout << "Podaj punkt startowy zakresu: ";
	std::cin >> a;
	std::cout << "Podaj punkt koncowy zakresu: ";
	std::cin >> b;

	GenerateRandomly(n, a, b);
}

void Menu::GenerateRandomly(int n, int a, int b)
{
	srand(time(NULL));

	if (g) delete g;
	g = new Graph(n);

	for (int i = 0; i < n; i++)
	{
		int *row = new int[n];
		for (int j = 0; j < n; j++)
		{
			if (i == j)
			{
				row[j] = -1;
				continue;
			}

			int num = rand() % (b - a + 1) + a;
			row[j] = num;
		}
		g->CompleteDistancesRow(i, row);
	}
}

void Menu::SetStopCondition()
{
	std::cout << "[Ustawienie warunku zakonczenia algorytmow]\n";

	double stopTime;
	std::cout << "Podaj czas stopu[s]: ";
	std::cin >> stopTime;
	Graph::stopCondition = stopTime;
	std::cout << "Algorytm zostanie zakonczony po " << stopTime << "s od rozpoczecia.\n";
}

void Menu::DisplayData()
{
	std::cout << "[Aktualne dane]\n";
	if (!g)
	{
		std::cout << "Brak danych do wyswietlenia!\n";
		return;
	}

	g->Display();
}

void Menu::TimeMeasure()
{

}

void Menu::DisplayTabuSearch()
{
	std::cout << "[Poszukiwanie z zakazami(Tabu Search)]\n";
	if (!g)
	{
		std::cout << "Brak danych do uruchomienia algorytmu!\n";
		return;
	}

	g->TabuSearch();
}