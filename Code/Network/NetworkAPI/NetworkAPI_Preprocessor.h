//////////////////////////////////////
// Created by Dennis Andersen 2014 //
//////////////////////////////////////
#ifndef NETWORK_API_NETWORK_API_PREPROCESSOR_H
#define NETWORK_API_NETWORK_API_PREPROCESSOR_H

#ifdef NETWORKAPI_EXPORT
	#define NET_API_EXPORT __declspec(dllexport)
#else
	#define NET_API_EXPORT __declspec(dllimport)
#endif

#endif // !NETWORK_API_NETWORK_API_PREPROCESSOR_H
