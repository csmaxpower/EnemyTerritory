//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Players.h"
#include "Main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BMShapedForm"
#pragma resource "*.dfm"
Tfrm_Players *frm_Players;
//---------------------------------------------------------------------------
__fastcall Tfrm_Players::Tfrm_Players(TComponent* Owner)
        : TForm(Owner)
{
        m_bSortAscending = true;
        m_iLastColumnSort = 0;
}
//---------------------------------------------------------------------------
void Tfrm_Players::dockToMainWindow()
{
        Left = frm_Main->Left + (frm_Main->Width-12);
        Top = frm_Main->Top + (frm_Main->Height-345);
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_Players::FormShow(TObject *Sender)
{
        dockToMainWindow();        
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_Players::lstvw_PlayersCompare(TObject *Sender,
      TListItem *Item1, TListItem *Item2, int Data, int &Compare)
{
        switch (Data)
        {
                case 0: // Sort by Name
                        if (Item1->Caption <  Item2->Caption) Compare = -1;
                        else if (Item1->Caption >  Item2->Caption) Compare = 1;
                        else if (Item1->Caption ==  Item2->Caption) Compare = 0;
                        break;

                case 1: // Sort by Score
                        if (Item1->SubItems->operator [](0).ToInt() <  Item2->SubItems->operator [](0).ToInt()) Compare = -1;
                        else if (Item1->SubItems->operator [](0).ToInt() >  Item2->SubItems->operator [](0).ToInt()) Compare = 1;
                        else if (Item1->SubItems->operator [](0).ToInt() ==  Item2->SubItems->operator [](0).ToInt()) Compare = 0;
                        break;

                case 2: // Sort by Ping
                        if (Item1->SubItems->operator [](1).ToInt() <  Item2->SubItems->operator [](1).ToInt()) Compare = -1;
                        else if (Item1->SubItems->operator [](1).ToInt() >  Item2->SubItems->operator [](1).ToInt()) Compare = 1;
                        else if (Item1->SubItems->operator [](1).ToInt() ==  Item2->SubItems->operator [](1).ToInt()) Compare = 0;
                        break;

                default:
                        break;
        }
        
        if (!m_bSortAscending) Compare = Compare * -1;        
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_Players::lstvw_PlayersColumnClick(TObject *Sender,
      TListColumn *Column)
{
        frm_Players->lstvw_Players->SortType = Listactns::stData;
        if (m_iLastColumnSort == Column->Index) m_bSortAscending = !m_bSortAscending;
        else m_iLastColumnSort = Column->Index;
        frm_Players->lstvw_Players->CustomSort(NULL, Column->Index);        
}
//---------------------------------------------------------------------------
