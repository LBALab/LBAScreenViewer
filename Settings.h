//---------------------------------------------------------------------------

#ifndef SettingsH
#define SettingsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class Tfrm_settings : public TForm
{
__published:	// IDE-managed Components
   TGroupBox *GroupBox1;
   TCheckBox *cblim;
   TCheckBox *cbpal;
   TCheckBox *cblsp;
   TCheckBox *cbbrk;
   TCheckBox *cblsr;
   TCheckBox *cblba;
   TCheckBox *cbhqr;
   TCheckBox *cbile;
   TImage *Image1;
   TImage *Image2;
   TImage *Image3;
   TImage *Image4;
   TImage *Image5;
   TImage *Image6;
   TImage *Image7;
   TImage *Image8;
   TCheckBox *cbbmp;
   TCheckBox *cbjpg;
   TCheckBox *cbgif;
   TCheckBox *cbpng;
   TCheckBox *cbpcx;
   TCheckBox *cbfla;
   TImage *Image9;
   TImage *Image10;
   TImage *Image11;
   TImage *Image12;
   TImage *Image13;
   TImage *Image14;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
   void loadSettings();
   void saveSettings();
   void SetKey(AnsiString Key, AnsiString Icon, AnsiString Text);
   void DelKey(AnsiString Key);
   void SetAssociation();
public:		// User declarations
   void ShowSettings();
   int file_exists(char * filename);
   __fastcall Tfrm_settings(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE Tfrm_settings *frm_settings;
//---------------------------------------------------------------------------
#endif
