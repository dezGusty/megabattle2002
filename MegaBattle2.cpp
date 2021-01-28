//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USERES("MegaBattle2.res");
USEFORM("UnitMain.cpp", BattleForm);
USEFORM("UnitMesaj.cpp", Mesaj);
USEUNIT("UTSoldat.cpp");
USERES("MCursors.res");
USEUNIT("UTLogicButton.cpp");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->Title = "Megalithic Battle Test ver 1.04";
                 Application->CreateForm(__classid(TBattleForm), &BattleForm);
                 Application->CreateForm(__classid(TMesaj), &Mesaj);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
