//---------------------------------------------------------------------------
#ifndef UnitMesajH
#define UnitMesajH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ImgList.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include "UnitMain.h"
#include <System.ImageList.hpp>

#include "src/soldier.h"

//---------------------------------------------------------------------------
class TMesaj : public TForm
{__published:	// IDE-managed Components
        TImageList *ListaImagini;
        TImage *ImagineFundal;
        TImageList *SmallIcons;
        void __fastcall FormPaint(TObject *Sender);
private:	// User declarations
        Soldier* soldier_;
        void Deseneaza();
public:		// User declarations
		__fastcall TMesaj(TComponent* Owner, Soldier* soldier);
};
//---------------------------------------------------------------------------
extern PACKAGE TMesaj *Mesaj;
//---------------------------------------------------------------------------
#endif
