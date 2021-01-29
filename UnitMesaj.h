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
//---------------------------------------------------------------------------
class TMesaj : public TForm
{__published:	// IDE-managed Components
        TImageList *ListaImagini;
        TImage *ImagineFundal;
        TImageList *SmallIcons;
        void __fastcall FormPaint(TObject *Sender);
private:	// User declarations
        void Deseneaza();
public:		// User declarations
        int Juc,Lot;
		int UDamage,UArmor,UProtection,UChance;
        __fastcall TMesaj(TComponent* Owner,int tjuc,int tlot);
};
//---------------------------------------------------------------------------
extern PACKAGE TMesaj *Mesaj;
//---------------------------------------------------------------------------
#endif
