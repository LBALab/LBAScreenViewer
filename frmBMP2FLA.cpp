//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "frmBMP2FLA.h"
#include "LBAScreenViewer_main.h"
#include "FLALib.h"

#include "bmptofla.cpp"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "FolderDialog"
#pragma link "CGAUGES"
#pragma resource "*.dfm"
Tfrm_BMP2FLA *frm_BMP2FLA;

extern AnsiString Path;
extern AnsiString palPath;

//---------------------------------------------------------------------------
__fastcall Tfrm_BMP2FLA::Tfrm_BMP2FLA(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_BMP2FLA::bt_browseClick(TObject *Sender)
{
   if(FolderDialog1->Execute())
   {
       edt_folder->Text = FolderDialog1->Directory;
   }
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_BMP2FLA::bt_clearClick(TObject *Sender)
{
   edt_folder->Text = "";
   edt_prefix->Text = "";
   edt_frames->Text = "";
   edt_keyframes->Text = "";
   pgr_fla->Progress = 0;
   cb_usesample->Checked = False;
   bt_playfla->Enabled = False;
   bt_createfla->Enabled = True;
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_BMP2FLA::cb_usesampleClick(TObject *Sender)
{
   if(cb_usesample->Checked == True)
   {
       lb_sampleidx->Enabled = True;
       edt_sampleidx->Enabled = True;
       lb_freq->Enabled = True;
       edt_freq->Enabled = True;
       lb_repeat->Enabled = True;
       edt_repeat->Enabled = True;
   }
   else
   {
       lb_sampleidx->Enabled = False;
       edt_sampleidx->Enabled = False;
       lb_freq->Enabled = False;
       edt_freq->Enabled = False;
       lb_repeat->Enabled = False;
       edt_repeat->Enabled = False;
   }
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_BMP2FLA::bt_createflaClick(TObject *Sender)
{
   if(!edt_folder->Text.IsEmpty() && !edt_prefix->Text.IsEmpty() && !edt_frames->Text.IsEmpty())
   {
      int argc;
      char** argv;
      int count=1;
      int pos = 0;
      AnsiString tmp4;
      //AnsiString tmp5;

      bt_createfla->Enabled = False;

      AnsiString tmp = edt_folder->Text;
      tmp = tmp + "\\";
      tmp = tmp + edt_prefix->Text;

       AnsiString tmp2 = edt_frames->Text;

      argc = 11;

   /*   if(cb_usesample->Checked)
           argc+=6;
     */
       if(!edt_keyframes->Text.IsEmpty())
       {
           AnsiString tmp3 = edt_keyframes->Text;
           while(pos = tmp3.AnsiPos(" "))
           {
               tmp3 = tmp3.SubString(pos+1, tmp3.Length());
               count++;
           }
           argc+=count+1;
       }

	    argv = (char**)malloc(argc * sizeof(char*));
	    argv[ 0] = "";
	    argv[ 1] = "-n";
	    argv[ 2] = tmp2.c_str();
	    argv[ 3] = "-p";
	    argv[ 4] = tmp.c_str();

       if(cb_usesample->Checked)
       {
	        argv[ 5] = "-s";
	        argv[ 6] = edt_sampleidx->Text.c_str();
	        argv[ 7] = edt_freq->Text.c_str();
	        argv[ 8] = edt_repeat->Text.c_str();
	        argv[ 9] = "90";
	        argv[10] = "90";
       }
       else
       {
           argv[ 5] = "-s";
	        argv[ 6] = "1";
	        argv[ 7] = "1";
	        argv[ 8] = "1";
	        argv[ 9] = "1";
	        argv[10] = "1";
       }

       if(!edt_keyframes->Text.IsEmpty())
       {
           tmp4 = edt_keyframes->Text;
           argv[11] = "-k";
           for(int i=0; i < count; i++)
           {
               pos = tmp4.AnsiPos(" ");
               if(i+1 == count)
                   pos+=2;
               int tmp5 = tmp4.SubString(0, pos-1).ToInt();
               char *number = (char*)malloc(tmp4.SubString(0, pos-1).Length()*sizeof(char));
               itoa(tmp5,number,10);
               argv[12+i] = number;
               tmp4 = tmp4.SubString(pos+1, tmp4.Length());
           }
       }

	    /*argv[11] = "-f";
	    argv[12] = "3";
	    argv[13] = "5";*/
       BMPtoFLA instance(argc, argv);
   }
   else
       ShowMessage("You must fill the obligatory fields!");
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_BMP2FLA::bt_playflaClick(TObject *Sender)
{
   frm_BMP2FLA->Close();

   AnsiString tmp = edt_folder->Text;
   tmp = tmp + "\\";
   tmp = tmp + edt_prefix->Text;
   tmp = tmp + ".fla";

   Path = tmp;

   frm_img->status_img->Panels->Items[2]->Text = "Info: FLA succesfully opened!";
   frm_img->status_path->Panels->Items[0]->Text = "Path: " + Path;
   frm_img->edt_index->Value = 0;
   frm_img->cb_autodraw->Enabled = False;
   frm_img->pl_bkg->Visible = False;
   palPath = "";
   OpenFLA(Path.c_str(), frm_img->LBAimg->Canvas);
   frm_img->menu_closeall->Enabled = True;
   frm_img->closeall->Enabled = True;
   frm_img->cb_autodraw->Enabled = False;
   frm_img->CopyImageToClipboard1->Enabled = True;
   frm_img->PasteImageFromClipboard1->Enabled = True;
}
//---------------------------------------------------------------------------
