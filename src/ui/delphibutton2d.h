//#pragma once
//
//#include "src/coord.h"
//
//class DelphiButton2D {
//protected:
//	Coord position;
//	Coord size;
//	int left;
//	int top;
//	int FBottom;
//	int FWidth;
//	int FHeight;
//	int FPhase;
//	int FIndex;
//
//	void SeteazaSTG(int valoare);
//	void SeteazaSUS(int valoare);
//	void SeteazaDRP(int valoare);
//	void SeteazaJOS(int valoare);
//	void SeteazaWidth(int valoare);
//	void SeteazaHeight(int valoare);
//	void SeteazaFaza(int valoare);
//
//public:
//	DelphiButton2D() {
//		Left = 0;
//		Top = 0;
//		Width = 10;
//		Height = 10;
//		Phase = 0;
//	}
//	__property int Left = {read = FLeft, write = SeteazaSTG};
//	__property int Top = {read = FTop, write = SeteazaSUS};
//	__property int Right = {read = FRight, write = SeteazaDRP};
//	__property int Bottom = {read = FBottom, write = SeteazaJOS};
//	__property int Height = {read = FHeight, write = SeteazaHeight};
//	__property int Width = {read = FWidth, write = SeteazaWidth};
//	__property int Phase = {read = FPhase, write = SeteazaFaza};
//
//	TRect GetRect() {
//		TRect ret(Left, Top, Right, Bottom);
//		return ret;
//	}
//
//	int VisibleIndex;
//	int SelectedIndex;
//	int PressedIndex;
//
//	void Draw(TImageList *lista, TCanvas *UnCanvas) {
//		lista->Draw(UnCanvas, Left, Top, FIndex, true);
//	}
//};
