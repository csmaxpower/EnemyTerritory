//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Serverbrowser.h"
#include "gamestats.h"
#include "QueryThread.h"
#include "Main.h"
#include "Options.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tfrm_Serverbrowser *frm_Serverbrowser;
//---------------------------------------------------------------------------
__fastcall Tfrm_Serverbrowser::Tfrm_Serverbrowser(TComponent* Owner)
        : TForm(Owner)
{
        m_bStopp = false;
        m_bThreadActive = false;
        m_bSortAscending = true;
        m_iLastColumnSort = 0;
        m_iVersion = 0;
        m_iSearchPos = 0;
        m_strSearch = "";
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_Serverbrowser::btn_RefreshClick(TObject *Sender)
{
        TQueryThread* pQueryThread = new TQueryThread(true);

        if (frm_Serverbrowser->btn_Refresh->Caption == "Refresh")
        {
                m_bStopp = false;
                m_bThreadActive = true;
                m_iVersion = radgrp_Version->ItemIndex;
                pQueryThread->Resume();
        }
        else
        {       m_bStopp = true;
        }

}
//---------------------------------------------------------------------------
void __fastcall Tfrm_Serverbrowser::btn_CloseClick(TObject *Sender)
{
        Close();        
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_Serverbrowser::btn_AddServerClick(TObject *Sender)
{
        int iCount = 0;
        SERVERNODE server;
        AnsiString strTemp = "";
        AnsiString strTemp2 = "";
        bool bAddOk = true;

        for (int i=0; i<lstvw_Servers->Items->Count; i++)
        {
                if (lstvw_Servers->Items->Item[i]->Checked)
                {

                        bAddOk = true;
                        strTemp =  lstvw_Servers->Items->Item[i]->SubItems->operator [](6) + ":" + lstvw_Servers->Items->Item[i]->SubItems->operator [](7);

                        // Cycle through the server list to check if the server already exists
                        for (int j=0; j<frm_Main->m_Servers.size(); j++)
                        {
                                if (strTemp == frm_Main->m_Servers[j].szServerIP)
                                {
                                        strTemp2 = "Duplicate server address!\n\nExisting server: \"";
                                        strTemp2 += frm_Main->m_Servers[j].szServerName;
                                        strTemp2 += "\"\nNew server:  \"";
                                        strTemp2 += lstvw_Servers->Items->Item[i]->SubItems->operator [](0);
                                        strTemp2 += "\"\n\nDo you still want to add the new server?";
                                        if (Application->MessageBoxA(strTemp2.c_str(), "ET Starter Pro - Duplicate server address", MB_YESNO | MB_ICONQUESTION) == IDNO) bAddOk = false;
                                }
                        }

                        //Actually add the server
                        if (bAddOk)
                        {
                                strcpy (server.szServerName, lstvw_Servers->Items->Item[i]->SubItems->operator [](0).c_str());
                                strcpy (server.szServerIP, strTemp.c_str());
                                strcpy (server.szPrivatePass, "");
                                server.iETVersion = m_iVersion;
                                frm_Main->m_Servers.push_back(server);
                                iCount++;
                        }
                }
        }

        if (iCount)
        {
                strTemp = iCount;
                strTemp += " server(s) successfully added!\n\n Now bringing you to the options screen.";
                frm_Options->reloadServerList();
                Application->MessageBoxA(strTemp.c_str(), "ET Starter Pro", MB_ICONINFORMATION | MB_OK);
                frm_Options->tab_Server->Show();

                if (!frm_Options->Visible) frm_Options->ShowModal();
                this->BringToFront();
        }
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_Serverbrowser::lstvw_ServersColumnClick(
      TObject *Sender, TListColumn *Column)
{
        if (!m_bThreadActive)
        {
                lstvw_Servers->SortType = Listactns::stData;
                if (m_iLastColumnSort == Column->Index) m_bSortAscending = !m_bSortAscending;
                else m_iLastColumnSort = Column->Index;
                lstvw_Servers->CustomSort(NULL, Column->Index);
        }
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_Serverbrowser::btn_SearchClick(TObject *Sender)
{
        AnsiString strMessage = "";

        m_strSearch = InputBox("Find servers by name", "Search Term:", "");

        if (m_strSearch != "")
        {
                for (int i=0; i<lstvw_Servers->Items->Count; i++)
                {
                        if (AnsiPos(m_strSearch.LowerCase(), lstvw_Servers->Items->Item[i]->SubItems->operator [](0).LowerCase()) ||
                            AnsiPos(m_strSearch, lstvw_Servers->Items->Item[i]->SubItems->operator [](6)) )
                        {
                                lstvw_Servers->ItemIndex = i;
                                lstvw_Servers->Items->Item[i]->MakeVisible(false);
                                m_iSearchPos = i;
                                return;
                        }
                }
                strMessage = "Your search for \"" + m_strSearch + "\" did not return any results!";
                Application->MessageBoxA(strMessage.c_str(), "ET Starter Pro", MB_OK | MB_ICONEXCLAMATION);
                return;
        }
        Application->MessageBoxA("Please enter a message to search for!", "ET Starter Pro", MB_OK | MB_ICONEXCLAMATION);
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_Serverbrowser::btn_SearchNextClick(TObject *Sender)
{
        if (m_strSearch != "")
        {
                for (int i=m_iSearchPos+1; i<lstvw_Servers->Items->Count; i++)
                {
                        if (AnsiPos(m_strSearch.LowerCase(), lstvw_Servers->Items->Item[i]->SubItems->operator [](0).LowerCase()) ||
                        AnsiPos(m_strSearch, lstvw_Servers->Items->Item[i]->SubItems->operator [](6)) )
                        {
                                lstvw_Servers->ItemIndex = i;
                                lstvw_Servers->Items->Item[i]->MakeVisible(false);
                                m_iSearchPos = i;
                                return;
                        }
                }
        }
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_Serverbrowser::btn_SearchPrevClick(TObject *Sender)
{
        if (m_strSearch != "")
        {
                if (m_iSearchPos > lstvw_Servers->Items->Count-1) m_iSearchPos = lstvw_Servers->Items->Count-1; 
                for (int i=m_iSearchPos-1; i>=0; i--)
                {
                        if (AnsiPos(m_strSearch.LowerCase(), lstvw_Servers->Items->Item[i]->SubItems->operator [](0).LowerCase()) ||
                        AnsiPos(m_strSearch, lstvw_Servers->Items->Item[i]->SubItems->operator [](6)) )
                        {
                                lstvw_Servers->ItemIndex = i;
                                lstvw_Servers->Items->Item[i]->MakeVisible(false);
                                m_iSearchPos = i;
                                return;
                        }
                }
        }
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_Serverbrowser::lstvw_ServersCompare(TObject *Sender,
      TListItem *Item1, TListItem *Item2, int Data, int &Compare)
{
        AnsiString strIp1, strIp2;
        int iIp1 = 0;
        int iIp2 = 0;

        switch (Data)
        {
                case 0: // Sort by Id
                        if (Item1->Caption.ToInt() <  Item2->Caption.ToInt()) Compare = -1;
                        else if (Item1->Caption.ToInt() >  Item2->Caption.ToInt()) Compare = 1;
                        else if (Item1->Caption.ToInt() ==  Item2->Caption.ToInt()) Compare = 0;
                        break;

                case 1: // Sort by Name
                        if (Item1->SubItems->operator [](0) <  Item2->SubItems->operator [](0)) Compare = -1;
                        else if (Item1->SubItems->operator [](0) >  Item2->SubItems->operator [](0)) Compare = 1;
                        else if (Item1->SubItems->operator [](0) ==  Item2->SubItems->operator [](0)) Compare = 0;
                        break;

                case 2: // Sort by Locked
                        if (Item1->SubItems->operator [](1).ToInt() <  Item2->SubItems->operator [](1).ToInt()) Compare = -1;
                        else if (Item1->SubItems->operator [](1).ToInt() >  Item2->SubItems->operator [](1).ToInt()) Compare = 1;
                        else if (Item1->SubItems->operator [](1).ToInt() ==  Item2->SubItems->operator [](1).ToInt()) Compare = 0;
                        break;

                case 3: // Sort by Players
                        strIp1 = Item1->SubItems->operator [](2);
                        strIp2 = Item2->SubItems->operator [](2);
                        iIp1 = StrToInt(strIp1.SubString(0, strIp1.AnsiPos("/")-1));
                        iIp2 = StrToInt(strIp2.SubString(0, strIp2.AnsiPos("/")-1));

                        if (iIp1 < iIp2) Compare = -1;
                        else if (iIp1 > iIp2) Compare = 1;
                        else if (iIp1 == iIp2) Compare = 0;
                        break;
                        break;

                case 4: // Sort by Ping
                        if (Item1->SubItems->operator [](3).ToInt() <  Item2->SubItems->operator [](3).ToInt()) Compare = -1;
                        else if (Item1->SubItems->operator [](3).ToInt() >  Item2->SubItems->operator [](3).ToInt()) Compare = 1;
                        else if (Item1->SubItems->operator [](3).ToInt() ==  Item2->SubItems->operator [](3).ToInt()) Compare = 0;
                        break;

                case 5: // Sort by Mod
                        if (Item1->SubItems->operator [](4) <  Item2->SubItems->operator [](4)) Compare = -1;
                        else if (Item1->SubItems->operator [](4) >  Item2->SubItems->operator [](4)) Compare = 1;
                        else if (Item1->SubItems->operator [](4) ==  Item2->SubItems->operator [](4)) Compare = 0;
                        break;

                case 6: // Sort by Map
                        if (Item1->SubItems->operator [](5) <  Item2->SubItems->operator [](5)) Compare = -1;
                        else if (Item1->SubItems->operator [](5) >  Item2->SubItems->operator [](5)) Compare = 1;
                        else if (Item1->SubItems->operator [](5) ==  Item2->SubItems->operator [](5)) Compare = 0;
                        break;

                case 7: // Sort by IP
                        strIp1 = Item1->SubItems->operator [](6);
                        strIp2 = Item2->SubItems->operator [](6);
                        iIp1 = StrToInt(strIp1.SubString(0, strIp1.AnsiPos(".")-1));
                        iIp2 = StrToInt(strIp2.SubString(0, strIp2.AnsiPos(".")-1));

                        if (iIp1 < iIp2) Compare = -1;
                        else if (iIp1 > iIp2) Compare = 1;
                        else if (iIp1 == iIp2) Compare = 0;
                        break;

                case 8: // Sort by Port
                        if (Item1->SubItems->operator [](7).ToInt() <  Item2->SubItems->operator [](7).ToInt()) Compare = -1;
                        else if (Item1->SubItems->operator [](7).ToInt() >  Item2->SubItems->operator [](7).ToInt()) Compare = 1;
                        else if (Item1->SubItems->operator [](7).ToInt() ==  Item2->SubItems->operator [](7).ToInt()) Compare = 0;
                        break;

                default:
                        m_iLastColumnSort = 0;
                        break;
        }

        if (!m_bSortAscending) Compare = Compare * -1;
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_Serverbrowser::btn_JoinClick(TObject *Sender)
{
        AnsiString strTemp = "";

        if (!FileExists(frm_Options->txt_ETPath->Text))
        {
                Application->MessageBoxA("Please specify a valid path to et.exe in the options dialog!", "ET Starter Pro", MB_OK | MB_ICONEXCLAMATION);
                return;
        }

        if (lstvw_Servers->ItemIndex != -1)
        {
                if (!frm_Main->changeVersion(m_iVersion))
                {
                        Application->MessageBoxA("Could not copy patch files to your ET directory!\n\nMake sure that files are not write protected or in use.\nAlso make sure that the directory \"VersionData\" exists in the ETStarter Pro directory.", "ET Starter Pro - Error", MB_ICONERROR | MB_OK);
                        return;
                }
                strTemp =  " +connect " + lstvw_Servers->Items->Item[lstvw_Servers->ItemIndex]->SubItems->operator [](6) + ":" + lstvw_Servers->Items->Item[lstvw_Servers->ItemIndex]->SubItems->operator [](7);
                ShellExecute(NULL, "open", frm_Options->txt_ETPath->Text.c_str(), strTemp.c_str(), ExtractFileDir(frm_Options->txt_ETPath->Text).c_str(), SW_SHOWNORMAL);
                Application->Terminate();
                
        }
        else Application->MessageBoxA("Please selecet a server to join!", "ET Starter Pro", MB_OK | MB_ICONEXCLAMATION);
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_Serverbrowser::radgrp_VersionClick(TObject *Sender)
{
        lstvw_Servers->Clear();        
}
//---------------------------------------------------------------------------


