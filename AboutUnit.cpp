//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "AboutUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormHelp *FormHelp;
//---------------------------------------------------------------------------
__fastcall TFormHelp::TFormHelp(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormHelp::CloseButtonClick(TObject *Sender)
{Close();
}
//---------------------------------------------------------------------------

