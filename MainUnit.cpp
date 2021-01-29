//---------------------------------------------------------------------------
#include <vcl.h>
#include <tchar.h>

#pragma hdrstop
#include "MainUnit.h"
#include "AboutUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEditForm *EditForm;

void __citeste(FILE *ff)
{
	char c;
	do
	{
		fscanf(ff,"%c",&c);
	}
	while(c!='^' && !feof(ff));
}


//---------------------------------------------------------------------------
__fastcall TEditForm::TEditForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TEditForm::ComboBox1Change(TObject *Sender)
{AnsiString temp = ComboBox1->Text;
 int kappa=atoi(temp.c_str());
 if(kappa>0)
   {Edit1->Visible=true;UpDown1->Visible=true;Merc1->Visible=true;}
 else
   {Edit1->Visible=false;UpDown1->Visible=false;Merc1->Visible=false;}
 if(kappa>1)
   {Edit2->Visible=true;UpDown2->Visible=true;Merc2->Visible=true;}
 else
   {Edit2->Visible=false;UpDown2->Visible=false;Merc2->Visible=false;}
 if(kappa>2)
   {Edit3->Visible=true;UpDown3->Visible=true;Merc3->Visible=true;}
 else
   {Edit3->Visible=false;UpDown3->Visible=false;Merc3->Visible=false;}
 if(kappa>3)
   {Edit4->Visible=true;UpDown4->Visible=true;Merc4->Visible=true;}
 else
   {Edit4->Visible=false;UpDown4->Visible=false;Merc4->Visible=false;}
 if(kappa>4)
   {Edit5->Visible=true;UpDown5->Visible=true;Merc5->Visible=true;}
 else
   {Edit5->Visible=false;UpDown5->Visible=false;Merc5->Visible=false;}
 if(kappa>5)
   {Edit6->Visible=true;UpDown6->Visible=true;Merc6->Visible=true;}
 else
   {Edit6->Visible=false;UpDown6->Visible=false;Merc6->Visible=false;}
}
//---------------------------------------------------------------------------

void __fastcall TEditForm::ExitEditor1Click(TObject *Sender)
{Close();
}
//---------------------------------------------------------------------------

void __fastcall TEditForm::CreateNewHero1Click(TObject *Sender)
{ResetariDate();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TEditForm::ResetariDate()
{ComboBox1->Text="0";
 EditNumeErou->Text="new hero";
 UpDownHero->Position=0;
 ComboBox1Change(this);
 char buf[20]="(0)Peasant";
 int Level=0;
 FelErou->Text=buf;
 Merc1->Text=buf;UpDown1->Position=(short)Level;
 Merc2->Text=buf;UpDown2->Position=(short)Level;
 Merc3->Text=buf;UpDown3->Position=(short)Level;
 Merc4->Text=buf;UpDown4->Position=(short)Level;
 Merc1->Text=buf;UpDown1->Position=(short)Level;
 Merc1->Text=buf;UpDown1->Position=(short)Level;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void ScrieUnitate(FILE *ff,int fel,int level,int y_position)
{int exp,exp_max, hp, hpmax, movesmax, dmg_min, dmg_max;
 int armor, protection, chance, upkeep,imag_asoc, port_asoc;
 int ranged, ammo, retaliate;
 switch(fel)
 {case 0://peasant
         hp=hpmax=40;
         movesmax=3;
         dmg_min=5;
         dmg_max=10;
         armor=0;
         protection=0;
         chance=70;
         upkeep=1;
         ranged=0;
         ammo=-1;
         retaliate=1;
         break;
  case 1://peasant archer
         hp=hpmax=30;
         movesmax=3;
         dmg_min=5;
         dmg_max=10;
         armor=0;
         protection=0;
         chance=70;
         upkeep=1;
         ranged=1;
         ammo=22;
         retaliate=0;
         break;
  case 2://pikeman
         hp=hpmax=50;
         movesmax=3;
         dmg_min=8;
         dmg_max=12;
         armor=1;
         protection=15;
         chance=70;
         upkeep=3;
         ranged=0;
         ammo=-1;
         retaliate=1;
         break;
  case 3://lizardman
         hp=hpmax=40;
         movesmax=3;
         dmg_min=10;
         dmg_max=12;
         armor=0;
         protection=20;
         chance=90;
         upkeep=4;
         ranged=0;
         ammo=-1;
         retaliate=2;
         break;
  case 4://crossbowman
         hp=hpmax=40;
         movesmax=3;
         dmg_min=9;
         dmg_max=10;
         armor=1;
         protection=0;
         chance=80;
         upkeep=5;
         ranged=1;
         ammo=25;
         retaliate=0;
         break;
  case 5://halberdier
         hp=hpmax=70;
         movesmax=3;
         dmg_min=13;
         dmg_max=18;
         armor=2;
         protection=20;
         chance=75;
         upkeep=7;
         ranged=0;
         ammo=-1;
         retaliate=1;
         break;
  case 6://paladin
         hp=hpmax=75;
         movesmax=3;
         dmg_min=16;
         dmg_max=17;
         armor=4;
         protection=20;
         chance=80;
         upkeep=8;
         ranged=0;
         ammo=-1;
         retaliate=1;
         break;
  case 7://longbowman
         hp=hpmax=45;
         movesmax=3;
         dmg_min=15;
         dmg_max=18;
         armor=2;
         protection=20;
         chance=75;
         upkeep=8;
         ranged=1;
         ammo=25;
         retaliate=0;
         break;
  case 8://magic sprite
         hp=hpmax=60;
         movesmax=4;
         dmg_min=8;
         dmg_max=10;
         armor=2;
         protection=30;
         chance=95;
         upkeep=6;
         ranged=0;
         ammo=-1;
         retaliate=1;
         break;
  case 9://magic element
         hp=hpmax=100;
         movesmax=2;
         dmg_min=10;
         dmg_max=20;
         armor=3;
         protection=50;
         chance=80;
         upkeep=10;
         ranged=0;
         ammo=-1;
         retaliate=1;
         break;
 }
 //pt toti;
 exp=0;
 if(level< 0) level=0;
 if(level>15) level=15;
 if(level>=8) movesmax++;
 exp_max=level*10;
 imag_asoc=port_asoc=fel;
 //plusuri pt nivele;
 hp+=3*level;hpmax=hp;
 dmg_min+=dmg_min*level/10;
 dmg_max+=dmg_max*level/10;
 armor+=armor*level/50;
 protection+=protection*level/60;
 chance+=(100-chance)*level/15;
 fprintf(ff,"\n^");
 fprintf(ff,"%d %d %d %d %d ",level,exp,exp_max,hp,hpmax);
 fprintf(ff,"%d %d %d %d %d ",movesmax,dmg_min,dmg_max,armor,protection);
 fprintf(ff,"%d %d %d %d %d ",chance,upkeep,imag_asoc,port_asoc,ranged);
 fprintf(ff,"%d %d %d",ammo,retaliate,y_position);
}

void __fastcall TEditForm::About1Click(TObject *Sender)
{FormHelp=new TFormHelp(this);
 FormHelp->ShowModal();
 delete FormHelp;
}
//---------------------------------------------------------------------------

void __fastcall TEditForm::SaveCurrentHero1Click(TObject *Sender)
{ FILE *ff;
  char buff[100];
  sprintf(buff, "Saving Hero : %s", EditNumeErou->Text.c_str());
  SaveDialog1->InitialDir=DirectorJoc+"\\heroes";
  SaveDialog1->Title = buff;
  if (SaveDialog1->Execute())
  {ff = fopen(SaveDialog1->FileName.c_str(), "wt");
    if (ff)
	 {int kappa=atoi(ComboBox1->Text.c_str());
	  if(kappa>=0)
       {//pt erou;
        char nume[25];
        strcpy(nume,EditNumeErou->Text.c_str());
        for(int i=0;i<=(int)strlen(nume);i++)
          if(nume[i]==' ') nume[i]='_';
        fprintf(ff,"^%s",nume);
        fprintf(ff,"\n^%d",kappa);
        char cifra=FelErou->Text.c_str()[1];
        int lvl=atoi(EditHeroLevel->Text.c_str());
        ScrieUnitate(ff,cifra-48,lvl,0);
        if(kappa>=1)
         {char cifra=Merc1->Text.c_str()[1];
          int lvl=atoi(Edit1->Text.c_str());
          ScrieUnitate(ff,cifra-48,lvl,1);
         }
        if(kappa>=2)
         {char cifra=Merc2->Text.c_str()[1];
          int lvl=atoi(Edit2->Text.c_str());
          ScrieUnitate(ff,cifra-48,lvl,2);
         }
        if(kappa>=3)
         {char cifra=Merc3->Text.c_str()[1];
          int lvl=atoi(Edit3->Text.c_str());
          ScrieUnitate(ff,cifra-48,lvl,3);
         }
        if(kappa>=4)
         {char cifra=Merc4->Text.c_str()[1];
          int lvl=atoi(Edit4->Text.c_str());
          ScrieUnitate(ff,cifra-48,lvl,4);
         }
        if(kappa>=5)
         {char cifra=Merc5->Text.c_str()[1];
		  int lvl=atoi(Edit5->Text.c_str());
          ScrieUnitate(ff,cifra-48,lvl,5);
         }
        if(kappa>=6)
         {char cifra=Merc6->Text.c_str()[1];
          int lvl=atoi(Edit6->Text.c_str());
          ScrieUnitate(ff,cifra-48,lvl,6);
         }
       }
      else
        MessageDlg( "Datele nu au fost introduse corect",mtInformation,TMsgDlgButtons() >> mbOK,0);
    }
    fclose(ff);
  }

}
//---------------------------------------------------------------------------




void __fastcall TEditForm::OpenExistingHero1Click(TObject *Sender)
{OpenDialog1->InitialDir=DirectorJoc+"\\heroes";
 if (OpenDialog1->Execute())
  {FILE *f=fopen(OpenDialog1->FileName.c_str(),"r");
   __citeste(f);
   char nume[25];
   fscanf(f,"%s",&nume);
   EditNumeErou->Text=nume;
   __citeste(f);
   int angajati;
   fscanf(f,"%d",&angajati);
   ComboBox1->Text=angajati;
   ComboBox1Change(this);
   for(int i=0;i<=angajati;i++)
   {__citeste(f);
    int k;
    int Level,ImagineAsoc;
    fscanf(f,"%d%d%d%d%d",&Level,&k,&k,&k,&k);
    fscanf(f,"%d%d%d%d%d",&k,&k,&k,&k,&k);
    fscanf(f,"%d%d%d%d%d",&k,&k,&ImagineAsoc,&k,&k);
    fscanf(f,"%d%d%d",&k,&k,&k);
    char buf[20];
    switch(ImagineAsoc)
    {case 0:strcpy(buf,"(0)Peasant");break;
     case 1:strcpy(buf,"(1)Peasant Archer");break;
     case 2:strcpy(buf,"(2)Pikeman");break;
     case 3:strcpy(buf,"(3)Lizardman");break;
     case 4:strcpy(buf,"(4)Crossbowman");break;
     case 5:strcpy(buf,"(5)Dark Halberdier");break;
     case 6:strcpy(buf,"(6)Paladin");break;
     case 7:strcpy(buf,"(7)Longbowman");break;
     case 8:strcpy(buf,"(8)Magic Sprite");break;
     case 9:strcpy(buf,"(9)Magic Elemental");break;
    }
    switch(i)
    {case 0:FelErou->Text=buf;UpDownHero->Position=(short)Level;break;
     case 1:Merc1->Text=buf;UpDown1->Position=(short)Level;break;
     case 2:Merc2->Text=buf;UpDown2->Position=(short)Level;break;
     case 3:Merc3->Text=buf;UpDown3->Position=(short)Level;break;
     case 4:Merc4->Text=buf;UpDown4->Position=(short)Level;break;
     case 5:Merc1->Text=buf;UpDown1->Position=(short)Level;break;
     case 6:Merc1->Text=buf;UpDown1->Position=(short)Level;break;
    }

   }
   fclose(f);
  }
}
//---------------------------------------------------------------------------

void __fastcall TEditForm::FormActivate(TObject *Sender)
{DirectorJoc=GetCurrentDir();
}
//---------------------------------------------------------------------------

