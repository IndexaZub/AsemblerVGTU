#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

int main(int argc, char** argv)
{
	int skaiciausIlgis;
	int int_Rezultatas;
	char* parametras = argv[1]; // Pirmas parametruose paduotas stringas
	if (argc < 2) {
		printf("Nepateiktas parametras*/\n"); //Patikrinam ar įvestas parametras
		return(0);
	}
	_asm {
		push eax
		push ebx	//Aprašomi kintamieji
		push ecx
		push edx
		xor edx, edx // Nusinulinam edx
		xor eax, eax // Nusinulinam eax
		mov ebx, 0	// Nusinuliname ebx
		mov ecx, parametras // ecx = parametras.
		dec ecx		//Sumažiname ecx registrą nes jį poto vistiek padidinsim

		for_loop ://Ciklas
		inc ecx		//Padidiname indeksa ecx (Stringo parametro kintamasis)
			mov al, byte ptr[ecx]	//į 'al' registrą įsistatome ecx baito adresą

			cmp al, '0'
			jl exit_loop	//Tikriname ar 'al' registras yra maziau uz '0' chara, tada exit
			cmp al, '9'
			jg exit_loop //Tikriname ar daugiau 

			inc ebx	// Didiname registrą 'ebx', nes čia saugome stringo ilgio skaičių

			sub al, 48	//Atemame iš 'al' registro 48, tam, kad gautume int vertes, pagal ascii lentelę, nes prasideda nuo 48
			imul edx, 10 //Dauginame iš dešimt, kad gautume dešimtainius, šimtainius ir t.t skaičius
			add edx, eax //Poto sulipdome bendrą int skaičių, sudedant ką turime

			jmp for_loop	//Toliau sokam į ciklo pradžią, nebent yra nurodymas išeiti iš ciklo 'exit_loop'
			exit_loop :
		mov skaiciausIlgis, ebx	//iš registro 'ebx įsistatomę stringo ilgį į 'letterCount' kintamojo adresą
			mov int_Rezultatas, edx	//iš 'edx' registro įsistatomę gautą INT reikšmę į 'result' kintamojo adresą
			pop eax
			pop ebx
			pop ecx	//Ištriname iš atminties kintamuosius
			pop edx
	}
	if (skaiciausIlgis <= 10) { // Int registro max ilgis yra 32 bitai, o didziausias 32bito binary paverstas i desimtaini yra 2147493647 (10 simboliu)
		cout << "String " << parametras << " INT atitikmuo yra: " << int_Rezultatas << endl;	//Tikriname ar nėra daugiau simbolių paduoda į parametr1 nei 10

	}
	else {
		cout << "Neteisingai ivestas parametras!"; //Jei daugiau nei 10, išvedame, kad neteisingas parametras
	}
	float X[4];
	float Y[4];	// Kadangi dirbame su dideliais skaičiais kuriems nereikia didelio tikslumo, kad išsaugoti atminties, naudojame float
	float Z[4];
	float pitagoroSkaicius[4];


	ofstream isvedimas("rezultatai.txt");

	isvedimas << fixed << "X " << setw(10) << " Y " << setw(10) << " Z" << endl;	//Spausdinamas ir t.t

	for (int i = 1; i < 997; i += 4) //Imame po 4 simbolius iškarto ir apribojame iki 997, nes kai bus i+3, gausime, kad atsidursime 1000 indekse
	{
		X[0] = (float)i;
		X[1] = (float)i + 1;
		X[2] = (float)i + 2;
		X[3] = (float)i + 3;
		for (int j = i + 1; j < 1000; j++) // Apribojame iki 1000, nes kitame cikle jau yra apribojimas iki 997
		{
			Y[0] = (float)j;
			Y[1] = (float)j + 1;
			Y[2] = (float)j + 2;
			Y[3] = (float)j + 3;

			_asm // 𝑥^2 + 𝑦^2 = 𝑧^2
			{
				movups xmm0, X	// Į pirmą registrą įstatome X(pirmus keturis skaičius)				
				movups xmm1, Y	// Į antrą registrą įstatome Y(sekančius keturis skaičius)		
				mulps xmm0, xmm0	// Pakeliame kvadratu pirmus keturis float formato skaičius 		
				mulps xmm1, xmm1	// Pakeliame sekančius keturis skaičius kvadratu
				addps xmm0, xmm1	// Sudedame abu registrus		
				sqrtps xmm1, xmm0		// Iš 'xmm0' registro turinio ištraukiame šaknį ir įsistatome į 'xmm1' registrą	
				roundps xmm1, xmm1, 1	//	Ištraukiame sveiką dalį iš 'xmm1' registro	
				movups	Z, xmm1		// Gautą atsakymą įsistatome į Z masyvą, nes čia saugome pitagoro teoremos atsakymą 'Z'	
				mulps xmm1, xmm1	// Vėl pakeliame kvadratu, nes prieš tai ištraukėme, kad gauti 'Z'				
				subps xmm0, xmm1	// Atėmame iš 'xmm1' registro 'xmm0' registr1
				movups pitagoroSkaicius, xmm0	// Iš 'xmm0' registro perkeliame jame saugomą reikšmę į kintamajį 'num'

			}
			for (int n = 0; n < 4; n++)
			{
				bool pirminis = true;	//Ar pirminis skaičius salyga

				for (int k = 2; k <= (int)X[n] / 2; k++)	//Tikriname ar pirminis skaičius(Ar dalinasi TIKTAIS iš savęs arba '1'
				{
					if ((int)X[n] % k == 0)	//Jeigu dalinant liekana yra = 0, vadinasi ne pirminis
					{
						pirminis = false;
						break;	// Išeiname iš ciklo
					}
				}

				if (pitagoroSkaicius[n] == 0 && pirminis)	// Išvedame skaičius tik tada, jeigu gautas pitagoro pirminis skaičius
				{
					isvedimas << fixed << setprecision(0) << X[n] << setw(10) << Y[n] << setw(10) << Z[n] << endl;
				}
			}
		}
	}
	isvedimas.close();	//Uždarome failą ir išeiname iš programos
	return 0;
}