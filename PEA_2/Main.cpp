#include "Menu.h"

int main()
{
	Menu *p = new Menu();

	do {
		p->Display();
		p->Choose();
		p->DoTask();
	} while (p->GetOption() != 0);

	delete p;
	return 0;
}