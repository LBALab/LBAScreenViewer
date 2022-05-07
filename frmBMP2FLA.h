//---------------------------------------------------------------------------

#ifndef frmBMP2FLAH
#define frmBMP2FLAH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "FolderDialog.hpp"
#include "CGAUGES.h"
//---------------------------------------------------------------------------
class Tfrm_BMP2FLA : public TForm
{
__published:	// IDE-managed Components
   TFolderDialog *FolderDialog1;
   TEdit *edt_folder;
   TButton *bt_browse;
   TLabel *Label1;
   TLabel *Label2;
   TEdit *edt_prefix;
   TLabel *Label3;
   TEdit *edt_frames;
   TLabel *Label4;
   TGroupBox *GroupBox1;
   TEdit *edt_sampleidx;
   TCheckBox *cb_usesample;
   TLabel *lb_sampleidx;
   TLabel *lb_freq;
   TEdit *edt_freq;
   TLabel *lb_repeat;
   TEdit *edt_repeat;
   TCGauge *pgr_fla;
   TButton *bt_createfla;
   TButton *bt_playfla;
   TButton *bt_clear;
   TLabel *Label5;
   TEdit *edt_keyframes;
   TLabel *Label6;
   TLabel *Label7;
   void __fastcall bt_browseClick(TObject *Sender);
   void __fastcall bt_clearClick(TObject *Sender);
   void __fastcall cb_usesampleClick(TObject *Sender);
   void __fastcall bt_createflaClick(TObject *Sender);
   void __fastcall bt_playflaClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
   __fastcall Tfrm_BMP2FLA(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE Tfrm_BMP2FLA *frm_BMP2FLA;
//---------------------------------------------------------------------------
#endif
