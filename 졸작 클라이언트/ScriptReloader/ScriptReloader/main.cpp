#include "stdafx.h"
#include "ScriptReloader.h"

int main()
{
	string Filename;
	CScript* lpscript = CScript::Create();
	while (1)
	{
		cout << "[Map Script Data Arragement]" << endl;
		cout << "Script File input :";
		cin >> Filename;

		if (Filename == "exit")
			exit(1);

		lpscript->loadRawScript(Filename);
		if (lpscript->GetNumOfObject() == 0)
		{
			cout << "Empty file Or Wrong Filename" << endl;
			continue;
		}
		lpscript->ScriptPrintOut();
		cout << "All Arragements are Finished" << endl;

		lpscript->Initialize();
	}
}
