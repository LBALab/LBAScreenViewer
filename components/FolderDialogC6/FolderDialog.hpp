// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'FolderDialog.pas' rev: 6.00

#ifndef FolderDialogHPP
#define FolderDialogHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <Windows.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Folderdialog
{
//-- type declarations -------------------------------------------------------
#pragma option push -b-
enum TOptions { fdoBrowseForComputer, fdoBrowseForPrinter, fdoDontgoBelowDomain, fdoReturnFSAncestors, fdoReturnOnlyFSDirs, fdoStatusText, fdoNewDialogStyle, fdoNoNewFolderButton };
#pragma option pop

typedef Set<TOptions, fdoBrowseForComputer, fdoNoNewFolderButton>  TFolderOptions;

#pragma option push -b-
enum TDialogPosition { fdpDefault, fdpScreenCenter, fdpXY };
#pragma option pop

class DELPHICLASS TFolderDialog;
class PASCALIMPLEMENTATION TFolderDialog : public Classes::TComponent 
{
	typedef Classes::TComponent inherited;
	
private:
	AnsiString FTitle;
	AnsiString FDirectory;
	TFolderOptions FOptions;
	int FImageIndex;
	int FDialogX;
	int FDialogY;
	TDialogPosition FDialogPosition;
	void __fastcall SetTitle(const AnsiString Value);
	void __fastcall SetDirectory(const AnsiString Value);
	void __fastcall SetOptions(const TFolderOptions Value);
	void __fastcall SetDialogPosition(const TDialogPosition Value);
	void __fastcall SetDialogX(const int Value);
	void __fastcall SetDialogY(const int Value);
	AnsiString __fastcall GetVersion();
	void __fastcall SetVersion(const AnsiString Value);
	
protected:
	int __fastcall GetVersionNr(void);
	
public:
	__fastcall virtual TFolderDialog(Classes::TComponent* Aowner);
	bool __fastcall Execute(void);
	
__published:
	__property AnsiString Title = {read=FTitle, write=SetTitle};
	__property AnsiString Directory = {read=FDirectory, write=SetDirectory};
	__property int ImageIndex = {read=FImageIndex, nodefault};
	__property TFolderOptions Options = {read=FOptions, write=SetOptions, default=64};
	__property TDialogPosition DialogPosition = {read=FDialogPosition, write=SetDialogPosition, default=0};
	__property int DialogX = {read=FDialogX, write=SetDialogX, nodefault};
	__property int DialogY = {read=FDialogY, write=SetDialogY, nodefault};
	__property AnsiString Version = {read=GetVersion, write=SetVersion};
public:
	#pragma option push -w-inl
	/* TComponent.Destroy */ inline __fastcall virtual ~TFolderDialog(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------
static const Shortint MAJ_VER = 0x1;
static const Shortint MIN_VER = 0x0;
static const Shortint REL_VER = 0x0;
static const Shortint BLD_VER = 0x0;

}	/* namespace Folderdialog */
using namespace Folderdialog;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// FolderDialog
