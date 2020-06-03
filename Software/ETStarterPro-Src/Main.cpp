//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Main.h"
#include "gamestat.h"
#include "Serverbrowser.h"
#include "Players.h"
#include "ETProTools.h"
#include <direct.h>
#include <tlhelp32.h>
#include <fstream.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "trayicon"
#pragma link "BMShapedForm"
#pragma resource "*.dfm"

#include "Options.h"
#include "About.h"

using namespace std;

Tfrm_Main *frm_Main;
//---------------------------------------------------------------------------
__fastcall Tfrm_Main::Tfrm_Main(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
bool Tfrm_Main::startMinimizer()
{
        AnsiString strTemp;

        if (processExists("etmin.exe")) return false;

        strTemp = "\"" + ExtractFileDir(Application->ExeName) + "\\Apps\\etmin.exe\"";
        if ((int) ShellExecute(NULL, "open", strTemp.c_str(), NULL, NULL, SW_SHOWNORMAL) > 32) return true;
        else return false;
}
//---------------------------------------------------------------------------
bool Tfrm_Main::startTSVentrilo()
{
        AnsiString strTemp;
        char szBuffer[512];

        if (processExists("TeamSpeak.exe") || processExists("Ventrilo.exe") || frm_Options->txt_TSServer->Text.IsEmpty()) return false;

        if (frm_Options->radgrp_TSVentrilo->ItemIndex == 0)
        {
                strTemp  = "teamspeak://" + frm_Options->txt_TSServer->Text;
                strTemp += "/nickname=" + frm_Options->txt_TSNick->Text;
                strTemp += "?loginname=" + frm_Options->txt_TSUser->Text;
                strTemp += "?password=" + frm_Options->txt_TSPass->Text;
                strTemp += "?channel=" + frm_Options->txt_TSChan->Text;
                strTemp += "?channelpassword=" + frm_Options->txt_TSChanPass->Text;
                if ((int) ShellExecute(NULL, "open", strTemp.c_str(), NULL, NULL, SW_SHOWMINNOACTIVE) > 32)
                {
                        // Give TS a few seconds to start up before continuing
                        Sleep(3000);
                        return true;
                }
                else return false;
        }
        else if (frm_Options->radgrp_TSVentrilo->ItemIndex == 1)
        {
                strTemp  = "ventrilo://" + frm_Options->txt_TSServer->Text;
                strTemp += "/servername=" + frm_Options->txt_TSServer->Text;
                if (!frm_Options->txt_TSPass->Text.IsEmpty()) strTemp += "&serverpassword=" + frm_Options->txt_TSPass->Text;
                if (!frm_Options->txt_TSChan->Text.IsEmpty()) strTemp += "&channelname=" + frm_Options->txt_TSChan->Text;
                if (!frm_Options->txt_TSChanPass->Text.IsEmpty()) strTemp += "&channelpassword=" + frm_Options->txt_TSChanPass->Text;

                TReplaceFlags replaceFlag;
                replaceFlag << rfReplaceAll;
                strTemp = StringReplace(strTemp, " ", "%20", replaceFlag);

                if ((int) ShellExecute(NULL, "open", strTemp.c_str(), NULL, NULL, SW_SHOWMINNOACTIVE) > 32)
                {
                        // Give Ventrilo a few seconds to start up before continuing
                        Sleep(3000);
                        return true;
                }
                else return false;
        }
        
        return false;
}
//---------------------------------------------------------------------------
bool Tfrm_Main::deleteETKey()
{
        AnsiString strTemp;

        strTemp = ExtractFileDir(frm_Options->txt_ETPath->Text) + "\\etmain\\etkey";
        if (FileExists(strTemp)) return DeleteFileA(strTemp);
        else return false;
}
//---------------------------------------------------------------------------
bool Tfrm_Main::deleteProfilePid()
{
        AnsiString strFileName = "profile.pid";
        deleteFileFromDir(ExtractFileDir(frm_Options->txt_ETPath->Text), strFileName, true);

        return true;
}
//---------------------------------------------------------------------------
bool Tfrm_Main::runCustomCommand()
{
        if (!frm_Options->txt_CustomAppPath->Text.IsEmpty())
        {
                if ((int) ShellExecute(NULL, "open", frm_Options->txt_CustomAppPath->Text.c_str() , NULL, ExtractFilePath(frm_Options->txt_CustomAppPath->Text).c_str(), SW_SHOWNORMAL) > 32) return true;
                else return false;
        }
        else return false;
}
//---------------------------------------------------------------------------
bool Tfrm_Main::terminateCusomApp()
{
        if (!frm_Options->txt_CustomAppTerminate->Text.IsEmpty())
        {
                TStringList* strList = new TStringList();
                parseAppList(strList, frm_Options->txt_CustomAppTerminate->Text);

                for (int i=0; i<strList->Count; i++)
                {
                        processTerminate(strList->operator [](i).c_str());
                }
                delete strList;
                return true;
        }
        else return false;
}
//---------------------------------------------------------------------------
char* Tfrm_Main::encryptString(char* szSource, char* szKey)
{
        int iKeyCount = 0;

        for (size_t i=0; i<strlen(szSource); i++)
        {
                if (i>=strlen(szKey)) iKeyCount = 0;
                szSource[i]=szSource[i]^szKey[iKeyCount];
                iKeyCount++;
        }
        return szSource;
}
//---------------------------------------------------------------------------
void Tfrm_Main::deleteFileFromDir(AnsiString strDirectory, AnsiString strFileName, bool bRecursive)
{
        AnsiString strPidFile = strDirectory + "\\*";
        AnsiString strTemp;
        AnsiString strTemp2;
        WIN32_FIND_DATA FindFileData;
        HANDLE hFind = INVALID_HANDLE_VALUE;

        hFind = FindFirstFile(strPidFile.c_str(), &FindFileData);

        if (hFind != INVALID_HANDLE_VALUE)
        {
                do
                {
                        if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                        {
                                if (bRecursive && !(!strcmp(FindFileData.cFileName, ".") ||
                                                    !strcmp(FindFileData.cFileName, "..") ||
                                                    !strcmp(FindFileData.cFileName, "")))
                                {
                                        strTemp2 =  strDirectory + "\\";
                                        strTemp2 += FindFileData.cFileName;
                                        deleteFileFromDir(strTemp2, strFileName, true);
                                }
                        }
                        else
                        {
                                if (strFileName == FindFileData.cFileName)
                                {
                                        strTemp = strDirectory + "\\";
                                        strTemp += strFileName;
                                        DeleteFileA(strTemp);
                                }
                        }
                } while (FindNextFile(hFind, &FindFileData) != 0);
                FindClose(hFind);
        }
        else return;
}
//---------------------------------------------------------------------------
bool Tfrm_Main::parseAppList(TStringList* strList, AnsiString strSource)
{
        int iPos = 0;

        while (iPos = strSource.AnsiPos(","))
        {
                strList->Add(strSource.SubString(0, iPos-1).Trim());
                strSource = strSource.SubString(iPos+1, strSource.Length()-iPos).Trim();
        }
        strList->Add(strSource.Trim());
        return true;
}
//---------------------------------------------------------------------------
void Tfrm_Main::highlightPlayers(AnsiString strPattern)
{
        for (int i=0; i<frm_Players->lstvw_Players->Items->Count; i++)
        {
                if (AnsiPos(strPattern.LowerCase(), frm_Players->lstvw_Players->Items->Item[i]->Caption.LowerCase()))
                {
                        frm_Players->lstvw_Players->Items->Item[i]->Selected = true;
                }
        }

        return;
}
//---------------------------------------------------------------------------
bool Tfrm_Main::queryServer()
{
        GS_SERVERINFO* pServerInfo = NULL;
        GS_PLAYERINFO* pPlayers    = NULL;
        AnsiString strHost = m_Servers[cmb_Server->ItemIndex].szServerIP;
        AnsiString strBuffer = "";
        int iPos = 0;
        int i = 0;
        unsigned short usPort = 27960;


        if (!frm_Options->chk_QueryServer->Checked)
        {
                lbl_ServerName->Caption = "";
                lbl_ServerName->Hint = "";
                lbl_ServerStatus->Caption = "QUERY OFF";
                lbl_ServerStatus->Font->Color = clCream;
                lbl_Players->Caption = "N/A";
                lbl_Map->Caption = "N/A";
                lbl_Mod->Caption = "N/A";
                lbl_Ping->Caption = "N/A";
                img_NotLocked->Visible = false;
                img_Locked->Visible = false;
                // Clear the list view
                frm_Players->lstvw_Players->Clear();
                return false;
        }

        if (iPos = AnsiPos(":", strHost))
        {
                usPort =  strHost.SubString(iPos+1, strHost.Length()-iPos).ToInt();
                strHost = strHost.SubString(0, iPos-1);
        }
        else strHost = m_Servers[cmb_Server->ItemIndex].szServerIP;

        GSSetOptInt(GS_OPTION_TIMEOUT, 1000);

        pServerInfo = GSQueryServer(GS_TYPE_WOLFET, strHost.c_str(), usPort);

        if (pServerInfo)
        {
                lbl_ServerName->Caption = Trim((AnsiString) GSStripColor(GS_TYPE_WOLFET, pServerInfo->host, pServerInfo->host));
                lbl_ServerName->Hint = pServerInfo->host;
                lbl_ServerStatus->Caption = "ONLINE";
                lbl_ServerStatus->Font->Color = clGreen;
                strBuffer = "(";
                strBuffer += pServerInfo->numplayers;
                strBuffer += "/";
                strBuffer += pServerInfo->maxplayers;
                strBuffer += ")";
                lbl_Players->Caption = strBuffer;
                lbl_Map->Caption = GSStripColor(GS_TYPE_WOLFET, pServerInfo->map, pServerInfo->map);
                lbl_Mod->Caption = pServerInfo->mod;
                lbl_Ping->Caption = pServerInfo->ping;

                if (!pServerInfo->passworded)
                {
                        img_NotLocked->Visible = false;
                        img_Locked->Visible = true;
                }
                else
                {
                        img_Locked->Visible = false;
                        img_NotLocked->Visible = true;
                }

                // Clear the list view and tell it not to sort
                frm_Players->lstvw_Players->Clear();
                frm_Players->lstvw_Players->SortType = Listactns::stNone;

                // Iterate through players and display them in the list view
                pPlayers = pServerInfo->players;
                while (pPlayers)
                {
                        frm_Players->lstvw_Players->Items->Add();
                        frm_Players->lstvw_Players->Items->Item[i]->Caption = GSStripColor(GS_TYPE_WOLFET, pPlayers->name, pPlayers->name);
                        frm_Players->lstvw_Players->Items->Item[i]->SubItems->Add(IntToStr(pPlayers->score));
                        frm_Players->lstvw_Players->Items->Item[i]->SubItems->Add(IntToStr(pPlayers->ping));

                        pPlayers = pPlayers->next;
                        i++;
                }
                GSFreeServerInfo(pServerInfo);

                // Check wether to highlight players
                if (!frm_Options->txt_Highlight->Text.IsEmpty())
                {
                        TStringList* strList = new TStringList();
                        parseAppList(strList, frm_Options->txt_Highlight->Text);

                        for (int i=0; i<strList->Count; i++)
                        {
                                highlightPlayers(strList->operator [](i));
                        }
                        delete strList;
                }

                return true;
        }

        // Some error occured if we get here
        lbl_ServerName->Caption = "";
        lbl_ServerName->Hint = "";
        lbl_ServerStatus->Caption = "OFFLINE";
        lbl_ServerStatus->Font->Color = clRed;
        lbl_Players->Caption = "N/A";
        lbl_Map->Caption = "N/A";
        lbl_Mod->Caption = "N/A";
        lbl_Ping->Caption = "N/A";
        img_NotLocked->Visible = false;
        img_Locked->Visible = false;
        // Clear the list view
        frm_Players->lstvw_Players->Clear();

        return false;
}
//---------------------------------------------------------------------------
bool Tfrm_Main::processExists(char* szProcName)
{
        HANDLE procSnap = NULL;
        PROCESSENTRY32 procInfo;

        procInfo.dwSize = sizeof(procInfo);

        if (procSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0))
        {
                if (Process32First(procSnap, &procInfo))
                {
                        if (strcmp(procInfo.szExeFile, szProcName)==0) return true;
                        while (Process32Next(procSnap, &procInfo))
                        {
                                if (strcmp(procInfo.szExeFile, szProcName)==0) return true;
                        }
                }
        }
        return false;
}
//---------------------------------------------------------------------------
bool Tfrm_Main::processTerminate(char* szProcName)
{
        HANDLE procSnap = NULL, procHandle = NULL;
        PROCESSENTRY32 procInfo;

        procInfo.dwSize = sizeof(procInfo);

        if (procSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0))
        {
                if (Process32First(procSnap, &procInfo))
                {
                        do
                        {
                                if (strcmp(procInfo.szExeFile, szProcName)==0)
                                {
                                        if (procHandle = OpenProcess(PROCESS_TERMINATE, false, procInfo.th32ProcessID))
                                        {
                                                if (TerminateProcess(procHandle, 0)) return true;
                                        }
                                }
                        } while (Process32Next(procSnap, &procInfo));
                }
        }
        return false;
}
//---------------------------------------------------------------------------
bool Tfrm_Main::changeVersion(int iVersion)
{
        AnsiString strEt, strCgame, strMpbin, strPak1, strQagame, strUi, strVersion, strSrcPath, strDestPath, strTemp;
        bool bAllOk = true;

        if (iVersion == -1) iVersion = cmb_ETVersion->ItemIndex;

        if (iVersion == 0) strVersion = "2.55";
        else if (iVersion == 1) strVersion = "2.56";
        else if (iVersion == 2) strVersion = "2.60";
        else if (iVersion == 3) strVersion = "2.60b";
        else return false;

        strSrcPath      = ExtractFileDir(Application->ExeName)+"\\VersionData\\" + strVersion + "\\";
        strDestPath     = ExtractFileDir(frm_Options->txt_ETPath->Text) + "\\";

        strEt           = "ET.exe";
        strCgame        = "cgame_mp_x86.dll";
        strMpbin        = "mp_bin.pk3";
        strPak1         = "pak1.pk3";
        strQagame       = "qagame_mp_x86.dll";
        strUi           = "ui_mp_x86.dll";

        if (!CopyFileA((strSrcPath+strEt).c_str(), (strDestPath + strEt).c_str(), 0)) bAllOk = false;
        strDestPath += "etmain\\";

        // Version 2.60b has only changed the ET.exe, so the rest of the files
        // comes from 2.60
        if (iVersion == 3) strSrcPath = ExtractFileDir(Application->ExeName)+"\\VersionData\\2.60\\";

        if (!CopyFileA((strSrcPath+strCgame).c_str(), (strDestPath + strCgame).c_str(), 0)) bAllOk = false;
        if (!CopyFileA((strSrcPath+strMpbin).c_str(), (strDestPath + strMpbin).c_str(), 0)) bAllOk = false;
        if (!CopyFileA((strSrcPath+strPak1).c_str(), (strDestPath + strPak1).c_str(), 0)) bAllOk = false;
        if (!CopyFileA((strSrcPath+strQagame).c_str(), (strDestPath + strQagame).c_str(), 0)) bAllOk = false;
        if (!CopyFileA((strSrcPath+strUi).c_str(), (strDestPath + strUi).c_str(), 0)) bAllOk = false;

        return bAllOk;
}
//---------------------------------------------------------------------------
void Tfrm_Main::readIniFile()
{
        int iNumServers = 0;
        AnsiString strTemp = "";
        char szBuffer[512];
        SERVERNODE newServer;
        memset (szBuffer, 0, sizeof(szBuffer));

        if (FileExists(m_IniFile))
        {
                // Clear the server list in memory
                m_Servers.clear();

                // Fetch infos from the ini file
                // Fetch ETPath
                GetPrivateProfileStringA("General", "ETPath", "", szBuffer, 512, m_IniFile.c_str());
                if (strcmp(szBuffer, "") != 0) frm_Options->txt_ETPath->Text=szBuffer;
                else frm_Options->txt_ETPath->Text = "";
                memset (szBuffer, 0, sizeof(szBuffer));

                // Fetch ETArgs
                GetPrivateProfileStringA("General", "ETArgs", "", szBuffer, 512, m_IniFile.c_str());
                if (strcmp(szBuffer, "") != 0) frm_Options->txt_ETArgs->Text=szBuffer;
                else frm_Options->txt_ETArgs->Text = "";
                memset (szBuffer, 0, sizeof(szBuffer));

                // Fetch Custom App Path
                GetPrivateProfileStringA("General", "CustomApp", "", szBuffer, 512, m_IniFile.c_str());
                if (strcmp(szBuffer, "") != 0) frm_Options->txt_CustomAppPath->Text=szBuffer;
                else frm_Options->txt_CustomAppPath->Text = "";
                memset (szBuffer, 0, sizeof(szBuffer));

                // Fetch Custom Apps to terminate
                GetPrivateProfileStringA("General", "CustomAppsQuit", "", szBuffer, 512, m_IniFile.c_str());
                if (strcmp(szBuffer, "") != 0) frm_Options->txt_CustomAppTerminate->Text=szBuffer;
                else frm_Options->txt_CustomAppTerminate->Text = "";
                memset (szBuffer, 0, sizeof(szBuffer));

                // Fetch player highlight tags
                GetPrivateProfileStringA("General", "HighlightTags", "", szBuffer, 512, m_IniFile.c_str());
                if (strcmp(szBuffer, "") != 0) frm_Options->txt_Highlight->Text=szBuffer;
                else frm_Options->txt_Highlight->Text = "";
                memset (szBuffer, 0, sizeof(szBuffer));

                // Fetch main form checkbox values
                GetPrivateProfileStringA("General", "ChkStartETMin", "", szBuffer, 512, m_IniFile.c_str());
                if (strcmp(szBuffer, "") != 0) chk_ETMin->Checked = StrToInt(szBuffer);
                else chk_ETMin->Checked = true;
                memset (szBuffer, 0, sizeof(szBuffer));

                GetPrivateProfileStringA("General", "ChkStartTS", "", szBuffer, 512, m_IniFile.c_str());
                if (strcmp(szBuffer, "") != 0) chk_Teamspeak->Checked = StrToInt(szBuffer);
                else chk_Teamspeak->Checked = true;
                memset (szBuffer, 0, sizeof(szBuffer));

                GetPrivateProfileStringA("General", "ChkDeleteETKey", "", szBuffer, 512, m_IniFile.c_str());
                if (strcmp(szBuffer, "") != 0) chk_ETKey->Checked = StrToInt(szBuffer);
                else chk_ETKey->Checked = true;
                memset (szBuffer, 0, sizeof(szBuffer));

                GetPrivateProfileStringA("General", "ChkCustomArgs", "", szBuffer, 512, m_IniFile.c_str());
                if (strcmp(szBuffer, "") != 0) chk_CustomArgs->Checked = StrToInt(szBuffer);
                else chk_CustomArgs->Checked = true;
                memset (szBuffer, 0, sizeof(szBuffer));

                GetPrivateProfileStringA("General", "ChkStartCustomApp", "", szBuffer, 512, m_IniFile.c_str());
                if (strcmp(szBuffer, "") != 0) chk_CustomApp->Checked = StrToInt(szBuffer);
                else chk_CustomApp->Checked = true;
                memset (szBuffer, 0, sizeof(szBuffer));

                GetPrivateProfileStringA("General", "ChkTerminateCustomApp", "", szBuffer, 512, m_IniFile.c_str());
                if (strcmp(szBuffer, "") != 0) chk_CustomAppTerminate->Checked = StrToInt(szBuffer);
                else chk_CustomAppTerminate->Checked = true;
                memset (szBuffer, 0, sizeof(szBuffer));

                GetPrivateProfileStringA("General", "ChkDeleteProfilePid", "", szBuffer, 512, m_IniFile.c_str());
                if (strcmp(szBuffer, "") != 0) chk_ProfilePid->Checked = StrToInt(szBuffer);
                else chk_CustomAppTerminate->Checked = true;
                memset (szBuffer, 0, sizeof(szBuffer));

                // Fetch number of servers
                GetPrivateProfileStringA("General", "NumServers", "", szBuffer, 512, m_IniFile.c_str());
                if (strcmp(szBuffer, "") != 0) iNumServers = atoi(szBuffer);
                memset (szBuffer, 0, sizeof(szBuffer));

                // Fetch Teamspeak config
                GetPrivateProfileStringA("Teamspeak", "AppToUse", "0", szBuffer, 512, m_IniFile.c_str());
                frm_Options->radgrp_TSVentrilo->ItemIndex = StrToInt(szBuffer);
                memset (szBuffer, 0, sizeof(szBuffer));
                GetPrivateProfileStringA("Teamspeak", "Server", "", szBuffer, 512, m_IniFile.c_str());
                if (strcmp(szBuffer, "") != 0) frm_Options->txt_TSServer->Text = szBuffer;
                memset (szBuffer, 0, sizeof(szBuffer));
                GetPrivateProfileStringA("Teamspeak", "Chan", "", szBuffer, 512, m_IniFile.c_str());
                if (strcmp(szBuffer, "") != 0) frm_Options->txt_TSChan->Text = szBuffer;
                memset (szBuffer, 0, sizeof(szBuffer));
                GetPrivateProfileStringA("Teamspeak", "ChanPass", "", szBuffer, 512, m_IniFile.c_str());
                if (strcmp(szBuffer, "") != 0) frm_Options->txt_TSChanPass->Text = szBuffer;
                memset (szBuffer, 0, sizeof(szBuffer));
                GetPrivateProfileStringA("Teamspeak", "Nick", "", szBuffer, 512, m_IniFile.c_str());
                if (strcmp(szBuffer, "") != 0) frm_Options->txt_TSNick->Text = szBuffer;
                memset (szBuffer, 0, sizeof(szBuffer));
                GetPrivateProfileStringA("Teamspeak", "User", "", szBuffer, 512, m_IniFile.c_str());
                if (strcmp(szBuffer, "") != 0) frm_Options->txt_TSUser->Text = szBuffer;
                memset (szBuffer, 0, sizeof(szBuffer));
                GetPrivateProfileStringA("Teamspeak", "Pass", "", szBuffer, 512, m_IniFile.c_str());
                if (strcmp(szBuffer, "") != 0) frm_Options->txt_TSPass->Text = szBuffer;
                memset (szBuffer, 0, sizeof(szBuffer));

                // Fetch servers if there are any
                for (int i=0; i<iNumServers; i++)
                {
                        strTemp = i;
                        newServer.iETVersion = 0;
                        strcpy(newServer.szServerName, "");
                        strcpy(newServer.szServerIP, "");
                        strcpy(newServer.szPrivatePass, "");

                        GetPrivateProfileStringA(strTemp.c_str(), "ServerName", "", szBuffer, 512, m_IniFile.c_str());
                        if (strcmp(szBuffer, "") != 0) strcpy(newServer.szServerName, szBuffer);
                        memset (szBuffer, 0, sizeof(szBuffer));
                        GetPrivateProfileStringA(strTemp.c_str(), "ServerIP", "", szBuffer, 512, m_IniFile.c_str());
                        if (strcmp(szBuffer, "") != 0) strcpy(newServer.szServerIP, szBuffer);
                        memset (szBuffer, 0, sizeof(szBuffer));
                        GetPrivateProfileStringA(strTemp.c_str(), "PrivatePass", "", szBuffer, 512, m_IniFile.c_str());
                        if (strcmp(szBuffer, "") != 0) strcpy(newServer.szPrivatePass, szBuffer);
                        memset (szBuffer, 0, sizeof(szBuffer));
                        GetPrivateProfileStringA(strTemp.c_str(), "ETVersion", "", szBuffer, 512, m_IniFile.c_str());
                        if (strcmp(szBuffer, "") != 0) newServer.iETVersion = atoi(szBuffer);
                        memset (szBuffer, 0, sizeof(szBuffer));

                        m_Servers.push_back(newServer);
                }

                // Fetch whether to query servers or not
                GetPrivateProfileStringA("General", "Query", "", szBuffer, 512, m_IniFile.c_str());
                if (strcmp(szBuffer, "") != 0)
                {
                        if (strcmp(szBuffer, "1") == 0) frm_Options->chk_QueryServer->Checked = true;
                        else frm_Options->chk_QueryServer->Checked = false;
                }
        }

}
//---------------------------------------------------------------------------
void Tfrm_Main::fillForms(bool bPlayerWinCheck)
{
        // Fill main form server dropdown
        cmb_Server->Clear();
        for (unsigned int i=0; i<m_Servers.size(); i++)
        {
                cmb_Server->Items->Add(m_Servers[i].szServerName);
        }

        // Fill options server list
        frm_Options->reloadServerList();

        // Set the last chosen server as selected
        if (cmb_Server->Items->Count)
        {
                if (m_iLastServerIndex <= (cmb_Server->Items->Count - 1))
                {
                        cmb_Server->ItemIndex = m_iLastServerIndex;
                        // Set the appropriate ETVersion
                        cmb_ETVersion->ItemIndex = m_Servers[m_iLastServerIndex].iETVersion;
                }
                else
                {
                        cmb_Server->ItemIndex = 0;
                        cmb_ETVersion->ItemIndex = m_Servers[0].iETVersion;
                }
                frm_Options->lst_Servers->ItemIndex=0;

        }

        // Check wether to enable/disable buttons
        if (!frm_Options->txt_ETPath->Text.IsEmpty())
        {
                btn_PatchOnly->Enabled = true;
                if (cmb_Server->ItemIndex != -1)
                {
                        btn_PatchConnect->Enabled = true;
                }
                else
                {
                        btn_PatchConnect->Enabled = false;
                }
        }
        else
        {
                btn_PatchOnly->Enabled = false;
                btn_PatchConnect->Enabled = false;
        }

        // Check wether to hide or show the player window
        if (bPlayerWinCheck)
        {
                if (frm_Options->chk_QueryServer->Checked)
                {
                        if (frm_Players) frm_Players->Show();
                        btn_Players->Caption = "<";
                }
                else
                {
                        if (frm_Players) frm_Players->Hide();
                        btn_Players->Caption = ">";
                }
        }

}
//---------------------------------------------------------------------------
void __fastcall Tfrm_Main::men_OptionsClick(TObject *Sender)
{
        frm_Options->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_Main::men_ExitClick(TObject *Sender)
{
        Application->Terminate();
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_Main::men_AboutClick(TObject *Sender)
{
        frm_About->ShowModal();        
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_Main::cmb_ServerChange(TObject *Sender)
{
        // Choose the correct version in the versions combo box
        cmb_ETVersion->ItemIndex = m_Servers[cmb_Server->ItemIndex].iETVersion;

        // Query the server
        queryServer();

        // Store the last chosen index
        m_iLastServerIndex = cmb_Server->ItemIndex;
}
//---------------------------------------------------------------------------


void __fastcall Tfrm_Main::FormShow(TObject *Sender)
{
        m_IniFile = ChangeFileExt( Application->ExeName, ".ini" );
        HKEY regKey = NULL;
        char szBuffer[512];
        DWORD bufSize = sizeof(szBuffer);
        AnsiString strDefCmdLine = "\"" + Application->ExeName + "\" ";
        AnsiString strCmdLine, strServer, strVersion, strMessage, strConnect;
        LPTSTR lpCmdLine = NULL;
        int iVersion = 0;

        memset(szBuffer, 0, sizeof(szBuffer));

       // Check wether there is an ini file to read
        if (FileExists(m_IniFile))
        {
                // There is an ini file available so read its contents
                readIniFile();
                // Populate the Server Lists
                fillForms(false);
        }

        // ET Path already chosen?
        if (frm_Options->txt_ETPath->Text.IsEmpty())
        {
                // Probably the first time the app is starting
                // so pop up Options dialog
                MessageBoxA(this->Handle, "The path to ET.exe has not been chosen yet.\n\nET Starter Pro will try to determine the path automatically.\nNow bringing you to the options dialog...", "Path to ET.exe", MB_OK | MB_ICONINFORMATION);
                // QUery the registry for the ET Path
                if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE", NULL, KEY_READ, &regKey) == ERROR_SUCCESS)
                {
                        if (RegOpenKeyEx(regKey, "Activision", NULL, KEY_READ, &regKey) == ERROR_SUCCESS)
                        {
                                if (RegOpenKeyEx(regKey, "Wolfenstein - Enemy Territory", NULL, KEY_QUERY_VALUE, &regKey) ==  ERROR_SUCCESS)
                                {
                                       if (RegQueryValueEx(regKey, "InstallPath", NULL, NULL, szBuffer, &bufSize) == ERROR_SUCCESS)
                                       {
                                                strcat(szBuffer, "\\ET.exe");
                                                frm_Options->txt_ETPath->Text = szBuffer;
                                                RegCloseKey(regKey);
                                       }
                                }
                        }
                }

                frm_Options->ShowModal();
        }

        // Check if we received command line args
        strCmdLine = lpCmdLine = GetCommandLineA();
        if (lpCmdLine && (strDefCmdLine != strCmdLine))
        {
                // Parse data out of argument string
                strCmdLine = strCmdLine.SubString(strDefCmdLine.Length()+19, strCmdLine.Length()-strDefCmdLine.Length()-20);
                strServer = strCmdLine.SubString(0, strCmdLine.AnsiPos("&")-1);
                strVersion = strCmdLine.SubString(strCmdLine.AnsiPos("&")+9, strCmdLine.Length()-strCmdLine.AnsiPos("&")-8);

                strMessage = "Start ET and connect?\n\nHost:       " + strServer + "\nVersion:   " + strVersion;

                if (Application->MessageBoxA(strMessage.c_str(), "ET Starter Pro - Connect", MB_ICONQUESTION | MB_YESNO) == IDYES)
                {
                        if (strVersion == "2.55") iVersion = 0;
                        else if (strVersion == "2.56") iVersion = 1;
                        else if (strVersion == "2.60") iVersion = 2;
                        else iVersion = -2;
                        if (!frm_Main->changeVersion(iVersion))
                        {
                                Application->MessageBoxA("Could not copy patch files to your ET directory!\n\nMake sure that files are not write protected or in use.\nAlso make sure that the directory \"VersionData\" exists in the ETStarter Pro directory.", "ET Starter Pro - Error", MB_ICONERROR | MB_OK);
                                Application->Terminate();
                        }
                        else
                        {
                                strConnect =  " +connect " + strServer;
                                ShellExecute(NULL, "open", frm_Options->txt_ETPath->Text.c_str(), strConnect.c_str(), ExtractFileDir(frm_Options->txt_ETPath->Text).c_str(), SW_SHOWNORMAL);
                        }
                }
                Application->Terminate();
        }

        // Position the window
        if (frm_Players->Visible) Left = (Screen->Width/2) - ((Width + frm_Players->Width)/2);
        else Left = (Screen->Width/2) - (Width /2);
        Height = Screen->Height/2 - Height/2;

        // Query server if wanted
        if (cmb_Server->ItemIndex != -1)
        {
                if (queryServer())
                {
                        if (frm_Players)
                        {
                                frm_Players->Show();
                                btn_Players->Caption = "<";
                        }
                }
                else
                {
                        if (frm_Players)
                        {
                                frm_Players->Hide();
                                btn_Players->Caption = ">";
                        }
                }
        }
}
//---------------------------------------------------------------------------


void __fastcall Tfrm_Main::btn_PatchConnectClick(TObject *Sender)
{
        AnsiString strTemp = "";
        AnsiString strParams = "";

        // Hide windows
        frm_Players->Hide();
        frm_Main->Hide();

        // Check wether to start ETMin
        if (chk_ETMin->Checked)
        {
               startMinimizer();
        }

        // Check wether to start Teamspeak/Ventrilo
        if (chk_Teamspeak->Checked)
        {
                startTSVentrilo();
        }

        // Check wether to delete the etkey file
        if (chk_ETKey->Checked)
        {
                deleteETKey();
        }

        // Check wether to delete the profile.pid file
        if (chk_ProfilePid->Checked)
        {
                deleteProfilePid();
        }

        // Check wether to run custom command
        if (chk_CustomApp->Checked)
        {
                runCustomCommand();
        }

        // Check wether to terminate custom Apps
        if (chk_CustomAppTerminate->Checked)
        {
                terminateCusomApp();
        }

        // Change ET Version
        if (!changeVersion())
        {
                frm_Players->Show();
                frm_Main->Show();
                Application->MessageBoxA("Could not copy patch files to your ET directory!\n\nMake sure that files are not write protected or in use.\nAlso make sure that the directory \"VersionData\" exists in the ETStarter Pro directory.", "ET Starter Pro - Error", MB_ICONERROR | MB_OK);
                return;
        }

        // Build the connection string
        strParams = " +connect ";
        strParams += m_Servers[cmb_Server->ItemIndex].szServerIP;

        if (strcmp(m_Servers[cmb_Server->ItemIndex].szPrivatePass, "") != 0)
        {
                strParams += " +set password ";
                strParams += m_Servers[cmb_Server->ItemIndex].szPrivatePass;
        }

        // Start the game
        ShellExecute(NULL, "open", frm_Options->txt_ETPath->Text.c_str(), strParams.c_str(), ExtractFileDir(frm_Options->txt_ETPath->Text).c_str(), SW_SHOWNORMAL);

        Application->Terminate();
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_Main::btn_PatchOnlyClick(TObject *Sender)
{
        AnsiString strParams = "";

        // Change Version
        if (changeVersion())
        {
                if (Application->MessageBoxA("Changed version successfully!\n\nStart ET now?", "ET Starter Pro", MB_ICONINFORMATION | MB_YESNO)==IDYES)
                {
                        // Check wether to start ETMin
                        if (chk_ETMin->Checked)
                        {
                                startMinimizer();
                        }

                        // Build Params
                        if (!frm_Options->txt_ETArgs->Text.IsEmpty() && chk_CustomArgs->Checked) strParams += frm_Options->txt_ETArgs->Text;

                        // Start the game
                        ShellExecute(NULL, "open", frm_Options->txt_ETPath->Text.c_str(), strParams.c_str(), ExtractFileDir(frm_Options->txt_ETPath->Text).c_str(), SW_SHOWNORMAL);
                        Application->Terminate();
                }
                else return;
        }
        else Application->MessageBoxA("Could not copy patch files to your ET directory!\n\nMake sure that files are not write protected or in use.\nAlso make sure that the directory \"VersionData\" exists in the ETStarter Pro directory.", "ET Starter Pro - Error", MB_ICONERROR | MB_OK);
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_Main::men_ServerbrowserClick(TObject *Sender)
{
        frm_Serverbrowser->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_Main::men_ExportClick(TObject *Sender)
{
        if (FileExists(m_IniFile))
        {
                if (dlg_Export->Execute())
                {
                        if (CopyFileA(m_IniFile.c_str(), dlg_Export->FileName.c_str(), 0))
                        {
                                Application->MessageBoxA("Configuration successfully exported!", "ET Starter Pro - Info", MB_OK | MB_ICONINFORMATION);
                                return;
                        }
                }
        }

        else Application->MessageBoxA("Unable to export config!", "ET Starter Pro - Error", MB_OK | MB_ICONERROR);
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_Main::men_ImportClick(TObject *Sender)
{
        if (dlg_Import->Execute())
        {
                if (FileExists(dlg_Import->FileName))
                {
                        if (Application->MessageBoxA("Current configruation will be overwritten!\n\nProceed?", "ET Starter Pro - Import Configuration", MB_YESNO | MB_ICONQUESTION) == ID_YES)
                        {
                                if (CopyFileA(dlg_Import->FileName.c_str(), m_IniFile.c_str(), 0))
                                {
                                        // There is an ini file available so read its contents
                                        readIniFile();
                                        // Populate the Server Lists
                                        fillForms();
                                        if (cmb_Server->ItemIndex != -1 && frm_Options->chk_QueryServer->Checked) queryServer();
                                        Application->MessageBoxA("Configuration successfully imported!", "ET Starter Pro - Info", MB_OK | MB_ICONINFORMATION);
                                        return;
                                }
                                else Application->MessageBoxA("Unable to import config!", "ET Starter Pro - Error", MB_OK | MB_ICONERROR);
                        }
                        else return;
                }
        }
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_Main::btn_RefreshClick(TObject *Sender)
{
        queryServer();        
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_Main::popmen_ImportClick(TObject *Sender)
{
        men_ImportClick(this);        
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_Main::popmen_ExportClick(TObject *Sender)
{
        men_ExportClick(this);        
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_Main::popmen_ExitClick(TObject *Sender)
{
        men_ExitClick(this);        
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_Main::popmen_ServerbrowserClick(TObject *Sender)
{
        men_ServerbrowserClick(this);        
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_Main::popmen_OptionsClick(TObject *Sender)
{
        men_OptionsClick(this);        
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_Main::popmen_AboutClick(TObject *Sender)
{
        men_AboutClick(this);        
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_Main::btn_PlayersClick(TObject *Sender)
{
        if (!frm_Players->Visible)
        {
                frm_Players->Show();
                btn_Players->Caption = "<";
        }
        else
        {
                frm_Players->Hide();
                btn_Players->Caption = ">";
        }
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_Main::OnFormMove(TWMMove aMsg)
{
        if (frm_Players)
        {
                if (frm_Players->Visible)
                {
                        frm_Players->dockToMainWindow();
                }
        }
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_Main::popmen_ExitMainClick(TObject *Sender)
{
        men_ExitClick(this);        
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_Main::chk_ETMinClick(TObject *Sender)
{
        WritePrivateProfileStringA("General", "ChkStartETMin", IntToStr((int)chk_ETMin->Checked).c_str(), frm_Main->m_IniFile.c_str());
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_Main::chk_TeamspeakClick(TObject *Sender)
{
        WritePrivateProfileStringA("General", "ChkStartTS", IntToStr((int)chk_Teamspeak->Checked).c_str(), frm_Main->m_IniFile.c_str());
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_Main::chk_ETKeyClick(TObject *Sender)
{
        WritePrivateProfileStringA("General", "ChkDeleteETKey", IntToStr((int)chk_ETKey->Checked).c_str(), frm_Main->m_IniFile.c_str());
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_Main::chk_CustomAppClick(TObject *Sender)
{
        WritePrivateProfileStringA("General", "ChkStartCustomApp", IntToStr((int)chk_CustomApp->Checked).c_str(), frm_Main->m_IniFile.c_str());
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_Main::chk_CustomAppTerminateClick(TObject *Sender)
{
        WritePrivateProfileStringA("General", "ChkTerminateCustomApp", IntToStr((int)chk_CustomAppTerminate->Checked).c_str(), frm_Main->m_IniFile.c_str());        
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_Main::img_ExitClick(TObject *Sender)
{
        men_ExitClick(this);        
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_Main::img_MinimizeClick(TObject *Sender)
{
        Application->Minimize();        
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_Main::img_MenuClick(TObject *Sender)
{
        popmen_Main->Popup(frm_Main->Left + img_Menu->Left, frm_Main->Top + img_Menu->Top + 60);
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_Main::popmen_GammaFixClick(TObject *Sender)
{
        // This code was posted by Rain in the etpro forums
        HDC curdc;
	WORD gamma[3][256];
	int x, y;

	curdc = GetDC(0);
	for (x = 0; x < 3; x++)
        {
		for (y = 0; y < 256; y++)
                {
			gamma[x][y] = (int)(65535.0 * (y / 255.0));
		}
	}
	SetDeviceGammaRamp(curdc, gamma);
        Application->MessageBoxA("Gamma has been reset!", "ET Starter Pro - Gamma Reset", MB_OK | MB_ICONINFORMATION);
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_Main::chk_ProfilePidClick(TObject *Sender)
{
        WritePrivateProfileStringA("General", "ChkDeleteProfilePid", IntToStr((int)chk_ProfilePid->Checked).c_str(), frm_Main->m_IniFile.c_str());
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_Main::chk_CustomArgsClick(TObject *Sender)
{
        WritePrivateProfileStringA("General", "ChkCustomArgs", IntToStr((int)chk_CustomArgs ->Checked).c_str(), frm_Main->m_IniFile.c_str());
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_Main::img_StartMinimizerClick(TObject *Sender)
{
        startMinimizer();        
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_Main::img_StartTSVentriloClick(TObject *Sender)
{
        startTSVentrilo();        
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_Main::img_DeleteETKeyClick(TObject *Sender)
{
        if (deleteETKey()) Application->MessageBoxA("Successfully deleted etkey file!", "ET Starter Pro - Delete etkey file", MB_OK | MB_ICONINFORMATION);
        else Application->MessageBoxA("The etkey file either does not exist or could not be deleted!", "ET Starter Pro - Delete etkey file", MB_OK | MB_ICONERROR);

}
//---------------------------------------------------------------------------

void __fastcall Tfrm_Main::img_DeleteProfilePidClick(TObject *Sender)
{
        deleteProfilePid();
        Application->MessageBoxA("Operation complete!", "ET Starter Pro - Delete profile.pid files", MB_OK | MB_ICONINFORMATION);

}
//---------------------------------------------------------------------------

void __fastcall Tfrm_Main::img_RunCusomCommandClick(TObject *Sender)
{
        if (!runCustomCommand())
        {
                Application->MessageBoxA("There was an error running your command!\nPlease check your settings!", "ET Starter Pro - Run custom command", MB_OK | MB_ICONERROR);
        }
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_Main::img_QuitCustomAppClick(TObject *Sender)
{
        if (terminateCusomApp()) Application->MessageBoxA("Termination signals sent!", "ET Starter Pro - Quit custom applications", MB_OK | MB_ICONINFORMATION);
        else Application->MessageBoxA("No apps specified!\nPlease check your settings!", "ET Starter Pro - Quit custom applications", MB_OK | MB_ICONERROR);
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_Main::popmen_ETProToolsClick(TObject *Sender)
{
        frm_ETProTools->ShowModal();        
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_Main::popmen_ExportETKeyClick(TObject *Sender)
{
        AnsiString strETKeyFile = ExtractFileDir(frm_Options->txt_ETPath->Text) + "\\etmain\\etkey";

        if (FileExists(strETKeyFile))
        {
                if (dlg_ETKeyExport->Execute())
                {
                        if (CopyFileA(strETKeyFile.c_str(), dlg_ETKeyExport->FileName.c_str(), 0))
                        {
                                Application->MessageBoxA("Etkey file successfully exported!", "ET Starter Pro - Info", MB_OK | MB_ICONINFORMATION);
                                return;
                        }
                        else Application->MessageBoxA("Unable to backup etkey file!", "ET Starter Pro - Error", MB_OK | MB_ICONERROR);
                }
        }
        else Application->MessageBoxA("No etkey file available for backup!", "ET Starter Pro - Error", MB_OK | MB_ICONERROR);
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_Main::popmen_ImportETKeyClick(TObject *Sender)
{
        AnsiString strETKeyFile = ExtractFileDir(frm_Options->txt_ETPath->Text) + "\\etmain\\etkey";

        if (dlg_ETKeyImport->Execute())
        {
                if (FileExists(dlg_ETKeyImport->FileName))
                {
                        if (Application->MessageBoxA("Current etkey file will be overwritten!\n\nProceed?", "ET Starter Pro - Import etkey file", MB_YESNO | MB_ICONQUESTION) == ID_YES)
                        {
                                if (CopyFileA(dlg_ETKeyImport->FileName.c_str(), strETKeyFile.c_str(), 0))
                                {
                                        Application->MessageBoxA("Etkey file successfully imported!", "ET Starter Pro - Info", MB_OK | MB_ICONINFORMATION);
                                        return;
                                }
                                Application->MessageBoxA("Could not import etkey file!", "ET Starter Pro - Error", MB_OK | MB_ICONERROR);
                        }
                        else return;
                }
        }        
}
//---------------------------------------------------------------------------

