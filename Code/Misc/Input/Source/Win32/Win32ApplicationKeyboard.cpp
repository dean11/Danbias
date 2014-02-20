#include "../../Include/Win32/Win32ApplicationKeyboard.h"
#include "Utilities.h"

using namespace ::Input;
using namespace ::Utility::Value;
using ::std::wstring;

Win32ApplicationKeyboard::Win32ApplicationKeyboard() : ApplicationKeyboard() { /* DO nothing */ }

Win32ApplicationKeyboard::~Win32ApplicationKeyboard() { /* DO nothing */ }

void Win32ApplicationKeyboard::CaptureText( UINT msg, WPARAM param )
{
	if( !this->textTarget | !this->isEnabled )
		return;

	switch( msg )
	{
	case WM_CHAR:
		this->textTarget->insert( this->writePos, 1, (wchar_t)param );
		++this->writePos;
		break;
	case WM_KEYDOWN:
		{
			switch( param )
			{
			case VK_BACK:
				if( this->writePos > 0 )
				{
					--this->writePos;
					this->textTarget->erase( this->writePos, 1 );
				}
				break;
			case VK_DELETE:
				if( this->writePos < this->textTarget->size() )
				{
					this->textTarget->erase( this->writePos, 1 );
				}
				break;
			case VK_LEFT:
				this->writePos = Max( this->writePos - 1, (wstring::size_type)0 );
				break;
			case VK_RIGHT:
				this->writePos = Min( this->writePos + 1, this->textTarget->size() );
				break;
			default: break;
			}
		}
	default: break;
	}	
}
