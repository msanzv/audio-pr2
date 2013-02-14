/*
 * INSTRODUCCIÓN A LA ADQUISICIÓN, SÍNTESIS Y PROCESAMIENTO DE AUDIO
 *
 * Materiales para procesamiento de archivos RIFF-WAVE (.wav)
 *
 * ARCHIVO WavFile.h
 * Especificación de las clases InputWav y OutputWav
 *
 * Creado por Alvaro Doménech en noviembre de 2007
 * Actualizado en febrero de 2010 
 *
 */

//#include <string.h>
#include <fstream>
using namespace std;

// Descripción del archivo
struct strChunkFormatPCM {
	unsigned long ChunkSize;
	unsigned short AudioFormat;
	unsigned short AudioChannels;
	unsigned long SampleRate;
	unsigned long ByteRate;
	unsigned short BlockAlign;
	unsigned short BitsPerSample;
};

// Métodos de lectura
class InputWav {
	public:
		int Open(string nombrearchivo);
		short GetChannels();
		long GetFrames();
		long GetDataBytes();
		long GetSampleRate();
		short GetBytesPerSample();
		void ReadData(void * Buffer, long llarg);
		void Close();
	private:
		ifstream archivo;
		strChunkFormatPCM format;
		long llargDades;
		long llargarchivo;
};


// Métodos de escritura
class OutputWav {	
	public:
		OutputWav();
 		int Open(string archivo);
		void SetChannels(short NC);
		void WriteData(void * Buffer, long llarg);
		void SetSampleRate(long SR);
		void Close();
	protected:
		strChunkFormatPCM format;
		ofstream archivo;
};
