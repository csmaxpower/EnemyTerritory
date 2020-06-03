//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ETProTools.h"
#include "Main.h"
#include "Options.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tfrm_ETProTools *frm_ETProTools;
//---------------------------------------------------------------------------
__fastcall Tfrm_ETProTools::Tfrm_ETProTools(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void Tfrm_ETProTools::playSelectedDemo()
{
        AnsiString strDemoDir = ExtractFileDir(frm_Options->txt_ETPath->Text) + "\\etpro\\demos";
        AnsiString strParams = "+set fs_game etpro +demo etspro.default.dm_84";
        AnsiString strSrc, strDest;

        if (lst_Demos->ItemIndex != -1 && frm_Main->changeVersion(2))
        {
                strSrc = strDest = strDemoDir + "\\";
                strSrc += lst_Demos->Items->operator [](lst_Demos->ItemIndex);
                strDest += "etspro.default.dm_84";
                if (CopyFile(strSrc.c_str(), strDest.c_str(), false))
                {
                        ShellExecute(NULL, "open", frm_Options->txt_ETPath->Text.c_str(), strParams.c_str(), ExtractFileDir(frm_Options->txt_ETPath->Text).c_str(), SW_SHOWNORMAL);
                }
        }
        return;
}
//---------------------------------------------------------------------------
void Tfrm_ETProTools::fillDemoListbox()
{
        AnsiString strDemoDir = ExtractFileDir(frm_Options->txt_ETPath->Text) + "\\etpro\\demos";
        AnsiString strETMainDemoDir = ExtractFileDir(frm_Options->txt_ETPath->Text) + "\\etmain\\demos";
        AnsiString strETProDir = ExtractFileDir(frm_Options->txt_ETPath->Text) + "\\etpro";
        AnsiString strSearchFile;
        int iCount = 0;
        WIN32_FIND_DATA FindFileData;
        HANDLE hFind = INVALID_HANDLE_VALUE;

        lst_Demos->Clear();

        // List demo files in etpro demo dir
        strSearchFile = strDemoDir + "\\*";

        hFind = FindFirstFile(strSearchFile.c_str(), &FindFileData);

        if (hFind != INVALID_HANDLE_VALUE)
        {
                do
                {
                        if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                        {
                                if (strcmp(FindFileData.cFileName, "etspro.default.dm_84"))
                                {
                                        lst_Demos->Items->Add(FindFileData.cFileName);
                                        iCount++;
                                }
                        }
                } while (FindNextFile(hFind, &FindFileData) != 0);
                FindClose(hFind);

                if (iCount > 0) lst_Demos->ItemIndex = 0;
        }
}
//---------------------------------------------------------------------------
bool Tfrm_ETProTools::deleteDirectory(LPCTSTR lpszDir, bool noRecycleBin)
{
  int len = _tcslen(lpszDir);
  TCHAR *pszFrom = new TCHAR[len+2];
  _tcscpy(pszFrom, lpszDir);
  pszFrom[len] = 0;
  pszFrom[len+1] = 0;
  
  SHFILEOPSTRUCT fileop;
  fileop.hwnd   = NULL;    // no status display
  fileop.wFunc  = FO_DELETE;  // delete operation
  fileop.pFrom  = pszFrom;  // source file name as double null terminated string
  fileop.pTo    = NULL;    // no destination needed
  fileop.fFlags = FOF_NOCONFIRMATION|FOF_SILENT;  // do not prompt the user
  
  if(!noRecycleBin)
    fileop.fFlags |= FOF_ALLOWUNDO;

  fileop.fAnyOperationsAborted = FALSE;
  fileop.lpszProgressTitle     = NULL;
  fileop.hNameMappings         = NULL;

  int ret = SHFileOperation(&fileop);
  delete [] pszFrom;  
  return (ret == 0);
}
//---------------------------------------------------------------------------
void Tfrm_ETProTools::checkButtonStates()
{
        AnsiString strBackupDir = ExtractFileDir(frm_Options->txt_ETPath->Text) + "\\etpro.etspro.bak";
        AnsiString strEtProDir = ExtractFileDir(frm_Options->txt_ETPath->Text) + "\\etpro";

        if (DirectoryExists(strBackupDir))
        {
                btn_CleanETPro->Enabled = false;
                btn_RestoreETPro->Enabled = true;
                lst_Demos->Enabled = true;
                btn_PlayDemo->Enabled = true;
                btn_Refresh->Enabled = true;
        }
        else
        {
                btn_CleanETPro->Enabled = true;
                btn_RestoreETPro->Enabled = false;
                lst_Demos->Enabled = false;
                btn_PlayDemo->Enabled = false;
                btn_Refresh->Enabled = false;
        }

        if (!FileExists(frm_Options->txt_ETPath->Text) || !DirectoryExists(strEtProDir))
        {
                btn_CleanETPro->Enabled = false;
                btn_RestoreETPro->Enabled = false;
                lst_Demos->Enabled = false;
                btn_PlayDemo->Enabled = false;
                btn_Refresh->Enabled = false;
        }
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_ETProTools::FormShow(TObject *Sender)
{
        checkButtonStates();
        fillDemoListbox();
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_ETProTools::btn_CleanETProClick(TObject *Sender)
{
        AnsiString strBackupDir = ExtractFileDir(frm_Options->txt_ETPath->Text) + "\\etpro.etspro.bak";
        AnsiString strETProDir = ExtractFileDir(frm_Options->txt_ETPath->Text) + "\\etpro";
        AnsiString strETProResDir = ExtractFileDir(Application->ExeName) + "\\VersionData\\etpro_3_2_0";
        AnsiString strFileExt, strSrcFile, strDestFile, strSearchFile, strParams;
        int iCount = 0;
        WIN32_FIND_DATA FindFileData;
        HANDLE hFind = INVALID_HANDLE_VALUE;

        strSearchFile = strETProDir + "\\*";
        hFind = FindFirstFile(strSearchFile.c_str(), &FindFileData);

        if (hFind != INVALID_HANDLE_VALUE)
        {
                do
                {
                        if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                        {
                                strFileExt = ExtractFileExt(FindFileData.cFileName);
                                if (strFileExt == ".pk3" || strFileExt == ".dll")
                                {
                                        strSrcFile = strETProDir + "\\";
                                        strSrcFile += FindFileData.cFileName;
                                        strDestFile = strBackupDir + "\\";
                                        strDestFile += FindFileData.cFileName;
                                        // Back it up
                                        if (!DirectoryExists(strBackupDir)) CreateDirectoryA(strBackupDir.c_str(), NULL);
                                        if (MoveFile(strSrcFile.c_str(), strDestFile.c_str())) iCount++;
                                }
                        }
                } while (FindNextFile(hFind, &FindFileData) != 0);
                FindClose(hFind);
        }

        // Copy a fresh copy of ETPro 3.2.0 over
        TStringList* p_strlstFiles = new TStringList;
        p_strlstFiles->Add("\\etpro-3_2_0.pk3");
        p_strlstFiles->Add("\\qagame_mp_x86.dll");
        p_strlstFiles->Add("\\cgame_mp_x86.dll");
        p_strlstFiles->Add("\\ui_mp_x86.dll");

        for (int i=0; i<p_strlstFiles->Count; i++)
        {
                strSrcFile = strETProResDir + p_strlstFiles->operator [](i);
                strDestFile = strETProDir + p_strlstFiles->operator [](i);
                CopyFile(strSrcFile.c_str(), strDestFile.c_str(), false);
        }

        delete p_strlstFiles;
        checkButtonStates();

        //Application->MessageBoxA("Finished cleaning up ET Pro trash!", "ET Starter Pro - Clean Up ET Pro Trash", MB_OK | MB_ICONINFORMATION);

        /*
        if (frm_Main->changeVersion(2))
        {
                if (Application->MessageBoxA("Done!\n\nDo you want to start ET Pro now?", "ET Starter Pro", MB_YESNO | MB_ICONINFORMATION) == ID_YES)
                {
                        strParams = "+set fs_game etpro";
                        ShellExecute(NULL, "open", frm_Options->txt_ETPath->Text.c_str(), strParams.c_str(), ExtractFileDir(frm_Options->txt_ETPath->Text).c_str(), SW_SHOWNORMAL);
                }
        }
        else Application->MessageBoxA("Could not copy patch files to your ET directory!\n\nMake sure that files are not write protected or in use.\nAlso make sure that the directory \"VersionData\" exists in the ETStarter Pro directory.", "ET Starter Pro - Error", MB_ICONERROR | MB_OK);
        */


}
//---------------------------------------------------------------------------
void __fastcall Tfrm_ETProTools::btn_RestoreETProClick(TObject *Sender)
{
        AnsiString strBackupDir = ExtractFileDir(frm_Options->txt_ETPath->Text) + "\\etpro.etspro.bak";
        AnsiString strETProDir = ExtractFileDir(frm_Options->txt_ETPath->Text) + "\\etpro";
        AnsiString strSrcFile, strDestFile, strSearchFile;
        int iCount = 0;
        WIN32_FIND_DATA FindFileData;
        HANDLE hFind = INVALID_HANDLE_VALUE;

        strSearchFile = strBackupDir + "\\*";
        hFind = FindFirstFile(strSearchFile.c_str(), &FindFileData);

        if (hFind != INVALID_HANDLE_VALUE)
        {
                do
                {
                        if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                        {
                                strSrcFile = strBackupDir + "\\";
                                strSrcFile += FindFileData.cFileName;
                                strDestFile = strETProDir + "\\";
                                strDestFile += FindFileData.cFileName;
                                // Restore it
                                if (MoveFile(strSrcFile.c_str(), strDestFile.c_str())) iCount++;
                        }
                } while (FindNextFile(hFind, &FindFileData) != 0);
                FindClose(hFind);
        }

        // Delete the backup directory
        deleteDirectory(strBackupDir.c_str());
        checkButtonStates();

        //Application->MessageBoxA("Finished restoring ET Pro trash!", "ET Starter Pro - Restore ET Pro Trash", MB_OK | MB_ICONINFORMATION);
}
//---------------------------------------------------------------------------
void __fastcall Tfrm_ETProTools::lst_DemosDblClick(TObject *Sender)
{
        playSelectedDemo();
}
//---------------------------------------------------------------------------


void __fastcall Tfrm_ETProTools::btn_PlayDemoClick(TObject *Sender)
{
        playSelectedDemo();        
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_ETProTools::FormCloseQuery(TObject *Sender,
      bool &CanClose)
{
        if (btn_RestoreETPro->Enabled)
        {
                if (Application->MessageBoxA("Really close without restoring your ET Pro files?", "ET Starter Pro", MB_YESNO | MB_ICONWARNING) == ID_NO) CanClose = false;
        }
        else CanClose=true;
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_ETProTools::btn_RefreshClick(TObject *Sender)
{
        fillDemoListbox();
}
//---------------------------------------------------------------------------

void __fastcall Tfrm_ETProTools::FormClose(TObject *Sender,
      TCloseAction &Action)
{
        AnsiString strEtsproDefaultDemo = ExtractFileDir(frm_Options->txt_ETPath->Text) + "\\etpro\\demos\\etspro.default.dm_84";

        if (FileExists(strEtsproDefaultDemo))
        {
                DeleteFile(strEtsproDefaultDemo);
        }

}
//---------------------------------------------------------------------------

