#include "our_text_spot.h"

// flags copied from z_en_wonder_talk.c
// targetable, "friendly" (no effect), no lock-on
#define FLAGS (ACTOR_FLAG_0 | ACTOR_FLAG_3 | ACTOR_FLAG_27)

void OurTextSpot_Init(Actor* thisx, PlayState* play);
void OurTextSpot_Destroy(Actor* thisx, PlayState* play);
void OurTextSpot_Update(Actor* thisx, PlayState* play);
void OurTextSpot_Draw(Actor* thisx, PlayState* play);

ActorInit OurTextSpot_InitVars = {
    /**/ ACTOR_OUR_TEXT_SPOT,
    /**/ ACTORCAT_MISC,
    /**/ FLAGS,
    /**/ OBJECT_GAMEPLAY_KEEP,
    /**/ sizeof(OurTextSpotActor),
    /**/ OurTextSpot_Init,
    /**/ OurTextSpot_Destroy,
    /**/ OurTextSpot_Update,
    /**/ OurTextSpot_Draw,
};

void OurTextSpot_Init(Actor* thisx, PlayState* play) {
    OurTextSpotActor* this = (OurTextSpotActor*)thisx;

    this->actor.targetMode = 4;
}

void OurTextSpot_Destroy(Actor* thisx, PlayState* play) {
    OurTextSpotActor* this = (OurTextSpotActor*)thisx;
}

void OurTextSpot_Update(Actor* thisx, PlayState* play) {
    OurTextSpotActor* this = (OurTextSpotActor*)thisx;

    this->actor.textId = this->actor.params;
    if (Actor_OfferTalkExchange(&this->actor, play, 50.0f, 100.0f, EXCH_ITEM_NONE)) {
        this->actor.flags |= ACTOR_FLAG_0;
    } else {
        // only be targetable when in range to talk
        // (hacky but I'm not sure how to do this properly)
        this->actor.flags &= ~ACTOR_FLAG_0;
    }
}

void OurTextSpot_Draw(Actor* thisx, PlayState* play) {
    OurTextSpotActor* this = (OurTextSpotActor*)thisx;
}
