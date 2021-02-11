//---------------------------------------------------------------------------
#ifndef LauncherMainUnitH
#define LauncherMainUnitH
//---------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <process.h>
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <Dialogs.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TButton *ButtonClose;
        TButton *ButtonLaunch;
        TImage *ImagineFundal;
        TTimer *Timer1;
        TImage *Image1;
        TButton *NumeErou2;
        TButton *NumeErou1;
        TOpenDialog *OpenDialog1;
        TCheckBox *CheckBox1;
        TEdit *Edit1;
        TUpDown *UpDown1;
        TStaticText *StaticText1;
        TCheckBox *CheckBox2;
        void __fastcall ButtonCloseClick(TObject *Sender);
        void __fastcall ButtonLaunchClick(TObject *Sender);
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall FormActivate(TObject *Sender);
        void __fastcall NumeErou1Click(TObject *Sender);
        void __fastcall NumeErou2Click(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall Edit1Change(TObject *Sender);
	void __fastcall CheckBox2Click(TObject *Sender);
	void __fastcall CheckBox1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        TRect cpRect;
        int felteren;
        int ShowHexes;
        int control2;
		std::string fisier1;
        std::string fisier2;
        AnsiString InitialGameDir;
        __fastcall TForm1(TComponent* Owner);
        void __citeste(FILE *ff){char c;do{fscanf(ff,"%c",&c);}while(c!='^' && !feof(ff));}

		void ScrieINI();
        bool LoadIniFile(const std::string& file_name);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
