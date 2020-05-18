//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "QueryThread.h"
#include "Serverbrowser.h"
#include "gamestat.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Wichtig: Methoden und Eigenschaften von Objekten der VCL können nur
//   in Methoden verwendet werden, die Synchronize aufrufen, z.B.:
//
//      Synchronize(UpdateCaption);
//
//   wobei UpdateCaption so aussehen könnte:
//
//      void __fastcall TQueryThread::UpdateCaption()
//      {
//        Form1->Caption = "In Thread aktualisiert";
//      }
//---------------------------------------------------------------------------

__fastcall TQueryThread::TQueryThread(bool CreateSuspended)
        : TThread(CreateSuspended)
{
}
//---------------------------------------------------------------------------
void __fastcall TQueryThread::Execute()
{
        //---- Hier den Thread-Code plazieren----

        GS_SERVER* pServer = NULL;
        GS_SERVER* pAktServer = NULL;
        GS_SERVER* pTempServer = NULL;
        GS_SERVERINFO* pServerInfo = NULL;
        unsigned short usProtocol = 84;
        int iCount = 0;
        int iTempCount = 0;
        bool bMasterSuccess = false;
        bool bFiltered = false;
        AnsiString strPlayers;

        frm_Serverbrowser->btn_Refresh->Enabled = false;
        frm_Serverbrowser->btn_Search->Enabled = false;
        frm_Serverbrowser->radgrp_Version->Enabled = false;
        frm_Serverbrowser->btn_AddServer->Enabled = false;
        frm_Serverbrowser->btn_Join->Enabled = false;
        frm_Serverbrowser->btn_SearchNext->Enabled = false;
        frm_Serverbrowser->btn_SearchPrev->Enabled = false;
        frm_Serverbrowser->btn_Close->Enabled = false;
        frm_Serverbrowser->lbl_Status->Caption = "Retrieving server list from master...";
        frm_Serverbrowser->lstvw_Servers->Clear();
        frm_Serverbrowser->lstvw_Servers->SortType = Listactns::stNone;
        frm_Serverbrowser->prgbar_QueryStatus->Position = 0;

        if (frm_Serverbrowser->radgrp_Version->ItemIndex == 0) usProtocol = 82;
        else if (frm_Serverbrowser->radgrp_Version->ItemIndex == 1) usProtocol = 83;
        else if (frm_Serverbrowser->radgrp_Version->ItemIndex == 2) usProtocol = 84;

        GSSetOptInt(GS_OPTION_TIMEOUT, 1000);

        pTempServer = pAktServer = pServer = GSQueryMaster(GS_MASTER_WOLFET, NULL, NULL, GS_MASTER_FILTER_NONE, usProtocol);

        if (pServer) bMasterSuccess = true;

        frm_Serverbrowser->btn_Refresh->Kind = bkAbort;
        frm_Serverbrowser->btn_Refresh->Kind = bkCustom;
        frm_Serverbrowser->btn_Refresh->ModalResult = mrNone;
        frm_Serverbrowser->btn_Refresh->Caption = "Stop";
        frm_Serverbrowser->btn_Refresh->Enabled = true;

        // Count ´em
        while (pTempServer)
        {
                iTempCount++;
                pTempServer = pTempServer->next;
        }
        frm_Serverbrowser->prgbar_QueryStatus->Max = iTempCount;
        iTempCount = 0;
        frm_Serverbrowser->lbl_Status->Caption = "Querying servers...";

        GSSetOptInt(GS_OPTION_TIMEOUT, 300);

        while (pAktServer)
        {
                if (pServerInfo = GSQueryServer(GS_TYPE_WOLFET, pAktServer->host, pAktServer->port))
                {
                        bFiltered = false;

                        // Check server filters
                        // Hide full servers?
                        if (frm_Serverbrowser->chk_HideFull->Checked && (pServerInfo->numplayers == pServerInfo->maxplayers)) bFiltered = true;
                        // Hide empty servers
                        if (frm_Serverbrowser->chk_HideEmpty->Checked && (pServerInfo->numplayers == 0)) bFiltered = true;
                        // Hide passworded servers
                        if (frm_Serverbrowser->chk_HidePassworded->Checked && (pServerInfo->passworded == 1)) bFiltered = true;

                        // Hide servers with ping higher than ...
                        if (frm_Serverbrowser->chk_HidePing->Checked && (pServerInfo->ping > StrToInt(frm_Serverbrowser->txt_Pinglimit->Text))) bFiltered = true;
                        // Only show servers running mod ...
                        if (frm_Serverbrowser->chk_Mod->Checked && (frm_Serverbrowser->cmb_Mod->Text != pServerInfo->mod)) bFiltered = true;

                        if (!bFiltered)
                        {
                                strPlayers = pServerInfo->numplayers;
                                strPlayers += "/";
                                strPlayers += pServerInfo->maxplayers;

                                frm_Serverbrowser->lstvw_Servers->Items->Add();
                                frm_Serverbrowser->lstvw_Servers->Items->Item[iCount]->Caption = IntToStr(iCount);
                                frm_Serverbrowser->lstvw_Servers->Items->Item[iCount]->SubItems->Add(GSStripColor(GS_TYPE_WOLFET, pServerInfo->host, pServerInfo->host));
                                frm_Serverbrowser->lstvw_Servers->Items->Item[iCount]->SubItems->Add(pServerInfo->passworded);
                                frm_Serverbrowser->lstvw_Servers->Items->Item[iCount]->SubItems->Add(strPlayers);
                                frm_Serverbrowser->lstvw_Servers->Items->Item[iCount]->SubItems->Add(pServerInfo->ping);
                                frm_Serverbrowser->lstvw_Servers->Items->Item[iCount]->SubItems->Add(pServerInfo->mod);
                                frm_Serverbrowser->lstvw_Servers->Items->Item[iCount]->SubItems->Add(pServerInfo->map);
                                frm_Serverbrowser->lstvw_Servers->Items->Item[iCount]->SubItems->Add(pAktServer->host);
                                frm_Serverbrowser->lstvw_Servers->Items->Item[iCount]->SubItems->Add(pAktServer->port);

                                iCount++;
                        }

                        GSFreeServerInfo(pServerInfo);
                        pServerInfo = NULL;
                }
                iTempCount++;
                frm_Serverbrowser->prgbar_QueryStatus->StepIt();
                pAktServer = pAktServer->next;

                if (frm_Serverbrowser->m_bStopp) break;
        }

        if (pServer) GSFreeServers(pServer);

        frm_Serverbrowser->btn_Refresh->Kind = bkRetry;
        frm_Serverbrowser->btn_Refresh->Kind = bkCustom;
        frm_Serverbrowser->btn_Refresh->ModalResult = mrNone;
        frm_Serverbrowser->btn_Refresh->Caption = "Refresh";

        frm_Serverbrowser->radgrp_Version->Enabled = true;
        frm_Serverbrowser->btn_AddServer->Enabled = true;
        frm_Serverbrowser->btn_Close->Enabled = true;
        frm_Serverbrowser->btn_Search->Enabled = true;
        frm_Serverbrowser->btn_Join->Enabled = true;
        frm_Serverbrowser->btn_SearchNext->Enabled = true;
        frm_Serverbrowser->btn_SearchPrev->Enabled = true;
        if (bMasterSuccess) frm_Serverbrowser->lbl_Status->Caption = "Idle...";
        else frm_Serverbrowser->lbl_Status->Caption = "Could not connect to master server...";
        frm_Serverbrowser->prgbar_QueryStatus->Position = 0;
        frm_Serverbrowser->lstvw_Servers->ColumnClick = true;

        frm_Serverbrowser->m_bThreadActive = false;
}
//---------------------------------------------------------------------------
