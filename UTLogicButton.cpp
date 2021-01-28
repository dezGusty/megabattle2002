//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "UTLogicButton.h"
//---------------------------------------------------------------------------
void TLogicButton::SeteazaSTG(int valoare)
   {FLeft=valoare;
    FRight=valoare+FWidth;
   }
void TLogicButton::SeteazaSUS(int valoare)
   {FTop=valoare;
    FBottom=valoare+FHeight;
   }
void TLogicButton::SeteazaDRP(int valoare)
   {FRight=valoare;
    FLeft=valoare-FWidth;
   }
void TLogicButton::SeteazaJOS(int valoare)
   {FBottom=valoare;
    FTop=valoare-FHeight;
   }
void TLogicButton::SeteazaWidth(int valoare)
   {FWidth=valoare;
    FRight=FLeft+FWidth;
   }
void TLogicButton::SeteazaHeight(int valoare)
   {FHeight=valoare;
    FBottom=FTop+FHeight;
   }
void TLogicButton::SeteazaFaza(int valoare)
   {FPhase=valoare;//0=visible //1=selected //2=pressed
    switch(FPhase)
     {case 0:FIndex=VisibleIndex;break;
      case 1:FIndex=SelectedIndex;break;
      case 2:FIndex=PressedIndex;break;
     }
   }
//---------------------------------------------------------------------------
#pragma package(smart_init)
