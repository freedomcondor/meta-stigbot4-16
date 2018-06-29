#include"function_opengl.h"
#include"tcp_socket.h"
#include <stdio.h>
#include <iostream>

int main(int argc, char* argv[])
{
	int left, right;
	int16_t left16,right16;

	TCPSocket *tcpSocket = new TCPSocket("server",8080);
	tcpSocket->Open();
	char buffer[200];
	int len;

	function_init();
	//while (scanf("%d %d",&left,&right) == 2)
	while (1)
	{
		tcpSocket->Read(buffer,&len);
		if (len == 16)
		{
			reinterpret_cast<int16_t&>(left16) = buffer[0]<<8 | buffer[1];
			reinterpret_cast<int16_t&>(right16) =buffer[2]<<8 | buffer[3];
			///*
			float fKp, fKi, fKd;
			uint32_t temp1_32, temp2_32, temp3_32, temp4_32, temp;
			temp1_32 = buffer[4];
			temp2_32 = buffer[5];
			temp3_32 = buffer[6];
			temp4_32 = buffer[7];

			temp = temp1_32<<24 | temp2_32 <<16 | temp3_32 << 8 | temp4_32;
			fKp = *(reinterpret_cast<float*>(&temp));
			temp1_32 = buffer[8];
			temp2_32 = buffer[9];
			temp3_32 = buffer[10];
			temp4_32 = buffer[11];
			temp = temp1_32<<24 | temp2_32 <<16 | temp3_32 << 8 | temp4_32;
			fKi = *(reinterpret_cast<float*>(&temp));

			temp1_32 = buffer[12];
			temp2_32 = buffer[13];
			temp3_32 = buffer[14];
			temp4_32 = buffer[15];
			temp = temp1_32<<24 | temp2_32 <<16 | temp3_32 << 8 | temp4_32;
			fKd = *(reinterpret_cast<float*>(&temp));

			left = left16;
			right = right16;

			targetLeftSpeed = left;
			targetRightSpeed = right;
			targetKp = fKp;
			targetKi = fKi;
			targetKd = fKd;

			function_step();
		}
		printf("int: %f %f\n",fbLeftErrorInt,fbRightErrorInt);

		char buffer2[200] = {
			reinterpret_cast<char*>(&fbLeftSpeed)[1],
			reinterpret_cast<char*>(&fbLeftSpeed)[0],
			reinterpret_cast<char*>(&fbRightSpeed)[1],
			reinterpret_cast<char*>(&fbRightSpeed)[0],

			reinterpret_cast<uint8_t*>(&fbLeftError)[1],
			reinterpret_cast<uint8_t*>(&fbLeftError)[0],
			reinterpret_cast<uint8_t*>(&fbRightError)[1],
			reinterpret_cast<uint8_t*>(&fbRightError)[0],

			reinterpret_cast<uint8_t*>(&fbLeftErrorInt)[3],
			reinterpret_cast<uint8_t*>(&fbLeftErrorInt)[2],
			reinterpret_cast<uint8_t*>(&fbLeftErrorInt)[1],
			reinterpret_cast<uint8_t*>(&fbLeftErrorInt)[0],

			reinterpret_cast<uint8_t*>(&fbRightErrorInt)[3],
			reinterpret_cast<uint8_t*>(&fbRightErrorInt)[2],
			reinterpret_cast<uint8_t*>(&fbRightErrorInt)[1],
			reinterpret_cast<uint8_t*>(&fbRightErrorInt)[0],

			reinterpret_cast<uint8_t*>(&fbLeftErrorDev)[1],
			reinterpret_cast<uint8_t*>(&fbLeftErrorDev)[0],
			reinterpret_cast<uint8_t*>(&fbRightErrorDev)[1],
			reinterpret_cast<uint8_t*>(&fbRightErrorDev)[0],
		};

		tcpSocket->Write(buffer2,20);
	}

	function_exit();
}
