//---------------------------------------------------------------------------

#ifndef PlayersH
#define PlayersH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "BMShapedForm.h"
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class Tfrm_Players : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
        TBMShapedForm *shapedfrm_Players;
        TListView *lstvw_Players;
        TLabel *lbl_Playerinfo;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall lstvw_PlayersCompare(TObject *Sender,
          TListItem *Item1, TListItem *Item2, int Data, int &Compare);
        void __fastcall lstvw_PlayersColumnClick(TObject *Sender,
          TListColumn *Column);
private:	// Anwender-Deklarationen
public:		// Anwender-Deklarationen
        __fastcall Tfrm_Players(TComponent* Owner);

        bool    m_bSortAscending;
        int     m_iLastColumnSort;

        void dockToMainWindow();
};
//---------------------------------------------------------------------------
extern PACKAGE Tfrm_Players *frm_Players;
//---------------------------------------------------------------------------
#endif
