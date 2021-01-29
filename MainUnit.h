//---------------------------------------------------------------------------
#ifndef MainUnitH
#define MainUnitH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Menus.hpp>
//---------------------------------------------------------------------------
class TEditForm : public TForm
{
__published:	// IDE-managed Components
        TMainMenu *MainMenu1;
        TMenuItem *Hero1;
        TMenuItem *CreateNewHero1;
        TMenuItem *OpenExistingHero1;
        TMenuItem *SaveCurrentHero1;
        TMenuItem *N1;
        TMenuItem *ExitEditor1;
        TMenuItem *Help1;
        TMenuItem *About1;
        TLabel *Label1;
        TEdit *EditNumeErou;
        TComboBox *ComboBox1;
        TLabel *Label2;
        TPanel *Panel1;
        TComboBox *Merc1;
        TComboBox *Merc2;
        TComboBox *Merc3;
        TComboBox *Merc4;
        TComboBox *Merc5;
        TComboBox *Merc6;
        TComboBox *FelErou;
        TLabel *Label3;
        TEdit *EditHeroLevel;
        TLabel *Label4;
        TUpDown *UpDownHero;
        TEdit *Edit1;
        TUpDown *UpDown1;
        TEdit *Edit2;
        TUpDown *UpDown2;
        TEdit *Edit3;
        TUpDown *UpDown3;
        TEdit *Edit4;
        TUpDown *UpDown4;
        TEdit *Edit5;
        TUpDown *UpDown5;
        TEdit *Edit6;
        TUpDown *UpDown6;
        TLabel *Label5;
        TSaveDialog *SaveDialog1;
        TOpenDialog *OpenDialog1;
        void __fastcall ComboBox1Change(TObject *Sender);
        void __fastcall ExitEditor1Click(TObject *Sender);
        void __fastcall CreateNewHero1Click(TObject *Sender);
        void __fastcall About1Click(TObject *Sender);
        void __fastcall SaveCurrentHero1Click(TObject *Sender);
        void __fastcall OpenExistingHero1Click(TObject *Sender);
        void __fastcall FormActivate(TObject *Sender);
private:	// User declarations
public:		// User declarations
        AnsiString DirectorJoc;
        void ResetariDate();
		__fastcall TEditForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TEditForm *EditForm;
//---------------------------------------------------------------------------
#endif
