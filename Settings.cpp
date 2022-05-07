//---------------------------------------------------------------------------

#include <vcl.h>
#include <IniFiles.hpp>
#include <Registry.hpp>
#include <io.h>
#pragma hdrstop

#include "Settings.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tfrm_settings *frm_settings;
//---------------------------------------------------------------------------
__fastcall Tfrm_settings::Tfrm_settings(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void Tfrm_settings::saveSettings(){
   TIniFile *ini;
   ini = new TIniFile(ExtractFileDir(Application->ExeName)+"\\config.ini");
   ini->WriteBool("Association","lim",cblim->Checked);
   ini->WriteBool("Association","pal",cbpal->Checked);
   ini->WriteBool("Association","lsp",cblsp->Checked);
   ini->WriteBool("Association","brk",cbbrk->Checked);
   ini->WriteBool("Association","lsr",cblsr->Checked);
   ini->WriteBool("Association","fla",cbfla->Checked);
   ini->WriteBool("Association","lba",cblba->Checked);
   ini->WriteBool("Association","hqr",cbhqr->Checked);
   ini->WriteBool("Association","ile",cbile->Checked);
   ini->WriteBool("Association","bmp",cbbmp->Checked);
   ini->WriteBool("Association","jpg",cbjpg->Checked);
   ini->WriteBool("Association","gif",cbgif->Checked);
   ini->WriteBool("Association","png",cbpng->Checked);
   ini->WriteBool("Association","pcx",cbpcx->Checked);
   delete ini;
}
//---------------------------------------------------------------------------

void Tfrm_settings::loadSettings()
{
   TIniFile *ini;
   ini = new TIniFile(ExtractFileDir(Application->ExeName)+"\\config.ini");
   cblim->Checked = ini->ReadBool("Association","lim",true);
   cbpal->Checked = ini->ReadBool("Association","pal",true);
   cblsp->Checked = ini->ReadBool("Association","lsp",true);
   cbbrk->Checked = ini->ReadBool("Association","brk",true);
   cblsr->Checked = ini->ReadBool("Association","lsr",true);
   cbfla->Checked = ini->ReadBool("Association","fla",true);   
   cblba->Checked = ini->ReadBool("Association","lba",false);
   cbhqr->Checked = ini->ReadBool("Association","hqr",false);
   cbile->Checked = ini->ReadBool("Association","ile",false);
   cbbmp->Checked = ini->ReadBool("Association","bmp",false);
   cbjpg->Checked = ini->ReadBool("Association","jpg",false);
   cbgif->Checked = ini->ReadBool("Association","gif",false);
   cbpng->Checked = ini->ReadBool("Association","png",false);
   cbpcx->Checked = ini->ReadBool("Association","pcx",false);
   delete ini;
}
//---------------------------------------------------------------------------

void Tfrm_settings::SetKey(AnsiString Key, AnsiString Icon, AnsiString Text){
   TRegIniFile *Rejestr = new TRegIniFile("");
   Rejestr->RootKey = HKEY_CLASSES_ROOT;
   Rejestr->WriteString("."+Key,"","LBA_"+Key+"_screenviewer");
   Rejestr->WriteString("LBA_"+Key+"_screenviewer","",Text);
   Rejestr->WriteString("LBA_"+Key+"_screenviewer","Created by","LBA Screen Viewer");
   Rejestr->WriteString("LBA_"+Key+"_screenviewer\\DefaultIcon","",Icon);
   Rejestr->WriteString("LBA_"+Key+"_screenviewer\\shell","","open");
   Rejestr->WriteString("LBA_"+Key+"_screenviewer\\shell\\open\\command","","\""+Application->ExeName+"\" "+"\"%1\"");
   Rejestr->Free();
}
//---------------------------------------------------------------------------
void Tfrm_settings::DelKey(AnsiString Key){
   TRegIniFile *Rejestr = new TRegIniFile("");
   Rejestr->RootKey = HKEY_CLASSES_ROOT;
   if(Rejestr->ReadString("LBA_"+Key+"_screenviewer","Created by","") == "LBA Screen Viewer"){
       Rejestr->EraseSection("."+Key);
       Rejestr->EraseSection("LBA_"+Key+"_screenviewer");
   }
   Rejestr->Free();
}
//---------------------------------------------------------------------------

void Tfrm_settings::SetAssociation(){
   (cblim->Checked) ? SetKey("lim",Application->ExeName+",1","LBA image") : DelKey("lim");
   (cbpal->Checked) ? SetKey("pal",Application->ExeName+",2","LBA palette") : DelKey("pal");
   (cblsp->Checked) ? SetKey("lsp",Application->ExeName+",3","LBA sprite") : DelKey("lsp");
   (cbbrk->Checked) ? SetKey("brk",Application->ExeName+",4","LBA brick") : DelKey("brk");
   (cblsr->Checked) ? SetKey("lsr",Application->ExeName+",5","LBA raw image") : DelKey("lsr");
   (cbfla->Checked) ? SetKey("fla",Application->ExeName+",6","LBA movie") : DelKey("fla");
   (cblba->Checked) ? SetKey("lba",Application->ExeName+",7","LBA saved game") : DelKey("lba");
   (cbhqr->Checked) ? SetKey("hqr",Application->ExeName+",8","LBA resource package") : DelKey("hqr");
   (cbile->Checked) ? SetKey("ile",Application->ExeName+",9","LBA island package") : DelKey("ile");
   (cbbmp->Checked) ? SetKey("bmp",Application->ExeName+",10","BMP Images (LBAScreenViewer)") : DelKey("bmp");
   (cbjpg->Checked) ? SetKey("jpg",Application->ExeName+",11","JPG Images (LBAScreenViewer)") : DelKey("jpg");
   (cbgif->Checked) ? SetKey("gif",Application->ExeName+",12","GIF Images (LBAScreenViewer)") : DelKey("gif");
   (cbpng->Checked) ? SetKey("png",Application->ExeName+",13","PNG Images (LBAScreenViewer)") : DelKey("png");
   (cbpcx->Checked) ? SetKey("pcx",Application->ExeName+",14","PCX Images (LBAScreenViewer)") : DelKey("pcx");
}
//---------------------------------------------------------------------------

void Tfrm_settings::ShowSettings(){
   loadSettings();
   SetAssociation();
   frm_settings->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_settings::FormClose(TObject *Sender,
      TCloseAction &Action)
{
   saveSettings();
   SetAssociation();
}
//---------------------------------------------------------------------------

int Tfrm_settings::file_exists(char *filename)
{
   return (access(filename, 0) == 0);
}
//---------------------------------------------------------------------------
