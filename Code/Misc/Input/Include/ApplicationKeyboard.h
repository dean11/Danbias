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
		virtual ~ApplicationKeyboard();

		bool IsActive() const;

		void Activate();
		void Deactivate();

		void BindTextTarget( ::std::wstring *field );
		void ReleaseTextTarget();

	protected:
		::std::wstring *textTarget;
		::std::wstring::size_type writePos;

		ApplicationKeyboard();

	private:
		bool active;
	};
}

#endif // !INPUT_KEBOARD_H
