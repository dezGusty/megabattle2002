//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//constante control
#define COMPUTER 1
#define HUMAN 0

// C/C++ includes
#include <string>
#include <sstream>
#include <vector>

// Other includes in this project.
#include "src/terrain_type.h"
#include "src/filehelperfunctions.h"
#include "src/screen2d.h"

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

    Screen2D::StretchSize = Coord{ ClientWidth, ClientHeight };

	SetUpGameWindow();
}

// ---------------------------------------------------------------------------
void __fastcall TBattleForm::FormClose(TObject *Sender, TCloseAction &Action) {
	SelectObject(backgrounddc, bkbmp);
	DeleteObject(bkbmp);
	SelectObject(workdc, wkbmp);
	DeleteObject(wkbmp);

	SelectObject(stretchdc, stretchbmp);
	DeleteObject(stretchbmp);

	DeleteDC(backgrounddc);
	DeleteDC(workdc);
	DeleteDC(stretchdc);
}

// ---------------------------------------------------------------------------

void TBattleForm::SetUpGameWindow(){
	// TODO: separate screen size and root (non-upscaled) window size

	Width = Screen2D::StretchSize.x;
	Height = Screen2D::StretchSize.y;

	Left = 0;
	Top = 0;
	game.gamePhase = GamePhase::LoadingScreen;
    game.AssignHeroFactory(this);
	MouseSelectX = -84;
	MouseSelectY = -84;
	randomize();
	_InitializariSunete();
	_InitializariFundal();
}

//---------------------------------------------------------------------------
void __fastcall TBattleForm::FormKeyDown(TObject *Sender, WORD &Key,
	TShiftState Shift) {
	if (game.gamePhase == GamePhase::LoadingScreen) {
		IntraInJoc();
	}
}

//---------------------------------------------------------------------------
void __fastcall TBattleForm::FormMouseDown(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int mouse_x, int mouse_y) {
	if (game.gamePhase == GamePhase::LoadingScreen) {
		IntraInJoc();
		return;
	}

	int mouse_button_idx = -1;
	if (Button == TMouseButton::mbLeft) {
		mouse_button_idx = 0;
	}
	else if (Button == TMouseButton::mbRight) {
		mouse_button_idx = 1;
	}
	ui->OnMouseDown(mouse_x, mouse_y,  mouse_button_idx);
	if (game.WaitingForOrder) {
		Coord map_coords = Screen2D::GetGameHexFromScreenCoords(Coord{mouse_x, mouse_y});

		if (Button == mbLeft) {
			if (mouse_x > 20 && mouse_x < 780 && mouse_y > 110 && mouse_y < 530 && Cursor != TCursor(3))
			{
				// mutare
				if (Cursor == TCursor(1)) {
					if (Screen2D::CanMapScreenCoordsToMap(Coord{mouse_x, mouse_y})) {
						Muta(map_coords.x, map_coords.y);
					}
				}
				// atac
				if (Cursor == TCursor(2)) {
					if (Screen2D::CanMapScreenCoordsToMap(Coord{mouse_x, mouse_y})) {
						if (game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]
							->Ranged && game.players_[game.active_player_index_]->army_slots
							[game.active_unit_index_]->Ammo > 0) {
							AtacArcas(map_coords.x, map_coords.y);
						}
						else {
							AtacNormal(map_coords.x, map_coords.y);
						}
					}
				}
			}
		}

		if (Button == mbRight) {
			if (mouse_x > 20 && mouse_x < 780 && mouse_y > 110 && mouse_y < 530)
				if (Screen2D::CanMapScreenCoordsToMap(Coord{mouse_x, mouse_y})) {
					if (game.teren[map_coords.x][map_coords.y]) {
						int tjuc = game.teren[map_coords.x][map_coords.y] / 20;
						int tlot = game.teren[map_coords.x][map_coords.y] % 10;

						TMesaj *UnMesaj = new TMesaj(this, tjuc, tlot, game.players_[tjuc]->army_slots[tlot]);
						UnMesaj->ShowModal();
						delete UnMesaj;
					}
					else {
						int mt =
							game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]
							->MovesLeft;

						bool pathWasFound = false;
						Coord source_coord = Coord {game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]->x, game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]->y};
						std::vector<Coord> debug_path = game.SearchPathOnMap(
							source_coord, map_coords, mt, pathWasFound);

						if (pathWasFound) {
							for (auto coord : debug_path) {
								RenderSingleHex(Canvas, coord.x, coord.y, 2);
							}
						}
					}
				}
		}
	}
}

//---------------------------------------------------------------------------
void __fastcall TBattleForm::FormMouseMove(TObject *Sender, TShiftState Shift,
	int mouse_x, int mouse_y) {
	if (!game.WaitingForOrder) {
		return;
	}

	if (Screen2D::CanMapScreenCoordsToMap(Coord{mouse_x, mouse_y})) {
		Coord map_coords = Screen2D::GetGameHexFromScreenCoords(Coord{mouse_x, mouse_y});
		switch (game.selected[map_coords.x][map_coords.y]) {
		case 1:
			if (MouseSelectX != map_coords.x || MouseSelectY != map_coords.y) {
				if (MouseSelectX != -84) {
					DesenHexCopy(MouseSelectX, MouseSelectY, 2);
				}
				MouseSelectX = map_coords.x;
				MouseSelectY = map_coords.y;
				RenderSingleHex(Canvas, MouseSelectX, MouseSelectY, 4);
				RenderSingleHex(CanvasLucru, MouseSelectX, MouseSelectY, 4);
				Cursor = TCursor(1);
			}
			break;
		case 2: {
				Cursor = TCursor(2);
				if (MouseSelectX != -84) {
					DesenHexCopy(MouseSelectX, MouseSelectY, 2);
				}

				MouseSelectX = -84;
			} break;
		case 0:
			_CursoareSet0();
			break;
		}
	}
	else {
		_CursoareSet0();
	}

    this->ui->OnMouseMove(mouse_x, mouse_y);
}

//---------------------------------------------------------------------------
void __fastcall TBattleForm::FormMouseUp(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int mouse_x, int mouse_y) {
	if (game.WaitingForOrder) {

		int mouse_button_idx = -1;
		if (Button == TMouseButton::mbLeft) {
			mouse_button_idx = 0;
		}
		else if (Button == TMouseButton::mbRight) {
			mouse_button_idx = 1;
		}
		ui->OnMouseUp(mouse_x, mouse_y, mouse_button_idx);
	}
}

//---------------------------------------------------------------------------
void __fastcall TBattleForm::FormPaint(TObject *Sender) {

	TRect stretchedRect(0, 0, Screen2D::StretchSize.x, Screen2D::StretchSize.y);
	Canvas->CopyRect(stretchedRect, CanvasLucru, allRect);
}

// ---------------------------------------------------------------------------
void __fastcall TBattleForm::AITimerTimer(TObject *Sender) {
	AIAflaOrdin();
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TBattleForm::_InitializariFundal() {
	TRect temp(0, 0, Screen2D::RootSize.x, Screen2D::RootSize.y);

	allRect = temp;
	battleRect = temp;
	battleRect.Bottom -= 50;

	CanvasFundal = new TCanvas;
	CanvasLucru = new TCanvas;
	stretchedCanvas = new TCanvas();

	// Setup pt canvasul de fundal
	/* HDC */ backgrounddc = CreateCompatibleDC(Canvas->Handle);
	/* HBITMAP */
	bkbmp = CreateCompatibleBitmap(Canvas->Handle, Screen2D::RootSize.x, Screen2D::RootSize.y);
	SelectObject(backgrounddc, bkbmp);
	CanvasFundal->Handle = backgrounddc;
	// Setup pt canvasul de lucru
	/* HDC */ workdc = CreateCompatibleDC(Canvas->Handle);
	/* HBITMAP */
	wkbmp = CreateCompatibleBitmap(Canvas->Handle, Screen2D::RootSize.x, Screen2D::RootSize.y);
	SelectObject(workdc, wkbmp);

	// Setup the stretch canvas
	stretchdc = CreateCompatibleDC(Canvas->Handle);
	stretchbmp = CreateCompatibleBitmap(Canvas->Handle, ClientWidth, ClientHeight);
	SelectObject(stretchdc, stretchbmp);
    stretchedCanvas->Handle = stretchdc;

	CanvasLucru->Handle = workdc;

    this->ui = new DelphiUI(CanvasFundal, ImagButon);

	RenderLoadingPhaseMessage(CanvasFundal);
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
	case TerrainType::PurePink:
		if (doesFileExist("./img/pink.bmp")){
			ImagineTeren->Picture->LoadFromFile("./img/pink.bmp");
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
	int cell_type) {

	auto cell_width = 80;
	auto cell_height = 60;
	auto output_coord_x = 20 + cell_width * cell_x;
	auto output_coord_y = 100 + cell_height * cell_y;
	auto x_axis_correction = 0;

	if (cell_y % 2 == 1) {
		x_axis_correction = 40;
	}

	ImagHex->Draw(UnCanvas, x_axis_correction + output_coord_x, output_coord_y,
		cell_type, true);

	if (game.ShowDebugIndices) {
		std::stringstream ss;
		ss << cell_x << ", " << cell_y;
		std::string coord_text = ss.str();

		UnCanvas->TextOut(
			x_axis_correction + output_coord_x + cell_width / 2,
			output_coord_y + cell_height / 2,
			coord_text.c_str());
	}
}

// ---------------------------------------------------------------------------
void TBattleForm::_DesenHexuri() {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
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
	for (int j = 0; j < MAP_HEIGHT; j++) {
		for (int i = 0; i < MAP_WIDTH; i++) {
			if (game.teren[i][j]) {
				int tjuc = game.teren[i][j] / 20;
				int tlot = game.teren[i][j] % 10;
				SoldierDraw(*game.players_[tjuc]->army_slots[tlot], ImagUnit,
					CanvasLucru);
			}
		}
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
	game.PlacePlayerArmyOnTerrainMatrix(*game.players_[0]);
	game.PlacePlayerArmyOnTerrainMatrix(*game.players_[1]);
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

	if (game.teren[x][y] && game.active_player_index_ == game.teren[x][y]
		/ 20 && game.active_unit_index_ == game.teren[x][y] % 10) {
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
		SoldierDraw(*game.players_[tjuc]->army_slots[tlot], ImagUnit, UnCanvas);
	}
}

//---------------------------------------------------------------------------
void TBattleForm::AIAflaOrdin() {
	AITimer->Enabled = false;
	int tempx, tempy;
	int mt = game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]
		->MovesLeft;
	game.MarkSelectionOnCachedMap(Coord
	{game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]->x,
			game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]->y
	}, mt, game.active_player_index_);

	if (game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]
		->Ranged && game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]
		->Ammo > 0) {
		game.MarkCellForRangedAttack
			(game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]->x,
			game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]->y);
	}

	AI_find_target(tempx, tempy);
	if (tempx == -1 && tempy == -1) // adica nu s-a gasit vreo tinta de atacat;
	{
		AI_GasesteMutare(tempx, tempy);
		Muta(tempx, tempy);
	}
	else {
		if (game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]
			->Ranged && game.players_[game.active_player_index_]->army_slots
			[game.active_unit_index_]->Ammo > 0) {
			AtacArcas(tempx, tempy);
		}
		else {
			AtacNormal(tempx, tempy);
		}
	}
}

// ---------------------------------------------------------------------------
void TBattleForm::AI_find_target(int &tempx, int &tempy) {
	int oponent = (game.active_player_index_ + 1) % 2;
	float raport = 500; // din cate lovituri iese un kill;
	int meddmg = (game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]->DamageMax +
		game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]->DamageMin) / 2;
	int curent; // slotul inamicului selectat
	int i, j; // pt parcurgerea matricei
	for (i = 0; i < MAP_WIDTH; i++)
		for (j = 0; j < MAP_HEIGHT; j++)
			if (game.selected[i][j] == 2 && game.teren[i][j] && game.teren[i][j]
				/ 20 == oponent) {
				int lot = game.teren[i][j] % 10;
				int k = meddmg;
				k -= game.players_[oponent]->army_slots[lot]->Armor;
				k -= k * game.players_[oponent]->army_slots[lot]->Protection / 100;
				if (game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]
					->Ranged && game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]
					->Ammo == 0)
					k /= 2;
				if (k <= 0)
					k = 1;
				float tempraport = game.players_[oponent]->army_slots[lot]->Hp / k;
				if (raport > tempraport) {
					raport = tempraport;
					curent = lot;
				}
			}
	if (raport != 500) {
		tempx = game.players_[oponent]->army_slots[curent]->x;
		tempy = game.players_[oponent]->army_slots[curent]->y;
	}
	else {
		tempx = -1;
		tempy = -1;
	}
}

// ---------------------------------------------------------------------------
void TBattleForm::AI_GasesteMutare(int &tempx, int &tempy) {
	int oponent = (game.active_player_index_ + 1) % 2;
	float raport = 500; // din cate lovituri iese un kill;
	int meddmg = (game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]->DamageMax +
		game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]->DamageMin) / 2;
	int curent; // slotul inamicului selectat

	for (int lot = 0; lot <= game.players_[oponent]->angajati; lot++) {
		if (game.players_[oponent]->army_slots[lot]->alive) {
			int k = meddmg;
			k -= game.players_[oponent]->army_slots[lot]->Armor;
			k -= k * game.players_[oponent]->army_slots[lot]->Protection / 100;
			if (game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]
				->Ranged && game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]
				->Ammo == 0)
				k /= 2;
			if (k <= 0)
				k = 1;
			float tempraport = game.players_[oponent]->army_slots[lot]->Hp / k;
			if (raport > tempraport) {
				raport = tempraport;
				curent = lot;
			}
		}
	}

	int tx = game.players_[oponent]->army_slots[curent]->x;
	int ty = game.players_[oponent]->army_slots[curent]->y;
	int x = game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]->x;
	int y = game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]->y;
	Coord selectedCell{x, y};
	Coord moveCell{x, y};
	Coord opponentCell{tx, ty};

	int mut = game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]->MovesLeft;
	for (int i = 0; i < mut; i++) {
		int aok = 0;
		if (moveCell.y > opponentCell.y) {
			if (moveCell.x > opponentCell.x) {
				if (game.IsNeighbourTerrainAvailable(moveCell, TOPLEFT)) {
					moveCell = game.GetNeighbourCell(moveCell, TOPLEFT);
					aok = 1;
				}
			}
			else {
				if (game.IsNeighbourTerrainAvailable(moveCell, TOPRIGHT)) {
					moveCell = game.GetNeighbourCell(moveCell, TOPRIGHT);
					aok = 1;
				}
			}
		}
		if (!aok && moveCell.y == opponentCell.y) {
			if (moveCell.x > opponentCell.x) {
				if (game.IsNeighbourTerrainAvailable(moveCell, LEFT)) {
					moveCell = game.GetNeighbourCell(moveCell, LEFT);
					aok = 1;
				}
			}
			else {
				if (game.IsNeighbourTerrainAvailable(moveCell, RIGHT)) {
					moveCell = game.GetNeighbourCell(moveCell, RIGHT);
					aok = 1;
				}
			}
		}

		if (!aok && moveCell.y < ty) {
			if (moveCell.x > tx) {
				if (game.IsNeighbourTerrainAvailable(moveCell, BOTLEFT)) {
					moveCell = game.GetNeighbourCell(moveCell, BOTLEFT);
					aok = 1;
				}
			}
			else {
				if (game.IsNeighbourTerrainAvailable(moveCell, BOTRIGHT)) {
					moveCell = game.GetNeighbourCell(moveCell, BOTRIGHT);
					aok = 1;
				}
			}
		}
	}
	tempx = moveCell.x;
	tempy = moveCell.y;
}

//---------------------------------------------------------------------------
void TBattleForm::AtacArcas(int tintax, int tintay) {
	if (game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]->Ammo > 0) {
		game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]->Ammo--;
		ExecutaAtac(tintax, tintay, true);
		::Sleep(100);
		SelecteazaUrmator();
	}
}

//---------------------------------------------------------------------------
void TBattleForm::AtacNormal(int tintax, int tintay) {
	int mt = game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]->MovesLeft;

	Coord source_coord = {
		game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]->x,
			game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]->y};
	bool pathWasFound = false;

	std::vector<Coord>atack_path = game.SearchPathOnMap(
		source_coord, Coord{tintax, tintay}, mt, pathWasFound);
	// we have the full path to the target.
	// remove the target from the array so that the current unit won't go on top of it.
	if (pathWasFound) {
        atack_path.pop_back();
	}

	StergeHexuriSelectate();
	for (auto coord : atack_path) {
		MutaUnitate(coord.x, coord.y);
		::Sleep(75);
	}

	ExecutaAtac(tintax, tintay, false);
	DesenHexuriSelectate();
	::Sleep(100);
	SelecteazaUrmator();
}

Hero* TBattleForm::CreateHero(const std::string& input) {
	return new THero(input);
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
	if (y > 1) {
		_PuneUnitate(CanvasFundal, x, y - 2); // desen sus sus
	}

	if (game.DoesNeighbourExist(Coord {x, y}, TOPLEFT)) {
		Coord neigh = game.GetNeighbourCell({x, y}, TOPLEFT);
		_PuneUnitate(CanvasFundal, neigh.x, neigh.y);
	}

	if (game.DoesNeighbourExist(Coord {x, y}, TOPRIGHT)) {
		Coord neigh = game.GetNeighbourCell({x, y}, TOPRIGHT);
		_PuneUnitate(CanvasFundal, neigh.x, neigh.y);
	}

	_PuneUnitate(CanvasFundal, x, y);

	if (game.DoesNeighbourExist(Coord {x, y}, BOTLEFT)) {
		Coord neigh = game.GetNeighbourCell({x, y}, BOTLEFT);
		_PuneUnitate(CanvasFundal, neigh.x, neigh.y);
	}

	if (game.DoesNeighbourExist(Coord {x, y}, BOTRIGHT)) {
		Coord neigh = game.GetNeighbourCell({x, y}, BOTRIGHT);
		_PuneUnitate(CanvasFundal, neigh.x, neigh.y);
	}

	if (y < 5) {
		_PuneUnitate(CanvasFundal, x, y + 2); // desen jos jos
	}
	CanvasLucru->CopyRect(FigRect, CanvasFundal, FigRect);
	Canvas->CopyRect(FigRect, CanvasLucru, FigRect);
}

//---------------------------------------------------------------------------
inline void TBattleForm::DesenHexuriSelectate() {
	for (int j = 0; j < MAP_HEIGHT; j++)
		for (int i = 0; i < MAP_WIDTH; i++) {
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
	int juc = game.active_player_index_;
	int lot = game.active_unit_index_;
	int k = random(100) + 1;
	// MessageDlg(k,mtInformation,TMsgDlgButtons() <<mbOK,0);
	if (k <= game.players_[juc]->army_slots[lot]->ChancesToHit) {
		int auxx = game.players_[juc]->army_slots[lot]->DamageMax -
			game.players_[juc]->army_slots[lot]->DamageMin;
		k = game.players_[juc]->army_slots[lot]->DamageMin + random(auxx);
		k -= game.players_[tjuc]->army_slots[tlot]->Armor;
		k -= k * game.players_[tjuc]->army_slots[tlot]->Protection / 100;
		if (game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]->Ranged && game.players_
			[game.active_player_index_]->army_slots[game.active_unit_index_]->Ammo <= 0) {
			k /= 2;
		}

		if (k <= 0) {
			k = 1;
		}

		ShowComment(juc, lot, k, 0);
		if (!range) {
			PlaySoundForAction(SimpleSoundAction::AttackHitMelee);
			DisplayAtac(game.players_[tjuc]->army_slots[tlot]->x,
				game.players_[tjuc]->army_slots[tlot]->y, 0);
		}
		else {
			PlaySoundForAction(SimpleSoundAction::AttackHitRange);
			DisplayAtac(game.players_[tjuc]->army_slots[tlot]->x,
				game.players_[tjuc]->army_slots[tlot]->y, 2);
		}
		game.players_[tjuc]->army_slots[tlot]->Hp -= k;
		if (game.players_[tjuc]->army_slots[tlot]->Hp <= 0) {
			game.teren[tx][ty] = 0;
			game.players_[tjuc]->army_slots[tlot]->alive = false;
			PlaySoundForAction(SimpleSoundAction::Die);
			Selecteaza(tx, ty, false, 0);
		}
	}
	else {
		ShowComment(juc, lot, 0, 0);
		PlaySoundForAction(SimpleSoundAction::AttackMiss);
		DisplayAtac(game.players_[tjuc]->army_slots[tlot]->x,
			game.players_[tjuc]->army_slots[tlot]->y, 1);
	}

	// urmeaza retaliation
	if (!range && game.players_[tjuc]->army_slots[tlot]->alive && game.players_[tjuc]
		->army_slots[tlot]->Retal > 0) {
		::Sleep(150);
		game.players_[tjuc]->army_slots[tlot]->Retal--;
		k = random(100) + 1;
		if (k <= game.players_[tjuc]->army_slots[tlot]->ChancesToHit) {
			k = game.players_[tjuc]->army_slots[tlot]->DamageMin +
				random(game.players_[tjuc]->army_slots[tlot]->DamageMax -
				game.players_[tjuc]->army_slots[tlot]->DamageMin);
			k -= game.players_[juc]->army_slots[lot]->Armor;
			k -= k * game.players_[juc]->army_slots[lot]->Protection / 100;
			if (k <= 0) {
				k = 1;
			}
			ShowComment(tjuc, tlot, k, 1);
			PlaySoundForAction(SimpleSoundAction::AttackHitMelee);
			DisplayAtac(game.players_[juc]->army_slots[lot]->x,
				game.players_[juc]->army_slots[lot]->y, 0);
			game.players_[juc]->army_slots[lot]->Hp -= k;
			if (game.players_[juc]->army_slots[lot]->Hp <= 0) {
				game.teren[game.players_[juc]->army_slots[lot]->x]
					[game.players_[juc]->army_slots[lot]->y] = 0;
				game.players_[juc]->army_slots[lot]->alive = false;
				PlaySoundForAction(SimpleSoundAction::Die);

				Selecteaza(game.players_[juc]->army_slots[lot]->x,
					game.players_[juc]->army_slots[lot]->y, false, 0);
			}
		}
		else {
			ShowComment(tjuc, tlot, 0, 1);
			PlaySoundForAction(SimpleSoundAction::AttackMiss);
			DisplayAtac(game.players_[juc]->army_slots[lot]->x,
				game.players_[juc]->army_slots[lot]->y, 1);
		}
	}
}

//---------------------------------------------------------------------------
void TBattleForm::IntraInJoc() {
	game.LoadBattleIniFile();

	LoadBattleBackgroundPictureForType(game.felteren);

    game.gamePhase = GamePhase::Battle;
	RenderBorderAndBackground();
	if (game.ShowHexes){
		_DesenHexuri();
	}

	CanvasLucru->CopyRect(allRect, CanvasFundal, allRect);
	Canvas->CopyRect(allRect, CanvasLucru, allRect);

	// If there are any entries in the game players, clear them
	game.players_.clear();
	game.players_.emplace_back(this->CreateHero(game.left_player_cfg_file.c_str()));
	game.players_.emplace_back(this->CreateHero(game.right_player_cfg_file.c_str()));

	game.players_[0]->control = HUMAN;
	game.players_[0]->player_id = 0;
	game.players_[1]->player_id = 1;

	if (game.control2){
		game.players_[1]->control = COMPUTER;
	}
	else{
		game.players_[1]->control = HUMAN;
	}

	// set right-hand side player settings
	for (int i = 0; i <= 1; i++) {
		for (int j = 0; j <= game.players_[i]->angajati; j++) {
			if (i == 1) {
				game.players_[i]->army_slots[j]->x = 8;
				game.players_[i]->army_slots[j]->Orientare = 1;
			}
		}
	}

	_CursoareInitializari();

//	game.ResetSelectionMatrix();

	// display logo
	CanvasFundal->Draw(50, 550, logoImage->Picture->Bitmap);
	CanvasLucru->Draw(50, 550, logoImage->Picture->Bitmap);
	Canvas->CopyRect(allRect, CanvasLucru, allRect);
	Joc();
}
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
void TBattleForm::Joc() {
	game.active_player_index_ = 0;
	game.active_unit_index_ = 0;
	game.WaitingForOrder = true;
	CanvasFundal->Draw(10, 10, ImagineTeren->Picture->Bitmap);
	if (game.ShowHexes) {
		_DesenHexuri();
	}

	_InitializariMatrice();
	game.ResetSelectionMatrix();
	_DesenUnitati();
	Canvas->CopyRect(allRect, CanvasLucru, allRect);
	int mt = game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]->MovesLeft;
	game.MarkSelectionOnCachedMap(Coord { game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]->x,
		game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]->y}, mt, game.active_player_index_);

	if (game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]->Ranged && game.players_
		[game.active_player_index_]->army_slots[game.active_unit_index_]->Ammo > 0)
		game.MarkCellForRangedAttack(game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]->x,
		game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]->y);

	DesenHexuriSelectate();
	Selecteaza(game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]->x,
		game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]->y, 1, 1);

	TLogicButton* btn1 = this->ui->CreateAndOwnButton(0, 550, 50, 50);
	btn1->VisibleIndex = 0;
	btn1->SelectedIndex = 1;
	btn1->PressedIndex = 2;
    using std::placeholders::_1;
	std::function<void(int)> cb1 (std::bind(&TBattleForm::OnMenuClicked, this, _1));
	btn1->setCallback(cb1);

	TLogicButton* btn2 = this->ui->CreateAndOwnButton(750, 550, 50, 50);
	btn2->VisibleIndex = 3;
	btn2->SelectedIndex = 4;
	btn2->PressedIndex = 5;
	using std::placeholders::_1;
	std::function<void(int)> cb2 (std::bind(&TBattleForm::OnSkipTurnClicked, this, _1));
	btn2->setCallback(cb2);
}

//---------------------------------------------------------------------------

void TBattleForm::OnSkipTurnClicked(int){
	OrdinSkipTurn();
}

void TBattleForm::OnMenuClicked(int){
	PopupMenu1->Popup(40, 560);
}


//---------------------------------------------------------------------------
inline void TBattleForm::Muta(int newx, int newy) {
	int mt = game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]->MovesLeft;

	bool pathWasFound = false;
	Coord source_coord {game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]->x, game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]->y};

	std::vector<Coord> move_path = game.SearchPathOnMap(
		source_coord, Coord {newx, newy}, mt, pathWasFound);

	StergeHexuriSelectate();
	for (auto coord : move_path) {
		MutaUnitate(coord.x, coord.y);
		::Sleep(50);
	}

	DesenHexuriSelectate();
	::Sleep(100);
	SelecteazaUrmator();
}

// ---------------------------------------------------------------------------
inline void TBattleForm::MutaUnitate(int newx, int newy) {
	int tx = game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]->x;
	int ty = game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]->y;
	if (newx != tx || newy != ty) {
		PlaySoundForAction(SimpleSoundAction::Move);
		Selecteaza(tx, ty, false, 0);
		game.teren[tx][ty] = 0;
		game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]->x = newx;
		game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]->y = newy;
		game.teren[newx][newy] = game.active_player_index_ * 10 + game.active_unit_index_ + 10;
		Selecteaza(newx, newy, true, 0);
	}
}

// ---------------------------------------------------------------------------
void TBattleForm::OrdinSkipTurn() {
	SelecteazaUrmator();
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

	if (game.DoesNeighbourExist({x, y}, TOPLEFT)) {
		Coord neigh = game.GetNeighbourCell({x, y}, TOPLEFT);
		_PuneUnitate(CanvasFundal, neigh.x, neigh.y);
	}

	if (game.DoesNeighbourExist({x, y}, TOPRIGHT)) {
		Coord neigh = game.GetNeighbourCell({x, y}, TOPRIGHT);
		_PuneUnitate(CanvasFundal, neigh.x, neigh.y);
	}

	// desen hex
	if (game.ShowHexes || felhex == 1 || felhex == 3 || felhex == 4) {
		RenderSingleHex(CanvasFundal, x, y, felhex);
	}

    // desen unitate
	if (ShowPlayer && game.teren[x][y]) {
		SoldierDraw(*game.players_[juc]->army_slots[lot], ImagUnit, CanvasFundal);
	}

	if (game.DoesNeighbourExist({x, y}, BOTLEFT)) {
		Coord neigh = game.GetNeighbourCell({x, y}, BOTLEFT);
		_PuneUnitate(CanvasFundal, neigh.x, neigh.y);
	}

	if (game.DoesNeighbourExist(Coord {x, y}, BOTRIGHT)) {
		Coord neigh = game.GetNeighbourCell({x, y}, BOTRIGHT);
		_PuneUnitate(CanvasFundal, neigh.x, neigh.y);
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
		int exJuc = game.active_player_index_;
		int exSlot = game.active_unit_index_;
		// setari pt. mutari
		if (game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]->alive)
			game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]->MovesLeft =
				game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]->MovesMax;

		// find next unit to select.
		// cautarea urmatorului
		bool gasit = 0;
		do {
			if (game.active_player_index_ == 0){
				game.active_player_index_ = 1;
			}
			else {
				// al doilea juc
					if (game.active_unit_index_ < game.players_[game.active_player_index_]->angajati) {
					game.active_unit_index_++;
					game.active_player_index_ = 0;
				}
				else {
					game.active_unit_index_ = 0;
					game.active_player_index_ = 0;
				}
			}

			// is there a player unit ?
			if (game.active_player_index_ <= 1
				&& game.active_unit_index_ <= game.players_[game.active_player_index_]->angajati) {
				if (game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]->alive) {
					gasit = 1;
				}
			}
		}
		while (!gasit);

		// setari pt retaliations
		game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]->Retal =
			game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]->RetalNum;
		// matricea selected e deja plina => s-o golim
		game.ResetSelectionMatrix();
		// desenari de curatare a formului daca cel dinainte a fost human
		if (game.players_[exJuc]->control == HUMAN) {
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
			Selecteaza(game.players_[exJuc]->army_slots[exSlot]->x,
				game.players_[exJuc]->army_slots[exSlot]->y, 1, 0);
		}
		Selecteaza(game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]->x,
			game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]->y, 1, 1);
		// desenari (in asteptarea noii comenzi) valabil doar pt control uman
		if (game.players_[game.active_player_index_]->control == HUMAN) {
			int mt = game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]
				->MovesLeft;
			game.MarkSelectionOnCachedMap(Coord{game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]->x,
				game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]->y}, mt, game.active_player_index_);
			if (game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]
				->Ranged && game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]
				->Ammo > 0) {
				game.MarkCellForRangedAttack(game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]
				->x, game.players_[game.active_player_index_]->army_slots[game.active_unit_index_]->y);
			}
			DesenHexuriSelectate();
			Canvas->CopyRect(battleRect, CanvasLucru, battleRect);
			_CursoareSet0();
		}
		if (game.active_player_index_ == 1 && game.players_[1]->control == COMPUTER) {
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
		message.append(game.players_[k - 1]->nume.c_str());
		message.append("\nCongratulations to the winner!");
		MessageDlg(message.c_str(), mtInformation, TMsgDlgButtons() << mbOK, 0);
		Close();
	}
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
				game.players_[juc]->army_slots[lot]->GetNume().c_str(), damage);
		}
		else {
			sprintf(buf,
				"The %s tried to attack the enemy, but failed miserably",
				game.players_[juc]->army_slots[lot]->GetNume().c_str());
		}
	else {
		if (damage > 0)
			sprintf(buf,
			"The %s succesfully retaliated against the enemy, dealing %d points of damage",
			game.players_[juc]->army_slots[lot]->GetNume().c_str(), damage);
		else {
			sprintf(buf, "The %s tried to retaliate, but failed miserably",
				game.players_[juc]->army_slots[lot]->GetNume().c_str());
		}
	}
	CanvasLucru->TextOut(75, 555 + 20*position, buf);
	Canvas->CopyRect(cpyRect, CanvasLucru, cpyRect);
}

// ---------------------------------------------------------------------------
inline void TBattleForm::StergeHexuriSelectate() {
	std::vector<Coord> cells = game.GetSelectedCellsOnCachedMap();
	game.ResetSelectionMatrix();
	for (Coord cell : cells) {
		DesenHexCopy(cell.x, cell.y, 0);
	}

}

// ---------------------------------------------------------------------------
int TBattleForm::Victorie() {
	int ret = 0;
	int i, alv;
	alv = 0;
	for (i = 0; i <= game.players_[0]->angajati; i++)
		if (game.players_[0]->army_slots[i]->alive)
			alv = 1;
	if (!alv)
		ret = 2;
	alv = 0;
	for (i = 0; i <= game.players_[1]->angajati; i++)
		if (game.players_[1]->army_slots[i]->alive)
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
	for (int i = 0; i <= game.players_[game.active_player_index_]->angajati; i++) {
		game.players_[game.active_player_index_]->army_slots[i]->alive = false;
    }
	SelecteazaUrmator();
}
// ---------------------------------------------------------------------------
void __fastcall TBattleForm::renderTimerTimer(TObject *Sender)
{
	RenderScene();
}
//---------------------------------------------------------------------------

void TBattleForm::RenderLoadingPhaseMessage(TCanvas* canvas) {
	canvas->Brush->Color = clBlack;
	canvas->Rectangle(0, 0, Screen2D::RootSize.x, Screen2D::RootSize.y);
	canvas->Brush->Color = (TColor)RGB(255, 175, 75);
	canvas->Font->Color = (TColor)RGB(255, 175, 75);
	canvas->Brush->Style = bsClear;
	canvas->TextOut(290, 230, "Megalithic Battle 2 v.2.00 mar.2021");
	canvas->TextOut(331, 250, "A game by Gusty");
	canvas->TextOut(290, 270, "Press a key or click to begin combat");
	std::stringstream ss;
	ss << "Using root screen " << Screen2D::RootSize.x << " x " <<
		Screen2D::RootSize.y << ", " << " displayed as " <<
		Screen2D::StretchSize.x << " x " << Screen2D::StretchSize.y << "." <<
		std::endl;
	canvas->TextOut(290, 290, ss.str().c_str());
}

//---------------------------------------------------------------------------

void TBattleForm::RenderScene(){
	if (game.gamePhase == GamePhase::LoadingScreen) {
		static int x_append = 3;
		static int y_append = 3;

		static int x_draw_unit = 100;
		static int y_draw_unit = 100;

        static int unit_index = random(10);

		if (x_draw_unit <= 0 || x_draw_unit > Screen2D::RootSize.x - 40) {
			x_append = -x_append;
		}
		if (y_draw_unit <= 0 || y_draw_unit > Screen2D::RootSize.y - 100) {
			y_append = -y_append;
		}

		x_draw_unit += x_append;
		y_draw_unit += y_append;

		// Clear the canvas and redraw the text
		RenderLoadingPhaseMessage(CanvasFundal);

        // Draw a unit on the screen.
		ImagUnit->Draw(CanvasFundal, x_draw_unit, y_draw_unit, unit_index, true);
		TRect stretchedRect(0, 0, Screen2D::StretchSize.x, Screen2D::StretchSize.y);
		stretchedCanvas->CopyRect(stretchedRect, CanvasFundal, allRect);
		Canvas->CopyRect(stretchedRect, stretchedCanvas, stretchedRect);
	}
	else if (game.gamePhase == GamePhase::Battle) {
		ui->Render(CanvasLucru);
		Canvas->CopyRect(allRect, CanvasLucru, allRect);
    }
#if 0
	// Draw everything onto the work canvas
	game.gamePhase == GamePhase::LoadingScreen
	if (game.gamePhase == GamePhase::LoadingScreen) {
		RenderBorderAndBackground();
		if (game.ShowHexes){
			_DesenHexuri();
		}
		_DesenUnitati();
		DesenHexuriSelectate();
	}

	// Stretch the contents onto the stretched canvas

	TRect stretchedRect(0, 0, Screen2D::StretchSize.x, Screen2D::StretchSize.y);
	Canvas->CopyRect(stretchedRect, CanvasLucru, allRect);
#endif
}
// ---------------------------------------------------------------------------
