//---------------------------------------------------------------------------

#ifndef OptionsH
#define OptionsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class Tfrm_Options : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
        TPanel *pnl_Headline;
        TBitBtn *btn_OK;
        TOpenDialog *dlg_ETPath;
        TBitBtn *btn_Cancel;
        TLabeledEdit *txt_ETPath;
        TBitBtn *btn_GetETPath;
        TListBox *lst_Servers;
        TBitBtn *btn_AddServer;
        TBitBtn *btn_EditServer;
        TBitBtn *btn_DeleteServer;
        TLabeledEdit *txt_TSServer;
        TLabeledEdit *txt_TSNick;
        TLabeledEdit *txt_TSChan;
        TLabeledEdit *txt_TSChanPass;
        TLabeledEdit *txt_TSUser;
        TLabeledEdit *txt_TSPass;
        TLabeledEdit *txt_CustomAppPath;
        TBitBtn *btn_GetCustomAppPath;
        TOpenDialog *dlg_CustomAppPath;
        TLabeledEdit *txt_CustomAppTerminate;
        TPageControl *pg_Options;
        TTabSheet *tab_Server;
        TTabSheet *tab_TS;
        TTabSheet *tab_General;
        TBitBtn *btn_ServerUp;
        TBitBtn *btn_ServerDown;
        TLabel *lbl_Optional;
        TBevel *bvl_Bottom;
        TBitBtn *btn_ShowServerList;
        TCheckBox *chk_QueryServer;
        TLabeledEdit *txt_Highlight;
        TRadioGroup *radgrp_TSVentrilo;
        TLabeledEdit *txt_ETArgs;
        void __fastcall btn_GetETPathClick(TObject *Sender);
        void __fastcall btn_AddServerClick(TObject *Sender);
        void __fastcall btn_EditServerClick(TObject *Sender);
        void __fastcall btn_DeleteServerClick(TObject *Sender);
        void __fastcall btn_OKClick(TObject *Sender);
        void __fastcall btn_CancelClick(TObject *Sender);
        void __fastcall btn_GetCustomAppPathClick(TObject *Sender);
        void __fastcall btn_ServerUpClick(TObject *Sender);
        void __fastcall btn_ServerDownClick(TObject *Sender);
        void __fastcall lst_ServersClick(TObject *Sender);
        void __fastcall btn_ShowServerListClick(TObject *Sender);
        void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
        void __fastcall radgrp_TSVentriloClick(TObject *Sender);
private:	// Anwender-Deklarationen
public:		// Anwender-Deklarationen
        __fastcall Tfrm_Options(TComponent* Owner);
        void reloadServerList(int iNewItemIndex = 0);
        void setButtonStates();
};
//---------------------------------------------------------------------------
extern PACKAGE Tfrm_Options *frm_Options;
//---------------------------------------------------------------------------
#endif
