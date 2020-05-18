//---------------------------------------------------------------------------

#include <vcl.h>
#include "Main.h"
#pragma hdrstop

#include "ServerDetails.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tfrm_ServerDetails *frm_ServerDetails;
//---------------------------------------------------------------------------
__fastcall Tfrm_ServerDetails::Tfrm_ServerDetails(TComponent* Owner)
        : TForm(Owner)
{
        m_bAddServer = true;
        m_iServerId = -1;
}
//---------------------------------------------------------------------------
void Tfrm_ServerDetails::clearForm()
{
        txt_ServerName->Text = "";
        txt_ServerIP->Text = "";
        txt_PrivatePass->Text = "";
        cmb_ETVersion->ItemIndex = 0;
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_ServerDetails::btn_CancelClick(TObject *Sender)
{
        clearForm();
        this->Close();
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_ServerDetails::btn_OKClick(TObject *Sender)
{
        if (txt_ServerName->Text.IsEmpty() || txt_ServerIP->Text.IsEmpty())
        {
                Application->MessageBoxA("Please fill in a server name and ip!", "ET Starter Pro - Warning", MB_OK | MB_ICONWARNING);
                return;
        }

        if (m_bAddServer)
        // We will add a server
        {
                SERVERNODE newServer;
                newServer.iETVersion = cmb_ETVersion->ItemIndex;
                strcpy(newServer.szServerName, txt_ServerName->Text.c_str());
                strcpy(newServer.szServerIP, txt_ServerIP->Text.c_str());
                strcpy(newServer.szPrivatePass, txt_PrivatePass->Text.c_str());

                frm_Main->m_Servers.push_back(newServer);
        }
        else
        // We will update a server
        {
                frm_Main->m_Servers[m_iServerId].iETVersion = cmb_ETVersion->ItemIndex;
                strcpy(frm_Main->m_Servers[m_iServerId].szServerName, txt_ServerName->Text.c_str());
                strcpy(frm_Main->m_Servers[m_iServerId].szServerIP, txt_ServerIP->Text.c_str());
                strcpy(frm_Main->m_Servers[m_iServerId].szPrivatePass, txt_PrivatePass->Text.c_str());
        }
        clearForm();
        this->Close();
}
//---------------------------------------------------------------------------

