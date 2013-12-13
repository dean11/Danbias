#ifndef NETWORK_DEPENDENCIES_I_POST_BOX_H
#define NETWORK_DEPENDENCIES_I_POST_BOX_H

/////////////////////////////////////
// Created by Pontus Fransson 2013 //
/////////////////////////////////////

namespace Oyster
{
	template <class T> 
	class IPostBox
	{
	public:
		virtual ~IPostBox() {}
		virtual void Post(T message) = 0;
		virtual T Fetch() = 0;
		virtual bool IsEmpty() = 0;

	};
}

#endif