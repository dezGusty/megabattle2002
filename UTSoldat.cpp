// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
#include <vcl.h>
#include <string.h>
#include <io.h>
#include <stdio.h>

#pragma hdrstop

#include "src/soldier.h"
#include "src/hero.h"

void SoldierDraw(const Soldier& soldier, TImageList *lista, TCanvas *UnCanvas) {
	if (soldier.y % 2 == 0) {
		lista->Draw(UnCanvas, 20 + soldier.x * 80, soldier.y * 60 + 20,
			soldier.ImagineAsoc * 2 + soldier.Orientare, true);
	}
	else {
		lista->Draw(UnCanvas, 60 + soldier.x * 80, soldier.y * 60 + 20,
			soldier.ImagineAsoc * 2 + soldier.Orientare, true);
	}
}


class THero : public Hero {
private:
public:
	THero(const std::string& filename) : Hero() {

		if (false == IncarcaErou(filename)) {
			std::string message("The file [");
			message.append(filename);
			message.append("] could not be found.");
			MessageDlg(message.c_str(), mtInformation,
				TMsgDlgButtons() << mbOK, 0);
		}
	}
};
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
#pragma package(smart_init)
