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
typedef struct
{int x,y;
}TSCOORD;
//---------------------------------------------------------------------------
class TBattleForm : public TForm
{//friend TMesaj;
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
		bool bActive;
		void __fastcall DDrawStart();
		HDC backgrounddc;
		HBITMAP  bkbmp;
		HDC workdc;
		HBITMAP  wkbmp;
public:		// User declarations

        Game game;

		bool PathwasFound;
		bool WaitingForOrder;

		int FazaJoc;
		int selected[9][7];//matr selectarilor
		int teren[9][7];//matricea sistem
		int SelectedPlayer;
        int SelectedSlot;
        int MouseSelectX;
        int MouseSelectY;
        int MouseX;
        int MouseY;
        int TargetX;
        int TargetY;
        TSCOORD path[15];
        TLogicButton *button[2];

        TCanvas *CanvasLucru;
        TCanvas *CanvasFundal;
        TImage *ImagineTeren;
        TRect allRect;
        TRect battleRect;
        THero *Player[2];
		void __citeste(FILE *ff){char c;do{fscanf(ff,"%c",&c);}while(c!='^' && !feof(ff));}
		bool __exista(char *filename){return (access(filename, 0) == 0);}
		bool __ExistaPlayer(int juc,int lot){bool ret=true;if(juc>1 ||lot>Player[juc]->angajati) ret=false;return ret;}
		void _CursoareInitializari();
		void _CursoareSet0();
		void _DesenFundal();
		void _DesenHex(TCanvas *UnCanvas, int x, int y, int fel);
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
		bool ExistaCoord(int mx,int my);
		bool ExistaHex(int x,int y,int dir);
		bool ExistaHex2(int x,int y,int dir);
		int GetPosX(int mx,int my);
		int GetPosY(int mx,int my);
		int GetX(int x,int y,int dir);
		int GetY(int x,int y,int dir);
		void IntraInJoc();

		void LoadBattleBackgroundPictureForType(TerrainType terrain_type);

		void Muta(int newx,int newy);
		void MutaUnitate(int newx,int newy);
		void OrdinSkipTurn();
		void PathFinding(int x,int y,int mut,int pas);

		void PlaySoundForAction(SimpleSoundAction action);

		void SeteazaHex(int x,int y,int mut);
		void SeteazaHex2(int x,int y,int mut,int exdir);
		void SeteazaHexArcas(int x,int y);
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
