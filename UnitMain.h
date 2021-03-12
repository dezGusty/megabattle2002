//---------------------------------------------------------------------------
#ifndef UnitMainH
#define UnitMainH
#include <System.Classes.hpp>
#include <System.ImageList.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.MPlayer.hpp>
//---------------------------------------------------------------------------

#include <string>

#include "src/game.h"
#include "src/simple_sound_action.h"
#include "src/coord.h"
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ImgList.hpp>
#include <ddraw.h>
#include "UTSoldat.cpp"//pt thero si tsoldat
#include "UTLogicButton.h"
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <Vcl.MPlayer.hpp>
#include <Menus.hpp>
#include <System.ImageList.hpp>//pt btns

//---------------------------------------------------------------------------
class TBattleForm : public TForm
{
__published:	// IDE-managed Components
		TImageList *ImagHex;
		TImageList *ImagBorder;
		TImageList *ImagUnit;
		TImageList *ImagPortret;
		TImageList *ImagButon;
		TImageList *ImagAtac;
		TImage *logoImage;
		TMediaPlayer *MediaPlayer1;
		TMediaPlayer *MediaPlayer2;
		TMediaPlayer *MediaPlayer3;
		TMediaPlayer *MediaPlayer4;
		TMediaPlayer *MediaPlayer5;
		TTimer *AITimer;
		TPopupMenu *PopupMenu1;
		TMenuItem *Surrender1;
		TMenuItem *N1;
		TMenuItem *N2;
		void __fastcall FormCreate(TObject *Sender);
		void __fastcall FormPaint(TObject *Sender);
		void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
		  TShiftState Shift);
		void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y);
		void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
		void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift,
		  int X, int Y);
		void __fastcall FormActivate(TObject *Sender);
		void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y);
		void __fastcall AITimerTimer(TObject *Sender);
		void __fastcall Surrender1Click(TObject *Sender);
private:	// User declarations
		LPDIRECTDRAW            lpDD;           // DirectDraw object
		HDC backgrounddc;
		HBITMAP  bkbmp;
		HDC workdc;
		HBITMAP  wkbmp;
public:		// User declarations

        Game game;

		int SelectedPlayer;
		int SelectedSlot;
		int MouseSelectX;
		int MouseSelectY;
        TLogicButton *button[2];

        TCanvas *CanvasLucru;
        TCanvas *CanvasFundal;
        TImage *ImagineTeren;
        TRect allRect;
        TRect battleRect;
        THero *Player[2];
		bool __ExistaPlayer(int juc,int lot){bool ret=true;if(juc>1 ||lot>Player[juc]->angajati) ret=false;return ret;}
		void _CursoareInitializari();
		void _CursoareSet0();
		void _DesenHexuri();
		void _DesenUnitati();
		void _InitializariFundal();
		void _InitializariMatrice();
		void _InitializariMatriceS();
		void _InitializariSunete();
		void _PuneUnitate(TCanvas *UnCanvas,int x,int y);
		void AIAflaOrdin();
		void AI_find_target(int &tempx,int &tempy);
		void AI_GasesteMutare(int &tempx,int &tempy);
		void AtacArcas(int tintax,int tintay);
		void AtacNormal(int tintax,int tintay);
		void DesenHexCopy(int x, int y, int fel);
		void DesenHexuriSelectate();
		void DisplayAtac(int x,int y,int felatac);
		void ExecutaAtac(int tx,int ty,bool range);
		void IntraInJoc();

		void LoadBattleBackgroundPictureForType(TerrainType terrain_type);

		void Muta(int newx,int newy);
		void MutaUnitate(int newx,int newy);
		void OrdinSkipTurn();

		void PlaySoundForAction(SimpleSoundAction action);
		void RenderBorderAndBackground();
        void RenderSingleHex(TCanvas* canvas, int x, int y, int fel);


		void SeteazaHex(int x,int y,int mut);
		void SeteazaHex2(int x,int y,int mut,int exdir);
		void ShowComment(int juc,int lot,int damage,int position);
		void StergeHexuriSelectate();
		void Joc();
        int Victorie();
        void Selecteaza(int x,int y,bool ShowPlayer,int felhex);
        void SelecteazaUrmator();
        __fastcall TBattleForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TBattleForm *BattleForm;
//---------------------------------------------------------------------------
#endif
