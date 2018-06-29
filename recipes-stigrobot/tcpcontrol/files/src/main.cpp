#include"function_opengl.h"
#include"tcp_socket.h"
#include <stdio.h>
#include <iostream>

int main(int argc, char* argv[])
{
	int16_t left16,right16;

	TCPSocket *tcpSocket = new TCPSocket("server",8080);
	tcpSocket->Open();
	int len;

	function_init();
	while (1)
	{
		char buffer[200];
		tcpSocket->Read(buffer,&len);
		if (len == 4)
		{
			reinterpret_cast<int16_t&>(left16) = buffer[0]<<8 | buffer[1];
			reinterpret_cast<int16_t&>(right16) =buffer[2]<<8 | buffer[3];

			targetLeftSpeed = left16;
			targetRightSpeed = right16;

			function_step();
		}

		char buffer2[200] = {
			reinterpret_cast<char*>(&fbLeftSpeed)[1],
			reinterpret_cast<char*>(&fbLeftSpeed)[0],
			reinterpret_cast<char*>(&fbRightSpeed)[1],
			reinterpret_cast<char*>(&fbRightSpeed)[0],
		};

		tcpSocket->Write(buffer2,4);
	}

	function_exit();
}
