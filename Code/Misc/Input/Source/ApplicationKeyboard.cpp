#include "../Include/ApplicationKeyboard.h"

using namespace ::Input;

ApplicationKeyboard::ApplicationKeyboard() :
	InputObject( Enum::SAIType_ApplicationKeyboard )
{
	this->textTarget = nullptr;
	this->writePos = 0;
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

void ApplicationKeyboard::BindTextTarget( ::std::wstring *field )
{
	this->textTarget = field;
	
	if( field )
	{
		this->writePos = field->size();
	}
}

void ApplicationKeyboard::ReleaseTextTarget( )
{
	this->BindTextTarget( nullptr );
}