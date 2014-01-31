///////////////////////
// Sam Svensson 2013 //
///////////////////////

#include "../../Input/L_inputClass.h"
#include <vector>
#include "IEventButton.h"

namespace Oyster
{
	namespace Event
	{
		template <class owner>
		class EventButton : public IEventButton
		{
		private:
	
			struct PrivData
			{
				static unsigned int currID;
				unsigned int ID;

				owner owner;
				void (*EventFunc)( ButtonEvent e );
			};

			PrivData privData;

		public:
			EventButton();
			EventButton(owner owner);
			EventButton(void (*EventFunc)( ButtonEvent e));
			EventButton(void (*EventFunc)( ButtonEvent e), owner owner);
			
			~EventButton();
			
			void checkCollision(InputClass *input);

			void SetEventFunc(void (*EventFunc)( ButtonEvent e )); //?
			
			unsigned int GetID();
			owner& GetOwner(); 
		
		};


		template <class owner>
		EventButton<owner>::EventButton()
		{
			this->privData.ID = privData.currID;
			this->privData.currID += 1;
			this->privData.owner = NULL;
			this->privData.EventFunc = NULL;
		}

		template <class owner>
		EventButton<owner>::EventButton(owner owner)
		{
			this->privData.ID = privData.currID;
			this->privData.currID += 1;
			this->privData.owner = owner;
			this->privData.EventFunc = NULL;
		}

		template <class owner>
		EventButton<owner>::EventButton(void (*EventFunc)( ButtonEvent e))
		{
			this->privData.ID = privData.currID;
			this->privData.currID += 1;
			this->privData.owner = NULL;
			this->privData.EventFunc = EventFunc;
		}

		template <class owner>
		EventButton<owner>::EventButton(void (*EventFunc)( ButtonEvent e), owner owner)
		{
			this->privData.ID = privData.currID;
			this->privData.currID += 1;
			this->privData.owner = owner;
			this->privData.EventFunc = EventFunc;
		}

		template <class owner>
		EventButton<owner>~EventButton()
		{

		}
		
		template <class owner>
		void EventButton<owner>::checkCollision(InputClass *input)
		{
			//??????????????? TODO: everything
		}

		template <class owner>
		void EventButton<owner>::SetEventFunc(void (*EventFunc)( ButtonEvent e ))
		{
			this->privData.EventFunc = EventFunc;
		}
		
		template <class owner>
		unsigned int EventButton<owner>::GetID()
		{
			return this->privData.ID;
		}

		template <class owner>
		owner& EventButton<owner>::GetOwner()
		{
			return this->privData.owner;
		}
	}
}