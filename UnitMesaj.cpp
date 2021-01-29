//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "UnitMesaj.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMesaj *Mesaj;
//---------------------------------------------------------------------------
__fastcall TMesaj::TMesaj(TComponent* Owner,int tjuc,int tlot)
        : TForm(Owner)
{Juc=tjuc;
 Lot=tlot;
 Caption="Unit Properties";
}
//---------------------------------------------------------------------------
void TMesaj::Deseneaza()
{Canvas->Brush->Color=(TColor)RGB(0,0,0);
 Canvas->Rectangle(0,0,ClientWidth,ClientHeight);
 int i;
 for(i=10;i<ClientWidth-10;i+=10)
  {ListaImagini->Draw(Canvas,i,0,1,true);
   ListaImagini->Draw(Canvas,i,ClientHeight-10,1,true);}
 for(i=10;i<ClientHeight-10;i+=10)
  {ListaImagini->Draw(Canvas,0,i,2,true);
   ListaImagini->Draw(Canvas,ClientWidth-10,i,2,true); }
 ListaImagini->Draw(Canvas,0,0,0,true);
 ListaImagini->Draw(Canvas,ClientWidth-10,ClientHeight-10,0,true);
 ListaImagini->Draw(Canvas,0,ClientHeight-10,0,true);
 ListaImagini->Draw(Canvas,ClientWidth-10,0,0,true);
 TRect drp(10,10,ClientWidth-10,ClientHeight-10);
 Canvas->StretchDraw(drp,ImagineFundal->Picture->Bitmap);
 ListaImagini->Draw(Canvas,0,70,0,true);
 ListaImagini->Draw(Canvas,70,0,0,true);
 ListaImagini->Draw(Canvas,70,70,0,true);
 for(i=10;i<70;i+=10) ListaImagini->Draw(Canvas,i,70,1,true);
 for(i=10;i<70;i+=10) ListaImagini->Draw(Canvas,70,i,2,true);
 char buf[255];
 Canvas->Brush->Color =(TColor)RGB(255, 175, 75);
 Canvas->Font->Color=(TColor)RGB(255, 175, 75);
 Canvas->Brush->Style = bsClear;
 int temp=BattleForm->Player[Juc]->army_slots[Lot]->PortretAsoc;
 BattleForm->ImagPortret->Draw(Canvas,10,10,temp,true);
 Canvas->Font->Size=13;
 Canvas->TextOut(88,26,BattleForm->Player[Juc]->army_slots[Lot]->GetNume().c_str());
 Canvas->Font->Size=12;
 sprintf(buf,"Damage:%3d - %3d",BattleForm->Player[Juc]->army_slots[Lot]->DamageMin,BattleForm->Player[Juc]->army_slots[Lot]->DamageMax);
 Canvas->TextOut(30,80,buf);SmallIcons->Draw(Canvas,12,82,0,true);
 sprintf(buf,"Armor:%3d",BattleForm->Player[Juc]->army_slots[Lot]->Armor);
 Canvas->TextOut(30,95,buf);SmallIcons->Draw(Canvas,12,97,1,true);
 sprintf(buf,"Protection:%3d",BattleForm->Player[Juc]->army_slots[Lot]->Protection);
 Canvas->TextOut(30,110,buf);SmallIcons->Draw(Canvas,12,112,1,true);
 sprintf(buf,"Health:%3d/%3d",BattleForm->Player[Juc]->army_slots[Lot]->Hp,BattleForm->Player[Juc]->army_slots[Lot]->HpMax);
 Canvas->TextOut(30,125,buf);SmallIcons->Draw(Canvas,12,127,2,true);
 sprintf(buf,"Moves:%3d/%3d",BattleForm->Player[Juc]->army_slots[Lot]->MovesLeft,BattleForm->Player[Juc]->army_slots[Lot]->MovesMax);
 Canvas->TextOut(30,140,buf);SmallIcons->Draw(Canvas,12,142,3,true);
 sprintf(buf,"Chances to hit:%3d\%",BattleForm->Player[Juc]->army_slots[Lot]->ChancesToHit);
 Canvas->TextOut(30,155,buf);SmallIcons->Draw(Canvas,12,157,0,true);
 if(BattleForm->Player[Juc]->army_slots[Lot]->Ranged)
  {sprintf(buf,"Ammunition:%3d",BattleForm->Player[Juc]->army_slots[Lot]->Ammo);
   Canvas->TextOut(30,170,buf);}
 sprintf(buf,"Retaliations:%3d/%3d",BattleForm->Player[Juc]->army_slots[Lot]->Retal,BattleForm->Player[Juc]->army_slots[Lot]->RetalNum);
 Canvas->TextOut(30,185,buf);SmallIcons->Draw(Canvas,12,187,4,true);
 sprintf(buf,"Level:%3d",BattleForm->Player[Juc]->army_slots[Lot]->Level);
 Canvas->TextOut(30,215,buf);SmallIcons->Draw(Canvas,12,217,4,true);
 sprintf(buf,"Experience:%3d/%3d",BattleForm->Player[Juc]->army_slots[Lot]->Xp,BattleForm->Player[Juc]->army_slots[Lot]->XpMax);
 Canvas->TextOut(30,230,buf);
 sprintf(buf,"Upkeep:%3d",BattleForm->Player[Juc]->army_slots[Lot]->Upkeep);
 Canvas->TextOut(30,245,buf);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void __fastcall TMesaj::FormPaint(TObject *Sender)
{Deseneaza();
}
//---------------------------------------------------------------------------

