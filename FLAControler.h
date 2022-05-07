//---------------------------------------------------------------------------

#ifndef FLAControlerH
#define FLAControlerH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class Tfrm_controlers : public TForm
{
__published:	// IDE-managed Components
   TGroupBox *GroupBox1;
   TLabel *Label1;
   TBitBtn *BitBtn1;
   TBitBtn *BitBtn2;
   TBitBtn *BitBtn3;
   TBitBtn *BitBtn4;
   TBitBtn *BitBtn5;
   TTrackBar *TrackBar1;
   TBitBtn *BitBtn6;
   TBitBtn *BitBtn7;
   TTrackBar *TrackBar2;
   TLabel *Label2;
   TGroupBox *GroupBox2;
   TLabel *lb_version;
   TLabel *lb_numframes;
   TLabel *lb_speed;
   TLabel *lb_numsamples;
   void __fastcall BitBtn2Click(TObject *Sender);
   void __fastcall BitBtn3Click(TObject *Sender);
   void __fastcall BitBtn4Click(TObject *Sender);
   void __fastcall TrackBar2Change(TObject *Sender);
   void __fastcall TrackBar1Change(TObject *Sender);
   void __fastcall BitBtn1Click(TObject *Sender);
   void __fastcall BitBtn6Click(TObject *Sender);
   void __fastcall BitBtn5Click(TObject *Sender);
   void __fastcall BitBtn7Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
   __fastcall Tfrm_controlers(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE Tfrm_controlers *frm_controlers;
//---------------------------------------------------------------------------
#endif
