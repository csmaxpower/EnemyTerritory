//---------------------------------------------------------------------------

#ifndef ServerDetailsH
#define ServerDetailsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class Tfrm_ServerDetails : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
        TLabeledEdit *txt_ServerName;
        TLabeledEdit *txt_ServerIP;
        TLabeledEdit *txt_PrivatePass;
        TComboBox *cmb_ETVersion;
        TLabel *lbl_ETVersion;
        TBitBtn *btn_OK;
        TBitBtn *btn_Cancel;
        void __fastcall btn_CancelClick(TObject *Sender);
        void __fastcall btn_OKClick(TObject *Sender);
private:	// Anwender-Deklarationen
public:		// Anwender-Deklarationen
        __fastcall Tfrm_ServerDetails(TComponent* Owner);
        bool    m_bAddServer;
        int     m_iServerId;

        void clearForm();
};
//---------------------------------------------------------------------------
extern PACKAGE Tfrm_ServerDetails *frm_ServerDetails;
//---------------------------------------------------------------------------
#endif
