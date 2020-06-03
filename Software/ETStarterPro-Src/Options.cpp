//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Main.h"
#include "Options.h"
#include "ServerDetails.h"
#include "Serverbrowser.h"


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tfrm_Options *frm_Options;
//---------------------------------------------------------------------------
__fastcall Tfrm_Options::Tfrm_Options(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void Tfrm_Options::setButtonStates()
{
        int iId = lst_Servers->ItemIndex;

        if (lst_Servers->Count < 2)
        {
                btn_ServerUp->Enabled = false;
                btn_ServerDown->Enabled = false;
                return;
        }
        else
        {
                btn_ServerUp->Enabled = true;
                btn_ServerDown->Enabled = true;
                if (iId == 0) btn_ServerUp->Enabled = false;
                if (iId == lst_Servers->Count-1) btn_ServerDown->Enabled = false;
        }
        return;
}
//---------------------------------------------------------------------------
void Tfrm_Options::reloadServerList(int iNewItemIndex)
{
        lst_Servers->Clear();
        for (unsigned int i=0; i<frm_Main->m_Servers.size(); i++)
        {
                lst_Servers->Items->Add(frm_Main->m_Servers[i].szServerName);
        }

        // Try to set new ItemIndex
        if (iNewItemIndex >= 0 && iNewItemIndex < frm_Main->m_Servers.size())
        {
                lst_Servers->ItemIndex = iNewItemIndex;
        }
        else lst_Servers->ItemIndex = 0;

        setButtonStates();

        return;
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_Options::btn_GetETPathClick(TObject *Sender)
{
        if (dlg_ETPath->Execute()) txt_ETPath->Text = dlg_ETPath->FileName;
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_Options::btn_AddServerClick(TObject *Sender)
{
        frm_ServerDetails->m_bAddServer = true;
        frm_ServerDetails->ShowModal();

        reloadServerList();
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_Options::btn_EditServerClick(TObject *Sender)
{
        int iId = lst_Servers->ItemIndex;

        if (iId != -1)
        {
                frm_ServerDetails->m_bAddServer = false;
                frm_ServerDetails->m_iServerId = lst_Servers->ItemIndex;

                // Fill the server details form with the former data
                frm_ServerDetails->txt_ServerName->Text = frm_Main->m_Servers[iId].szServerName;
                frm_ServerDetails->txt_ServerIP->Text = frm_Main->m_Servers[iId].szServerIP;
                frm_ServerDetails->txt_PrivatePass->Text = frm_Main->m_Servers[iId].szPrivatePass;
                frm_ServerDetails->cmb_ETVersion->ItemIndex = frm_Main->m_Servers[iId].iETVersion;

                frm_ServerDetails->ShowModal();
        }

        reloadServerList();
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_Options::btn_DeleteServerClick(TObject *Sender)
{
        int iId = lst_Servers->ItemIndex;
        vector<SERVERNODE>::iterator where;


        if (iId != -1)
        {
                where = frm_Main->m_Servers.begin() + iId;
                frm_Main->m_Servers.erase(where);
        }

        reloadServerList();
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_Options::btn_OKClick(TObject *Sender)
{
        AnsiString numServers = lst_Servers->Count;
        AnsiString strTemp, strTemp2;
        char szTemp[512];
        char szTemp1[2], szTemp2[2], szTemp3[2], szTemp4[2], szTemp5[2], szTemp6[2], szTemp7[2];

        strTemp = strTemp2 = "";

        // Check if the Path is set and if the file exists
        if (txt_ETPath->Text.IsEmpty() || !FileExists(txt_ETPath->Text))
        {
                Application->MessageBoxA("The specified path to ET.exe is incorrect!", "ET Starter Pro - Warning", MB_OK | MB_ICONWARNING);
                return;
        }

        // OK, so remember the main form checkbox values
        GetPrivateProfileStringA("General", "ChkStartETMin", "1", szTemp1, 2, frm_Main->m_IniFile.c_str());
        GetPrivateProfileStringA("General", "ChkStartTS", "1", szTemp2, 2, frm_Main->m_IniFile.c_str());
        GetPrivateProfileStringA("General", "ChkDeleteETKey", "1", szTemp3, 2, frm_Main->m_IniFile.c_str());
        GetPrivateProfileStringA("General", "ChkStartCustomApp", "1", szTemp4, 2, frm_Main->m_IniFile.c_str());
        GetPrivateProfileStringA("General", "ChkTerminateCustomApp", "1", szTemp5, 2, frm_Main->m_IniFile.c_str());
        GetPrivateProfileStringA("General", "ChkDeleteProfilePid", "1", szTemp6, 2, frm_Main->m_IniFile.c_str());
        GetPrivateProfileStringA("General", "ChkCustomArgs", "1", szTemp7, 2, frm_Main->m_IniFile.c_str());

        // Delete the ini file
        DeleteFileA(frm_Main->m_IniFile);

        // Write the et-path to the new ini file
        WritePrivateProfileStringA("General", "ETPath", txt_ETPath->Text.c_str(), frm_Main->m_IniFile.c_str());

        // Write the custom command line arguments to the ini file
        WritePrivateProfileStringA("General", "ETArgs", txt_ETArgs->Text.c_str(), frm_Main->m_IniFile.c_str());

        // Write custom App path to the ini
        WritePrivateProfileStringA("General", "CustomApp", txt_CustomAppPath->Text.c_str(), frm_Main->m_IniFile.c_str());

        // Write custom Apps to terminate to the ini
        WritePrivateProfileStringA("General", "CustomAppsQuit", txt_CustomAppTerminate->Text.c_str(), frm_Main->m_IniFile.c_str());

        // Write player highlight tags to ini
        WritePrivateProfileStringA("General", "HighlightTags", txt_Highlight->Text.c_str(), frm_Main->m_IniFile.c_str());

        // Write the number of servers to the ini file
        WritePrivateProfileStringA("General", "NumServers", numServers.c_str(), frm_Main->m_IniFile.c_str());

        // Restore the main form checkbox values
        WritePrivateProfileStringA("General", "ChkStartETMin", szTemp1, frm_Main->m_IniFile.c_str());
        WritePrivateProfileStringA("General", "ChkStartTS", szTemp2, frm_Main->m_IniFile.c_str());
        WritePrivateProfileStringA("General", "ChkDeleteETKey", szTemp3, frm_Main->m_IniFile.c_str());
        WritePrivateProfileStringA("General", "ChkStartCustomApp", szTemp4, frm_Main->m_IniFile.c_str());
        WritePrivateProfileStringA("General", "ChkTerminateCustomApp", szTemp5, frm_Main->m_IniFile.c_str());
        WritePrivateProfileStringA("General", "ChkDeleteProfilePid", szTemp6, frm_Main->m_IniFile.c_str());
        WritePrivateProfileStringA("General", "ChkCustomArgs", szTemp7, frm_Main->m_IniFile.c_str());

        // Write the Teamspeak/Ventrilo config to the ini file
        WritePrivateProfileStringA("Teamspeak", "AppToUse", IntToStr(radgrp_TSVentrilo->ItemIndex).c_str(), frm_Main->m_IniFile.c_str());
        WritePrivateProfileStringA("Teamspeak", "Server", txt_TSServer->Text.c_str(), frm_Main->m_IniFile.c_str());
        WritePrivateProfileStringA("Teamspeak", "Chan", txt_TSChan->Text.c_str(), frm_Main->m_IniFile.c_str());
        WritePrivateProfileStringA("Teamspeak", "ChanPass", txt_TSChanPass->Text.c_str(), frm_Main->m_IniFile.c_str());
        WritePrivateProfileStringA("Teamspeak", "Nick", txt_TSNick->Text.c_str(), frm_Main->m_IniFile.c_str());
        WritePrivateProfileStringA("Teamspeak", "User", txt_TSUser->Text.c_str(), frm_Main->m_IniFile.c_str());
        WritePrivateProfileStringA("Teamspeak", "Pass", txt_TSPass->Text.c_str(), frm_Main->m_IniFile.c_str());

        // Write the servers to the ini file
        for (int i=0; i<lst_Servers->Count; i++)
        {
                strTemp = i;
                strTemp2 = frm_Main->m_Servers[i].iETVersion;
                WritePrivateProfileStringA(strTemp.c_str(), "ServerName", frm_Main->m_Servers[i].szServerName, frm_Main->m_IniFile.c_str());
                WritePrivateProfileStringA(strTemp.c_str(), "ServerIP", frm_Main->m_Servers[i].szServerIP, frm_Main->m_IniFile.c_str());
                WritePrivateProfileStringA(strTemp.c_str(), "PrivatePass", frm_Main->m_Servers[i].szPrivatePass, frm_Main->m_IniFile.c_str());
                WritePrivateProfileStringA(strTemp.c_str(), "ETVersion", strTemp2.c_str(), frm_Main->m_IniFile.c_str());
        }

        // Write to the ini file if we should query the servers
        if (chk_QueryServer->Checked) WritePrivateProfileStringA("General", "Query", "1", frm_Main->m_IniFile.c_str());
        else WritePrivateProfileStringA("General", "Query", "0", frm_Main->m_IniFile.c_str());

        frm_Main->readIniFile();
        frm_Main->fillForms();

        if (frm_Main->cmb_Server->ItemIndex >= 0) frm_Main->cmb_ServerChange(this);

        this->Close();
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_Options::btn_CancelClick(TObject *Sender)
{
        frm_Main->readIniFile();
        frm_Main->fillForms();
        
        this->Close();
}
//---------------------------------------------------------------------------


void __fastcall Tfrm_Options::btn_GetCustomAppPathClick(TObject *Sender)
{
        if (dlg_CustomAppPath->Execute())
        {
                txt_CustomAppPath->Text = dlg_CustomAppPath->FileName;
        }
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_Options::btn_ServerUpClick(TObject *Sender)
{
        int iId = lst_Servers->ItemIndex;

        if (iId > 0)
        {
                SERVERNODE nTemp;
                nTemp = frm_Main->m_Servers[iId];
                frm_Main->m_Servers[iId] = frm_Main->m_Servers[iId-1];
                frm_Main->m_Servers[iId-1] = nTemp;

                reloadServerList();
                lst_Servers->ItemIndex = iId-1;
                setButtonStates();
        }
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_Options::btn_ServerDownClick(TObject *Sender)
{
        int iId = lst_Servers->ItemIndex;
        int iCount = lst_Servers->Count;

        if (iId > -1 && iId < (iCount-1))
        {
                SERVERNODE nTemp;
                nTemp = frm_Main->m_Servers[iId];
                frm_Main->m_Servers[iId] = frm_Main->m_Servers[iId+1];
                frm_Main->m_Servers[iId+1] = nTemp;

                reloadServerList();
                lst_Servers->ItemIndex = iId+1;
                setButtonStates();
        }
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_Options::lst_ServersClick(TObject *Sender)
{
        setButtonStates();
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_Options::btn_ShowServerListClick(TObject *Sender)
{
        frm_Serverbrowser->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_Options::FormCloseQuery(TObject *Sender,
      bool &CanClose)
{
        btn_CancelClick(this);        
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_Options::radgrp_TSVentriloClick(TObject *Sender)
{
        if (radgrp_TSVentrilo->ItemIndex == 0)
        {
                // TS selected
                txt_TSUser->Enabled = true;
                txt_TSNick->Enabled = true;
        }
        else if (radgrp_TSVentrilo->ItemIndex == 1)
        {
                // Ventrilo selected
                txt_TSUser->Enabled = false;
                txt_TSNick->Enabled = false;
        }
}
//---------------------------------------------------------------------------

