#ifndef DANBIASSERVER_SERVER_INIT_READER_H
#define DANBIASSERVER_SERVER_INIT_READER_H

#include <fstream>
#include <string>

namespace DanBias
{
	enum InitPath
	{
		InitPath_ServerIni,
	};
	std::string	GetInitPath(InitPath file)
	{
		std::string type = "";
		std::string path = "";
		std::string flag = "";

		switch (file)
		{
			case DanBias::InitPath_ServerIni:
				flag = "ServerInit";
			break;
		}

		std::fstream in;
		in.open("..\\Settings\\serversearchpath.ini", std::ios::in);
		if(!in.is_open()) return "";

		while (!in.eof() && type != flag)
		{
			in >> type;
			in >> path;
		}

		in.close();
		return path;
	}
}

#endif // !DANBIASSERVER_SERVER_INIT_READER_H
