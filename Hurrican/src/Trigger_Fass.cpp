// --------------------------------------------------------------------------------------
// Das Fass
//
// Steht rum und wartet darauf, abgeballert zu werden, worauf es
// explodiert und feurige Splitter veteilt, die die Gegner verletzen k?nnen
// --------------------------------------------------------------------------------------

#include "stdafx.h"
#include "Trigger_Fass.h"

// --------------------------------------------------------------------------------------
// Konstruktor
// --------------------------------------------------------------------------------------

GegnerFass::GegnerFass(int Wert1, int Wert2, bool Light)
{
    Handlung		= GEGNER_STEHEN;
    BlickRichtung	= LINKS;
    Energy			= 10;
    Value1			= Wert1;
    Value2			= Wert2;
    AnimPhase		= 0;
    ChangeLight		= Light;
    Destroyable		= true;
}

// --------------------------------------------------------------------------------------
// "Bewegungs KI"
// --------------------------------------------------------------------------------------

void GegnerFass::DoKI(void)
{

    // Feststellen ob der Hurri auf dem Fass steht
    PlattformTest(GegnerRect[GegnerArt]);

    // Fass bewegen
    for (int p = 0; p < NUMPLAYERS; p++)
        if (SpriteCollision(Player[p].xpos,
                            Player[p].ypos,
                            Player[p].CollideRect,
                            xPos, yPos, GegnerRect[GegnerArt]) == true)
        {
            xPos += Player[p].xspeed / 2 SYNC;
            Player[p].xpos -= Player[p].xspeed / 2 SYNC;

        }

// Je nach Handlung richtig verhalten
    switch (Handlung)
    {

    // Warten auf den Hurri =)
    case GEGNER_STEHEN:
    {
        if (!(blocku & BLOCKWERT_WAND) &&
                !(blocku & BLOCKWERT_PLATTFORM))
        {
            ySpeed = 1.0f;
            yAcc   = 8.0f;

            Handlung = GEGNER_FALLEN;
        }
    }
    break;

    // Fass f?llt nach unten
    case GEGNER_FALLEN:
    {
        // Auf den Boden gecrasht ? Dann explodieren lassen
        if (blocku & BLOCKWERT_WAND ||
                blocku & BLOCKWERT_PLATTFORM)

            Energy = 0.0f;

    }
    break;

    default :
        break;
    } // switch
}

// --------------------------------------------------------------------------------------
// Fass explodiert
// --------------------------------------------------------------------------------------

void GegnerFass::GegnerExplode(void)
{
    SoundManager.PlayWave(25, 128, 11025, SOUND_EXPLOSION1);

    for (int i=0; i<5; i++)
        PartikelSystem.PushPartikel(xPos-50+rand()%48, yPos-50+rand()%56, EXPLOSION_BIG);

    for (int i=0; i<8; i++)
        PartikelSystem.PushPartikel(xPos-30+rand()%48, yPos-30+rand()%56, EXPLOSION_MEDIUM);

    for(int i=0; i<20; i++)
        PartikelSystem.PushPartikel(xPos-2+rand()%48, yPos-2+rand()%12, FUNKE);

    // Flammen-Splitter auswerfen
    for (int i=0; i<16; i++)
        Projectiles.PushProjectile(xPos-24+rand()%48, yPos+rand()%20, FLAMME);
}
