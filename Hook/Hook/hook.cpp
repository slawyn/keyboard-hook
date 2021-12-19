// hookdll.h



/////////////////////////////////////////////////////////////

// hookdll.c

// Der Keyboardhook wird beim aufruf die Funktion SetupHook (HWND)
// vom Hauptprogramm aus gesetzt. Dabei wird das Fensterhandle des
// aufrufendes Programms an die Funktion übergeben. Als Ergebnis
// sendet die hookdll.dll über die Funktion KeyboardHookProc 
// jedes mal wenn eine Taste betätigt wird eine Nachricht 
// vom Typ (WM_USER + 2) an das Hauptprogramm. Dabaei steht der
// Tastencode der gedrückten Taste in dem Paramter "wParam".
// Wenn der Hook nicht mehr benötigt wird, löscht man ihn über die
// Funkion UninstallHook ()

#include <windows.h>
#include "hookdll.hpp"    /* -> Siehe Oben */
#include <iostream>
using namespace std;
// Weise den Compiler an, die Variable hWindow in einem
// separaten Abschnitt namens Shared unterzubringen
// Darüber hinaus ist dann auch noch dem Linker mitzuteilen,
// dass die Daten in diesem Abschnitt von allen Instanzen 
// dieser Anwendung gemeinsam verwendet werden sollen.
// Ganz wichtig dabei ist, dass die Variablen initialisiert
// sein müssen.

#pragma data_seg ("Shared")
HWND hWindow = 0;
#pragma data_seg ()

// Weise den Compilern, den Abschnitt Shared als lesbar,
// beschreibbar und zur gemeinsamen Verwendung zu deklarieren - "RSW".

#pragma comment (linker, "/section:Shared,RWS")

LRESULT CALLBACK KeyboardHookProc(int, WPARAM, LPARAM);
// Der Prototyp der Funktion KeyboardHookProc

HHOOK       hhkHook;
HINSTANCE   hDllInstance;
// Definition globaler Variablen

/************************************************************************/
/* DllMain: wird automatisch aufgerufen, wenn die DLL mit LoadLibrary() */
/*          geladen, oder mit FreeLibrary wieder freigegeben wird.      */
/* Eingabe Parameter: Systemspezifische Daten                           */
/* Return-Wert: TRUE                                                    */
/************************************************************************/

int APIENTRY DllMain(HINSTANCE hInstance, DWORD fdwReason, PVOID pvReserved) {
    switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
        // Die DLL wird in den Adressraum des aktuellen Prozesses
        // eingeblendet.
        hDllInstance = hInstance;
        // Initialisierung der globalen Variable
        break;
    }
    return TRUE;
}

/***********************************************************/
/* SetupHook: EXPORT Funktion, setzt den Keyboardhook      */
/* Eingabe Parameter: HWND des aufrufenden Fensters        */
/* Return-Wert: TRUE                                       */
/***********************************************************/

EXPORT BOOL CALLBACK SetupHook(HWND hParent) {
    hWindow = hParent;
    // Initialisierung der globalen Variable mit dem Eingabe Parameter
    hhkHook = SetWindowsHookEx(WH_KEYBOARD, KeyboardHookProc, hDllInstance, NULL);
    // Keyboardhook setzen und das Ergebnis in der globalen Variable
    // hhkHook speichern.

    if (hhkHook != NULL) {
        return TRUE;
    }
    else {
        return FALSE;
    }
 
}

/*******************************************************************/
/* KeyboardHookProc: wird jedes mal aufgerufen, wenn eine Taste    */
/*                   betätigt wird                                 */
/* Eingabe Parameter: Systemspezifische Daten                     */
/* Return-Wert: s. u.                                              */
/*******************************************************************/

LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        // Verhindern das eine Nachricht mehrmals verarbeitet wird.
        if ((lParam & 1073741824) != 1073741824) {
            // Überprüfen ab vor dem Aufruf dieser Funktion die Taste bereits gedrückt war
            //std::cout << "NEW INPUT" << std::endl;
            SendMessage((HWND)hWindow, (WM_USER + 2), (WPARAM)wParam, (LPARAM)lParam);
            // Senden der Nachricht (WM_USER + 2) und den Tastencode der gedrückten 
            // Taste (gespeichert in "wParam") an das in der globalen Variable 
            // hWindow gespeicherte Fensterhandle.
        }
    }
    return CallNextHookEx(hhkHook, nCode, wParam, lParam);
    // Die Nachrichten an den nächsten Hook weiterreichen.
}

/*********************************************************************/
/* UninstalHook: EXPORT Funktion, löscht den über die Funktion       */
/*               SetupHook gesetzten Hook                            */
/* Eingabe Parameter: keine                                          */
/* Return-Wert: TRUE                                                 */
/*********************************************************************/

EXPORT BOOL CALLBACK UninstallHook(void) {
    UnhookWindowsHookEx(hhkHook);
    // den in der globalen Variable hhkHook gespeicherten Hook löschen.
    return TRUE;
}