//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USERES("HeroEdit.res");
USEFORM("MainUnit.cpp", EditForm);
USEFORM("AboutUnit.cpp", FormHelp);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->Title = "Megalithic Editor";
                 Application->CreateForm(__classid(TEditForm), &EditForm);
                 Application->CreateForm(__classid(TFormHelp), &FormHelp);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
