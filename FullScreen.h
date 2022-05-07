//---------------------------------------------------------------------------

#ifndef FullScreenH
#define FullScreenH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class Tfrm_fullscreen : public TForm
{
__published:	// IDE-managed Components
   TImage *Image1;
   void __fastcall PaintBox1Paint(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormKeyPress(TObject *Sender, char &Key);
   void __fastcall Image1ContextPopup(TObject *Sender, TPoint &MousePos,
          bool &Handled);
private:	// User declarations
public:		// User declarations
   __fastcall Tfrm_fullscreen(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE Tfrm_fullscreen *frm_fullscreen;
//---------------------------------------------------------------------------
#endif
