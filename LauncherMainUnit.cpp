//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <algorithm>
#include <string>
#include <sstream>

#include "src/thirdparty/mini/ini.h"
#include "src/terrain_type.h"
#include "src/filehelperfunctions.h"

#include "LauncherMainUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TLauncherForm *LauncherForm;

bool SaveBattleIniFile(std::string ini_file_name, bool show_hexes, bool cpu_control, int terrain_type,
	std::string fisier1, std::string fisier2, bool show_debug_indices) {
	// first, create a file instance
	mINI::INIFile file(ini_file_name);

	// next, create a structure that will hold data
	mINI::INIStructure ini;

	std::stringstream ss;
	ss.str(std::string());

	ss << terrain_type;
	std::string value_terrain_type = ss.str();
	ini["battle"]["terrain"] = value_terrain_type;
	ss.str(std::string());

	ss << static_cast<int>(show_hexes);
	std::string value_show_hexes = ss.str();
	ini["battle"]["show_hexes"] = value_show_hexes;
	ss.str(std::string());

    ss << static_cast<int>(show_debug_indices);
	std::string value_show_debug_indices = ss.str();
	ini["battle"]["show_debug_indices"] = value_show_debug_indices;
	ss.str(std::string());

	ss << static_cast<int>(cpu_control);
    std::string value_cpu_control = ss.str();
	ini["battle"]["right_player_control"] = value_cpu_control;
	ss.str(std::string());

	ini["battle"]["left_side_player_config"] = fisier1;

	ini["battle"]["right_side_player_config"] = fisier2;

	file.write(ini, true);
	return true;
}

//---------------------------------------------------------------------------
__fastcall TLauncherForm::TLauncherForm(TComponent* Owner)
		: TForm(Owner)
		, app_state_initialized(false)
{
}
//---------------------------------------------------------------------------

void __fastcall TLauncherForm::ButtonCloseClick(TObject *Sender)
{Close();
}
//---------------------------------------------------------------------------

void __fastcall TLauncherForm::ButtonLaunchClick(TObject *Sender) {
	SetCurrentDir(InitialGameDir);
	execl("MegaBattle2.exe", "MegaBattle2.exe", NULL);
}

//---------------------------------------------------------------------------
void __fastcall TLauncherForm::Timer1Timer(TObject *Sender)
{//aici lucru
 TRect tmpall(0,0,300,225);
 TRect small(0,0,100,75);
 ImagineFundal->Canvas->CopyRect(small,Image1->Picture->Bitmap->Canvas,cpRect);
 Canvas->StretchDraw(tmpall,ImagineFundal->Picture->Bitmap);
 cpRect.Left+=100;
 cpRect.Right+=100;
 if(cpRect.Left >=500)
   {cpRect.Top+=75;cpRect.Bottom+=75;
    cpRect.Left=0;cpRect.Right=100;}
 if(cpRect.Top>=450)
   {cpRect.Top=0;cpRect.Bottom=75;}
}
//---------------------------------------------------------------------------

bool TLauncherForm::LoadIniFile(const std::string& file_name) {

	// first, create a file instance
	mINI::INIFile file(file_name);

	// next, create a structure that will hold data
	mINI::INIStructure ini;

	// now we can read the file
	file.read(ini);

	std::string::size_type sz;

	// Load the settings

	// # Only for esthetics for now
	// # Terrain type impacts the picture drawn for the background
	// # values
	// #   1: Grass
	// #   2: Dirt
	std::string str_terrain_type = ini.get("battle").get("terrain");
	int num_terrain_type = 1;
	try{
		if (str_terrain_type.length() > 0) {
			num_terrain_type = std::stoi(str_terrain_type, &sz);
		}
	}
	catch(...) {
		// ignore?
    }
	felteren = num_terrain_type;

	// # Shows or hides the hexes
	// # values
	// #   0: don't show hexes
	// #   1: show hexes
	std::string str_show_hexes = ini.get("battle").get("show_hexes");
	ShowHexes = 1;
	try {
		if (str_show_hexes.length() > 0){
			ShowHexes = std::stoi(str_show_hexes, &sz);
		}
	} catch (...) {
		// ignore?
	}

    //# Shows or hides the debug indices
	//# values
	//#   0: don't show indices
	//#   1: show indices
	std::string str_show_dbg_indices = ini.get("battle").get("show_debug_indices");
	ShowDebugIndices = 1;
	try {
		if (str_show_dbg_indices.length() > 0){
			ShowDebugIndices = std::stoi(str_show_dbg_indices, &sz);
		}
	} catch (...) {
		// ignore?
	}

	// # Opponent type (right side player can be either human of CPU controlled; left side player is always HUMAN controlled)
	// # values
	// #   0: HUMAN
	// #   1: CPU
	std::string str_control2 = ini.get("battle").get("right_player_control");
	control2 = 1;
	try{
		if (str_control2.length() > 0) {
			control2 = std::stoi(str_control2, &sz);
		}
	} catch (...) {
        // ignore?
    }

	// # Config file for left side hero and army
	fisier1 = "";
	try {
		fisier1 = ini.get("battle").get("left_side_player_config");
	} catch (...) {
		// ignore
	}

	NumeErou1->Caption = fisier1.c_str();

	// # Config file for right side hero and army
	fisier2 = "";
	try{
		fisier2 = ini.get("battle").get("right_side_player_config");
	} catch (...) {
        // ignore
	}

	NumeErou2->Caption = fisier2.c_str();
	return true;
}

void __fastcall TLauncherForm::FormActivate(TObject *Sender) {
	TRect tmp(0, 0, 100, 75);
	cpRect = tmp;
	Timer1->Enabled = true;
	if (false == doesFileExist("battle.ini")) {
		MessageDlg(
			"The file <Battle.ini> does not exist in \n\the current directory",
			mtInformation, TMsgDlgButtons() << mbOK, 0);
            // optionally, call Close();
	}

	LoadIniFile("battle.ini");
	app_state_initialized = true;


	OpenDialog1->InitialDir = GetCurrentDir();
	InitialGameDir = GetCurrentDir();
	CheckBox1->Checked = control2;
	CheckBox2->Checked = ShowHexes;
    cbShowDebugIndices->Checked = ShowDebugIndices;
	Edit1->Text = felteren;
}

//---------------------------------------------------------------------------
void TLauncherForm::ScrieINI() {
	if (false == app_state_initialized) {
		return;
	}
	bool player_2_cpu_control = CheckBox1->Checked;
	bool show_map_hexes = CheckBox2->Checked;
	AnsiString text_value = Edit1->Text.c_str();
	std::string str_terrain_type = text_value.c_str();

	bool show_debug_indices = cbShowDebugIndices->Checked;

	std::string::size_type sz;
	int num_terrain_type = std::stoi(str_terrain_type, &sz);

	SaveBattleIniFile("battle.ini", show_map_hexes, player_2_cpu_control,
		num_terrain_type, fisier1, fisier2, show_debug_indices);
}

/**
	Bring the file path to a standard format.
	This tries to strip the base directory if provided.
*/
std::string normalize_file_name(const std::string& input, const std::string& base_dir_to_strip){

	std::string result(input);

	// Make the path relative to the current directory (kind of).
	// Strip the first part of the input if it matches.
	// E.g. "C:\games\megabattle\heroes\hero1.dta" with "C:\games\megabattle"
	//  => "\heroes\hero1.dta"
	if (input.rfind(base_dir_to_strip, 0) == 0) {
		// input begins with base_dir_to_strip
		result = result.substr(base_dir_to_strip.length());

		// Check if the remaining string begins with a backslash
		// E.g. "\heroes\hero1.dta" => "heroes\hero1.dta"
		if (result.length() > 0 && *result.begin() == '\\') {
			result = result.substr(1);
		}
	}

	// replace all occurrences of backslash with forward slash
	// E.g. "C:\games\megabattle\heroes\hero1.dta" => "C:/games/megabattle/heroes/hero1.dta"
	// or   "heroes\hero1.dta" => "heroes/hero1.dta"
	std::replace(result.begin(), result.end(), '\\', '/');

	return result;
}

void __fastcall TLauncherForm::NumeErou1Click(TObject *Sender) {
	OpenDialog1->InitialDir = InitialGameDir + "\\heroes";

	if (OpenDialog1->Execute()) {
		AnsiString ansi_file_name = OpenDialog1->FileName.c_str();
		std::string my_file_name = normalize_file_name(ansi_file_name.c_str(), InitialGameDir.c_str());

		fisier1 = my_file_name;
		NumeErou1->Caption = fisier1.c_str();
		ScrieINI();
	}
}
//---------------------------------------------------------------------------


void __fastcall TLauncherForm::NumeErou2Click(TObject *Sender) {
	OpenDialog1->InitialDir = InitialGameDir + "\\heroes";

	if (OpenDialog1->Execute()) {
		AnsiString ansi_file_name = OpenDialog1->FileName.c_str();
		std::string my_file_name = normalize_file_name(ansi_file_name.c_str(), InitialGameDir.c_str());

		fisier2 = my_file_name;
		NumeErou2->Caption = fisier2.c_str();
		ScrieINI();
	}
}
//---------------------------------------------------------------------------


void __fastcall TLauncherForm::FormDestroy(TObject *Sender)
{
//	ScrieINI();
}
//---------------------------------------------------------------------------

void __fastcall TLauncherForm::Edit1Change(TObject *Sender) {
	AnsiString text_value = Edit1->Text.c_str();
	std::string str_terrain_type(text_value.c_str());
	std::string::size_type sz;
	int num_terrain_type = std::stoi(str_terrain_type, &sz);

	if (num_terrain_type < 0 || num_terrain_type > 2) {
		Edit1->Text = 1;
	}

	ScrieINI();
}
// ---------------------------------------------------------------------------
void __fastcall TLauncherForm::CheckBox2Click(TObject *Sender)
{
	ScrieINI();
}
//---------------------------------------------------------------------------

void __fastcall TLauncherForm::CheckBox1Click(TObject *Sender)
{
	ScrieINI();
}
//---------------------------------------------------------------------------
void __fastcall TLauncherForm::cbShowDebugIndicesClick(TObject *Sender)
{
    ScrieINI();
}
//---------------------------------------------------------------------------

