//---------------------------------------------------------------------------

#ifndef CompletionComboBoxH
#define CompletionComboBoxH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Messages.hpp>
//---------------------------------------------------------------------------
class PACKAGE TCompletionComboBox : public TComboBox
{
private:
   bool FTextCompletion;
   bool __fastcall GetTextCompletion();
   void __fastcall SetTextCompletion(const bool Value);
protected:
   void __fastcall ComboWndProc(Messages::TMessage &Message, void* HWnd, void* ComboProc);
public:
   __fastcall TCompletionComboBox(TComponent* Owner);
__published:
   __property bool TextCompletion = { read = GetTextCompletion, write=SetTextCompletion};

};
//---------------------------------------------------------------------------
#endif
