#include "AudioManager.h"
using namespace Sound;
using namespace FmodUtil;
ChannelGroupData::ChannelGroupData()
{
	this->channelgroup = nullptr;
}
ChannelGroupData::~ChannelGroupData()
{

}
void ChannelGroupData::setVolym(float volym) 
{
	this->channelgroup->setVolume(volym);
}
void ChannelGroupData::stop()
{

}