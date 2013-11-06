#include "RawInput.h"

void RawInput::Mouse_Show()					   
{

}
void RawInput::Mouse_Hide()					   
{

}
void RawInput::Mouse_Lock()					   
{

}
void RawInput::Mouse_Unlock()				   
{

}
void RawInput::Mouse_IsBtnPressed(IN RIK)	   
{

}
int	 RawInput::Mouse_WheelDelta()			   
{
	return 0;
}
POINT RawInput::Mouse_Position()			   
{
	POINT p = {0, 0};
	//POINT p = { this->_activeInputData.data.MOUSE_DATA.MousePos_X, this->_activeInputData.data.MOUSE_DATA.MousePos_Y };
	return p;
}

void RawInput::Mouse_Enable()				   
{
	this->_mouseEnabled = true;
}
void RawInput::Mouse_Disable()				   
{
	this->_mouseEnabled = false;
}
bool RawInput::Keyboard_KeyUp(IN RIK key)	   
{
	return false;
}
bool RawInput::Keyboard_KeyDown(IN RIK key)	   
{
	return false;
}
void RawInput::Keyboard_Enable()			   
{
	this->_KeyboardEnabled = true;
}
void RawInput::Keyboard_Disable()
{
	this->_KeyboardEnabled = false;
}