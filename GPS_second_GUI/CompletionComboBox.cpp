// CompletionComboBox
// Distribué par http://www.developpez.com
// Auteur : Ricky81 ( http://ricky81.developpez.com )
//
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "CompletionComboBox.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
//  ValidCtrCheck est utilisé pour vérifier que les composants créés n'ont
// aucune fonction virtuelle pure.
//

static inline void ValidCtrCheck(TCompletionComboBox *)
{
   new TCompletionComboBox(NULL);
}
//---------------------------------------------------------------------------
__fastcall TCompletionComboBox::TCompletionComboBox(TComponent* Owner)
   : TComboBox(Owner)
{
}
//---------------------------------------------------------------------------
namespace Completioncombobox
{
   void __fastcall PACKAGE Register()
   {
       TComponentClass classes[1] = {__classid(TCompletionComboBox)};
       RegisterComponents("DVP", classes, 0);
   }
}
//---------------------------------------------------------------------------
bool __fastcall TCompletionComboBox::GetTextCompletion()
{
   return FTextCompletion;
}
//---------------------------------------------------------------------------
void __fastcall TCompletionComboBox::SetTextCompletion(const bool Value)
{
   FTextCompletion = Value;
}
//---------------------------------------------------------------------------
void __fastcall TCompletionComboBox::ComboWndProc(Messages::TMessage &Message, void* HWnd, void* ComboProc)
{
   int rc, len;
   if(Message.Msg == WM_KEYUP)
   {
      if ((Message.WParam != 8) && (Message.WParam != VK_DELETE)
         && (Message.WParam != VK_SHIFT) && (FTextCompletion == true))
      {
         rc = Perform(CB_FINDSTRING, -1,(int)Caption.c_str());
         if(rc != CB_ERR)
         {
            len = Text.Length();
            ItemIndex = rc;
            SelStart = len;
            SelLength = Text.Length() - len;
            Message.Result = 0;
         }
      }
   }
   else
   {
      TComboBox::ComboWndProc(Message,HWnd,ComboProc);
   }
}
//---------------------------------------------------------------------------
