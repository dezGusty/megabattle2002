//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("UnitMain.cpp", BattleForm);
USEFORM("UnitMesaj.cpp", Mesaj);
USERES("MegaBattle2.res");
USEUNIT("UTSoldat.cpp");
USERES("MCursors.res");
USEUNIT("UTLogicButton.cpp");
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
        Application->Title = "Megalithic Battle Test ver 1.04";
		Application->CreateForm(__classid(TBattleForm), &BattleForm);
		Application->CreateForm(__classid(TMesaj), &Mesaj);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
