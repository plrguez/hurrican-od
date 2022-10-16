// Datei : DX8Joystick.cpp

// --------------------------------------------------------------------------------------
//
// Joystick Klasse
//
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// Include Dateien
// --------------------------------------------------------------------------------------

#include "DX8Joystick.h"
#include "Logdatei.h"
#include "Gameplay.h"

// --------------------------------------------------------------------------------------
// Konstruktor
// --------------------------------------------------------------------------------------

DirectJoystickClass::DirectJoystickClass(void)
{
    lpDIJoystick = NULL;

    Active = false;
    JoystickX = 0;
    JoystickY = 0;
    JoystickPOV = -1;
    CanForceFeedback = false;
    NumButtons = 0;

    for(int i = 0; i<MAX_JOYSTICKBUTTONS; i++)
        JoystickButtons[i] = false;
}

// --------------------------------------------------------------------------------------
// Destruktor
// --------------------------------------------------------------------------------------

DirectJoystickClass::~DirectJoystickClass(void)
{
#if defined(OPENDINGUX)
  Shake_Quit();
  Protokoll.WriteText( false, "Shake: Shutdown completed !\n" );
#endif
}

// --------------------------------------------------------------------------------------
// ForceFeedback Effekt "nr" starten
// --------------------------------------------------------------------------------------

void DirectJoystickClass::ForceFeedbackEffect(int nr)
{
    if (UseForceFeedback == false ||
            CanForceFeedback == false)
        return;

#if defined(PLATFORM_DIRECTX) || defined(OPENDINGUX)
    switch (nr)
    {
    case 0:
        pFFE_SmallVib->Start(1,0);
        break;

    case 1:
        pFFE_BigVib->Start(1,0);
        break;

    case 2:
        pFFE_MaxVib->Start(1,0);
        break;

    case 3:
        pFFE_Blitz->Start(1,0);
        break;
    }
#elif defined(PLATFORM_SDL)
#endif
}

// --------------------------------------------------------------------------------------
// ForceFeedback Effekt "nr" anhalten
// --------------------------------------------------------------------------------------

void DirectJoystickClass::StopForceFeedbackEffect(int nr)
{
    if (UseForceFeedback == false ||
            CanForceFeedback == false)
        return;

#if defined(PLATFORM_DIRECTX) || defined(OPENDINGUX)
    switch (nr)
    {
    case 0:
        pFFE_SmallVib->Stop();
        break;

    case 1:
        pFFE_BigVib->Stop();
        break;

    case 2:
        pFFE_MaxVib->Stop();
        break;

    case 3:
        pFFE_Blitz->Stop();
        break;
    }
#elif defined(PLATFORM_SDL)
#endif
}

// --------------------------------------------------------------------------------------
// Joystick initialisieren
// --------------------------------------------------------------------------------------

#if defined(PLATFORM_DIRECTX)
bool DirectJoystickClass::Init(HWND hwnd, LPDIRECTINPUT8 lpDI)
{
    HRESULT             dirval;           // R�ckgabewert
    DIPROPRANGE         diprg;            // Joystick Eigenschaften

    // Joystick f�r enumerierte GUID erstellen
    dirval = lpDI->CreateDevice(guidJoystickDevice,&lpDIJoystick, NULL);
    if (dirval != DI_OK)
    {
        Protokoll.WriteText( false, "\n-> Joystick : CreateDevice error!\n" );
        return false;
    }
    Protokoll.WriteText( false, "Joystick : CreateDevice successful!\n" );

    // Datenformat f�r Joystick festlegen
    dirval = lpDIJoystick->SetDataFormat(&c_dfDIJoystick2);
    if (dirval != DI_OK)
    {
        Protokoll.WriteText( false, "\n-> Joystick : SetDataFormat error!\n" );
        return false;
    }
    Protokoll.WriteText( false, "Joystick : SetDataFormat successful!\n" );

    // Zusammenarbeit mit Windows regeln
    dirval = lpDIJoystick->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
    if (dirval != DI_OK)
    {
        Protokoll.WriteText( false, "\n-> Joystick : SetCooperativeLevel error!\n" );
        return false;
    }
    Protokoll.WriteText( false, "Joystick : SetCooperativeLevel	successful!\n" );

    // Joystick Objekt Eigenschaften festlegen
    diprg.diph.dwSize = sizeof(diprg);
    diprg.diph.dwHeaderSize = sizeof(diprg.diph);
    diprg.diph.dwObj = DIJOFS_X;                    // x-Achse
    diprg.diph.dwHow = DIPH_BYOFFSET;
    diprg.lMin = -1000;                             // Wertebereich
    diprg.lMax = +1000;                             // von, bis

    dirval = lpDIJoystick->SetProperty(DIPROP_RANGE, &diprg.diph);
    if (dirval != DI_OK)
    {
        Protokoll.WriteText( false, "\n-> Joystick : SetPropertyX error!\n" );
        return false;
    }
    Protokoll.WriteText( false, "Joystick : SetPropertyX sucessfull!\n" );

    diprg.diph.dwSize = sizeof(diprg);
    diprg.diph.dwHeaderSize = sizeof(diprg.diph);
    diprg.diph.dwObj = DIJOFS_Y;                    // y-Achse
    diprg.diph.dwHow = DIPH_BYOFFSET;
    diprg.lMin = -1000;                             // Wertebereich
    diprg.lMax = +1000;                             // von, bis

    dirval = lpDIJoystick->SetProperty(DIPROP_RANGE, &diprg.diph);
    if (dirval != DI_OK)
    {
        Protokoll.WriteText( false, "\n-> Joystick : SetPropertyY error!\n" );
        return false;
    }
    Protokoll.WriteText( false, "Joystick : SetPropertyY successful!\n" );

    // Joystick Objekt aktivieren
    if (lpDIJoystick)
    {
        dirval = lpDIJoystick->Acquire();
        if (dirval != DI_OK)
        {
            Protokoll.WriteText( false, "\n-> Joystick : Acquire error!\n" );
            return false;
        }
        Protokoll.WriteText( false, "Joystick : Acquire successful!\n" );
    }
    else
        return false;

    Active = true;

    //DKS - new code to set the number of buttons this joystick supports
    //      NOTE: this is completely untested, not even tested to compile yet:
    NumButtons = lpDIJoystick->Caps.NumberButtons;

    return true;
}
#elif defined(PLATFORM_SDL)

#if SDL_VERSION_ATLEAST(2,0,0)
    #define SDLJOYINDEX lpDIJoystick
#else
    #define SDLJOYINDEX joy
#endif

bool DirectJoystickClass::Init(int joy)
{

#if defined(OPENDINGUX)
    if (joy == -1)
    {
        Active = true;
        CanForceFeedback = false;       // Forced false for now until implemented
        NumButtons = 12;
        
        Protokoll.WriteText( false, "Joypad %d: Acquire successful!\nButtons: %d Name: %s\n", joy, NumButtons, JoystickName );
    } else {
#endif
    lpDIJoystick = SDL_JoystickOpen(joy);

    if (lpDIJoystick == NULL)
    {
        Protokoll.WriteText( false, "\n-> Joystick : Acquire error!\n" );
        return false;
    }

    SDL_JoystickEventState( SDL_IGNORE ); /* the joy events will be updated manually */

    Active = true;
    NumButtons = SDL_JoystickNumButtons(lpDIJoystick);
    CanForceFeedback = false;       // Forced false for now until implemented
    
#if defined(OPENDINGUX)
    // New OpenDingux joystick driver do not map buttons as joystick buttons 
    // so we simulate Joystick buttons
    NumButtons = 12;
#endif

    // Get joystick's name
    if (strlen(SDL_JoystickName(SDLJOYINDEX)) < sizeof(JoystickName)) {
        strcpy_s(JoystickName, SDL_JoystickName(SDLJOYINDEX));
    } else {
        strcpy_s(JoystickName, sizeof(JoystickName)-1, SDL_JoystickName(SDLJOYINDEX)); // Truncate to fit 
        JoystickName[sizeof(JoystickName)-1] = '\0';                                   // and null-terminate 
    }

    Protokoll.WriteText( false, "Joystick %d: Acquire successful!\nButtons: %d Name: %s\n", joy, NumButtons, JoystickName );
#if defined(OPENDINGUX)
    }
    
    if (Shake_Init() == SHAKE_OK && Shake_NumOfDevices() > 0) {
      CanForceFeedback = true;
      ForceFeedbackDevice = Shake_Open(0);
      Protokoll.WriteText( false, "Shake: Force feedbak detected !\n" );
    }
#endif

    return true;
}
#endif

// --------------------------------------------------------------------------------------
// Joystick updaten
// --------------------------------------------------------------------------------------

#if defined(PLATFORM_DIRECTX)
bool DirectJoystickClass::Update(void)
{
    HRESULT		hresult;
    DIJOYSTATE2	js;

    hresult = lpDIJoystick->Poll();
    hresult = lpDIJoystick->GetDeviceState (sizeof (DIJOYSTATE2), &js);

    // Joystick auf einmal weg?
    //
    if (hresult == DIERR_INPUTLOST)
    {
        // Versuchen, erneut zu holen
        //
        lpDIJoystick->Acquire();
        hresult = lpDIJoystick->GetDeviceState (sizeof (DIJOYSTATE2), &js);

        // immernoch wech? Dann gibts halt kein Joystick mehr
        //
        if (hresult != DI_OK)
        {
            Protokoll.WriteText( false, "\n-> Joystick : Re-Acquire Fehler !\n" );
            return false;
        }

        // Ansonsten State holen
        //
        else
        {
            hresult = lpDIJoystick->GetDeviceState (sizeof (DIJOYSTATE2), &js);

            // Wieder fehler? Dann wars das auch mit dem Joystick
            //
            if (hresult == DIERR_INPUTLOST)
            {
                Protokoll.WriteText( false, "\n-> Joystick : Re-Acquire Fehler !\n" );
                return false;
            }
        }
    }

    for (int i = 0; i < NumButtons; i++)
    {
        if (js.rgbButtons [i] & 0x80)
            JoystickButtons[i] = true;
        else
            JoystickButtons[i] = false;
    }

    JoystickX = js.lX;
    JoystickY = js.lY;
    JoystickPOV = js.rgdwPOV[0];

    return true;
}
#elif defined(PLATFORM_SDL)
bool DirectJoystickClass::Update(void)
{
    if (lpDIJoystick != NULL)
    {
        SDL_JoystickUpdate();

        for (int i = 0; i < NumButtons; i++)
        {
            if (SDL_JoystickGetButton( lpDIJoystick, i ) >= 1)
                JoystickButtons[i] = true;
            else
                JoystickButtons[i] = false;
        }

        if (SDL_JoystickNumAxes(lpDIJoystick) > 1)
        {
            //DKS - Map range of motion from SDL's (+/- 32768) to the original game's (+/- 1000)
            JoystickX = (int)((float)SDL_JoystickGetAxis( lpDIJoystick, 0 ) * (1000.0f / 32767.0f));
            JoystickY = (int)((float)SDL_JoystickGetAxis( lpDIJoystick, 1 ) * (1000.0f / 32767.0f));
        }
#if defined(OPENDINGUX)
        if (SDL_JoystickNumAxes(lpDIJoystick) == 4)
        {
            JoystickX2 = (int)((float)SDL_JoystickGetAxis( lpDIJoystick, 2 ) * (1000.0f / 32767.0f));
            JoystickY2 = (int)((float)SDL_JoystickGetAxis( lpDIJoystick, 3 ) * (1000.0f / 32767.0f));
        }
#endif

        if (SDL_JoystickNumHats(lpDIJoystick) > 0)
        {
            // DKS: Note - DirectX HAT values are -1 for centered, otherwise are in hundredths
            //      of a degree, from starting at 0 (UP, north) so right is 9000, down is 18000,
            //      left is 27000.
            uint8_t hat_state = SDL_JoystickGetHat( lpDIJoystick, 0 );
            switch (hat_state) {
                case SDL_HAT_UP:
                    JoystickPOV = 0;
                    break;
                case SDL_HAT_RIGHTUP:
                    JoystickPOV = 4500;
                    break;
                case SDL_HAT_RIGHT:
                    JoystickPOV = 9000;
                    break;
                case SDL_HAT_RIGHTDOWN:
                    JoystickPOV = 13500;
                    break;
                case SDL_HAT_DOWN:
                    JoystickPOV = 18000;
                    break;
                case SDL_HAT_LEFTDOWN:
                    JoystickPOV = 22500;
                    break;
                case SDL_HAT_LEFT:
                    JoystickPOV = 27000;
                    break;
                case SDL_HAT_LEFTUP:
                    JoystickPOV = 31500;
                    break;
                case SDL_HAT_CENTERED:
                default:
                    JoystickPOV = -1;
                    break;
            }
        }
    }
    
#if defined(OPENDINGUX)
    // Map Buttons to Joy Buttons
    JoystickButtons[0] = (KeyDown(DIK_LALT)) ? true : false;
    JoystickButtons[1] = (KeyDown(DIK_LCONTROL)) ? true : false;
    JoystickButtons[2] = (KeyDown(DIK_LSHIFT)) ? true : false;
    JoystickButtons[3] = (KeyDown(DIK_SPACE)) ? true : false;
    JoystickButtons[4] = (KeyDown(DIK_ESCAPE)) ? true : false;
    JoystickButtons[5] = (KeyDown(DIK_RETURN)) ? true : false;
    JoystickButtons[6] = (KeyDown(DIK_TAB)) ? true : false;
    JoystickButtons[7] = (KeyDown(DIK_BACK)) ? true : false;
    JoystickButtons[8] = (KeyDown(DIK_PRIOR)) ? true : false;
    JoystickButtons[9] = (KeyDown(DIK_NEXT)) ? true : false;
    JoystickButtons[10] = (KeyDown(DIK_DIVIDE)) ? true : false;
    JoystickButtons[11] = (KeyDown(DIK_DECIMAL)) ? true : false;

    //Map D-PAD to Hat
    if (KeyDown(DIK_UP) && KeyDown(DIK_LEFT))
      JoystickPOV = 31500;
    else if (KeyDown(DIK_UP) && KeyDown(DIK_RIGHT))
      JoystickPOV = 4500;
    else if (KeyDown(DIK_DOWN) && KeyDown(DIK_RIGHT))
      JoystickPOV = 13500;
    else if (KeyDown(DIK_DOWN) && KeyDown(DIK_LEFT))
      JoystickPOV = 22500;
    else if (KeyDown(DIK_UP))
      JoystickPOV = 0;
    else if (KeyDown(DIK_DOWN))
      JoystickPOV = 18000;
    else if (KeyDown(DIK_LEFT))
      JoystickPOV = 27000;
    else if (KeyDown(DIK_RIGHT))
      JoystickPOV = 9000;
    else
      JoystickPOV = -1;
#endif

    return true;
}
#endif

//DKS-Added these three for better joystick support, esp in menus
bool DirectJoystickClass::ButtonEnterPressed(void)
{
#if defined(OPENDINGUX)
    return JoystickButtons[1];   // Default is button 1
#else
    return JoystickButtons[0];   // Default is button 0
#endif
}

bool DirectJoystickClass::ButtonEscapePressed(void)
{
#if defined(OPENDINGUX)
    return JoystickButtons[0];   // Default is button 0
#else
    return JoystickButtons[1];   // Default is button 1
#endif
}

bool DirectJoystickClass::ButtonDeletePressed(void)
{
    return JoystickButtons[4];   // Default is button 4
}
