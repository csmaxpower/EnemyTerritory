//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <Buttons.hpp>
#include <ImgList.hpp>
#include <Graphics.hpp>
#include <Dialogs.hpp>
#include <ComCtrls.hpp>
#include "BMShapedForm.h"
#include <vector>

using namespace std;

#define CRYPTKEY "k49J#*lI#a9Hk3"

struct SERVERNODE
{
        int     iETVersion;
        char    szServerName[128];
        char    szServerIP[24];
        char    szPrivatePass[128];
};

//---------------------------------------------------------------------------
class Tfrm_Main : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
        TComboBox *cmb_ETVersion;
        TComboBox *cmb_Server;
        TMainMenu *menMainMenu;
        TMenuItem *men_File;
        TMenuItem *men_Exit;
        TMenuItem *men_Tools;
        TMenuItem *men_Options;
        TMenuItem *men_Help;
        TMenuItem *men_About;
        TBitBtn *btn_PatchConnect;
        TBitBtn *btn_PatchOnly;
        TCheckBox *chk_ETMin;
        TCheckBox *chk_Teamspeak;
        TCheckBox *chk_ETKey;
        TCheckBox *chk_CustomApp;
        TGroupBox *grpServerInfo;
        TLabel *lbl_HlServerStatus;
        TLabel *lbl_ServerStatus;
        TLabel *lbl_HlPlayers;
        TLabel *lbl_ServerName;
        TLabel *lbl_Players;
        TLabel *lbl_HlMap;
        TLabel *lbl_Map;
        TCheckBox *chk_CustomAppTerminate;
        TMenuItem *men_Serverbrowser;
        TLabel *lbl_HlMod;
        TLabel *lbl_HlPing;
        TLabel *lbl_Mod;
        TLabel *lbl_Ping;
        TMenuItem *men_Export;
        TMenuItem *men_Import;
        TMenuItem *N1;
        TSaveDialog *dlg_Export;
        TOpenDialog *dlg_Import;
        TMenuItem *N2;
        TBitBtn *btn_Refresh;
        TImage *img_Locked;
        TBMShapedForm *shapedfrm_Main;
        TLabel *lbl_Server;
        TLabel *lbl_ETVersion;
        TBevel *Bevel9;
        TBevel *Bevel8;
        TBevel *Bevel7;
        TBevel *Bevel6;
        TBevel *Bevel5;
        TBevel *Bevel4;
        TBevel *Bevel2;
        TBevel *Bevel12;
        TBevel *Bevel11;
        TBevel *Bevel10;
        TBevel *Bevel1;
        TPopupMenu *popmen_Main;
        TMenuItem *popmen_File;
        TMenuItem *popmen_Import;
        TMenuItem *popmen_Export;
        TMenuItem *N3;
        TMenuItem *popmen_Exit;
        TMenuItem *popmen_Tools;
        TMenuItem *popmen_Serverbrowser;
        TMenuItem *N4;
        TMenuItem *popmen_Options;
        TMenuItem *popmen_Help;
        TMenuItem *popmen_About;
        TBitBtn *btn_Players;
        TMenuItem *popmen_ExitMain;
        TImage *img_NotLocked;
        TImage *img_Exit;
        TImage *img_Minimize;
        TImage *img_Menu;
        TMenuItem *popmen_GammaFix;
        TBevel *Bevel3;
        TCheckBox *chk_ProfilePid;
        TBevel *Bevel14;
        TCheckBox *chk_CustomArgs;
        TBevel *Bevel15;
        TImage *img_StartMinimizer;
        TImage *img_StartTSVentrilo;
        TImage *img_DeleteETKey;
        TImage *img_DeleteProfilePid;
        TImage *img_RunCusomCommand;
        TImage *img_QuitCustomApp;
        TMenuItem *popmen_ETProTools;
        TMenuItem *popmen_ImportETKey;
        TMenuItem *popmen_ExportETKey;
        TMenuItem *N5;
        TSaveDialog *dlg_ETKeyExport;
        TOpenDialog *dlg_ETKeyImport;
        TBevel *Bevel13;
        void __fastcall men_OptionsClick(TObject *Sender);
        void __fastcall men_ExitClick(TObject *Sender);
        void __fastcall men_AboutClick(TObject *Sender);
        void __fastcall cmb_ServerChange(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall btn_PatchConnectClick(TObject *Sender);
        void __fastcall btn_PatchOnlyClick(TObject *Sender);
        void __fastcall men_ServerbrowserClick(TObject *Sender);
        void __fastcall men_ExportClick(TObject *Sender);
        void __fastcall men_ImportClick(TObject *Sender);
        void __fastcall btn_RefreshClick(TObject *Sender);
        void __fastcall popmen_ImportClick(TObject *Sender);
        void __fastcall popmen_ExportClick(TObject *Sender);
        void __fastcall popmen_ExitClick(TObject *Sender);
        void __fastcall popmen_ServerbrowserClick(TObject *Sender);
        void __fastcall popmen_OptionsClick(TObject *Sender);
        void __fastcall popmen_AboutClick(TObject *Sender);
        void __fastcall btn_PlayersClick(TObject *Sender);
        void __fastcall popmen_ExitMainClick(TObject *Sender);
        void __fastcall chk_ETMinClick(TObject *Sender);
        void __fastcall chk_TeamspeakClick(TObject *Sender);
        void __fastcall chk_ETKeyClick(TObject *Sender);
        void __fastcall chk_CustomAppClick(TObject *Sender);
        void __fastcall chk_CustomAppTerminateClick(TObject *Sender);
        void __fastcall img_ExitClick(TObject *Sender);
        void __fastcall img_MinimizeClick(TObject *Sender);
        void __fastcall img_MenuClick(TObject *Sender);
        void __fastcall popmen_GammaFixClick(TObject *Sender);
        void __fastcall chk_ProfilePidClick(TObject *Sender);
        void __fastcall chk_CustomArgsClick(TObject *Sender);
        void __fastcall img_StartMinimizerClick(TObject *Sender);
        void __fastcall img_StartTSVentriloClick(TObject *Sender);
        void __fastcall img_DeleteETKeyClick(TObject *Sender);
        void __fastcall img_DeleteProfilePidClick(TObject *Sender);
        void __fastcall img_RunCusomCommandClick(TObject *Sender);
        void __fastcall img_QuitCustomAppClick(TObject *Sender);
        void __fastcall popmen_ETProToolsClick(TObject *Sender);
        void __fastcall popmen_ExportETKeyClick(TObject *Sender);
        void __fastcall popmen_ImportETKeyClick(TObject *Sender);
private:	// Anwender-Deklarationen
public:		// Anwender-Deklarationen
        __fastcall Tfrm_Main(TComponent* Owner);

        vector <SERVERNODE>     m_Servers;
        AnsiString              m_IniFile;
        int                     m_iLastServerIndex;

        void readIniFile();
        void fillForms(bool bPlayerWinCheck = true);
        bool startMinimizer();
        bool startTSVentrilo();
        bool deleteETKey();
        bool deleteProfilePid();
        bool runCustomCommand();
        bool terminateCusomApp();
        bool changeVersion(int iVersion = -1);
        bool processExists(char* szProcName);
        bool processTerminate(char* szProcName);
        bool parseAppList(TStringList* strList, AnsiString strSource);
        bool queryServer();
        void highlightPlayers(AnsiString strPattern);
        void deleteFileFromDir(AnsiString strDirectory, AnsiString strFileName, bool bRecursive = false);

        char* encryptString(char* szSource, char* szKey);

        void __fastcall OnFormMove(TWMMove aMsg);
        protected:
        BEGIN_MESSAGE_MAP
        VCL_MESSAGE_HANDLER(WM_MOVE, TWMMove, OnFormMove)
        END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE Tfrm_Main *frm_Main;
//---------------------------------------------------------------------------
#endif
