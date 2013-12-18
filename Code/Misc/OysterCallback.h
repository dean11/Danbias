//////////////////////////////
//	Dennis Andersen 2013	//
//////////////////////////////
#ifndef MISC_OYSTER_CALLBACK_H
#define MISC_OYSTER_CALLBACK_H

#include "PostBox\IPostBox.h"

namespace Oyster
{
	namespace Callback
	{
		template <typename ReturnVal = void, typename ParamVal = void> 
		struct CallbackFunction
		{
			typedef ReturnVal (*FNC)(ParamVal);
		};
		
		template <typename ReturnVal = void, typename ParamVal = void> 
		struct CallbackObject
		{
			virtual ReturnVal ObjectCallback(ParamVal) = 0;
		};

		enum CallbackType
		{
			CallbackType_PostBox,
			CallbackType_Function,
			CallbackType_Object,
		};

		template <typename ReturnVal = void, typename ParamVal = void>
		union OysterCallback
		{
			IPostBox<ParamVal>* callbackPostBox;
			CallbackObject<ReturnVal, ParamVal> *callbackObject;
			typename CallbackFunction<ReturnVal, ParamVal>::FNC callbackFunction;

			OysterCallback() { memset(this, 0, sizeof(OysterCallback)); }
			OysterCallback(IPostBox<ReturnVal>* postbox) { callbackPostBox = postbox; }
			OysterCallback(CallbackObject<ReturnVal, ParamVal>* obj) { callbackObject = obj; }
			OysterCallback(typename CallbackFunction<ReturnVal, ParamVal>::FNC function) { callbackFunction = function; }
		};
	}
}

#endif