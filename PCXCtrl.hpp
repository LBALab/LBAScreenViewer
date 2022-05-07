// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Pcxctrl.pas' rev: 6.00

#ifndef PcxctrlHPP
#define PcxctrlHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <Dialogs.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Messages.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Pcxctrl
{
//-- type declarations -------------------------------------------------------
#pragma pack(push, 1)
struct TPCXTriple
{
	Byte r;
	Byte g;
	Byte b;
} ;
#pragma pack(pop)

typedef TPCXTriple TPCXColors256[256];

typedef TPCXTriple TPcxColors16[16];

#pragma pack(push, 2)
struct TPCXHeader
{
	Byte Maker;
	Byte Version;
	Byte Encoding;
	Byte Bpp;
	short Xmn;
	short Ymn;
	short Xmx;
	short Ymx;
	short HRes;
	short VRes;
	TPCXTriple CMap[16];
	Byte Reserved;
	Byte NPlanes;
	short NBpl;
	short PalType;
} ;
#pragma pack(pop)

#pragma option push -b-
enum TPCXColorModel { Color16, Color256 };
#pragma option pop

class DELPHICLASS TPCXBitmap;
class PASCALIMPLEMENTATION TPCXBitmap : public Graphics::TBitmap 
{
	typedef Graphics::TBitmap inherited;
	
private:
	TPCXHeader FHeader;
	TPCXColorModel FColorModel;
	Classes::TMemoryStream* __fastcall ConvertStream(Classes::TStream* Stream);
	
public:
	__fastcall virtual TPCXBitmap(void);
	__fastcall virtual ~TPCXBitmap(void);
	virtual void __fastcall LoadFromStream(Classes::TStream* Stream);
};


class DELPHICLASS TDecoder;
class PASCALIMPLEMENTATION TDecoder : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	int ImageHeight;
	int ImageWidth;
	int ImageBytesPerLine;
	Byte Buffer[1024];
	int BufferIndex;
	Word bmiSize;
	int ImageOffset;
	
protected:
	void __fastcall Decode_Row(Classes::TStream* Stream, int RL, Classes::TMemoryStream* ScanLine);
	virtual Windows::PBitmapInfo __fastcall MakeBMIInfo(Classes::TStream* Stream);
	virtual tagBITMAPINFOHEADER __fastcall MakeBMInfoHeader(void) = 0 ;
	virtual void __fastcall MakeBMIColors(Classes::TStream* Stream, Windows::PBitmapInfo BitmapInfo) = 0 ;
	virtual int __fastcall GetPaletteSize(void) = 0 ;
	tagBITMAPFILEHEADER __fastcall MakeBMFileHeader(Windows::PBitmapInfo Bmi);
	int __fastcall GetImageSize(void);
	
public:
	__fastcall TDecoder(void);
	__fastcall ~TDecoder(void);
	virtual int __fastcall DecodeStream(Classes::TStream* AStream, const TPCXHeader &AHeader, Classes::TMemoryStream* AImage) = 0 ;
};


class DELPHICLASS TDecoder256;
class PASCALIMPLEMENTATION TDecoder256 : public TDecoder 
{
	typedef TDecoder inherited;
	
protected:
	virtual tagBITMAPINFOHEADER __fastcall MakeBMInfoHeader();
	virtual void __fastcall MakeBMIColors(Classes::TStream* Stream, Windows::PBitmapInfo BitmapInfo);
	virtual int __fastcall GetPaletteSize(void);
	
public:
	virtual int __fastcall DecodeStream(Classes::TStream* AStream, const TPCXHeader &PCXHeader, Classes::TMemoryStream* AImage);
public:
	#pragma option push -w-inl
	/* TDecoder.Create */ inline __fastcall TDecoder256(void) : TDecoder() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TDecoder.Destroy */ inline __fastcall ~TDecoder256(void) { }
	#pragma option pop
	
};


class DELPHICLASS TDecoder16;
class PASCALIMPLEMENTATION TDecoder16 : public TDecoder 
{
	typedef TDecoder inherited;
	
protected:
	virtual tagBITMAPINFOHEADER __fastcall MakeBMInfoHeader();
	virtual void __fastcall MakeBMIColors(Classes::TStream* Stream, Windows::PBitmapInfo BitmapInfo);
	virtual int __fastcall GetPaletteSize(void);
	
public:
	virtual int __fastcall DecodeStream(Classes::TStream* AStream, const TPCXHeader &PCXHeader, Classes::TMemoryStream* AImage);
public:
	#pragma option push -w-inl
	/* TDecoder.Create */ inline __fastcall TDecoder16(void) : TDecoder() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TDecoder.Destroy */ inline __fastcall ~TDecoder16(void) { }
	#pragma option pop
	
};


class DELPHICLASS EPCXPropertyError;
class PASCALIMPLEMENTATION EPCXPropertyError : public Sysutils::Exception 
{
	typedef Sysutils::Exception inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EPCXPropertyError(const AnsiString Msg) : Sysutils::Exception(Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EPCXPropertyError(const AnsiString Msg, const System::TVarRec * Args, const int Args_Size) : Sysutils::Exception(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EPCXPropertyError(int Ident)/* overload */ : Sysutils::Exception(Ident) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EPCXPropertyError(int Ident, const System::TVarRec * Args, const int Args_Size)/* overload */ : Sysutils::Exception(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EPCXPropertyError(const AnsiString Msg, int AHelpContext) : Sysutils::Exception(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EPCXPropertyError(const AnsiString Msg, const System::TVarRec * Args, const int Args_Size, int AHelpContext) : Sysutils::Exception(Msg, Args, Args_Size, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EPCXPropertyError(int Ident, int AHelpContext)/* overload */ : Sysutils::Exception(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EPCXPropertyError(System::PResStringRec ResStringRec, const System::TVarRec * Args, const int Args_Size, int AHelpContext)/* overload */ : Sysutils::Exception(ResStringRec, Args, Args_Size, AHelpContext) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EPCXPropertyError(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Pcxctrl */
using namespace Pcxctrl;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Pcxctrl
