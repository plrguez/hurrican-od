// --------------------------------------------------------------------------------------
// Der Sch?del vom Skeletor
//
// Der Counter, wann sich der Sch?del wieder auf die Maschine setzt, darf erst abgezogen werden,
// wenn der Sch?del in der N?he der Maschine ist. Er kann ja auch vom Skeletor gespawnt werden.
// --------------------------------------------------------------------------------------

#include "stdafx.h"
#include "Boss_Skull.h"

// --------------------------------------------------------------------------------------
// Konstruktor
// --------------------------------------------------------------------------------------

GegnerSkull::GegnerSkull(int Wert1, int Wert2, bool Light)
{
    Handlung		= GEGNER_INIT;
    Value1			= Wert1;
    Value2			= Wert2;
    Energy			= 100000;
    ChangeLight		= Light;
    Destroyable		= true;
    TestBlock		= false;
    SmokeDelay		= 0.0f;
    ShotDelay		= 10.0f;
    ShotCount		= 0;
    Disappear	    = 20.0f;
    pMachine		= NULL;

    AnimCount = 0.0f;
    if (Value2 == 99)
        AnimCount = 20.0f;

    Handlung = GEGNER_SPECIAL;

    // Position der Endboss Wand rausfinden
    GegnerClass *pTemp;
    pTemp = Gegner.pStart;
    while (pTemp != NULL)
    {
        if (pTemp->GegnerArt == THEWALL)
        {
            mxPos = pTemp->xPos + 5;
            myPos = pTemp->yPos + 229;
            pMachine = pTemp;

            pTemp = NULL;
        }
        else
            pTemp = pTemp->pNext;
    }
}

// --------------------------------------------------------------------------------------
// "Bewegungs KI"
// --------------------------------------------------------------------------------------

void GegnerSkull::DoKI(void)
{
    Energy = 100000;
    BlickRichtung = LINKS;

    // Animphase abh?ngig von Position zum Spieler setzen
    //
    float dx = (float)(pAim->xpos + 35) - (xPos + 16);
    AnimPhase = (int)(dx / 30);
    if (AnimPhase < -5) AnimPhase =  -5;
    if (AnimPhase > 9)	AnimPhase = 9;
    AnimPhase += 5;
    if (xSpeed < 0.0f)
        AnimPhase = 20 - AnimPhase;
    if (AnimPhase < 0)  AnimPhase =  0;
    if (AnimPhase > 19)	AnimPhase = 19;

    // Antrieb
    //
    SmokeDelay -= 1.0f SYNC;
    if (SmokeDelay <= 0.0f)
    {
        SmokeDelay = 5.0f;

//		PartikelSystem.PushPartikel(xPos, yPos + 20, SMOKE);

        if (rand()%20 == 0)
            for (int i = 0; i < 10; i++)
                PartikelSystem.PushPartikel(xPos + 10, yPos + 40, FUNKE);
    }

    // Je nach Handlung richtig verhalten
    //
    switch (Handlung)
    {
    // In die Mitte fliegen und explodieren
    case GEGNER_SPECIAL3:
    {
        Disappear -= 1.0f SYNC;

        float endwert;
        endwert = 10.0f - (Disappear / 20.0f * 10.0f);

        if (endwert < 1.0f)
            endwert = 1.0f;

        Timer.SetMoveSpeed(endwert);

        // Explosionen
        if (AnimCount > 0.0f)
            AnimCount -= 1.0f SYNC;
        else
        {
            AnimCount = 0.8f;

            SoundManager.StopWave(SOUND_EXPLOSION2);
            SoundManager.PlayWave(75, 128, 8000 + (int)(endwert * 800), SOUND_EXPLOSION2);

            PartikelSystem.PushPartikel(xPos + rand()%30 - 30, yPos + rand()%30 - 30, EXPLOSION_MEDIUM2);

            for (int i = 0; i < 3; i++)
                PartikelSystem.PushPartikel(xPos + rand()%30 - 10, yPos + rand()%20 - 10, SPIDERSPLITTER);
        }

        if (xPos + 16 > TileEngine.XOffset + 320.0f)
            xAcc = -4.0f;
        else
            xAcc = 4.0f;

        if (yPos + 20 > TileEngine.YOffset + 200.0f)
            yAcc = -4.0f;
        else
            yAcc = 4.0f;

        if (xSpeed >  25.0f) xSpeed =  25.0f;
        if (xSpeed < -25.0f) xSpeed = -25.0f;
        if (ySpeed >  15.0f) ySpeed =  15.0f;
        if (ySpeed < -15.0f) ySpeed = -15.0f;

        if (Disappear < 0.0f)
        {
            // explodieren lassen
            SoundManager.PlayWave(100, 128, 11025, SOUND_EXPLOSION2);
            SoundManager.PlayWave(100, 128, 9000, SOUND_EXPLOSION3);
            SoundManager.PlayWave(100, 128, 7000, SOUND_EXPLOSION1);
            ShakeScreen(10.0f);
            Energy = 0.0f;
            SoundManager.PlayWave(100, 128, 7000, SOUND_MUTANT);

            PartikelSystem.PushPartikel (xPos + 20, yPos + 20, SHOCKEXPLOSION);
            PartikelSystem.PushPartikel (xPos + 20, yPos + 20, SHOCKEXPLOSION);
            PartikelSystem.PushPartikel (xPos + 20, yPos + 20, SHOCKEXPLOSION);

            int i;
            for (i = 0; i < 20; i++)
                PartikelSystem.PushPartikel(xPos + rand()%30,
                                              yPos + rand()%30,
                                              SPIDERSPLITTER);

            for (i = 0; i < 20; i++)
                PartikelSystem.PushPartikel(xPos - 50 + rand()%100,
                                              yPos - 50 + rand()%100, EXPLOSION_BIG);

            for (i = 0; i < 20; i++)
                PartikelSystem.PushPartikel(xPos - 50 + rand()%100,
                                              yPos - 50 + rand()%100, SPLITTER);

            for (int i = 0; i < 20; i++)
                PartikelSystem.PushPartikel(xPos - 50 + rand()%100,
                                              yPos - 50 + rand()%100, SCHROTT1);

            PartikelSystem.PushPartikel(xPos-80,
                                          yPos-80, EXPLOSION_GIGA);

            Timer.SetMoveSpeed(10.0f);

            // Flugs?cke spawnen
            Gegner.PushGegner((float)(TileEngine.XOffset + 200.0f),
                                (float)(TileEngine.YOffset - 40.0f),
                                REITFLUGSACK, 1, 0, false);

            if (NUMPLAYERS ==2 )
                Gegner.PushGegner((float)(TileEngine.XOffset + 300.0f),
                                    (float)(TileEngine.YOffset - 50.0f),
                                    REITFLUGSACK, 1, 0, false);
        }
    }
    break;

    // Der Sch?del fliegt um den Spieler herum. Darf nicht aus dem Screen gedr?ngt werden
    // So lange, bis der Counter abgelaufen ist
    // dann setzt er sich wieder auf die Endboss Maschine
    case GEGNER_SPECIAL:
    {
        if (AnimCount > 0.0f)
            AnimCount -= 1.0f SYNC;
        else
        {
            if (xPos + 16 > pAim->xpos + 35)
                xAcc = -2.0f;
            else
                xAcc = 2.0f;

            if (yPos + 20 > pAim->ypos + 10)
                yAcc = -2.0f;
            else
                yAcc = 2.0f;

            if (xSpeed >  25.0f) xSpeed =  25.0f;
            if (xSpeed < -25.0f) xSpeed = -25.0f;
            if (ySpeed >  15.0f) ySpeed =  15.0f;
            if (ySpeed < -15.0f) ySpeed = -15.0f;

            if ((xSpeed < 0.0f && xPos < TileEngine.XOffset) ||
                    (xSpeed > 0.0f && xPos > TileEngine.XOffset + 640.0f))
                xSpeed *= -1.0f;

            if ((ySpeed < 0.0f && yPos < TileEngine.YOffset) ||
                    (ySpeed > 0.0f && yPos > TileEngine.YOffset + 640.0f))
                ySpeed *= -1.0f;
        }
    }
    break;

    // Auf die Maschine zufliegen?
    case GEGNER_SPECIAL2:
    {
        xAcc = 0.0f;
        yAcc = 0.0f;

        if (xSpeed < 0.0f)	xSpeed += 1.0f SYNC;
        if (xSpeed > 0.0f)	xSpeed -= 1.0f SYNC;
        if (ySpeed < 0.0f)	ySpeed += 1.0f SYNC;
        if (ySpeed > 0.0f)	ySpeed -= 1.0f SYNC;

        if (xPos < mxPos)
        {
            xPos += 10.0f SYNC;

            if (xPos > mxPos)
                xPos = mxPos;
        }

        if (xPos > mxPos)
        {
            xPos -= 10.0f SYNC;

            if (xPos < mxPos)
                xPos = mxPos;
        }

        if (yPos < myPos)
        {
            yPos += 10.0f SYNC;

            if (yPos > myPos)
                yPos = myPos;
        }

        if (yPos > myPos)
        {
            yPos -= 10.0f SYNC;

            if (yPos < myPos)
                yPos = myPos;
        }

        // angekommen?
        if (xPos == mxPos &&
                yPos == myPos)
        {
            Energy = 0.0f;
            pMachine->Value1 = 1;

            SoundManager.PlayWave(50, 128, 15000, SOUND_KLONG);

            //DKS - Added function SongIsPlaying() to SoundManagerClass:
            if (!SoundManager.SongIsPlaying(MUSIC_BOSS))
                SoundManager.PlaySong(MUSIC_BOSS, false);
        }
    }
    break;

    default :
        break;
    } // switch

    // Auf den Spieler schiessen?
    if (Value1 == 99)
    {
        if (pMachine->Energy < 1500.0f)
            ShotDelay -= 1.0f SYNC;

        if (ShotDelay < 0.0f)
        {
            ShotDelay = 15.0f;
            Projectiles.PushProjectile(xPos - 10, yPos, SUCHSCHUSS2, pAim);
            SoundManager.PlayWave(50, 128, 14000 + rand()%2000, SOUND_GOLEMSHOT);
        }
    }

    if (Handlung != GEGNER_SPECIAL3)
        TestDamagePlayers(1.0f SYNC);
}

// --------------------------------------------------------------------------------------
// Sch?del explodiert
// --------------------------------------------------------------------------------------

void GegnerSkull::GegnerExplode(void)
{
}
