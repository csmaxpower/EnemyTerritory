//---------------------------------------------------------------------------

#ifndef AboutH
#define AboutH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class Tfrm_About : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
        TPanel *Panel1;
        TLabel *lbl_Appname;
        TLabel *lbl_Created;
        TLabel *lbl_AboutText1;
        TLabel *lbl_Url;
        TLabel *lbl_Version;
        TImage *img_WolfLogo;
        TImage *img_EtspLogo;
        TLabel *lbl_Warranty;
        TMemo *mem_Disclaimer;
        TLabel *lbl_Disclaimer;
        void __fastcall Panel1Click(TObject *Sender);
        void __fastcall lbl_AppnameClick(TObject *Sender);
        void __fastcall lbl_CreatedClick(TObject *Sender);
        void __fastcall lbl_UrlClick(TObject *Sender);
        void __fastcall lbl_VersionClick(TObject *Sender);
        void __fastcall img_WolfLogoClick(TObject *Sender);
private:	// Anwender-Deklarationen
public:		// Anwender-Deklarationen
        __fastcall Tfrm_About(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE Tfrm_About *frm_About;
//---------------------------------------------------------------------------
#endif
