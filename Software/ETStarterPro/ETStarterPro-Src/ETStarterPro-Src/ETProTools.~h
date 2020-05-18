//---------------------------------------------------------------------------

#ifndef ETProToolsH
#define ETProToolsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class Tfrm_ETProTools : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
        TBitBtn *btn_CleanETPro;
        TBitBtn *btn_RestoreETPro;
        TListBox *lst_Demos;
        TLabel *lbl_ETProDemos;
        TBitBtn *btn_PlayDemo;
        TBitBtn *btn_Refresh;
        TMemo *mem_Description;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall btn_CleanETProClick(TObject *Sender);
        void __fastcall btn_RestoreETProClick(TObject *Sender);
        void __fastcall lst_DemosDblClick(TObject *Sender);
        void __fastcall btn_PlayDemoClick(TObject *Sender);
        void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
        void __fastcall btn_RefreshClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// Anwender-Deklarationen
public:		// Anwender-Deklarationen
        __fastcall Tfrm_ETProTools(TComponent* Owner);

        void checkButtonStates();
        void fillDemoListbox();
        void playSelectedDemo();
        bool deleteDirectory(LPCTSTR lpszDir, bool noRecycleBin = false);
};
//---------------------------------------------------------------------------
extern PACKAGE Tfrm_ETProTools *frm_ETProTools;
//---------------------------------------------------------------------------
#endif
