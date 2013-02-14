/*
 * INSTRODUCCI�N A LA ADQUISICI�N, S�NTESIS Y PROCESAMIENTO DE AUDIO
 *
 * Materiales para procesamiento de archivos RIFF-WAVE (.wav)
 *
 * ARCHIVO consulta.cpp
 * Ejemplo de uso de las clases InputWav y OutputWav
 *
 * Creado por Alvaro Dom�nech en noviembre de 2007
 * Actualizado en febrero de 2012 
 *
 */

/* 
 * NOMBRE: 
 *
 */

using namespace std;
#include "WavFile.h"
#include <iostream>
#include <math.h>

//int _tmain(int argc, _TCHAR* argv[])
int main(int argc, char* argv[])
{

InputWav	Archivo;

// Para los par�metros PCM
short	NumCanales;
short	Cuantizacion;
long	NumFrames;
long	LongDades;
long	Fm;

// Buffers
typedef	short elementBufferST[2];
typedef short elementBufferM;
elementBufferST *	BufferST;
elementBufferM *	BufferM;

// Para buscar el m�ximo
short	actual, max;

//------ Comienza el programa
if (argc != 2) {
	cout << "Uso: " << argv[0] << " archivo.wav\n";
	exit(0);
}
cout << "Bienvenidos a " << argv[0] << "\n";

//------ Abre Archivo 

int error = Archivo.Open(argv[1]);
if (error == 0) cout << "Archivo " << argv[1] << " abierto\n";
else  { cout << "Error " << error << " al abrir el archivo " << argv[1] << "\n";
		exit(error); }

//------ Mira par�metros 

NumCanales = Archivo.GetChannels();
cout << NumCanales << " canal(es), ";
Cuantizacion = Archivo.GetBytesPerSample() * 8;
cout << Cuantizacion << " bits por muestra\n";
LongDades = Archivo.GetDataBytes();
cout << LongDades << " bytes totales (";
NumFrames = Archivo.GetFrames(); printf("HOLA\n");
cout << NumFrames << " frames)\n";
Fm = Archivo.GetSampleRate();
cout << "A " << Fm << ", " << float(NumFrames/Fm) << " seg.\n";

if (Cuantizacion != 16){
	cout << "Este programa s�lo trabaja con archivos de 16 bits de resoluci�n\n";
	exit(1);
}
if (NumCanales != 2){
	cout << "Este programa s�lo trabaja con archivos monoaurales o estereof�nicos\n";
	exit(2);
}

// Lee la onda

if (NumCanales==1) { // Archivo monoaural	
	BufferM = new elementBufferM[NumFrames];
	Archivo.ReadData(BufferM, LongDades);
	cout << NumFrames << " frames mono le�das\n";
} else {			// Archivo est�reo
	BufferST = new elementBufferST[NumFrames];
	Archivo.ReadData(BufferST, LongDades);
	cout << NumFrames << " frames est�reo le�das\n";
}

// Cierra archivo

Archivo.Close();

// Busca m�ximo

max = 0;
if (NumCanales==1) 	// Archivo monoaural	
	for(long f = 0; f < NumFrames; f++){
		actual = BufferM[f];
		if (actual < 0) actual = -actual;
		if (actual > max) max = actual;
	}
else 					// Archivo est�reo
	for(long f = 0; f < NumFrames; f++)
		for(short c = 0; c < 2; c++){
			actual = BufferST[f][c];
			if (actual < 0) actual = -actual;
			if (actual > max) max = actual;
		}

float dB = 20.0 * log10(max/(pow(2.0,Cuantizacion-1)-1));
cout << "M�xima amplitud = " << max << " (" << dB <<" dB)\n";

}

