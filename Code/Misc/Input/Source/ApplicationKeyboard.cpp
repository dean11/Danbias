#include "../Include/ApplicationKeyboard.h"

using namespace ::Input;

ApplicationKeyboard::ApplicationKeyboard() :
	InputObject( Enum::SAIType_ApplicationKeyboard )
{
	this->targetText = nullptr;
	this->isEnabled = true;
}

ApplicationKeyboard::~ApplicationKeyboard()
{ /* DO nothing */ }

bool ApplicationKeyboard::IsActive() const
{
	return this->isEnabled;
}

void ApplicationKeyboard::Activate()
{
	this->isEnabled = true;
}

void ApplicationKeyboard::Deactivate()
{
	this->isEnabled = false;
}

void ApplicationKeyboard::SetTargetText( ::std::wstring *field )
{
	this->targetText = field;
}