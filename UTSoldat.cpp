//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include <vcl.h>
#include <string.h>
#include <io.h>
#include <stdio.h>
#pragma hdrstop
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class TSoldat
{private:
         bool falive;
 public:
        __property bool alive={read=falive,write=falive};
        int RetalNum;
        int Retal;
        int Level;
        int Xp;
        int XpMax;
        int Hp;
        int HpMax;
        int MovesLeft;
        int MovesMax;
        int DamageMin;
        int DamageMax;
        int Armor;
        int Protection;
        int ChancesToHit;
        int ImagineAsoc;
        int PortretAsoc;
        bool Ranged;
        int Ammo;
        int Upkeep;
        int Orientare;
        int x;
        int y;
        TSoldat()
        { alive=1;
          Level=1;
          Xp=0;
          XpMax=1;
          Hp=1;
          HpMax=1;
          MovesLeft=1;
          MovesMax=1;
          DamageMin=1;
          DamageMax=2;
          Armor=0;
          Protection=0;
          ChancesToHit=50;
          ImagineAsoc=1;
          PortretAsoc=0;
          Ranged=false;
          Ammo=-1;
          Upkeep=0;
          RetalNum=1;
          Retal=1;
          Orientare=0;
          x=y=0;
        }
        void Desen(TImageList *lista, TCanvas *UnCanvas, int x, int y, int figindex)
        {lista->Draw(UnCanvas,x,y,figindex*2+Orientare,true);
        }
        void Draw(TImageList *lista, TCanvas *UnCanvas)
        {if(y%2==0)
            lista->Draw(UnCanvas,20+x*80,y*60+20,ImagineAsoc*2+Orientare,true);
         else
            lista->Draw(UnCanvas,60+x*80,y*60+20,ImagineAsoc*2+Orientare,true);
        }
        AnsiString GetNume();
};
//---------------------------------------------------------------------------
inline AnsiString TSoldat::GetNume()
{AnsiString ret;
 switch(PortretAsoc)
 {case 0:ret="Peasant";break;
  case 1:ret="Peasant Archer";break;
  case 2:ret="Pikeman";break;
  case 3:ret="Lizardman";break;
  case 4:ret="Crossbowman";break;
  case 5:ret="Halberdier";break;
  case 6:ret="Paladin";break;
  case 7:ret="Longbowman";break;
  case 8:ret="Magic Sprite";break;
  case 9:ret="Elemental";break;
  case 10:ret="Dead Unit";break;

  default:ret="Hero Unit";
 }
 return ret;
}
//---------------------------------------------------------------------------
bool __exista(char *filename){return (access(filename, 0) == 0);}
void __citeste(FILE *ff){char c;do{fscanf(ff,"%c",&c);}while(c!='^' && !feof(ff));}

class THero
{private:
 public:
        char nume[35];
        int portret;
        TSoldat *slot[7];
        int control;//1=human;2=computer;
        int angajati;//nr de mercenari
        THero(char numefisier[35])
        {IncarcaErou(numefisier);
        }
        void IncarcaErou(char numefisier[35]);
};
//---------------------------------------------------------------------------
void THero::IncarcaErou(char numefisier[35])
{if(__exista(numefisier))
  {
   FILE *f=fopen(numefisier,"r");
   __citeste(f);
   fscanf(f,"%s",&nume);
   __citeste(f);
   fscanf(f,"%d",&angajati);
   if(angajati>6) angajati=0;
   for(int i=0;i<=angajati;i++)
   {slot[i]=new TSoldat;
    __citeste(f);
    fscanf(f,"%d%d%d%d%d",&slot[i]->Level,&slot[i]->Xp,&slot[i]->XpMax,
        &slot[i]->Hp,&slot[i]->HpMax);
    fscanf(f,"%d%d%d%d%d",&slot[i]->MovesMax,&slot[i]->DamageMin,&slot[i]->DamageMax,
        &slot[i]->Armor,&slot[i]->Protection);
    fscanf(f,"%d%d%d%d%d",&slot[i]->ChancesToHit,&slot[i]->Upkeep,&slot[i]->ImagineAsoc,
        &slot[i]->PortretAsoc,&slot[i]->Ranged);
    fscanf(f,"%d%d%d",&slot[i]->Ammo,&slot[i]->RetalNum,&slot[i]->y);
    slot[i]->Retal=slot[i]->RetalNum;
    slot[i]->MovesLeft=slot[i]->MovesMax;
   }
   fclose(f);
  }
 else
   {char ff[255];
    sprintf(ff,"Fisierul numit <%s>nu exista!!",numefisier);
    MessageDlg(ff, mtInformation, TMsgDlgButtons() << mbOK, 0);
   }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#pragma package(smart_init)
