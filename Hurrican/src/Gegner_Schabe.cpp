// --------------------------------------------------------------------------------------
// Die Schabe
//
// Krabbe an allen W?nden rum
// --------------------------------------------------------------------------------------

#include "stdafx.h"
#include "Gegner_Schabe.h"

#define SPEED 5.0f

// --------------------------------------------------------------------------------------
// Konstruktor
// --------------------------------------------------------------------------------------

GegnerSchabe::GegnerSchabe(int Wert1, int Wert2, bool Light)
{
    Handlung	= GEGNER_LAUFEN;
    Energy		= 10;
    Value1		= Wert1;
    Value2		= Wert2;
    ChangeLight	= Light;
    Destroyable	= true;
    AnimSpeed   = 0.5f;
    AnimEnde    = 10;
    xSpeed      = SPEED;
    mirror		= false;
    ySpeed		= 0.0f;
    winkel	    = 0.0f;
    TestBlock	= false;
    OwnDraw		= true;
    ShotDelay   = 5.0f + rand()%5;

    if (Value1 != 0)
    {
        xSpeed = -SPEED;
        mirror = true;
    }
}

// --------------------------------------------------------------------------------------
// "Bewegungs KI"
// --------------------------------------------------------------------------------------

void GegnerSchabe::DoKI(void)
{
    // Schiessen
    if (PlayerAbstand()<600)
        ShotDelay -= 1.0f SYNC;

    if (ShotDelay < 0.0f)
    {
        ShotDelay = 8.0f + (4.0f - Skill) * 2;

        SoundManager.PlayWave (50, 128, 10000 + rand()%4000, SOUND_CANON);
        Projectiles.PushProjectile(xPos + 5.0f, yPos + 5.0f, SUCHSCHUSS);
    }

    // Animieren
    //
    SimpleAnimation();

    // An der Wand krabbeln
    int xl, yl;

    xl = (int)(xPos+10) / TILESIZE_X;
    yl = (int)(yPos+10) / TILESIZE_X;

    // Schabe l?uft nach links
    if (xSpeed < 0.0f)
    {
        // ....
        // .o..
        // ..xx
        // ..xx

        if (!(TileEngine.TileAt(xl-0, yl+1).Block & BLOCKWERT_WAND) &&
                !(TileEngine.TileAt(xl-1, yl-1).Block & BLOCKWERT_WAND) &&
                !(TileEngine.TileAt(xl+1, yl-1).Block & BLOCKWERT_WAND) &&
                !(TileEngine.TileAt(xl-1, yl+1).Block & BLOCKWERT_WAND) &&
                (TileEngine.TileAt(xl+1, yl+1).Block & BLOCKWERT_WAND))
        {
            xSpeed = 0.0f;
            ySpeed = SPEED;

            xPos = (float)xl*20.0f;
            yPos = (float)yl*20.0f;
        }

        // ..xx
        // ..xx
        // .o..
        // ....


        if (!(TileEngine.TileAt(xl-0, yl-1).Block & BLOCKWERT_WAND) &&
                !(TileEngine.TileAt(xl-1, yl-1).Block & BLOCKWERT_WAND) &&
                (TileEngine.TileAt(xl+1, yl-1).Block & BLOCKWERT_WAND) &&
                !(TileEngine.TileAt(xl-1, yl+1).Block & BLOCKWERT_WAND) &&
                !(TileEngine.TileAt(xl+1, yl+1).Block & BLOCKWERT_WAND))
        {
            xSpeed =  0.0f;
            ySpeed = -SPEED;

            xPos = (float)xl*20.0f;
            yPos = (float)yl*20.0f;
        }
    }

    // Schabe l?uft nach rechts
    if (xSpeed > 0.0f)
    {

        // ....
        // ..o.
        // xx..
        // xx..

        if (!(TileEngine.TileAt(xl+0, yl+1).Block & BLOCKWERT_WAND) &&
                !(TileEngine.TileAt(xl-1, yl-1).Block & BLOCKWERT_WAND) &&
                !(TileEngine.TileAt(xl+1, yl-1).Block & BLOCKWERT_WAND) &&
                (TileEngine.TileAt(xl-1, yl+1).Block & BLOCKWERT_WAND) &&
                !(TileEngine.TileAt(xl+1, yl+1).Block & BLOCKWERT_WAND))
        {
            xSpeed = 0.0f;
            ySpeed = SPEED;

            xPos = (float)xl*20.0f;
            yPos = (float)yl*20.0f;
        }

        // xx..
        // xx..
        // ..o.
        // ....


        if (!(TileEngine.TileAt(xl+0, yl-1).Block & BLOCKWERT_WAND) &&
                (TileEngine.TileAt(xl-1, yl-1).Block & BLOCKWERT_WAND) &&
                !(TileEngine.TileAt(xl+1, yl-1).Block & BLOCKWERT_WAND) &&
                !(TileEngine.TileAt(xl-1, yl+1).Block & BLOCKWERT_WAND) &&
                !(TileEngine.TileAt(xl+1, yl+1).Block & BLOCKWERT_WAND))
        {
            xSpeed = 0.0f;
            ySpeed = -SPEED;

            xPos = (float)xl*20.0f;
            yPos = (float)yl*20.0f;
        }
    }

    // Schabe l?uft nach oben
    if (ySpeed < 0.0f)
    {
        // ....
        // .o..
        // ..xx
        // ..xx

        if (!(TileEngine.TileAt(xl+1, yl+0).Block & BLOCKWERT_WAND) &&
                !(TileEngine.TileAt(xl-1, yl-1).Block & BLOCKWERT_WAND) &&
                (TileEngine.TileAt(xl+1, yl+1).Block & BLOCKWERT_WAND) &&
                !(TileEngine.TileAt(xl-1, yl+1).Block & BLOCKWERT_WAND) &&
                !(TileEngine.TileAt(xl+1, yl-1).Block & BLOCKWERT_WAND))
        {
            xSpeed = SPEED;
            ySpeed = 0.0f;

            xPos = (float)xl*20.0f;
            yPos = (float)yl*20.0f;
        }

        // ....
        // ..o.
        // xx..
        // xx..

        if (!(TileEngine.TileAt(xl-1, yl+0).Block & BLOCKWERT_WAND) &&
                !(TileEngine.TileAt(xl-1, yl-1).Block & BLOCKWERT_WAND) &&
                !(TileEngine.TileAt(xl+1, yl-1).Block & BLOCKWERT_WAND) &&
                (TileEngine.TileAt(xl-1, yl+1).Block & BLOCKWERT_WAND) &&
                !(TileEngine.TileAt(xl+1, yl+1).Block & BLOCKWERT_WAND))
        {
            xSpeed = -SPEED;
            ySpeed =  0.0f;

            xPos = (float)xl*20.0f;
            yPos = (float)yl*20.0f;
        }
    }

    // Schabe l?uft nach unten
    if (ySpeed > 0.0f)
    {
        // ..xx
        // ..xx
        // .o..
        // ....

        if (!(TileEngine.TileAt(xl+1, yl+0).Block & BLOCKWERT_WAND) &&
                !(TileEngine.TileAt(xl-1, yl-1).Block & BLOCKWERT_WAND) &&
                (TileEngine.TileAt(xl+1, yl-1).Block & BLOCKWERT_WAND) &&
                !(TileEngine.TileAt(xl-1, yl+1).Block & BLOCKWERT_WAND) &&
                !(TileEngine.TileAt(xl+1, yl+1).Block & BLOCKWERT_WAND))
        {
            xSpeed = SPEED;
            ySpeed = 0.0f;

            xPos = (float)xl*20.0f;
            yPos = (float)yl*20.0f;
        }

        // xx..
        // xx..
        // ..o.
        // ....

        if (!(TileEngine.TileAt(xl-1, yl+0).Block & BLOCKWERT_WAND) &&
                (TileEngine.TileAt(xl-1, yl-1).Block & BLOCKWERT_WAND) &&
                !(TileEngine.TileAt(xl+1, yl-1).Block & BLOCKWERT_WAND) &&
                !(TileEngine.TileAt(xl-1, yl+1).Block & BLOCKWERT_WAND) &&
                !(TileEngine.TileAt(xl+1, yl+1).Block & BLOCKWERT_WAND))
        {
            xSpeed = -SPEED;
            ySpeed =  0.0f;

            xPos = (float)xl*20.0f;
            yPos = (float)yl*20.0f;
        }
    }

//
//----- An die Wand gesto?en?
//
    if ((xSpeed < 0.0f &&
            TileEngine.TileAt(xl-1, yl-0).Block & BLOCKWERT_WAND) ||

            (xSpeed > 0.0f &&
             TileEngine.TileAt(xl+1, yl-0).Block & BLOCKWERT_WAND))
    {
        xSpeed = 0.0f;

        xPos = (float)xl*20.0f;
        yPos = (float)yl*20.0f;

        if (TileEngine.TileAt(xl-0, yl-1).Block & BLOCKWERT_WAND)
            ySpeed =  SPEED;
        else
            ySpeed = -SPEED;
    }

//
//----- An die Decke/Boden gesto?en?
//
    else if ((ySpeed > 0.0f &&
              TileEngine.TileAt(xl+0, yl+1).Block & BLOCKWERT_WAND) ||

             (ySpeed < 0.0f &&
              TileEngine.TileAt(xl+0, yl-1).Block & BLOCKWERT_WAND))
    {
        ySpeed = 0.0f;

        xPos = (float)xl*20.0f;
        yPos = (float)yl*20.0f;

        if (TileEngine.TileAt(xl-1, yl-0).Block & BLOCKWERT_WAND)
            xSpeed =  SPEED;
        else
            xSpeed = -SPEED;
    }

//
//----- Drehrichtung rausfinden
//
    if (ySpeed < 0.0f)
    {
        winkel = 90.0f;

        if (TileEngine.TileAt(xl-1, yl-1).Block & BLOCKWERT_WAND ||
                TileEngine.TileAt(xl-1, yl-0).Block & BLOCKWERT_WAND ||
                TileEngine.TileAt(xl-1, yl+1).Block & BLOCKWERT_WAND)
            mirror = true;
        else
            mirror = false;

        // umdrehen?
        if (TileEngine.TileAt(xl-0, yl-1).Block & BLOCKWERT_WENDEPUNKT)
            ySpeed = SPEED;
    }

    else if (ySpeed > 0.0f)
    {
        winkel = 270.0f;

        if (TileEngine.TileAt(xl-1, yl-1).Block & BLOCKWERT_WAND ||
                TileEngine.TileAt(xl-1, yl-0).Block & BLOCKWERT_WAND ||
                TileEngine.TileAt(xl-1, yl+1).Block & BLOCKWERT_WAND)
            mirror = false;
        else
            mirror = true;

        // umdrehen?
        if (TileEngine.TileAt(xl-0, yl+1).Block & BLOCKWERT_WENDEPUNKT)
            ySpeed = -SPEED;
    }

    if (xSpeed > 0.0f)
    {
        if (TileEngine.TileAt(xl-1, yl+1).Block & BLOCKWERT_WAND ||
                TileEngine.TileAt(xl+0, yl+1).Block & BLOCKWERT_WAND ||
                TileEngine.TileAt(xl+1, yl+1).Block & BLOCKWERT_WAND)
        {
            winkel = 0.0f;
            mirror = false;
        }
        else
        {
            winkel = 180.0f;
            mirror = true;
        }

        // umdrehen?
        if (TileEngine.TileAt(xl+1, yl+0).Block & BLOCKWERT_WENDEPUNKT)
            xSpeed = -SPEED;
    }
    else if (xSpeed < 0.0f)
    {
        if (TileEngine.TileAt(xl-1, yl+1).Block & BLOCKWERT_WAND ||
                TileEngine.TileAt(xl+0, yl+1).Block & BLOCKWERT_WAND ||
                TileEngine.TileAt(xl+1, yl+1).Block & BLOCKWERT_WAND)
        {
            winkel = 0.0f;
            mirror = true;
        }
        else
        {
            winkel = 180.0f;
            mirror = false;
        }

        // umdrehen?
        if (TileEngine.TileAt(xl-1, yl+0).Block & BLOCKWERT_WENDEPUNKT)
            xSpeed = SPEED;
    }

    TestDamagePlayers(2.0f SYNC);
}

// --------------------------------------------------------------------------------------
// Eigene Draw Funktion
// --------------------------------------------------------------------------------------

void GegnerSchabe::DoDraw(void)
{
    pGegnerGrafix[GegnerArt]->RenderSpriteRotated ((float)(xPos-TileEngine.XOffset),
            (float)(yPos-TileEngine.YOffset),
            winkel, AnimPhase, 0xFFFFFFFF, mirror);
}

// --------------------------------------------------------------------------------------
// Schabe explodiert
// --------------------------------------------------------------------------------------

void GegnerSchabe::GegnerExplode(void)
{
    // Explosion
    for (int i=0; i<2; i++)
    {
        PartikelSystem.PushPartikel(float(xPos - 20) + rand()%10,
                                      float(yPos - 20) + rand()%10, EXPLOSION_MEDIUM2);

        PartikelSystem.PushPartikel(float(xPos+5) + rand()%10,
                                      float(yPos+5) + rand()%10, FUNKE);

        PartikelSystem.PushPartikel(float(xPos+5) + rand()%10,
                                      float(yPos+5) + rand()%10, SPIDERSPLITTER);
    }

    SoundManager.PlayWave(100, 128, -rand()%2000+13000, SOUND_EXPLOSION1);	// Sound ausgeben

    Player[0].Score += 10;
}
