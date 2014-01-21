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
			CallbackType_Unknown,
		};

		template <typename ReturnVal = void, typename ParamVal = void>
		union OysterCallbackValue
		{
			IPostBox<ParamVal>* callbackPostBox;
			CallbackObject<ReturnVal, ParamVal> *callbackObject;
			typename CallbackFunction<ReturnVal, ParamVal>::FNC callbackFunction;

			OysterCallbackValue() { memset(this, 0, sizeof(OysterCallbackValue)); }
			OysterCallbackValue(IPostBox<ReturnVal>* postbox) { callbackPostBox = postbox; }
			OysterCallbackValue(CallbackObject<ReturnVal, ParamVal>* obj) { callbackObject = obj; }
			OysterCallbackValue(typename CallbackFunction<ReturnVal, ParamVal>::FNC function) { callbackFunction = function; }
		};

		template <typename ReturnVal = void, typename ParamVal = void>
		struct OysterCallback
		{
			OysterCallbackValue<ReturnVal, ParamVal> value;
			CallbackType callbackType;

			OysterCallback() :callbackType(CallbackType_Unknown){}
			bool operator()()
			{
				return true;
			}
			bool operator()(ParamVal e)
			{
				switch (callbackType)
				{
					case CallbackType_Function:
						if(value.callbackFunction) value.callbackFunction(e);
						return true;
					break;
					case CallbackType_Object:
						if(value.callbackObject) value.callbackObject->ObjectCallback(e);
						return true;
					break;
					case CallbackType_PostBox:
						value.callbackPostBox->Post(e);
						return true;
					break;
				}
				return false;
			}
			
		};
	}
}

#endif