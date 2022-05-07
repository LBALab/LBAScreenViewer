//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "About.h"
//--------------------------------------------------------------------- 
#pragma resource "*.dfm"
Tfrm_about *frm_about;
extern String vnum;
//---------------------------------------------------------------------
__fastcall Tfrm_about::Tfrm_about(TComponent* AOwner)
	: TForm(AOwner)
{
   Version->Caption = "Version " + vnum;
}
//--------------------------------------------------------------------- 
