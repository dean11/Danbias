#ifndef NETWORK_DEPENDENCIES_I_TRANSLATE
#define NETWORK_DEPENDENCIES_I_TRANSLATE

namespace Oyster
{
	namespace Network
	{
		class ITranslate
		{

		public:
			virtual char* Translate (const Protocols::ProtocolHeader &header ) = 0;
			virtual Protocols::ProtocolHeader& Translate ( char message[] ) = 0;

		};
	}
}
#endif