#include<math.h>
#include"function_opengl.h"
#include"packet_control_interface.h"

#include <iostream>

#define pi 3.1415926

#define Max_plot 100000

///////////////////  function definations /////////////////
//
//
///////////////////  dds  /////////////////////////
CPacketControlInterface *ddsInterface, *pmInterface;
int16_t currentLeftSpeed, currentRightSpeed;

int16_t targetLeftSpeed, targetRightSpeed;
int16_t fbLeftSpeed, fbRightSpeed;
int16_t fbLeftError, fbRightError;
float fbLeftErrorInt, fbRightErrorInt;
int16_t fbLeftErrorDev, fbRightErrorDev;

unsigned int uptime = 0;

float currentKp, currentKi, currentKd;
float targetKp, targetKi, targetKd;

///////////////////  functions  ///////////////////////////
int function_exit()
{
	uint8_t pnStopDriveSystemData[] = {0, 0, 0, 0,
										0,0,0,0,
										0,0,0,0,
										0,0,0,0
										};
	ddsInterface->SendPacket(
			CPacketControlInterface::CPacket::EType::SET_DDS_SPEED,
			pnStopDriveSystemData,
			sizeof(pnStopDriveSystemData));
#ifdef windows
	exit(0);
#else
	exit(0);
#endif
}
int function_init()
{
	ddsInterface = 
		      //new CPacketControlInterface("dds", "/dev/ttyUSB0", 57600);
		      new CPacketControlInterface("dds", "/dev/ttySC0", 57600);

	pmInterface = 
		      //new CPacketControlInterface("pm", "/dev/ttyUSB1", 57600);
		      new CPacketControlInterface("pm", "/dev/ttySC1", 57600);

	printf("---Establish reinterpret_cast<uint16_tConnection with MCUs---------------------\n");

	if (!ddsInterface->Open())
	{	printf("USB0 not open\n");	return -1;	}

	if (!pmInterface->Open())
	{	printf("USB0 not open\n");	return -1;	}
	printf("Both interfaces are open\n");

	printf("The interface is open\n");


	// get uptime and know which is which
	ddsInterface->SendPacket(CPacketControlInterface::CPacket::EType::GET_UPTIME);
	printf("ID check:\n");
	while(1)
	{
		ddsInterface->ProcessInput();

		if(ddsInterface->GetState() == CPacketControlInterface::EState::RECV_COMMAND)
		{
			const CPacketControlInterface::CPacket& cPacket = ddsInterface->GetPacket();
			if (cPacket.GetType() == CPacketControlInterface::CPacket::EType::GET_UPTIME)
			{
				if(cPacket.GetDataLength() == 4) 
				{
					const uint8_t* punPacketData = cPacket.GetDataPointer();
					uint32_t unUptime = 
						(punPacketData[0] << 24) |
						(punPacketData[1] << 16) |
						(punPacketData[2] << 8)  |
						(punPacketData[3] << 0); 
					printf("dds uptime: %u\n",unUptime);
					if (unUptime != 0)
					{
						// means reverse, exchange pm and dds
						printf("ddreinterpret_cast<uint16_ts get non-0 uptime, it should be pm\n");
						CPacketControlInterface* tempInterface;
						tempInterface = pmInterface;
						pmInterface = ddsInterface;
						ddsInterface = tempInterface;
					}
				} 
				break;
			}
		}
	}
	printf("ID check complete:\n");


	// Read Battery
	pmInterface->SendPacket(CPacketControlInterface::CPacket::EType::GET_BATT_LVL);
	if(pmInterface->WaitForPacket(1000, 5)) 
	//while (1)
	{
		const CPacketControlInterface::CPacket& cPacket = pmInterface->GetPacket();
		if(cPacket.GetType() == CPacketControlInterface::CPacket::EType::GET_BATT_LVL &&
				cPacket.GetDataLength() == 2) 
		{
			std::cout << "System battery: "
		              << 17u * cPacket.GetDataPointer()[0]
					  << "mV" << std::endl;
			std::cout << "Actuator battery: "
					  << 17u * cPacket.GetDataPointer()[1]
					  << "mV" << std::endl;
			//break;
		}
	}
	///*
	else 
	{
		std::cout << "Warning: Could not read the system/actuator battery levels" << std::endl;
	}
	//*/

	// Speed
	printf("---Initialize Actuator---------------------\n");
	enum class EActuatorInputLimit : uint8_t {
		LAUTO = 0, L100 = 1, L150 = 2, L500 = 3, L900 = 4
	};

	//unsigned int unTargetId = -1;
	//unsigned int unControlTick = -1;

	/* Override actuator input limit to 100mA */
	pmInterface->SendPacket(
			CPacketControlInterface::CPacket::EType::SET_ACTUATOR_INPUT_LIMIT_OVERRIDE,
			static_cast<const uint8_t>(EActuatorInputLimit::L100));

	/* Enable the actuator power domain */
	pmInterface->SendPacket(
			CPacketControlInterface::CPacket::EType::SET_ACTUATOR_POWER_ENABLE, 
			true);
				      
	/* Power up the differential drive system */
	ddsInterface->SendPacket(CPacketControlInterface::CPacket::EType::SET_DDS_ENABLE, true);

	/* Initialize the differential drive system */
	uint8_t pnStopDriveSystemData[] = {0, 0, 0, 0};
	ddsInterface->SendPacket(
			CPacketControlInterface::CPacket::EType::SET_DDS_SPEED,
			pnStopDriveSystemData,
			sizeof(pnStopDriveSystemData));
	printf("initialize complete\n");

	currentRightSpeed = 0;
	currentLeftSpeed = 0;

	currentKp = 0.707f;
	currentKi = 0.625f;
	currentKd = 0.056f;

	return 0;
}

int function_step()
{
	if ((targetLeftSpeed != currentLeftSpeed) || (targetRightSpeed != currentRightSpeed) ||
		(targetKp != currentKp) || (targetKi != currentKi) || (targetKd != currentKd))
	{
		currentLeftSpeed = targetLeftSpeed;
		currentRightSpeed = targetRightSpeed;
		currentKp = targetKp;
		currentKi = targetKi;
		currentKd = targetKd;

		//uint8_t temp = reinterpret_cast<uint8_t>(currentLeftSpeed & 255);
		uint8_t pnStopDriveSystemData[] = {
			reinterpret_cast<uint8_t*>(&currentLeftSpeed)[1],
			reinterpret_cast<uint8_t*>(&currentLeftSpeed)[0],
			reinterpret_cast<uint8_t*>(&currentRightSpeed)[1],
			reinterpret_cast<uint8_t*>(&currentRightSpeed)[0],

			///*
			reinterpret_cast<uint8_t*>(&currentKp)[3],
			reinterpret_cast<uint8_t*>(&currentKp)[2],
			reinterpret_cast<uint8_t*>(&currentKp)[1],
			reinterpret_cast<uint8_t*>(&currentKp)[0],

			reinterpret_cast<uint8_t*>(&currentKi)[3],
			reinterpret_cast<uint8_t*>(&currentKi)[2],
			reinterpret_cast<uint8_t*>(&currentKi)[1],
			reinterpret_cast<uint8_t*>(&currentKi)[0],

			reinterpret_cast<uint8_t*>(&currentKd)[3],
			reinterpret_cast<uint8_t*>(&currentKd)[2],
			reinterpret_cast<uint8_t*>(&currentKd)[1],
			reinterpret_cast<uint8_t*>(&currentKd)[0],
			//*/
		};
		printf("before send\n");
		ddsInterface->SendPacket(
			CPacketControlInterface::CPacket::EType::SET_DDS_SPEED,
			pnStopDriveSystemData,
			sizeof(pnStopDriveSystemData));
	}

	printf("currentspeed : %d %d\n",currentLeftSpeed,currentRightSpeed);

	printf("Asking dds params");
	ddsInterface->SendPacket(CPacketControlInterface::CPacket::EType::GET_DDS_PARAMS);
	if(ddsInterface->WaitForPacket(200, 3)) 
	//while (1)
	{
		//ddsInterface->ProcessInput();
		if(ddsInterface->GetState() == CPacketControlInterface::EState::RECV_COMMAND)
		{
			printf("received command\n");
			//std::cout << "received command" << std::endl;
			const CPacketControlInterface::CPacket& cPacket = ddsInterface->GetPacket();
			if (cPacket.GetType() == CPacketControlInterface::CPacket::EType::GET_DDS_PARAMS)
			{
				printf("packettype: 0x%x\n",(int)cPacket.GetType());
				if(cPacket.GetDataLength() == 20) 
				{
					const uint8_t* punPacketData = cPacket.GetDataPointer();
					reinterpret_cast<int16_t&>(fbLeftSpeed) = punPacketData[0]<<8 | punPacketData[1];
					reinterpret_cast<int16_t&>(fbRightSpeed) =punPacketData[2]<<8 | punPacketData[3];

					reinterpret_cast<int16_t&>(fbLeftError) = punPacketData[4]<<8 | punPacketData[5];
					reinterpret_cast<int16_t&>(fbRightError) =punPacketData[6]<<8 | punPacketData[7];

					reinterpret_cast<int32_t&>(fbLeftErrorInt) = 	
																(punPacketData[8] << 24) | 
																(punPacketData[9] << 16) |
																(punPacketData[10] << 8) |
																(punPacketData[11]);

					reinterpret_cast<int32_t&>(fbRightErrorInt) =	
																punPacketData[12] << 24 | 
																punPacketData[13] << 16 |
																punPacketData[14] << 8  |
																punPacketData[15];

					reinterpret_cast<int16_t&>(fbLeftErrorDev)= punPacketData[16]<<8 | 
																punPacketData[17];
					reinterpret_cast<int16_t&>(fbRightErrorDev)=punPacketData[18]<<8 | 
																punPacketData[19];

					/*
					printf("speed: %d\n",fbLeftSpeed);
					printf("speed: %d\n",fbRightSpeed);
					printf("error: %d\n",fbLeftError);
					printf("error: %d\n",fbRightError);
					printf("errorInt: %f\n",fbLeftErrorInt);
					printf("errorInt: %f\n",fbRightErrorInt);
					printf("errorDev: %d\n",fbLeftErrorDev);
					printf("errorDev: %d\n",fbRightErrorDev);
					*/
				} 

				//break;
			}
		}
	}// end of while 1


	return 0;
}
