// --------------------------------------------------------------------------------------
// Secret Area
//
// Wird sie vom Spieler ber?hrt, so wird der "Secret per Level" Counter erh?ht
// wird f?r die "Level-Endabrechnung/Summary Screen" ben?tigt
// --------------------------------------------------------------------------------------

#include "stdafx.h"
#include "Trigger_Secret.h"

// --------------------------------------------------------------------------------------
// Konstruktor
// --------------------------------------------------------------------------------------

GegnerSecret::GegnerSecret(int Wert1, int Wert2, bool Light)
{
    Handlung		= GEGNER_STEHEN;
    Energy			= 100;
    Value1			= Wert1;
    Value2			= Wert2;
    ChangeLight		= Light;
    Destroyable		= false;
    TestBlock		= false;
    DontMove		= true;
}

// --------------------------------------------------------------------------------------
// "Secret KI"
// --------------------------------------------------------------------------------------

void GegnerSecret::DoKI(void)
{
    // Secret nicht im Bild ?
    //
    if (xPos + 10 < TileEngine.XOffset		||
            xPos	  > TileEngine.XOffset + 640	||
            yPos + 10 < TileEngine.YOffset		||
            yPos	  > TileEngine.YOffset + 480)
    {
        return;
    }

    switch (Handlung)
    {
    case GEGNER_STEHEN:
    {
        // Spieler ber?hrt das Secret? Dann Counter erh?hen und deaktivieren
        //
        GegnerRect[GegnerArt].left   = -Value1 / 2 + 20;
        GegnerRect[GegnerArt].right  =  Value1 / 2 + 20;
        GegnerRect[GegnerArt].top    = -Value2 / 2 + 20;
        GegnerRect[GegnerArt].bottom =  Value2 / 2 + 20;

        for (int p = 0; p < NUMPLAYERS; p++)
            if (SpriteCollision(xPos, yPos, GegnerRect[GegnerArt],
                                Player[p].xpos,
                                Player[p].ypos,
                                Player[p].CollideRect) == true)
            {
                Player[0].SecretThisLevel++;
                Player[0].SecretFullGame++;
                Handlung = GEGNER_FALLEN;

                SoundManager.PlayWave(100, 128, 11025, SOUND_SECRET);

                for (int i = 0; i < 200; i++)
                    PartikelSystem.PushPartikel(xPos + 20, yPos + 20, KRINGELSECRET);

                WinkelUebergabe = -1.0f;
                PartikelSystem.PushPartikel(xPos + 20, yPos + 12, TEXTSECRET);
                PartikelSystem.PushPartikel(xPos - 100, yPos - 100, GRENADEFLARE);
                PartikelSystem.PushPartikel (xPos + 5, yPos + 5, EXPLOSION_KRINGEL);

            }
    }
    break;
    }
}

// --------------------------------------------------------------------------------------
// Secret explodiert (nicht)
// --------------------------------------------------------------------------------------

void GegnerSecret::GegnerExplode(void)
{
}
