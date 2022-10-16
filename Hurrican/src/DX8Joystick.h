// Datei : DX8Joystick.h

// --------------------------------------------------------------------------------------
//
// Joystick Klasse
//
// (c) 2002 J�rg M. Winterstein
//
// --------------------------------------------------------------------------------------

#ifndef __DX8Joystick_h__
#define __DX8Joystick_h__

// --------------------------------------------------------------------------------------
// Defines
// --------------------------------------------------------------------------------------

#define MAX_JOYSTICKBUTTONS		128

// --------------------------------------------------------------------------------------
// Include Dateien
// --------------------------------------------------------------------------------------

#if defined(PLATFORM_DIRECTX)
#include <dinput.h>
#endif
#if defined(PLATFORM_SDL)
#include "SDL_port.h"
#endif
#if defined(OPENDINGUX)
#include <shake.h>

class IODFFEffect {
public:
    IODFFEffect(Shake_Device *ForceFeedbackDevice)
    {
      device = ForceFeedbackDevice;
    }
    void Start(uint16_t iterations, uint16_t flags) { Shake_Play(device, id); };
    virtual void Stop() { Shake_Stop(device, id); };
    virtual ~IODFFEffect() { Shake_EraseEffect(device, id); };
protected:
    Shake_Effect effect;
    int id;
    Shake_Device *device;
};

class OD_SmallVib : public IODFFEffect {
public:
  OD_SmallVib(Shake_Device *ForceFeedbackDevice) : IODFFEffect(ForceFeedbackDevice)
  {
    Shake_SimpleRumble(&effect, 0.6, 0.4, 0.25);
    id = Shake_UploadEffect(device, &effect);
  }
};

class OD_BigVib : public IODFFEffect {
public:
  OD_BigVib(Shake_Device *ForceFeedbackDevice) : IODFFEffect(ForceFeedbackDevice)
  {
    Shake_SimpleRumble(&effect, 0.9, 0.4, 0.25);
    id = Shake_UploadEffect(device, &effect);
  }
};

class OD_MaxVib : public IODFFEffect {
public:
  OD_MaxVib(Shake_Device *ForceFeedbackDevice) : IODFFEffect(ForceFeedbackDevice)
  {
    Shake_SimplePeriodic(&effect, SHAKE_PERIODIC_SINE, 1.0, 0.0, 0.15, 0.85);
    id = Shake_UploadEffect(device, &effect);
  }
};

class OD_Blitz : public IODFFEffect {
public:
  OD_Blitz(Shake_Device *ForceFeedbackDevice) :  IODFFEffect(ForceFeedbackDevice)
  {
    Shake_SimplePeriodic(&effect, SHAKE_PERIODIC_SINE, 0.5, 0.5, 60.0, 0.0);
    id = Shake_UploadEffect(device, &effect);
  }
};
#endif

// --------------------------------------------------------------------------------------
// Klassendeklaration
// --------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------
// Joystick Klasse
// --------------------------------------------------------------------------------------

class DirectJoystickClass
{
public:
    LPDIRECTINPUTDEVICE8	lpDIJoystick;			// Joystick Device Interface
#if defined(PLATFORM_DIRECTX)
    GUID					guidJoystickDevice;		// GUID des Joystick Devices
    LPDIRECTINPUTEFFECT		pFFE_SmallVib;			// Kurzes, schwaches Vibrieren
    LPDIRECTINPUTEFFECT		pFFE_BigVib;			// Kurzes, starkes Vibrieren
    LPDIRECTINPUTEFFECT		pFFE_MaxVib;			// Kurzes, heftiges Vibrieren
    LPDIRECTINPUTEFFECT		pFFE_Blitz;				// Blitz Effekt
#endif

    bool  CanForceFeedback;
    bool  Active;
    int   JoystickX;								// Joystick x-Koordinaten
    int   JoystickY;								// Joystick y-Koordinaten
    int   JoystickX2;								// Joystick x-Koordinaten 2. analog Stick
    int   JoystickY2;								// Joystick y-Koordinaten 2. analog Stick
    int   JoystickPOV;								// POV (f�r coolie hat)
    int   JoystickMode;								// Joypad oder Stickmode
    bool  JoystickButtons[MAX_JOYSTICKBUTTONS];		// Feuerknopf gedr�ckt?
    char  JoystickName[70];							// Joystick Produktname
    int   NumButtons;                               // How many buttons joystick supports
#if defined(OPENDINGUX)
    Shake_Device *ForceFeedbackDevice;
    IODFFEffect *pFFE_SmallVib;
    IODFFEffect *pFFE_BigVib;
    IODFFEffect *pFFE_MaxVib;
    IODFFEffect *pFFE_Blitz;
#endif

    DirectJoystickClass(void);
    ~DirectJoystickClass(void);

    void ForceFeedbackEffect	(int nr);
    void StopForceFeedbackEffect(int nr);

#if defined(PLATFORM_DIRECTX)
    bool Init(HWND hwnd, LPDIRECTINPUT8 lpDI);
#elif defined(PLATFORM_SDL)
    bool Init(int joy);
#endif

    bool Update(void);

    //DKS-Added these three for better joystick support, esp in menus
    // Returns true if button(s) serving as Enter key  are pressed (For menus)
    bool ButtonEnterPressed(void);

    // Returns true if button(s) serving as Escape key are pressed (For menus)
    bool ButtonEscapePressed(void);

    // Returns true if button(s) serving as Delete key are pressed (For menus, esp. button mapping menu)
    bool ButtonDeletePressed(void);
};
#endif
