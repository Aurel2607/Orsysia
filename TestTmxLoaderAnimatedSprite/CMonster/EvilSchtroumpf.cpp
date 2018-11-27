#include "EvilSchtroumpf.h"

CEvilSchtroumpf::CEvilSchtroumpf():
	CMonster(	"evilSchtroumpf",
				56, 48,
				CMonster::speedNormal,
				"sprites/evilSchtroumpf.png"),
	m_currentAnimState(defeated)
{
	// Set current animation
	setUpAnimation(	m_defeated,
					6,
					AnimatedSprite::CAnimation::once,
					56.f, 8.f, 2.f,
					48.f, 152.f);
	setUpAnimation(	m_oralWithSuccubus,
					4,
					AnimatedSprite::CAnimation::reverse,
					56.f, 8.f, 2.f,
					48.f, 218.f);
	setUpAnimation(	m_oralClimaxWithSuccubus,
					5,
					AnimatedSprite::CAnimation::loop,
					56.f, 8.f, 2.f,
					48.f, 282.f);
	setUpAnimation(	m_sexWithSuccubus,
					6,
					AnimatedSprite::CAnimation::loop,
					56.f, 8.f, 2.f,
					48.f, 348.f);
	setUpAnimation(	m_rougherSexWithSuccubus,
					6,
					AnimatedSprite::CAnimation::loop,
					56.f, 8.f, 2.f,
					48.f, 415.f);
	setUpAnimation(	m_rougherSexClimaxWithSuccubus,
					8,
					AnimatedSprite::CAnimation::reverse,
					56.f, 8.f, 2.f,
					48.f, 480.f);

	setAnim(m_currentAnimState);

}

CEvilSchtroumpf::~CEvilSchtroumpf()
{
	//dtor
}


void CEvilSchtroumpf::setAnim(CEvilSchtroumpf::animState_t anim)
{
	m_currentAnimState = anim;
	switch (m_currentAnimState)
	{
	case defeated:
		m_pCurrentAnimation = &m_defeated;
		break;

	case oralWithSuccubus:
		m_pCurrentAnimation = &m_oralWithSuccubus;
		break;

	case oralClimaxWithSuccubus:
		m_pCurrentAnimation = &m_oralClimaxWithSuccubus;
		break;

	case sexWithSuccubus:
		m_pCurrentAnimation = &m_sexWithSuccubus;
		break;

	case rougherSexWithSuccubus:
		m_pCurrentAnimation = &m_rougherSexWithSuccubus;
		break;

	case rougherSexClimaxWithSuccubus:
		m_pCurrentAnimation = &m_rougherSexClimaxWithSuccubus;
		break;

	default:
		m_pCurrentAnimation = &m_defeated;
		break;
	}
	m_animatedSprite.play(*m_pCurrentAnimation);
}

CEvilSchtroumpf::animState_t CEvilSchtroumpf::getAnim(void)
{
	return m_currentAnimState;
}

void CEvilSchtroumpf::nextAnim(void)
{
	if(getAnim() == defeated){
		setAnim(oralWithSuccubus);
		return;
	}
	if(getAnim() == oralWithSuccubus){
		setAnim(oralClimaxWithSuccubus);
		return;
	}
	if(getAnim() == oralClimaxWithSuccubus){
		setAnim(sexWithSuccubus);
		return;
	}
	if(getAnim() == sexWithSuccubus){
		setAnim(rougherSexWithSuccubus);
		return;
	}
	if(getAnim() == rougherSexWithSuccubus){
		setAnim(rougherSexClimaxWithSuccubus);
		return;
	}
	if(getAnim() == rougherSexClimaxWithSuccubus){
		setAnim(defeated);
		return;
	}
}

