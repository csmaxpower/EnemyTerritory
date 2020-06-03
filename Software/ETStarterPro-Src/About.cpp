//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "About.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tfrm_About *frm_About;
//---------------------------------------------------------------------------
__fastcall Tfrm_About::Tfrm_About(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_About::Panel1Click(TObject *Sender)
{
        this->Close();
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_About::lbl_AppnameClick(TObject *Sender)
{
        this->Close(); 
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_About::lbl_CreatedClick(TObject *Sender)
{
        this->Close();
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_About::lbl_UrlClick(TObject *Sender)
{
        //Bring ´em to http://www.pnd-funclan.com/
        ShellExecute(NULL, "open", "http://www.pnd-funclan.com/", NULL, NULL, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_About::lbl_VersionClick(TObject *Sender)
{
        this->Close(); 
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_About::img_WolfLogoClick(TObject *Sender)
{
        this->Close();        
}
//---------------------------------------------------------------------------

