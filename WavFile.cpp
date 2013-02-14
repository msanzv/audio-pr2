/*
 * INSTRODUCCIÓN A LA ADQUISICIÓN, SÍNTESIS Y PROCESAMIENTO DE AUDIO
 *
 * Materiales para procesamiento de archivos RIFF-WAVE (.wav)
 *
 * ARCHIVO WavFile.h
 * Implementación de las clases InputWav y OutputWav
 *
 * Creado por Alvaro Doménech en noviembre de 2007
 * Actualizado en febrero de 2010 
 *
 */


#include <fstream>
#include <string.h>
#include <iostream>
#include "WavFile.h"


using namespace std;

// Identificadores de chunk
char	idRIFF[5] = "RIFF";
char	idWAVE[5] = "WAVE";
char	chunkFormat[5] = "fmt ";
char	chunkData[5] = "data";


/* MÉTODOS DE LECTURA */

/****************************************************************
Open: este método hace la mayor parte del trabajo. Abre el archivo WAV, lee su geometría y
lo deja a punto para leer los datos PCM.

Devuelve errores:
 0 : Todo bien
-1 : No puede abrir el archivo
-2 : Ha abierto el archivo, pero no lo reconoce como (RIFF) WAV
-3 : El archivo es de tipo WAV, pero no encuentra la secciÃ³n de formato
-4 : El archivo es WAV y tiene formato, pero no encuentra datos PCM 
****************************************************************/
  
int InputWav::Open(string nombrearchivo){
char	id[5];
long	chunkLlarg;

//------ Abrir archivo

archivo.open(nombrearchivo.c_str(), ios::in | ios::binary);
if (!archivo) return (-1);

//------ Identificar archivo com RIFF-WAVE

archivo.read(id,4); id[4]='\0';
if (*id != *idRIFF) return(-2);
archivo.read(reinterpret_cast<char *>(& llargarchivo),4);
archivo.read(id,4); 
if (*id != *idWAVE) return(-2);

//------ Leer sección de formato

archivo.read(id,4); 
if (*id != *chunkFormat) return(-3);
archivo.read(reinterpret_cast<char *>(& format),20);

//------- Buscar sección de datos

archivo.read(id,4); 
while (*id != *chunkData) {
//	cout << id;
	if (archivo.eof()) return(-4);
	archivo.read(reinterpret_cast<char *>(& chunkLlarg),4);
	archivo.seekg(chunkLlarg,ios::cur);
	archivo.read(id,4);
	}

archivo.read(reinterpret_cast<char *>(& llargDades),4);
return(0);
}


/****************************************************************

Consultas variadas

****************************************************************/

// Consulta el número de canales
short InputWav::GetChannels(){   
	return (format.AudioChannels);
}

// Consulta el número de frames
long InputWav::GetFrames(){
	return(llargDades*8/(format.AudioChannels * format.BitsPerSample));
}

// Consulta la frecuencia de muestreo
long InputWav::GetSampleRate(){
	return(format.SampleRate);
}

// Consulta cuántos bytes ocupa cada muestra
short InputWav::GetBytesPerSample(){
	return(format.BitsPerSample / 8);
}


/****************************************************************

ReadData: Lee datos del archivo y los coloca donde corresponde

****************************************************************/

long InputWav::GetDataBytes(){
	return(llargDades);
}

void InputWav::ReadData(void * Buffer, long llarg){
	archivo.read(reinterpret_cast<char *>(Buffer), llarg);
}

void InputWav::Close(){
	archivo.close();
}

/* MÉTODOS DE ESCRITURA */


/***************************************************************

Constructor de un archivo para escritura. Le da un formato por omisión que se puede 
cambiar mediante otros métodos

****************************************************************/

OutputWav::OutputWav(){
// Valores de formato por omisión
	format.ChunkSize = 16;
	format.AudioFormat = 1;
	format.AudioChannels = 2;
	format.SampleRate = 44100;
	format.BitsPerSample = 16;
}

/****************************************************************
Open: Crea el archivo WAV, pero no escribe nada en él.

Devuelve errores:
 0 : Todo bien
-1 : No puede abrir el archivo
****************************************************************/

int OutputWav::Open(string nombrearchivo){

	archivo.open(nombrearchivo.c_str(), ios::out | ios::binary);
	if (!archivo) return (-1);

	return(0);
}

/****************************************************************
SetChannels: Fija el número de canales.
****************************************************************/

void OutputWav::SetChannels(short NC){
	format.AudioChannels = NC;
}


/****************************************************************
SetSampleRate: Fija la frecuencia de muestreo
****************************************************************/

void OutputWav::SetSampleRate(long SR){
	format.SampleRate = SR;
}


/****************************************************************
WriteData: Escribe el archivo
****************************************************************/


void OutputWav::WriteData(void * Buffer, long llarg){
	long RIFFllarg;

	format.BlockAlign = format.AudioChannels + format.BitsPerSample / 8;
	format.ByteRate = format.SampleRate * format.BitsPerSample * format.AudioChannels / 8;

	archivo.write(idRIFF,4);
	RIFFllarg =  llarg+36;
	archivo.write(reinterpret_cast<char *>(& RIFFllarg),4);
	archivo.write(idWAVE,4);
	archivo.write(chunkFormat,4);
	archivo.write(reinterpret_cast<char *>(& format),20);
	archivo.write(chunkData,4);
	archivo.write(reinterpret_cast<char *>(& llarg),4);
	archivo.write(reinterpret_cast<char *>(Buffer), llarg);
}

/* Cierre del archivo */

void OutputWav::Close(){
	archivo.close();
}
