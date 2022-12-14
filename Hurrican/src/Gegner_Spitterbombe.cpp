// --------------------------------------------------------------------------------------
// Die Spitter Bombe
//
// Bombe, aus der der Spitter rauskommt.
// geile Erkl?rung, oder ? ;)
// --------------------------------------------------------------------------------------

#include "stdafx.h"
#include "Gegner_Spitterbombe.h"

// --------------------------------------------------------------------------------------
// Konstruktor
// --------------------------------------------------------------------------------------

GegnerSpitterbombe::GegnerSpitterbombe(int Wert1, int Wert2, bool Light)
{
    Handlung		= GEGNER_LAUFEN;
    Energy			= 20;
    AnimEnde		= 6;
    AnimSpeed		= 0.5;
    ChangeLight		= Light;
    Destroyable		= true;
    ySpeed			= 15.0f;
}

// --------------------------------------------------------------------------------------
// "Bewegungs KI"
// --------------------------------------------------------------------------------------

void GegnerSpitterbombe::DoKI(void)
{
    // Animieren
    //
    AnimCount += SpeedFaktor;			// Animationscounter weiterz?hlen
    if (AnimCount > AnimSpeed)			// Grenze ?berschritten ?
    {
        AnimCount = 0;					// Dann wieder auf Null setzen
        AnimPhase++;					// Und n?chste Animationsphase

        PartikelSystem.PushPartikel (xPos - 5 + rand()%10, yPos, SMOKE);

        if (AnimPhase >= AnimEnde)		// Animation von zu Ende	?
            AnimPhase = AnimStart;
    }

    // Testen, ob der Spieler die Bombe ber?hrt hat
    //
    TestDamagePlayers(1.0f SYNC);

    // Auf den Boden aufgekommen ? Dann Spitter rauslassen
    if ((blocku & BLOCKWERT_WAND) ||
            (blocku & BLOCKWERT_PLATTFORM))
    {
        Energy = 0.0f;
        Gegner.PushGegner (xPos + 4, yPos + 20, SPITTER, 0, 0, false);
    }
}


// --------------------------------------------------------------------------------------
// Bombe explodiert
// --------------------------------------------------------------------------------------

void GegnerSpitterbombe::GegnerExplode(void)
{
    SoundManager.PlayWave (100, 128, 8000 + rand()%4000, SOUND_EXPLOSION1);

    for (int i = 0; i < 5; i++)
        PartikelSystem.PushPartikel(xPos - 25 + rand()%20, yPos - 30 + rand()%50, EXPLOSION_MEDIUM2);

    for (int i = 0; i < 5; i++)
        PartikelSystem.PushPartikel(xPos + rand()%20, yPos + rand()%40, SPIDERSPLITTER);

    // Nicht am Boden explodiert ? Dann wurde er abgeschossen
    if (!(blocku & BLOCKWERT_WAND) &&
            !(blocku & BLOCKWERT_PLATTFORM))
    {
        Projectiles.PushProjectile (xPos + 10, yPos + 30, SPITTERBOMBESHOTLO);
        Projectiles.PushProjectile (xPos + 10, yPos + 30, SPITTERBOMBESHOTLM);
        Projectiles.PushProjectile (xPos + 10, yPos + 30, SPITTERBOMBESHOTLU);
        Projectiles.PushProjectile (xPos + 10, yPos + 30, SPITTERBOMBESHOTRO);
        Projectiles.PushProjectile (xPos + 10, yPos + 30, SPITTERBOMBESHOTRM);
        Projectiles.PushProjectile (xPos + 10, yPos + 30, SPITTERBOMBESHOTRU);
        Player[0].Score += 200;
    }
}
