//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//constante directii
#define STGSUS 0
#define STG    1
#define STGJOS 2
#define DRPJOS 3
#define DRP    4
#define DRPSUS 5
//constante control
#define COMPUTER 1
#define HUMAN 0

// Other includes in this project.
#include "src/terrain_type.h"
#include "src/filehelperfunctions.h"

#include "UnitMain.h"
#include "UnitMesaj.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TBattleForm *BattleForm;


// ---------------------------------------------------------------------------
__fastcall TBattleForm::TBattleForm(TComponent* Owner) : TForm(Owner) {
	lpDD = NULL;
}

// ---------------------------------------------------------------------------
void __fastcall TBattleForm::FormActivate(TObject *Sender) {
	WindowState = wsMaximized;
}

// ---------------------------------------------------------------------------
void __fastcall TBattleForm::FormClose(TObject *Sender, TCloseAction &Action) {
	SelectObject(backgrounddc, bkbmp);
	DeleteObject(bkbmp);
	SelectObject(workdc, wkbmp);
	DeleteObject(wkbmp);
	DeleteDC(backgrounddc);
	DeleteDC(workdc);
}

// ---------------------------------------------------------------------------
void __fastcall TBattleForm::FormCreate(TObject *Sender) {
	Width = 800;
	Height = 600;
	Left = 0;
	Top = 0;
	game.FazaJoc = 0;
	MouseSelectX = -84;
	MouseSelectY = -84;
	randomize();
	_InitializariSunete();
	_InitializariFundal();
}

//---------------------------------------------------------------------------
void __fastcall TBattleForm::FormKeyDown(TObject *Sender, WORD &Key,
	TShiftState Shift) {
	if (game.FazaJoc == 0)
		IntraInJoc();
	else {
		// ignore
	}
}

//---------------------------------------------------------------------------
void __fastcall TBattleForm::FormMouseDown(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y) {
	if (game.FazaJoc == 0) {
		IntraInJoc();
	}
	else if (game.WaitingForOrder) {
		if (Button == mbLeft) {
			if (X > 20 && X < 780 && Y > 110 && Y < 530 && Cursor != TCursor(3))
			{
				if (Cursor == TCursor(1)) // mutare
					if (ExistaCoord(X, Y))
						Muta(GetPosX(X, Y), GetPosY(X, Y));
				if (Cursor == TCursor(2)) // atac
					if (ExistaCoord(X, Y))
						if (Player[SelectedPlayer]->army_slots[SelectedSlot]
							->Ranged && Player[SelectedPlayer]->army_slots
							[SelectedSlot]->Ammo > 0) {
							AtacArcas(GetPosX(X, Y), GetPosY(X, Y));
							}
						else{
							AtacNormal(GetPosX(X, Y), GetPosY(X, Y));
							}
			}
			// butoane logice
			for (int i = 0; i <= 1; i++)
				if (button[i]->Phase == 1) {
					button[i]->Phase = 2;
					button[i]->Draw(ImagButon, CanvasLucru);
					Canvas->CopyRect(button[i]->GetRect(), CanvasLucru,
						button[i]->GetRect());
				}
		}
		if (Button == mbRight) {
			if (X > 20 && X < 780 && Y > 110 && Y < 530)
				if (ExistaCoord(X, Y)) {
					int ax = GetPosX(X, Y);
					int ay = GetPosY(X, Y);
					if (game.teren[ax][ay]) {
						int tjuc = game.teren[ax][ay] / 20;
						int tlot = game.teren[ax][ay] % 10;
						TMesaj *UnMesaj = new TMesaj(this, tjuc, tlot);
						UnMesaj->ShowModal();
						delete UnMesaj;
					}
				}
		}
	}
}

//---------------------------------------------------------------------------
void __fastcall TBattleForm::FormMouseMove(TObject *Sender, TShiftState Shift,
	int X, int Y) {
	if (game.WaitingForOrder) {
		MouseX = X;
		MouseY = Y;
		if (ExistaCoord(MouseX, MouseY)) {
			int fx = GetPosX(MouseX, MouseY);
			int fy = GetPosY(MouseX, MouseY);
			switch (game.selected[fx][fy]) {
			case 1:
				if (MouseSelectX != fx || MouseSelectY != fy) {
					if (MouseSelectX != -84)
						DesenHexCopy(MouseSelectX, MouseSelectY, 2);
					MouseSelectX = fx;
					MouseSelectY = fy;
					RenderSingleHex(Canvas, MouseSelectX, MouseSelectY, 4);
					Cursor = TCursor(1);
				}
				break;
			case 2: {
					Cursor = TCursor(2);
					if (MouseSelectX != -84)
						DesenHexCopy(MouseSelectX, MouseSelectY, 2);
					MouseSelectX = -84;
				} break;
			case 0:
				_CursoareSet0();
				break;
			}
		}
		else
			_CursoareSet0();

		// butoane logice
		if (game.FazaJoc == 1) {
			for (int i = 0; i <= 1; i++)
				if (X > button[i]->Left && X < button[i]->Right && Y >
					button[i]->Top && Y < button[i]->Bottom) {
					if (button[i]->Phase == 0) {
						button[i]->Phase = 1;
						button[i]->Draw(ImagButon, CanvasLucru);
						Canvas->CopyRect(button[i]->GetRect(), CanvasLucru,
							button[i]->GetRect());
					}
				}
				else if (button[i]->Phase == 1) {
					button[i]->Phase = 0;
					button[i]->Draw(ImagButon, Canvas);
				}
		}
	}
}

//---------------------------------------------------------------------------
void __fastcall TBattleForm::FormMouseUp(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y) {
	if (game.WaitingForOrder) { // butoane logice
		if (button[1]->Phase == 2)
			OrdinSkipTurn();
		if (button[0]->Phase == 2)
			PopupMenu1->Popup(40, 560); // Close();
		for (int i = 0; i <= 1; i++)
			if (button[i]->Phase == 2) {
				button[i]->Phase = 0;
				button[i]->Draw(ImagButon, CanvasLucru);
				Canvas->CopyRect(button[i]->GetRect(), CanvasLucru,
					button[i]->GetRect());
			}
	}
}
//---------------------------------------------------------------------------
void __fastcall TBattleForm::FormPaint(TObject *Sender)
{Canvas->CopyRect(allRect,CanvasLucru,allRect);
}
//---------------------------------------------------------------------------
void __fastcall TBattleForm::AITimerTimer(TObject *Sender)
{AIAflaOrdin();
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TBattleForm::_InitializariFundal() {
	TRect temp(0, 0, 800, 600);
	allRect = temp;
	battleRect = temp;
	battleRect.Bottom -= 50;
	CanvasFundal = new TCanvas;
	CanvasLucru = new TCanvas;
	// Setup pt canvasul de fundal
	/* HDC */ backgrounddc = CreateCompatibleDC(Canvas->Handle);
	/* HBITMAP */
	bkbmp = CreateCompatibleBitmap(Canvas->Handle, ClientWidth, ClientHeight);
	SelectObject(backgrounddc, bkbmp);
	CanvasFundal->Handle = backgrounddc;
	// Setup pt canvasul de lucru
	/* HDC */ workdc = CreateCompatibleDC(Canvas->Handle);
	/* HBITMAP */
	wkbmp = CreateCompatibleBitmap(Canvas->Handle, ClientWidth, ClientHeight);
	SelectObject(workdc, wkbmp);
	CanvasLucru->Handle = workdc;
	CanvasFundal->Brush->Color = clBlack;
	CanvasFundal->Rectangle(0, 0, 800, 600);
	CanvasFundal->Brush->Color = (TColor)RGB(255, 175, 75);
	CanvasFundal->Font->Color = (TColor)RGB(255, 175, 75);
	CanvasFundal->Brush->Style = bsClear;
	CanvasFundal->TextOut(290, 230, "Megalithic Battle 2 v.1.04 sept.2002");
	CanvasFundal->TextOut(331, 250, "A game by Gusty");
	CanvasFundal->TextOut(290, 270, "Press a key or click to begin combat");
	CanvasLucru->CopyRect(allRect, CanvasFundal, allRect);
}

//---------------------------------------------------------------------------
void TBattleForm::LoadBattleBackgroundPictureForType(TerrainType terrain_type) {
	ImagineTeren = new TImage(this);
	switch (game.felteren) {
	case TerrainType::GrassPlains:
		if (doesFileExist("./img/grassy-field.bmp")) {
			ImagineTeren->Picture->LoadFromFile("./img/grassy-field.bmp");
		}
		break;
	case TerrainType::Dirt:
		if (doesFileExist("./img/cloudy-dirt.bmp")){
			ImagineTeren->Picture->LoadFromFile("./img/cloudy-dirt.bmp");
		}
		break;
	}
}

// ---------------------------------------------------------------------------
void TBattleForm::RenderBorderAndBackground() {
	int i;
	ImagBorder->Draw(CanvasFundal, 0, 0, 0, true);
	ImagBorder->Draw(CanvasFundal, 0, 540, 0, true);
	ImagBorder->Draw(CanvasFundal, 790, 0, 0, true);
	ImagBorder->Draw(CanvasFundal, 790, 540, 0, true);
	for (i = 10; i < 790; i += 10) {
		ImagBorder->Draw(CanvasFundal, i, 0, 1, true);
		ImagBorder->Draw(CanvasFundal, i, 540, 1, true);
	}
	for (i = 10; i < 540; i += 10) {
		ImagBorder->Draw(CanvasFundal, 0, i, 2, true);
		ImagBorder->Draw(CanvasFundal, 790, i, 2, true);
	}
	CanvasFundal->Draw(10, 10, ImagineTeren->Picture->Bitmap);
}

// ---------------------------------------------------------------------------
void TBattleForm::RenderSingleHex(TCanvas *UnCanvas, int cell_x, int cell_y,
	int fel) {
	if (cell_y % 2 == 0) {
		ImagHex->Draw(UnCanvas, 20 + 80*cell_x, 100 + 60*cell_y, fel, true);
	}
	else {
		ImagHex->Draw(UnCanvas, 60 + 80*cell_x, 100 + 60*cell_y, fel, true);
	}
}

// ---------------------------------------------------------------------------
void TBattleForm::_DesenHexuri() {
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 9; j++) {
			if (!game.selected[j][i]) {
				RenderSingleHex(CanvasFundal, j, i, 0);
			}
			else {
				RenderSingleHex(CanvasFundal, j, i, 2);
			}
		}
	}
	CanvasLucru->CopyRect(battleRect, CanvasFundal, battleRect);
}

// ---------------------------------------------------------------------------
void TBattleForm::_DesenUnitati() {
	for (int j = 0; j < 7; j++)
		for (int i = 0; i < 9; i++)
			if (game.teren[i][j]) {
				int tjuc = game.teren[i][j] / 20;
				int tlot = game.teren[i][j] % 10;
				SoldierDraw(*Player[tjuc]->army_slots[tlot], ImagUnit,
					CanvasLucru);
			}
	Canvas->CopyRect(battleRect, CanvasLucru, battleRect);
}

// ---------------------------------------------------------------------------
void TBattleForm::_CursoareInitializari() {
	Screen->Cursors[1] = LoadCursor(HInstance, "WALK"); //
	Screen->Cursors[2] = LoadCursor(HInstance, "SWORD"); //
	Screen->Cursors[3] = LoadCursor(HInstance, "ARROW"); //
	Cursor = TCursor(3); //
}

// ---------------------------------------------------------------------------
void TBattleForm::_CursoareSet0() {
	Cursor = TCursor(3);
	if (MouseSelectX != -84)
		DesenHexCopy(MouseSelectX, MouseSelectY, 2);
	MouseSelectX = -84;
}

// ---------------------------------------------------------------------------
void TBattleForm::_InitializariMatrice() {
	game.InitializeTerrainMatrix();
	game.PlaceAllArmiesOnTerrainMatrix();
}
//---------------------------------------------------------------------------
void TBattleForm::_InitializariMatriceS() {
	int i, j;
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 7; j++) {
			game.selected[i][j] = 0;
		}
	}
}
//---------------------------------------------------------------------------
void TBattleForm::_InitializariSunete() {
	MediaPlayer1->FileName = "sound\\sword.wav";
	MediaPlayer2->FileName = "sound\\arrow.wav";
	MediaPlayer3->FileName = "sound\\move.wav";
	MediaPlayer4->FileName = "sound\\missed.wav";
	MediaPlayer5->FileName = "sound\\die.wav";
	MediaPlayer1->Open();
	MediaPlayer2->Open();
	MediaPlayer3->Open();
	MediaPlayer4->Open();
	MediaPlayer5->Open();
}

// ---------------------------------------------------------------------------
void TBattleForm::_PuneUnitate(TCanvas *UnCanvas, int x, int y) {
	int tjuc, tlot, kappa = 1;
	if (game.selected[x][y]) {
		if (game.selected[x][y] == 1) {
			if (game.ShowHexes) {
				RenderSingleHex(UnCanvas, x, y, 2);
			}
		}
		kappa = 0;
		if (game.selected[x][y] == 2) {
			RenderSingleHex(UnCanvas, x, y, 3);
		}
		kappa = 0;
	}

	if (game.teren[x][y] && SelectedPlayer == game.teren[x][y]
		/ 20 && SelectedSlot == game.teren[x][y] % 10) {
		RenderSingleHex(UnCanvas, x, y, 1);
		kappa = 0;
	}
	// TODO: what was kappa?
	if (kappa)
		if (game.ShowHexes) {
			RenderSingleHex(UnCanvas, x, y, 0);
		}

	if (game.teren[x][y] != 0) {
		tjuc = game.teren[x][y] / 20;
		tlot = game.teren[x][y] % 10;
		SoldierDraw(*Player[tjuc]->army_slots[tlot], ImagUnit, UnCanvas);
	}
}

//---------------------------------------------------------------------------
void TBattleForm::AIAflaOrdin() {
	AITimer->Enabled = false;
	int tempx, tempy;
	int mt = Player[SelectedPlayer]->army_slots[SelectedSlot]->MovesLeft;
	SeteazaHex(Player[SelectedPlayer]->army_slots[SelectedSlot]->x,
		Player[SelectedPlayer]->army_slots[SelectedSlot]->y, mt);
	if (Player[SelectedPlayer]->army_slots[SelectedSlot]->Ranged && Player
		[SelectedPlayer]->army_slots[SelectedSlot]->Ammo > 0)
		SeteazaHexArcas(Player[SelectedPlayer]->army_slots[SelectedSlot]->x,
		Player[SelectedPlayer]->army_slots[SelectedSlot]->y);
	AI_find_target(tempx, tempy);
	if (tempx == -1 && tempy == -1) // adica nu s-a gasit vreo tinta de atacat;
	{
		AI_GasesteMutare(tempx, tempy);
		Muta(tempx, tempy);
	}
	else {
		if (Player[SelectedPlayer]->army_slots[SelectedSlot]->Ranged && Player
			[SelectedPlayer]->army_slots[SelectedSlot]->Ammo > 0)
			AtacArcas(tempx, tempy);
		else
			AtacNormal(tempx, tempy);
	}
}

// ---------------------------------------------------------------------------
void TBattleForm::AI_find_target(int &tempx, int &tempy) {
	int oponent = (SelectedPlayer + 1) % 2;
	float raport = 500; // din cate lovituri iese un kill;
	int meddmg = (Player[SelectedPlayer]->army_slots[SelectedSlot]->DamageMax +
		Player[SelectedPlayer]->army_slots[SelectedSlot]->DamageMin) / 2;
	int curent; // slotul inamicului selectat
	int i, j; // pt parcurgerea matricei
	for (i = 0; i <= 8; i++)
		for (j = 0; j <= 6; j++)
			if (game.selected[i][j] == 2 && game.teren[i][j] && game.teren[i][j]
				/ 20 == oponent) {
				int lot = game.teren[i][j] % 10;
				int k = meddmg;
				k -= Player[oponent]->army_slots[lot]->Armor;
				k -= k * Player[oponent]->army_slots[lot]->Protection / 100;
				if (Player[SelectedPlayer]->army_slots[SelectedSlot]
					->Ranged && Player[SelectedPlayer]->army_slots[SelectedSlot]
					->Ammo == 0)
					k /= 2;
				if (k <= 0)
					k = 1;
				float tempraport = Player[oponent]->army_slots[lot]->Hp / k;
				if (raport > tempraport) {
					raport = tempraport;
					curent = lot;
				}
			}
	if (raport != 500) {
		tempx = Player[oponent]->army_slots[curent]->x;
		tempy = Player[oponent]->army_slots[curent]->y;
	}
	else {
		tempx = -1;
		tempy = -1;
	}
}

// ---------------------------------------------------------------------------
void TBattleForm::AI_GasesteMutare(int &tempx, int &tempy) {
	int oponent = (SelectedPlayer + 1) % 2;
	float raport = 500; // din cate lovituri iese un kill;
	int meddmg = (Player[SelectedPlayer]->army_slots[SelectedSlot]->DamageMax +
		Player[SelectedPlayer]->army_slots[SelectedSlot]->DamageMin) / 2;
	int curent; // slotul inamicului selectat
	for (int lot = 0; lot <= Player[oponent]->angajati; lot++)
		if (Player[oponent]->army_slots[lot]->alive) {
			int k = meddmg;
			k -= Player[oponent]->army_slots[lot]->Armor;
			k -= k * Player[oponent]->army_slots[lot]->Protection / 100;
			if (Player[SelectedPlayer]->army_slots[SelectedSlot]
				->Ranged && Player[SelectedPlayer]->army_slots[SelectedSlot]
				->Ammo == 0)
				k /= 2;
			if (k <= 0)
				k = 1;
			float tempraport = Player[oponent]->army_slots[lot]->Hp / k;
			if (raport > tempraport) {
				raport = tempraport;
				curent = lot;
			}
		}
	int tx = Player[oponent]->army_slots[curent]->x;
	int ty = Player[oponent]->army_slots[curent]->y;
	int x = Player[SelectedPlayer]->army_slots[SelectedSlot]->x;
	int y = Player[SelectedPlayer]->army_slots[SelectedSlot]->y;
	int mut = Player[SelectedPlayer]->army_slots[SelectedSlot]->MovesLeft;
	for (int i = 0; i < mut; i++) {
		int aok = 0;
		if (y > ty) {
			if (x > tx) {
				if (ExistaHex2(x, y, STGSUS)) {
					x = GetX(x, y, STGSUS);
					y = GetY(x, y, STGSUS);
					aok = 1;
				}
			}
			else {
				if (ExistaHex2(x, y, DRPSUS)) {
					x = GetX(x, y, DRPSUS);
					y = GetY(x, y, DRPSUS);
					aok = 1;
				}
			}
		}
		if (!aok && y == ty) {
			if (x > tx) {
				if (ExistaHex2(x, y, STG)) {
					x = GetX(x, y, STG);
					y = GetY(x, y, STG);
					aok = 1;
				}
			}
			else {
				if (ExistaHex2(x, y, DRP)) {
					x = GetX(x, y, DRP);
					y = GetY(x, y, DRP);
					aok = 1;
				}
			}
		}

		if (!aok && y < ty) {
			if (x > tx) {
				if (ExistaHex2(x, y, STGJOS)) {
					x = GetX(x, y, STGJOS);
					y = GetY(x, y, STGJOS);
					aok = 1;
				}
			}
			else {
				if (ExistaHex2(x, y, DRPJOS)) {
					x = GetX(x, y, DRPJOS);
					y = GetY(x, y, DRPJOS);
					aok = 1;
				}
			}
		}
	}
	tempx = x;
	tempy = y;
}

//---------------------------------------------------------------------------
void TBattleForm::AtacArcas(int tintax, int tintay) {
	if (Player[SelectedPlayer]->army_slots[SelectedSlot]->Ammo > 0) {
		Player[SelectedPlayer]->army_slots[SelectedSlot]->Ammo--;
		ExecutaAtac(tintax, tintay, true);
		::Sleep(200);
		SelecteazaUrmator();
	}
}

//---------------------------------------------------------------------------
void TBattleForm::AtacNormal(int tintax, int tintay) {
	TargetX = tintax;
	TargetY = tintay;
	PathwasFound = false;
	int mt = Player[SelectedPlayer]->army_slots[SelectedSlot]->MovesLeft;
	PathFinding(Player[SelectedPlayer]->army_slots[SelectedSlot]->x,
		Player[SelectedPlayer]->army_slots[SelectedSlot]->y, mt, 0);
	StergeHexuriSelectate();
	for (int i = 1; i <= mt; i++) {
		if (TargetX == path[i].x && TargetY == path[i].y)
			i = mt + 1;
		else
			MutaUnitate(path[i].x, path[i].y);
		::Sleep(100);
	}
	ExecutaAtac(tintax, tintay, false);
	DesenHexuriSelectate();
	::Sleep(150);
	SelecteazaUrmator();
}

//---------------------------------------------------------------------------
void TBattleForm::DesenHexCopy(int x, int y, int fel) {
	TRect FigRect(20 + x*80, 60*y + 100, 100 + x*80, 60*y + 180);
	TRect temp2(10 + x*80, 60*y + 90, 90 + x*80, 60*y + 170);
	if (y % 2 == 1) {
		FigRect.Left += 40;
		temp2.Left += 40;
		FigRect.Right += 40;
		temp2.Right += 40;
	}
	CanvasFundal->CopyRect(FigRect, ImagineTeren->Canvas, temp2);
	if (y > 1)
		_PuneUnitate(CanvasFundal, x, y - 2); // desen sus sus
	if (ExistaHex(x, y, STGSUS)) {
		int ax = GetX(x, y, STGSUS);
		int ay = GetY(x, y, STGSUS);
		_PuneUnitate(CanvasFundal, ax, ay);
	}
	if (ExistaHex(x, y, DRPSUS)) {
		int ax = GetX(x, y, DRPSUS);
		int ay = GetY(x, y, DRPSUS);
		_PuneUnitate(CanvasFundal, ax, ay);
	}
	_PuneUnitate(CanvasFundal, x, y);
	if (ExistaHex(x, y, STGJOS)) {
		int ax = GetX(x, y, STGJOS);
		int ay = GetY(x, y, STGJOS);
		_PuneUnitate(CanvasFundal, ax, ay);
	}
	if (ExistaHex(x, y, DRPJOS)) {
		int ax = GetX(x, y, DRPJOS);
		int ay = GetY(x, y, DRPJOS);
		_PuneUnitate(CanvasFundal, ax, ay);
	}
	if (y < 5)
		_PuneUnitate(CanvasFundal, x, y + 2); // desen jos jos
	CanvasLucru->CopyRect(FigRect, CanvasFundal, FigRect);
	Canvas->CopyRect(FigRect, CanvasLucru, FigRect);
}

//---------------------------------------------------------------------------
inline void TBattleForm::DesenHexuriSelectate() {
	for (int j = 0; j < 7; j++)
		for (int i = 0; i < 9; i++) {
			if (game.selected[i][j] == 1) {
				if (game.ShowHexes) {
					DesenHexCopy(i, j, 2);
				}
			}
			else if (game.selected[i][j] == 2) {
				DesenHexCopy(i, j, 3);
			}
		}
}

//---------------------------------------------------------------------------
void TBattleForm::DisplayAtac(int x, int y, int felatac) {
	int k; // indicele in imglist
	TRect FigRect(20 + x*80, 60*y + 100, 100 + x*80, 60*y + 180);
	if (y % 2 == 1) {
		FigRect.Left += 40;
		FigRect.Right += 40;
	}
	k = felatac * 6;
	for (int i = 0; i <= 5; i++) {
		CanvasFundal->CopyRect(FigRect, CanvasLucru, FigRect);
		ImagAtac->Draw(CanvasFundal, 10 + FigRect.Left, 10 + FigRect.Top,
			k + i, true);
		Canvas->CopyRect(FigRect, CanvasFundal, FigRect);
		::Sleep(50);
	}
}

// ---------------------------------------------------------------------------
void TBattleForm::ExecutaAtac(int tx, int ty, bool range) {
	int tjuc = game.teren[tx][ty] / 20;
	int tlot = game.teren[tx][ty] % 10;
	int juc = SelectedPlayer;
	int lot = SelectedSlot;
	int k = random(100) + 1;
	// MessageDlg(k,mtInformation,TMsgDlgButtons() <<mbOK,0);
	if (k <= Player[juc]->army_slots[lot]->ChancesToHit) {
		int auxx = Player[juc]->army_slots[lot]->DamageMax -
			Player[juc]->army_slots[lot]->DamageMin;
		k = Player[juc]->army_slots[lot]->DamageMin + random(auxx);
		k -= Player[tjuc]->army_slots[tlot]->Armor;
		k -= k * Player[tjuc]->army_slots[tlot]->Protection / 100;
		if (Player[SelectedPlayer]->army_slots[SelectedSlot]->Ranged && Player
			[SelectedPlayer]->army_slots[SelectedSlot]->Ammo <= 0) {
			k /= 2;
		}

		if (k <= 0) {
			k = 1;
		}

		ShowComment(juc, lot, k, 0);
		if (!range) {
			PlaySoundForAction(SimpleSoundAction::AttackHitMelee);
			DisplayAtac(Player[tjuc]->army_slots[tlot]->x,
				Player[tjuc]->army_slots[tlot]->y, 0);
		}
		else {
			PlaySoundForAction(SimpleSoundAction::AttackHitRange);
			DisplayAtac(Player[tjuc]->army_slots[tlot]->x,
				Player[tjuc]->army_slots[tlot]->y, 2);
		}
		Player[tjuc]->army_slots[tlot]->Hp -= k;
		if (Player[tjuc]->army_slots[tlot]->Hp <= 0) {
			game.teren[tx][ty] = 0;
			Player[tjuc]->army_slots[tlot]->alive = false;
			PlaySoundForAction(SimpleSoundAction::Die);
			Selecteaza(tx, ty, false, 0);
		}
	}
	else {
		ShowComment(juc, lot, 0, 0);
		PlaySoundForAction(SimpleSoundAction::AttackMiss);
		DisplayAtac(Player[tjuc]->army_slots[tlot]->x,
			Player[tjuc]->army_slots[tlot]->y, 1);
	}

	// urmeaza retaliation
	if (!range && Player[tjuc]->army_slots[tlot]->alive && Player[tjuc]
		->army_slots[tlot]->Retal > 0) {
		::Sleep(250);
		Player[tjuc]->army_slots[tlot]->Retal--;
		k = random(100) + 1;
		if (k <= Player[tjuc]->army_slots[tlot]->ChancesToHit) {
			k = Player[tjuc]->army_slots[tlot]->DamageMin +
				random(Player[tjuc]->army_slots[tlot]->DamageMax -
				Player[tjuc]->army_slots[tlot]->DamageMin);
			k -= Player[juc]->army_slots[lot]->Armor;
			k -= k * Player[juc]->army_slots[lot]->Protection / 100;
			if (k <= 0) {
				k = 1;
			}
			ShowComment(tjuc, tlot, k, 1);
			PlaySoundForAction(SimpleSoundAction::AttackHitMelee);
			DisplayAtac(Player[juc]->army_slots[lot]->x,
				Player[juc]->army_slots[lot]->y, 0);
			Player[juc]->army_slots[lot]->Hp -= k;
			if (Player[juc]->army_slots[lot]->Hp <= 0) {
				game.teren[Player[juc]->army_slots[lot]->x]
					[Player[juc]->army_slots[lot]->y] = 0;
				Player[juc]->army_slots[lot]->alive = false;
				PlaySoundForAction(SimpleSoundAction::Die);

				Selecteaza(Player[juc]->army_slots[lot]->x,
					Player[juc]->army_slots[lot]->y, false, 0);
			}
		}
		else {
			ShowComment(tjuc, tlot, 0, 1);
			PlaySoundForAction(SimpleSoundAction::AttackMiss);
			DisplayAtac(Player[juc]->army_slots[lot]->x,
				Player[juc]->army_slots[lot]->y, 1);
		}
	}
}

// ---------------------------------------------------------------------------
inline bool TBattleForm::ExistaCoord(int mx, int my) {
	bool ret = true;
	if (mx > 20 && mx < 780 && my > 110 && my < 530) {
		int fx = mx - 20;
		int fy = my - 110;
		fy /= 60;
		if ((fy % 2 == 1 && mx < 60) || (fy % 2 == 0 && mx > 740)) {
			ret = false;
		}
	}
	else {
		ret = false;
	}
	return ret;
}

// ---------------------------------------------------------------------------
inline bool TBattleForm::ExistaHex(int x, int y, int dir) {
	bool ret = true;
	if (x >= 0 && x < 9 && y >= 0 && y < 7)
		switch (dir) {
		case STGSUS:
			if (y <= 0)
				ret = false;
			else if (y % 2 == 0) {
				if (x <= 0)
					ret = false;
			}
			break;
		case STG:
			if (x <= 0)
				ret = false;
			break;
		case STGJOS:
			if (y >= 6)
				ret = false;
			else if (y % 2 == 0) {
				if (x <= 0)
					ret = false;
			}
			break;
		case DRPJOS:
			if (y >= 6)
				ret = false;
			else if (y % 2 == 1) {
				if (x >= 8)
					ret = false;
			}
			break;
		case DRP:
			if (x >= 8)
				ret = false;
			break;
		case DRPSUS:
			if (y <= 0)
				ret = false;
			else if (y % 2 == 1) {
				if (x >= 8)
					ret = false;
			}
			break;
		}
	else
		ret = false;
	return ret;
}

// ---------------------------------------------------------------------------
inline bool TBattleForm::ExistaHex2(int x, int y, int dir) {
	bool ret = true;
	if (ExistaHex(x, y, dir)) {
		int ax = GetX(x, y, dir);
		int ay = GetY(x, y, dir);
		if (game.teren[ax][ay])
			ret = false;
	}
	else
		ret = false;
	return ret;
}

// ---------------------------------------------------------------------------
inline int TBattleForm::GetPosX(int mx, int my) {
	int ret = 0;
	int fx = mx - 20;
	int fy = my - 110;
	fy /= 60;
	if (fy % 2 == 1)
		fx -= 40;
	fx /= 80;
	ret = fx;
	return ret;
}

// ---------------------------------------------------------------------------
inline int TBattleForm::GetPosY(int mx, int my) {
	int ret = 0;
	int fy = my - 110;
	fy /= 60;
	ret = fy;
	return ret;
}

//---------------------------------------------------------------------------
inline int TBattleForm::GetX(int x, int y, int dir) {
	int ret = 0;
	switch (dir) {
	case STGSUS:
		if (y % 2 == 1)
			ret = x;
		else
			ret = x - 1;
		break;
	case STG:
		ret = x - 1;
		break;
	case STGJOS:
		if (y % 2 == 1)
			ret = x;
		else
			ret = x - 1;
		break;
	case DRPJOS:
		if (y % 2 == 0)
			ret = x;
		else
			ret = x + 1;
		break;
	case DRP:
		ret = x + 1;
		break;
	case DRPSUS:
		if (y % 2 == 0)
			ret = x;
		else
			ret = x + 1;
		break;
	}
	return ret;
}

//---------------------------------------------------------------------------
inline int TBattleForm::GetY(int x, int y, int dir) {
	int ret = 0;
	switch (dir) {
	case STGSUS:
	case DRPSUS:
		ret = y - 1;
		break;
	case DRP:
	case STG:
		ret = y;
		break;
	case STGJOS:
	case DRPJOS:
		ret = y + 1;
		break;
	}
	return ret;
}

//---------------------------------------------------------------------------
void TBattleForm::IntraInJoc() {
	game.LoadBattleIniFile();

	LoadBattleBackgroundPictureForType(game.felteren);

	game.FazaJoc = 1;
	RenderBorderAndBackground();
	if (game.ShowHexes){
		_DesenHexuri();
	}

	CanvasLucru->CopyRect(allRect, CanvasFundal, allRect);
	Canvas->CopyRect(allRect, CanvasLucru, allRect);
	Player[0] = new THero(game.left_player_cfg_file.c_str());
	Player[1] = new THero(game.right_player_cfg_file.c_str());
	Player[0]->control = HUMAN;
	if (game.control2){
		Player[1]->control = COMPUTER;
	}
	else{
		Player[1]->control = HUMAN;
	}

	// set right-hand side player settings
	for (int i = 0; i <= 1; i++) {
		for (int j = 0; j <= Player[i]->angajati; j++) {
			if (i == 1) {
				Player[i]->army_slots[j]->x = 8;
				Player[i]->army_slots[j]->Orientare = 1;
			}
		}
	}

	_CursoareInitializari();
	_InitializariMatrice();
	_InitializariMatriceS();
	_DesenUnitati();

	// display logo
	CanvasFundal->Draw(50, 550, logoImage->Picture->Bitmap);
	CanvasLucru->Draw(50, 550, logoImage->Picture->Bitmap);
	Canvas->CopyRect(allRect, CanvasLucru, allRect);
	Joc();
}

// ---------------------------------------------------------------------------
void TBattleForm::Joc() {
	SelectedPlayer = 0;
	SelectedSlot = 0;
	game.WaitingForOrder = true;
	CanvasFundal->Draw(10, 10, ImagineTeren->Picture->Bitmap);
	if (game.ShowHexes)
		_DesenHexuri();
	_InitializariMatrice();
	_InitializariMatriceS();
	_DesenUnitati();
	Canvas->CopyRect(allRect, CanvasLucru, allRect);
	int mt = Player[SelectedPlayer]->army_slots[SelectedSlot]->MovesLeft;
	SeteazaHex(Player[SelectedPlayer]->army_slots[SelectedSlot]->x,
		Player[SelectedPlayer]->army_slots[SelectedSlot]->y, mt);
	if (Player[SelectedPlayer]->army_slots[SelectedSlot]->Ranged && Player
		[SelectedPlayer]->army_slots[SelectedSlot]->Ammo > 0)
		SeteazaHexArcas(Player[SelectedPlayer]->army_slots[SelectedSlot]->x,
		Player[SelectedPlayer]->army_slots[SelectedSlot]->y);

	DesenHexuriSelectate();
	Selecteaza(Player[SelectedPlayer]->army_slots[SelectedSlot]->x,
		Player[SelectedPlayer]->army_slots[SelectedSlot]->y, 1, 1);

	button[0] = new TLogicButton;
	button[0]->Left = 0;
	button[0]->Top = 550;
	button[0]->Width = 50;
	button[0]->Height = 50;
	button[0]->VisibleIndex = 0;
	button[0]->SelectedIndex = 1;
	button[0]->PressedIndex = 2;
	button[0]->Phase = 0;
	button[0]->Draw(ImagButon, Canvas);
	button[1] = new TLogicButton;
	button[1]->Left = 750;
	button[1]->Top = 550;
	button[1]->Width = 50;
	button[1]->Height = 50;
	button[1]->VisibleIndex = 3;
	button[1]->SelectedIndex = 4;
	button[1]->PressedIndex = 5;
	button[1]->Phase = 0;
	button[1]->Draw(ImagButon, Canvas);
}

//---------------------------------------------------------------------------
inline void TBattleForm::Muta(int newx, int newy) {
	TargetX = newx;
	TargetY = newy;
	PathwasFound = false;
	int mt = Player[SelectedPlayer]->army_slots[SelectedSlot]->MovesLeft;
	PathFinding(Player[SelectedPlayer]->army_slots[SelectedSlot]->x,
		Player[SelectedPlayer]->army_slots[SelectedSlot]->y, mt, 0);
	StergeHexuriSelectate();
	for (int i = 1; i <= mt; i++) {
		MutaUnitate(path[i].x, path[i].y);
		if (TargetX == path[i].x && TargetY == path[i].y)
			i = mt + 1;
		::Sleep(100);
	}
	DesenHexuriSelectate();
	::Sleep(150);
	SelecteazaUrmator();
}

// ---------------------------------------------------------------------------
inline void TBattleForm::MutaUnitate(int newx, int newy) {
	int tx = Player[SelectedPlayer]->army_slots[SelectedSlot]->x;
	int ty = Player[SelectedPlayer]->army_slots[SelectedSlot]->y;
	if (newx != tx || newy != ty) {
		PlaySoundForAction(SimpleSoundAction::Move);
		Selecteaza(tx, ty, false, 0);
		game.teren[tx][ty] = 0;
		Player[SelectedPlayer]->army_slots[SelectedSlot]->x = newx;
		Player[SelectedPlayer]->army_slots[SelectedSlot]->y = newy;
		game.teren[newx][newy] = SelectedPlayer * 10 + SelectedSlot + 10;
		Selecteaza(newx, newy, true, 0);
	}
}

// ---------------------------------------------------------------------------
void TBattleForm::OrdinSkipTurn() {
	SelecteazaUrmator();
}

// ---------------------------------------------------------------------------
inline void TBattleForm::PathFinding(int x, int y, int mut, int pas) {
	if (!PathwasFound)
		if (pas <= mut) {
			path[pas].x = x;
			path[pas].y = y;
			if (TargetX == x && TargetY == y)
				PathwasFound = true;
			else if ((game.teren[x][y] == 0) ||
				(Player[SelectedPlayer]->army_slots[SelectedSlot]
				->x == x && Player[SelectedPlayer]->army_slots[SelectedSlot]
				->y == y)) {
				int ordin[6] = {STGSUS, DRPSUS, STG, DRP, STGJOS, DRPJOS};
				if (y > TargetY) // prioritate sus
				{
					if (x > TargetX) // prior stg
						if (x - TargetX > y - TargetY) {
							ordin[0] = STGSUS;
							ordin[1] = STG;
							ordin[2] = STGJOS;
							ordin[3] = DRPSUS;
							ordin[4] = DRP;
							ordin[5] = DRPJOS;
						}
						else {
							ordin[0] = STGSUS;
							ordin[1] = DRPSUS;
							ordin[2] = STG;
							ordin[3] = DRP;
							ordin[4] = STGJOS;
							ordin[5] = DRPJOS;
						}
					if (x < TargetX) // prior drp
						if (TargetX - x > y - TargetY) {
							ordin[0] = DRPSUS;
							ordin[1] = DRP;
							ordin[2] = DRPJOS;
							ordin[3] = STGSUS;
							ordin[4] = STG;
							ordin[5] = STGJOS;
						}
						else {
							ordin[0] = DRPSUS;
							ordin[1] = STGSUS;
							ordin[2] = DRP;
							ordin[3] = STG;
							ordin[4] = DRPJOS;
							ordin[5] = STGJOS;
						}
					if (x == TargetX) // prior sus
						if (y % 2 == 0) // drpsus
						{
							ordin[0] = DRPSUS;
							ordin[1] = DRP;
							ordin[2] = DRPJOS;
							ordin[3] = STGSUS;
							ordin[4] = STG;
							ordin[5] = STGJOS;
						}
						else // stgsus
						{
							ordin[0] = STGSUS;
							ordin[1] = STG;
							ordin[2] = STGJOS;
							ordin[3] = DRPSUS;
							ordin[4] = DRP;
							ordin[5] = DRPJOS;
						}
				}
				if (y == TargetY) // prioritate oriz
				{
					if (x > TargetX) // prior stg
					{
						ordin[0] = STG;
						ordin[1] = DRP;
						ordin[2] = STGSUS;
						ordin[3] = STGJOS;
						ordin[4] = DRPJOS;
						ordin[5] = DRPSUS;
					}
					if (x < TargetX) // prior drp
					{
						ordin[0] = DRP;
						ordin[1] = STG;
						ordin[2] = DRPSUS;
						ordin[3] = DRPJOS;
						ordin[4] = STGJOS;
						ordin[5] = STGSUS;
					}
				}
				if (y < TargetY) // prioritate jos
				{
					// prior stg
					if (x > TargetX) {
						if (TargetY - y >= x - TargetX) {
							ordin[0] = STGJOS;
							ordin[1] = DRPJOS;
							ordin[2] = STG;
							ordin[3] = DRP;
							ordin[4] = STGSUS;
							ordin[5] = DRPSUS;
						}
						else {
							ordin[0] = STGJOS;
							ordin[1] = STG;
							ordin[2] = STGSUS;
							ordin[3] = DRPJOS;
							ordin[4] = DRP;
							ordin[5] = DRPSUS;
						}
					}

					// prior drp
					if (x <= TargetX) {
						if (TargetY - y >= TargetX - x) {
							ordin[0] = DRPJOS;
							ordin[1] = STGJOS;
							ordin[2] = DRP;
							ordin[3] = STG;
							ordin[4] = STGJOS;
							ordin[5] = STGSUS;
						}
						else {
							ordin[0] = DRPJOS;
							ordin[1] = DRP;
							ordin[2] = DRPSUS;
							ordin[3] = STGJOS;
							ordin[4] = STG;
							ordin[5] = STGSUS;
						}
					}

					// drpjos
					if (x == TargetX) {
						if (y % 2 == 0) {
							ordin[0] = DRPJOS;
							ordin[1] = STGJOS;
							ordin[2] = DRP;
							ordin[3] = STG;
							ordin[4] = STGJOS;
							ordin[5] = STGSUS;
						}
						else // stgjos
						{
							ordin[0] = STGJOS;
							ordin[1] = DRPJOS;
							ordin[2] = STG;
							ordin[3] = DRP;
							ordin[4] = STGSUS;
							ordin[5] = DRPSUS;
						}
					}
				}
				for (int k = 0; k <= 5; k++) {
					int i = ordin[k];
					if (ExistaHex(x, y, i)) {
						int ax = GetX(x, y, i);
						int ay = GetY(x, y, i);
						PathFinding(ax, ay, mut, pas + 1);
					}
				}
			}
		}
}

void DeplhiPlaySound(TMediaPlayer* player) {
	player->Rewind();
	player->Play();
}

/**
 Plays a sound for a certain action.
 */
void TBattleForm::PlaySoundForAction(SimpleSoundAction action) {
	switch (action) {
	case SimpleSoundAction::AttackHitMelee:
		DeplhiPlaySound(MediaPlayer1);
		break;
	case SimpleSoundAction::AttackHitRange:
		DeplhiPlaySound(MediaPlayer2);
		break;
	case SimpleSoundAction::Move:
		DeplhiPlaySound(MediaPlayer3);
		break;
	case SimpleSoundAction::AttackMiss:
		DeplhiPlaySound(MediaPlayer4);
		break;
	case SimpleSoundAction::Die:
		DeplhiPlaySound(MediaPlayer5);
		break;

	default: ;
	}

}

// ---------------------------------------------------------------------------
inline void TBattleForm::Selecteaza(int x, int y, bool ShowPlayer, int felhex) {
	int juc = game.teren[x][y] / 20;
	int lot = game.teren[x][y] % 10;
	TRect FigRect(20 + x*80, 60*y + 20, 100 + x*80, 60*y + 180);
	TRect temp2(10 + x*80, 60*y + 10, 90 + x*80, 60*y + 170);
	if (y % 2 == 1) {
		FigRect.Left += 40;
		temp2.Left += 40;
		FigRect.Right += 40;
		temp2.Right += 40;
	}
	CanvasFundal->CopyRect(FigRect, ImagineTeren->Canvas, temp2);
	// desen fig sus sus
	if (y > 1) {
		_PuneUnitate(CanvasFundal, x, y - 2);
	}
	if (ExistaHex(x, y, STGSUS)) {
		int ax = GetX(x, y, STGSUS);
		int ay = GetY(x, y, STGSUS);
		_PuneUnitate(CanvasFundal, ax, ay);
	}
	if (ExistaHex(x, y, DRPSUS)) {
		int ax = GetX(x, y, DRPSUS);
		int ay = GetY(x, y, DRPSUS);
		_PuneUnitate(CanvasFundal, ax, ay);
	}
	// desen hex
	if (game.ShowHexes || felhex == 1 || felhex == 3 || felhex == 4) {
		RenderSingleHex(CanvasFundal, x, y, felhex);
	}
	if (!ShowPlayer && game.teren[x][y]) { // _DesenHex(CanvasFundal,x,y,4);
	}
	// desen unitate
	if (ShowPlayer && game.teren[x][y]) {
		SoldierDraw(*Player[juc]->army_slots[lot], ImagUnit, CanvasFundal);
	}
	if (ExistaHex(x, y, STGJOS)) {
		int ax = GetX(x, y, STGJOS);
		int ay = GetY(x, y, STGJOS);
		_PuneUnitate(CanvasFundal, ax, ay);
	}
	if (ExistaHex(x, y, DRPJOS)) {
		int ax = GetX(x, y, DRPJOS);
		int ay = GetY(x, y, DRPJOS);
		_PuneUnitate(CanvasFundal, ax, ay);
	}
	// desen fig jos jos
	if (y < 5) {
		_PuneUnitate(CanvasFundal, x, y + 2);
	}
	CanvasLucru->CopyRect(FigRect, CanvasFundal, FigRect);
	Canvas->CopyRect(FigRect, CanvasLucru, FigRect);
}

// ---------------------------------------------------------------------------
inline void TBattleForm::SelecteazaUrmator() {
	int k = Victorie();
	if (!k) {
		int exJuc = SelectedPlayer;
		int exSlot = SelectedSlot;
		// setari pt. mutari
		if (Player[SelectedPlayer]->army_slots[SelectedSlot]->alive)
			Player[SelectedPlayer]->army_slots[SelectedSlot]->MovesLeft =
				Player[SelectedPlayer]->army_slots[SelectedSlot]->MovesMax;
		// cautarea urmatorului
		bool gasit = 0;
		do {
			if (SelectedPlayer == 0)
				SelectedPlayer = 1;
			else // al doilea juc
				if (SelectedSlot < Player[SelectedPlayer]->angajati) {
				SelectedSlot++;
				SelectedPlayer = 0;
			}
			else {
				SelectedSlot = 0;
				SelectedPlayer = 0;
			}
			if (__ExistaPlayer(SelectedPlayer, SelectedSlot))
				if (Player[SelectedPlayer]->army_slots[SelectedSlot]->alive)
					gasit = 1;
		}
		while (!gasit);
		// setari pt retaliations
		Player[SelectedPlayer]->army_slots[SelectedSlot]->Retal =
			Player[SelectedPlayer]->army_slots[SelectedSlot]->RetalNum;
		// matricea selected e deja plina => s-o golim
		_InitializariMatriceS();
		// desenari de curatare a formului daca cel dinainte a fost human
		if (Player[exJuc]->control == HUMAN) {
			CanvasFundal->Draw(10, 10, ImagineTeren->Picture->Bitmap);
			if (game.ShowHexes) {
				_DesenHexuri();
			}
			else {
				CanvasLucru->CopyRect(battleRect, CanvasFundal, battleRect);
			}
			_DesenUnitati();
			Canvas->CopyRect(battleRect, CanvasLucru, battleRect);
		}
		else {
			Selecteaza(Player[exJuc]->army_slots[exSlot]->x,
				Player[exJuc]->army_slots[exSlot]->y, 1, 0);
		}
		Selecteaza(Player[SelectedPlayer]->army_slots[SelectedSlot]->x,
			Player[SelectedPlayer]->army_slots[SelectedSlot]->y, 1, 1);
		// desenari (in asteptarea noii comenzi) valabil doar pt control uman
		if (Player[SelectedPlayer]->control == HUMAN) {
			int mt = Player[SelectedPlayer]->army_slots[SelectedSlot]
				->MovesLeft;
			SeteazaHex(Player[SelectedPlayer]->army_slots[SelectedSlot]->x,
				Player[SelectedPlayer]->army_slots[SelectedSlot]->y, mt);
			if (Player[SelectedPlayer]->army_slots[SelectedSlot]
				->Ranged && Player[SelectedPlayer]->army_slots[SelectedSlot]
				->Ammo > 0)
				SeteazaHexArcas(Player[SelectedPlayer]->army_slots[SelectedSlot]
				->x, Player[SelectedPlayer]->army_slots[SelectedSlot]->y);
			DesenHexuriSelectate();
			Canvas->CopyRect(battleRect, CanvasLucru, battleRect);
			_CursoareSet0();
		}
		if (SelectedPlayer == 1 && Player[1]->control == COMPUTER) {
			game.WaitingForOrder = false;
		}
		else {
			game.WaitingForOrder = true;
		}
		if (!game.WaitingForOrder) { // adica e computer
			AITimer->Enabled = true; // adica e gata sa recpeteze ordinele
		}
	}
	else {
		std::string message("Battle won by ");
		message.append(Player[k - 1]->nume.c_str());
		message.append("\nCongratulations to the winner!");
		MessageDlg(message.c_str(), mtInformation, TMsgDlgButtons() << mbOK, 0);
		Close();
	}
}

// ---------------------------------------------------------------------------
inline void TBattleForm::SeteazaHex(int x, int y, int mut) {
	if (mut <= 0) {
		return;
	}
	for (int i = STGSUS; i <= DRPSUS; i++)
		if (ExistaHex(x, y, i)) {
			int ax = GetX(x, y, i);
			int ay = GetY(x, y, i);
			if (game.teren[ax][ay]) {
				if (game.teren[ax][ay] / 20 != SelectedPlayer) {
					game.selected[ax][ay] = 2;
				}
				else {
					game.selected[ax][ay] = 0;
				}
			}
			else {
				game.selected[ax][ay] = 1;
				SeteazaHex2(ax, ay, mut - 1, i);
			}
		}
}

// ---------------------------------------------------------------------------
inline void TBattleForm::SeteazaHex2(int x, int y, int mut, int exdir) {
	if (mut <= 0) {
		return;
	}
	for (int i = STGSUS; i <= DRPSUS; i++)
		if (i != (exdir + 3) % 6)
			if (ExistaHex(x, y, i)) {
				int ax = GetX(x, y, i);
				int ay = GetY(x, y, i);
				if (game.teren[ax][ay]) {
					if (game.teren[ax][ay] / 20 != SelectedPlayer) {
						game.selected[ax][ay] = 2;
					}
					else {
						game.selected[ax][ay] = 0;
					}
				}
				else {
					game.selected[ax][ay] = 1;
					SeteazaHex2(ax, ay, mut - 1, i);
				}
			}
}

// ---------------------------------------------------------------------------
inline void TBattleForm::SeteazaHexArcas(int x, int y) {
//TODO: move this to game class
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 7; j++)
			if (!game.selected[i][j] && game.teren[i][j])
				if (game.teren[i][j] / 20 != game.teren[x][y] / 20)
					game.selected[i][j] = 2;
}

// ---------------------------------------------------------------------------
inline void TBattleForm::ShowComment(int juc, int lot, int damage, int position)
{
	TRect cpyRect(50, 550 + 25*position, 750, 575 + 25*position);
	if (!position) {
		cpyRect.Bottom = 600;
	}
	CanvasLucru->Font->Color = (TColor)RGB(255, 175, 75);
	CanvasLucru->Font->Size = 10;
	CanvasLucru->Font->Name = "MS Reference Serif";
	CanvasLucru->Brush->Style = bsClear;
	CanvasLucru->CopyRect(cpyRect, CanvasFundal, cpyRect);
	char buf[255];
	if (position == 0)
		if (damage > 0) {
			sprintf(buf,
				"The %s succesfully attacked the enemy, dealing %d points of damage",
				Player[juc]->army_slots[lot]->GetNume().c_str(), damage);
		}
		else {
			sprintf(buf,
				"The %s tried to attack the enemy, but failed miserably",
				Player[juc]->army_slots[lot]->GetNume().c_str());
		}
	else {
		if (damage > 0)
			sprintf(buf,
			"The %s succesfully retaliated against the enemy, dealing %d points of damage",
			Player[juc]->army_slots[lot]->GetNume().c_str(), damage);
		else {
			sprintf(buf, "The %s tried to retaliate, but failed miserably",
				Player[juc]->army_slots[lot]->GetNume().c_str());
		}
	}
	CanvasLucru->TextOut(75, 555 + 20*position, buf);
	Canvas->CopyRect(cpyRect, CanvasLucru, cpyRect);
}

// ---------------------------------------------------------------------------
inline void TBattleForm::StergeHexuriSelectate() {
	for (int j = 0; j < 7; j++)
		for (int i = 0; i < 9; i++)
			if (game.selected[i][j]) {
				game.selected[i][j] = 0;
				DesenHexCopy(i, j, 0);
			}
}

// ---------------------------------------------------------------------------
int TBattleForm::Victorie() {
	int ret = 0;
	int i, alv;
	alv = 0;
	for (i = 0; i <= Player[0]->angajati; i++)
		if (Player[0]->army_slots[i]->alive)
			alv = 1;
	if (!alv)
		ret = 2;
	alv = 0;
	for (i = 0; i <= Player[1]->angajati; i++)
		if (Player[1]->army_slots[i]->alive)
			alv = 1;
	if (!alv)
		ret = 1;
	return ret;
}
// ---------------------------------------------------------------------------
// #pragma gusend.WINApi32.non-xp,mmxtension
// ---------------------------------------------------------------------------
// Gusty 04 sept. 2002 ; 11:12 PM finished first version of AI

void __fastcall TBattleForm::Surrender1Click(TObject *Sender) {
	int juc = SelectedPlayer;
	for (int i = 0; i <= Player[juc]->angajati; i++)
		Player[juc]->army_slots[i]->alive = false;
	SelecteazaUrmator();
}
// ---------------------------------------------------------------------------
