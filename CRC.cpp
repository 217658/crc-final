#include<iostream>
#include<string.h>
using namespace std;

class CRC {

    string vstup, delitel, delenec, vysledek;
    int len_delenec, len_gen, len_inp;

public:

    string fun_xor(string a, string b)              // funkce xor
    {
        string vysledek = "";
        if (a[0] == '0')                            // z 0110 udělá 110 -> 1101
        {
            return a.substr(1);
        }
        else
        {
            for (int i = 0; i < len_gen; i++)
            {
                vysledek = vysledek + (a[i] == b[i] ? '0' : '1');        //pokud jsou stejné vrátí 0, pokud jsou jiné 0 a 1 vrátí 1
            }
            return vysledek.substr(1);             // xor 1 a 1 je 0, ale my potřebujeme vrátit 1 do modulárního dělení
        }
    }

    void modulo_del()                                             //dělení 
    {
        string temp_del = delitel;
        string temp_delenec = delenec.substr(0, len_gen);
        int j = len_gen;


        while (j < len_delenec)
        {
            temp_delenec = fun_xor(temp_delenec, temp_del);
            temp_delenec = temp_delenec + delenec[j];
            j++;
        }

        vysledek = vstup + fun_xor(temp_delenec, temp_del);    //konečny výsledek - spojí zprácu k zakodování s CRC bity

    }

    void getdata()
    {

        /*   string vstup = " ";               // řešení aby vstup bral pouze 1 a 0, když jsem to zkusil 
                                                  přidat tak mi to rozbilo celou aplikaci, ale třeba za to bude bod :)) 
    bool badin = false;

    while (true)
    {
        cout << "Zadej zpravu k zakodovani (pouze v 1 a 0)\n";

        cin >> input;
        for (char& ch : vstup) 
            if (ch != 48 && ch != 49)
            {
                badin = true;
                cout << "in while\n";
            }

        if (!badin)
        {
            break;
        }

        badin = false;

    }

    cout << vstup;
*/
        cout << "\nZadejte kod k zakodovani: ";
        cin >> vstup;
        cout << "\nZadej generujici polynom: ";
        cin >> delitel;

        len_gen = delitel.length();                    
        len_inp = vstup.length();
        delenec = vstup;
        int r = len_gen - 1;                 // slouží ke kalkulaci počtu nul
        for (int i = 0; i < r; i++)          // vypíše potřebný počet nul 
        {
            delenec = delenec + '0';           
        }
        len_delenec = delenec.length();
        modulo_del();
    }

    void sender_side()
    {
        cout << "Kod k zakodovani " << vstup << "\n";
        cout << "Genericky kod: " << delitel << "\n";
        cout << "Data k odeslani: " << vysledek << "\n";

    }

    void receiver_side()
    {
        string data_rec;
        cout << "\nZadejte generujici polynom pro prijatou zpravu: ";
        cin >> data_rec;

        string temp_del = delitel;
        string temp_delenec = data_rec.substr(0, len_gen);
        int j = len_gen;
        while (j < data_rec.length())
        {
            temp_delenec = fun_xor(temp_delenec, temp_del);
            temp_delenec = temp_delenec + data_rec[j];
            j++;
        }
        string error = fun_xor(temp_delenec, temp_del);
        cout << "Zbytek je: " << error;

        bool flag = 0;
        for (int i = 0; i < len_gen - 1; i++)       //Kontroluje pokud je zbytek 0 nebo ne
        {
            if (error[i] == '1')
            {
                flag = 1;
                break;

            }
        }

        if (flag == 0)
            cout << "\nZprava byla dorucena bez chyby";
        else
            cout << "\nData obsahuji chybu";
    }

};


int main() {
    CRC crc;
    crc.getdata();
    crc.sender_side();
    crc.receiver_side();

    return 0;

}