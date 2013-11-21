#ifndef NETWORK_DEPENDENCIES_I_TRANSLATE
#define NETWORK_DEPENDENCIES_I_TRANSLATE

namespace Oyster
{
	namespace Network
	{
		class ITranslate
		{

		public:
			virtual unsigned char* Translate (Protocols::ProtocolHeader &header ) = 0;
			virtual Protocols::ProtocolHeader& Translate ( unsigned char message[] ) = 0;

		};
	}
}
#endif