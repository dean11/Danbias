#ifndef NETWORK_DEPENDENCIES_I_POST_BOX_H
#define NETWORK_DEPENDENCIES_I_POST_BOX_H

/////////////////////////////////////
// Created by Pontus Fransson 2013 //
/////////////////////////////////////

namespace Oyster
{
	namespace Network
	{
		template <class T>
		class IPostBox
		{
		public:
			virtual ~IPostBox() {}
			virtual void PostMessage(T& message) = 0;
			virtual T FetchMessage() = 0;
			virtual bool IsFull() = 0;

		};
	}
}

#endif