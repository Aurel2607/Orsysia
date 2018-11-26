#ifndef CEVILSCHTROUMPF_H
#define CEVILSCHTROUMPF_H

#include "Monster.h"

class CEvilSchtroumpf : public CMonster
{
	public:
		CEvilSchtroumpf();
		virtual ~CEvilSchtroumpf();

	protected:

	private:
   	AnimatedSprite::CAnimation m_oralWithSuccubus;
   	AnimatedSprite::CAnimation m_oralClimaxWithSuccubus;
    AnimatedSprite::CAnimation m_sexWithSuccubus;
   	AnimatedSprite::CAnimation m_rougherSexWithSuccubus;
   	AnimatedSprite::CAnimation m_rougherSexClimaxWithSuccubus;
};

#endif // CEVILSCHTROUMPF_H
