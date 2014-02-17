#ifndef GID_H
#define GID_H

#include <vector>

/** 
*	This class only purpos is to generate a uniqe global id, nothing else.. 
*/
class GID
{
	private:
		int id;
		int usft()									{ static int ID = 0; return ID++; }

	public:
		GID::GID()									{ this->id = usft(); }  
		GID::~GID()									{ }
		GID(const GID& o)							{ this->id = usft(); }
		const GID& operator=(const GID& o)			{ this->id = usft();  return *this; }

		operator int() const						{ return this->id; }
		bool operator == (const GID& object) const	{ return (this->id == object.id); }
		bool operator == (const int& id) const 		{ return (this->id == id); }
		int get() const								{ return this->id; }
};


#endif

