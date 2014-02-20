/////////////////////////////////////////////////////////////////////
// Created by [Dan Andersson] [2014]
/////////////////////////////////////////////////////////////////////
#ifndef INPUT_APPLICATION_KEBOARD_H
#define INPUT_APPLICATION_KEBOARD_H

#include "InputObject.h"
#include <string>

namespace Input
{
	class ApplicationKeyboard : public InputObject
	{
	public:
		bool IsActive() const;

		void Activate();
		void Deactivate();

		void SetTargetText( ::std::wstring *field );

	protected:
		::std::wstring *targetText;

		ApplicationKeyboard();
		~ApplicationKeyboard();

	private:
		bool active;
	};
}

#endif // !INPUT_KEBOARD_H
