#include "Menu.h"

int main()
{
	srand(time(NULL));

	Menu *p = new Menu();

	do {
		p->Display();
		p->Choose();
		p->DoTask();
	} while (p->GetOption() != 0);

	delete p;
	return 0;
}