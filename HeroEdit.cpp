//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("HeroEdit.cpp", Form3);
USEFORM("AboutUnit.cpp", FormHelp);
USEFORM("MainUnit.cpp", EditForm);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->Title = "Megalithic Editor";

		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TFormHelp), &FormHelp);
		Application->CreateForm(__classid(TEditForm), &EditForm);
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
