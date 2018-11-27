#ifndef CEVILSCHTROUMPF_H
#define CEVILSCHTROUMPF_H

#include "Monster.h"

class CEvilSchtroumpf : public CMonster
{
public:
	enum animState_t{
		defeated = 0,
		oralWithSuccubus,
		oralClimaxWithSuccubus,
		sexWithSuccubus,
		rougherSexWithSuccubus,
		rougherSexClimaxWithSuccubus
	};

public:
	CEvilSchtroumpf();
	virtual ~CEvilSchtroumpf();

	void 		setAnim(animState_t anim);
	animState_t getAnim(void);
	void 		nextAnim(void);
protected:

private:
	animState_t m_currentAnimState;
   	AnimatedSprite::CAnimation m_defeated;
   	AnimatedSprite::CAnimation m_oralWithSuccubus;
   	AnimatedSprite::CAnimation m_oralClimaxWithSuccubus;
    AnimatedSprite::CAnimation m_sexWithSuccubus;
   	AnimatedSprite::CAnimation m_rougherSexWithSuccubus;
   	AnimatedSprite::CAnimation m_rougherSexClimaxWithSuccubus;
};

#endif // CEVILSCHTROUMPF_H
