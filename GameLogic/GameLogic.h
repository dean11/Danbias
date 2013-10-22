#pragma once
#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <string>
#include "Ship.h"

namespace GameLogic
{
	enum Result { Success, Failure };
	Result init( const ::std::string &iniFile );
}

#endif