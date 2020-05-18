//---------------------------------------------------------------------------

#ifndef ServerbrowserH
#define ServerbrowserH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
#include <ImgList.hpp>
//---------------------------------------------------------------------------
class Tfrm_Serverbrowser : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
        TRadioGroup *radgrp_Version;
        TListView *lstvw_Servers;
        TBitBtn *btn_Refresh;
        TImageList *imglst_Icons;
        TBitBtn *btn_AddServer;
        TBitBtn *btn_Close;
        TProgressBar *prgbar_QueryStatus;
        TLabel *lbl_Status;
        TBitBtn *btn_Search;
        TBitBtn *btn_SearchNext;
        TBitBtn *btn_SearchPrev;
        TBitBtn *btn_Join;
        TGroupBox *GroupBox1;
        TCheckBox *chk_HideFull;
        TCheckBox *chk_HideEmpty;
        TCheckBox *chk_HidePassworded;
        TCheckBox *chk_HidePing;
        TCheckBox *chk_Mod;
        TEdit *txt_Pinglimit;
        TComboBox *cmb_Mod;
        void __fastcall btn_RefreshClick(TObject *Sender);
        void __fastcall btn_CloseClick(TObject *Sender);
        void __fastcall btn_AddServerClick(TObject *Sender);
        void __fastcall lstvw_ServersColumnClick(TObject *Sender,
          TListColumn *Column);
        void __fastcall btn_SearchClick(TObject *Sender);
        void __fastcall btn_SearchNextClick(TObject *Sender);
        void __fastcall btn_SearchPrevClick(TObject *Sender);
        void __fastcall lstvw_ServersCompare(TObject *Sender,
          TListItem *Item1, TListItem *Item2, int Data, int &Compare);
        void __fastcall btn_JoinClick(TObject *Sender);
        void __fastcall radgrp_VersionClick(TObject *Sender);
private:	// Anwender-Deklarationen
public:		// Anwender-Deklarationen
        bool            m_bStopp;
        bool            m_bThreadActive;
        bool            m_bSortAscending;
        int             m_iLastColumnSort;
        int             m_iVersion;
        int             m_iSearchPos;
        AnsiString      m_strSearch;
        __fastcall Tfrm_Serverbrowser(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE Tfrm_Serverbrowser *frm_Serverbrowser;
//---------------------------------------------------------------------------
#endif
