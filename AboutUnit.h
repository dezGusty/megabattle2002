//---------------------------------------------------------------------------
#ifndef AboutUnitH
#define AboutUnitH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormHelp : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TMemo *Memo1;
        TButton *CloseButton;
        void __fastcall CloseButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFormHelp(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormHelp *FormHelp;
//---------------------------------------------------------------------------
#endif
