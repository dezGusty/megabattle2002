//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "LauncherMainUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ButtonCloseClick(TObject *Sender)
{Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ButtonLaunchClick(TObject *Sender)
{SetCurrentDir(DirectorJoc);
 execl("MegaBattle2.exe","MegaBattle2.exe",NULL);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer1Timer(TObject *Sender)
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


void __fastcall TForm1::FormActivate(TObject *Sender)
{TRect tmp(0,0,100,75);
 cpRect=tmp;
 Timer1->Enabled=true;
 if(__exista("battle.ini"))
  {FILE *f=fopen("battle.ini","rb");
   felteren=1;ShowHexes=1;
   __citeste(f);
   fscanf(f,"%d",&ShowHexes);
   __citeste(f);
   fscanf(f,"%d",&control2);
   __citeste(f);
   fscanf(f,"%d",&felteren);
   __citeste(f);
   fscanf(f,"%s",fisier1);
   __citeste(f);
   fscanf(f,"%s",fisier2);
   NumeErou1->Caption=fisier1;
   NumeErou2->Caption=fisier2;
   fclose(f);
  }
 else
   {MessageDlg("The file <Battle.ini> does not exist in \n\the current directory", mtInformation, TMsgDlgButtons() << mbOK, 0);
    Close();}
 OpenDialog1->InitialDir=GetCurrentDir();
 DirectorJoc=GetCurrentDir();
 CheckBox1->Checked=control2;
 CheckBox2->Checked=ShowHexes;
 Edit1->Text=felteren;
}
//---------------------------------------------------------------------------
void TForm1::ScrieINI()
{control2=CheckBox1->Checked;
 ShowHexes=CheckBox2->Checked;
 felteren=atoi(Edit1->Text.c_str());

 SetCurrentDir(DirectorJoc);
 FILE *f=fopen("battle.ini","w");
 fprintf(f,"-------------Show hexes---");
 fprintf(f,"\n^%d\n",ShowHexes);
 fprintf(f,"--Inamicul este computer--");
 fprintf(f,"\n^%d\n",control2);
 fprintf(f,"--------Felul terenului---");
 fprintf(f,"\n^%d\n",felteren);
 fprintf(f,"----------------Player1---");
 fprintf(f,"\n^%s\n",fisier1);
 fprintf(f,"----------------Player2---");
 fprintf(f,"\n^%s\n",fisier2);
 fclose(f);
}

void __fastcall TForm1::NumeErou1Click(TObject *Sender)
{OpenDialog1->InitialDir=DirectorJoc+"\\heroes";
 if (OpenDialog1->Execute())
  {char buf[255]="\0";
   char invers[255]="\0";
   strcpy(buf,OpenDialog1->FileName.c_str());
   int k=strlen(buf);
   int kappa=strlen(buf);
   int slashuri=0;
   do
     {invers[kappa-k]=buf[k-1];
      if(slashuri>=2) invers[kappa-k]='\0';
      k--;
      if(buf[k]=='\\') slashuri++;
     }
   while (k>=0);
   strcpy(buf,"\0");for(int i=0;i<=255;i++) buf[i]='\0';
   kappa=strlen(invers);
   for(int i=0;i<=kappa-2;i++) buf[i]=invers[kappa-2-i];
//trebuie insa dublu \\backslash\\
   kappa=strlen(buf);
   for(int i=kappa-1;i>=0;i--)
     {buf[i+1]=buf[i];if(buf[i]=='\\') i=-1;}
   strcpy(fisier1,buf);
   NumeErou1->Caption=fisier1;
   ScrieINI();
//   MessageDlg("ok",mtInformation,TMsgDlgButtons() << mbOK,0);
  }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::NumeErou2Click(TObject *Sender)
{if (OpenDialog1->Execute())
  {char buf[255]="\0";
   char invers[255]="\0";
   strcpy(buf,OpenDialog1->FileName.c_str());
   int k=strlen(buf);
   int kappa=strlen(buf);
   int slashuri=0;
   do
     {invers[kappa-k]=buf[k-1];
      if(slashuri>=2) invers[kappa-k]='\0';
      k--;
      if(buf[k]=='\\') slashuri++;
     }
   while (k>=0);
   strcpy(buf,"\0");for(int i=0;i<=255;i++) buf[i]='\0';
   kappa=strlen(invers);
   for(int i=0;i<=kappa-2;i++) buf[i]=invers[kappa-2-i];
//trebuie insa dublu \\backslash\\
   kappa=strlen(buf);
   for(int i=kappa-1;i>=0;i--)
     {buf[i+1]=buf[i];if(buf[i]=='\\') i=-1;}
   strcpy(fisier2,buf);
   NumeErou2->Caption=fisier2;
   ScrieINI();
  }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::FormDestroy(TObject *Sender)
{ScrieINI();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Edit1Change(TObject *Sender)
{int valoare=atoi(Edit1->Text.c_str());
 if(valoare<1 || valoare>2) Edit1->Text=1;
}
//---------------------------------------------------------------------------

