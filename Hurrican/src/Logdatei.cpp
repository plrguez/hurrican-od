// Datei : Logdatei.cpp

// --------------------------------------------------------------------------------------
//
// Logdatei Klasse
// zum leichten Handhaben einer Protokoll Datei
//
// (c) 2002 J?rg M. Winterstein
//
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------------------

#include "Logdatei.h"									// Header-Datei einbinden
#if defined(PLATFORM_DIRECTX)
#include <windows.h>									// Windowsheader f?r Messagebox und Beep#
#elif defined(PLATFORM_SDL)
#include "SDL_port.h"
#endif
#include <string.h>										// F?r String Operationen
#include <stdio.h>										// F?r Datei Operationen
#if defined(ANDROID)
#include <android/log.h>
#endif

// --------------------------------------------------------------------------------------
// Variablen
// --------------------------------------------------------------------------------------

FILE *Logfile;											// Logdatei

extern HWND					g_hwnd;						// Fenster Handle des Hauptfensters
extern bool					GameRunning;				// L?uft das Spiel noch ?

// --------------------------------------------------------------------------------------
// Funktionen
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// Konstruktor
// erstellt eine neue Logdatei mit dem Namen "Name"
// --------------------------------------------------------------------------------------

Logdatei::Logdatei(const char Name[20])
{
    int len = strlen(Name) + 1;
    strcpy_s(itsFilename, len, Name);							// Namen sichern
    fopen_s(&Logfile, itsFilename, "w");					// Datei neu erstellen
    if (Logfile != NULL)
    {
        fclose(Logfile);									// und gleich wieder schliessen
    }

    delLogFile = false;
}

// --------------------------------------------------------------------------------------
// Destruktor
// --------------------------------------------------------------------------------------

Logdatei::~Logdatei()
{
}

// --------------------------------------------------------------------------------------
// WriteText
// schreibt den Text "Text" in die Logdatei
// bei Fehler-Eintrag ggf Messagebox ausgeben
// --------------------------------------------------------------------------------------

void Logdatei::WriteText(bool Abbruch, const char* text, ...)
{
#define BUFFER_SIZE 4096
    va_list fmtargs;
    char buffer[BUFFER_SIZE];

    va_start( fmtargs, text );
    vsnprintf( buffer, BUFFER_SIZE, text, fmtargs );
    va_end( fmtargs );

#if defined(ANDROID)
    __android_log_print(ANDROID_LOG_INFO, "libtcod", "%s", buffer);
#else
    printf( "%s", buffer );
#endif

    fopen_s(&Logfile, itsFilename, "a");					// Datei zum anf?gen ?ffnen
    if (Logfile != NULL)
    {
        fprintf_s(Logfile, buffer);								// und Text schreiben

        if(Abbruch == true)									// Abbruch nach Log-Eintrag ?
        {
            fprintf(Logfile, "\n    ^\n");
            fprintf(Logfile, "   /|\\\n");
            fprintf(Logfile, "    |\n");
            fprintf(Logfile, "    |\n");
            fprintf(Logfile, "    |\n");
            fprintf(Logfile, "  Error\n\n");
        }
        fclose(Logfile);									// Datei wieder schliessen
    }
    Logfile = NULL;

    if(Abbruch == true)									// Abbruch nach Log-Eintrag ?
    {
#if defined(PLATFORM_DIRECTX)
        MessageBox (g_hwnd, Text, "An error has occurred !", MB_OK | MB_ICONEXCLAMATION);
#elif defined(PLATFORM_SDL)
        printf( "An error has occurred !\n" );
#endif
        delLogFile  = false;
        GameRunning = false;
    }
}

// --------------------------------------------------------------------------------------
// WriteText
// schreibt den Wert "Value" in die Logdatei
// --------------------------------------------------------------------------------------

void Logdatei::WriteValue(int Value)
{
    fopen_s(&Logfile, itsFilename, "a");					// Datei zum anf?gen ?ffnen
    fprintf(Logfile, "%i\n", Value);					// und Wert schreiben
    fclose(Logfile);									// Datei wieder schliessen
}
