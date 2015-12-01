//---------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <registry.hpp>
#include <sstream>
#include <vcl.h>
#include <string>
#include <vector>
#pragma hdrstop
#include "Unit1.h"
#include "Unit2.h"
#include "Unit3.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
bool Run = true;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{}

std::string __fastcall TForm1::GetAlphabet()
{
        if(RadioButton1->Checked)
                return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        else if(RadioButton2->Checked)
                return "абвгґдеёєжзиіїйклмнопрстуфхцчшщъыьэюяАБВГҐДЕЁЄЖЗИІЇЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
                //return "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
        else
                return "абвгґдеёєжзиіїйклмнопрстуфхцчшщъыьэюяАБВГҐДЕЁЄЖЗИІЇЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

        // додаємо часто-вживані символи
        //alphabet+=" -_.,()!?";
}
//---------------------------------------------------------------------------

std::string Encipherment(std::string text, std::string primarykey, std::string alphabet)
{ 
        // знаходимо ключ (первинний ключ+автоключ)
        std::string key = primarykey+text.substr(0,text.length() - primarykey.length());

        // шифрування
        std::string result="";
        int index1, index2, text_len = text.length(), alph_len = alphabet.length();
        for(int i=0;i<text_len;i++)
        {
                // шукаємо в алфавіті і-тий символ тексту
                if((index1 = alphabet.find(text[i],0)) != -1)
                {
                        // шукаємо в алфавіті і-тий символ ключа
                        if((index2 = alphabet.find(key[i],0)) != -1)
                        {
                                index1 += index2;
                                index1 %= alph_len;
                                result += alphabet[index1];
                        }
                        // якщо такого символу немає в алфавіті
                        else
                        {
                                key.erase(i,1);
                                i--;
                        }
                }
                // якщо такого символу немає в алфавіті
                else
                {
                        text.erase(i,1);
                        text_len--;
                        i--;
                }
        }
        return result;
}
//---------------------------------------------------------------------------

std::string Decipherment(std::string text, std::string key, std::string alphabet)
{
        // розшифрування
        std::string result="";
        int index1, index2, text_len = text.length(), alph_len = alphabet.length();
        for(int i=0;i<text_len;i++)
        {
                // шукаємо в алфавіті і-тий символ тексту
                if((index1 = alphabet.find(text[i],0)) != -1)
                {
                        // шукаємо в алфавіті і-тий символ ключа
                        if((index2 = alphabet.find(key[i],0)) != -1)
                        {
                                index1-=index2;
                                while(index1<0)
                                        index1+=alph_len;
                                result += alphabet[index1];
                                key += alphabet[index1];
                        }
                        // якщо такого символу немає в алфавіті
                        else
                        {
                                key.erase(i,1);
                                i--;
                        }
                }
                // якщо такого символу немає в алфавіті
                else
                {
                        text.erase(i,1);
                        text_len--;
                        i--;
                }
        }
        return result;
}

//---------------ШИФРУВАННЯ--------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
        // зчитування введених даних
        std::string txt = Memo1->Text.c_str();
        std::string pr_key = Edit2->Text.c_str();

        std:string res = Encipherment(txt,pr_key,GetAlphabet());

        //виведення результату
        Form2->Memo1->Text = res.c_str();
        Form2->ShowModal();
}

//--------------РОЗШИФРУВАННЯ------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender)
{
        // зчитування введених даних
        std::string txt = Memo1->Text.c_str();
        std::string pr_key = Edit2->Text.c_str();

        std:string res = Decipherment(txt,pr_key,GetAlphabet());

        //виведення результату
        Form2->Memo1->Text = res.c_str();
        Form2->ShowModal();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::FormCreate(TObject *Sender)
{
        //створення форми
        Form3 = new TForm3(this);
        Form3->Width = 400;
        Form3->Height = 200;
        Form3->Caption = "";
        //Form3->OnClose = CloseForm;

        // серійний номер та дата біоса
        String StrSerial = GetVSNumber().c_str();
        String StrDate = GetBiosDate();//.c_str();

        // лабел з срійним номером та датою створення біоса
        TLabel* Lbl1 = new TLabel(Form3);
        Lbl1->Parent = Form3;
        Lbl1->Caption = "Hard Disck Serial Number: "+StrSerial+"; Bios Creating Date: "+StrDate;
        Lbl1->Height = 35;
        Lbl1->Left = (Form3->Width/2) - (Lbl1->Width/2) - 10;
        Lbl1->Top = (Form3->Height/2) - (Lbl1->Height/2) - 55;

        // лабел з повідомленням
        TLabel* Lbl2 = new TLabel(Form3);
        Lbl2->Parent = Form3;
        Lbl2->Caption = CheckForOriginality(&StrSerial,&StrDate); // перевірка на оригінальність програми
        Lbl2->Height = 35;
        Lbl2->Left = (Form3->Width/2) - (Lbl2->Width/2) - 10;
        Lbl2->Top = (Form3->Height/2) - (Lbl2->Height/2) - 20;

        // кнопка
        TButton* Btn1 = new TButton(Form3);
        Btn1->Parent = Form3;
        Btn1->Width = 120;
        Btn1->Height = 30;
        Btn1->Caption = "Ok";
        Btn1->Left = (Form3->Width/2) - (Btn1->Width/2) - 10;
        Btn1->Top = (Form3->Height/2) - (Btn1->Height/2) + 15;
        if(Run == true)
                Btn1->OnClick = Btn1_ClickToRun;
        else
                Btn1->OnClick = Btn1_ClickToTerminate;

        // показати форму
        Form3->ShowModal();

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Btn1_ClickToRun(TObject *Sender)
{
        Form3->Close();
        Form1->Show();
}

void __fastcall TForm1::Btn1_ClickToTerminate(TObject *Sender)
{
        Form3->Close();
        Application->Terminate();
        //Form1->Close();
}
//---------------------------------------------------------------------------

// функція, що перевіряє оригінальність програми
String __fastcall TForm1::CheckForOriginality(String* SerialNum, String* BiosDate)
{
     string param = SerialNum->c_str()+(string)" "+BiosDate->c_str();

     // зчитування із файлу
     ifstream in;
     in.open("info.txt");
     vector <String> vecFileData;
     string tmp;
     while(getline(in,tmp))
          vecFileData.push_back(tmp.c_str());
     in.close();

     // перевірка
     Run = true;
     if(vecFileData.empty())
     {
          ofstream out;
          out.open("info.txt");
          out << param;
          out.close();
          return "First start of this program (copies count = 0).";
     }
     else
     {
          int copies_count = vecFileData.size() - 1;
          vector<String>::iterator pos = find(vecFileData.begin(), vecFileData.end(), param.c_str());
          if(pos != vecFileData.end())//якщо знайшли елемент
          {
               int index = distance(vecFileData.begin(),pos);
               if(index==0)//якщо це оригінал
                    return "Original program. Copies count: "+IntToStr(copies_count);
               else //якщо копія                       
                    return "Copy #"+IntToStr(index)+" of original program. Copy count: "+IntToStr(copies_count);
               
          }
          else //якщо НЕ знайшли елемент
          {
               if(copies_count<4) // якщо ліміт копій НЕ перевищено
               {
                    ofstream out; 
                    out.open("info.txt", fstream::app);
                    out << "\n"+param;
                    out.close();
                    return "This is a new copy of original program and it's number = "+IntToStr(copies_count+1);
               }
               else
               {
                    Run = false;
                    return "Limit of copies!";
               }
          }
     }
}
//---------------------------------------------------------------------------
// взнати серійний номер жорсткого диску
std::string __fastcall TForm1::GetVSNumber()
{
     char NameBuffer[MAX_PATH];
     char SysNameBuffer[MAX_PATH];
     DWORD VSNumber;
     DWORD MCLength;
     DWORD FileSF;

     if(GetVolumeInformation("C:\\",NameBuffer, sizeof(NameBuffer),&VSNumber,&MCLength,&FileSF,SysNameBuffer,sizeof(SysNameBuffer)))
     {

         std::ostringstream oss;
         oss << VSNumber;
         std::string strVSNumber = oss.str();
		 return strVSNumber;
     }
     return "function error";
}

//-----------------------Дата створення БІОСа---------------------------

AnsiString __fastcall GetRegString(HKEY Reg_key, AnsiString Path, AnsiString Key)
{
        TRegistry *reg=new TRegistry();
        TRegDataType rdtKeyDataType;
        AnsiString Result;
        bool rescode;

        reg->Access=KEY_ALL_ACCESS;
        reg->RootKey = Reg_key;

        rescode=reg->OpenKey(Path, false);
        if(!rescode)
        {
                reg->CloseKey();
                delete reg;
                return (Result="");
        }
        try
        {
                if(reg->ValueExists(Key))
                {
                        rdtKeyDataType=reg->GetDataType(Key);
                        if(rdtKeyDataType==rdString || rdtKeyDataType==rdExpandString)
                                Result=reg->ReadString(Key);
                        else if(rdtKeyDataType==rdUnknown)
                                Result="";
                        else
                                Result="";
                }
                else
                {
                        Result="";
                }
        }
        catch(ERegistryException &xcpt)
        {
                Application->MessageBox(xcpt.Message.c_str(), "Error reading registry value", MB_OK|MB_ICONEXCLAMATION);
                Result="";
        }
        reg->CloseKey();
        delete reg;
        return Result;
}
//---------------------------------------------------------------------------
String __fastcall TForm1::GetBiosDate()
{
        return GetRegString(HKEY_LOCAL_MACHINE, "Hardware\\Description\\System", "SystemBiosDate");
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
        delete Form3;
}
//---------------------------------------------------------------------------

