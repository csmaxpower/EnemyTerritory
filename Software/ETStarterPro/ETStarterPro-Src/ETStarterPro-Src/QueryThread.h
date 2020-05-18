//---------------------------------------------------------------------------

#ifndef QueryThreadH
#define QueryThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class TQueryThread : public TThread
{            
private:
protected:
        void __fastcall Execute();
public:
        __fastcall TQueryThread(bool CreateSuspended);
};
//---------------------------------------------------------------------------
#endif
