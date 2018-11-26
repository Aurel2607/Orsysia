#include "EvilSchtroumpf.h"

CEvilSchtroumpf::CEvilSchtroumpf():
	CMonster(	"evilSchtroumpf",
				56, 48,
				CMonster::speedNormal,
				"sprites/evilSchtroumpf.png")

{
	// Set current animation
	setUpAnimation(	m_oralWithSuccubus,
					4,
					true,
					56.f, 8.f, 2.f,
					48.f, 218.f);
	setUpAnimation(	m_oralClimaxWithSuccubus,
					5,
					false,
					56.f, 8.f, 2.f,
					48.f, 282.f);
	setUpAnimation(	m_sexWithSuccubus,
					6,
					false,
					56.f, 8.f, 2.f,
					48.f, 348.f);
	setUpAnimation(	m_rougherSexWithSuccubus,
					6,
					false,
					56.f, 8.f, 2.f,
					48.f, 415.f);
	setUpAnimation(	m_rougherSexClimaxWithSuccubus,
					8,
					true,
					56.f, 8.f, 2.f,
					48.f, 480.f);
	m_pCurrentAnimation = &m_oralWithSuccubus;
}

CEvilSchtroumpf::~CEvilSchtroumpf()
{
	//dtor
}
