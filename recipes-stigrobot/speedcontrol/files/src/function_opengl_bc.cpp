#include<math.h>
#include"function_opengl.h"
#include"packet_control_interface.h"

#include <iostream>

#define pi 3.1415926

#define Max_plot 10000000

///////////////////  function definations /////////////////
//
//
///////////////////  dds  /////////////////////////
CPacketControlInterface *ddsInterface, *pmInterface;
unsigned int currentLeftSpeed, currentRightSpeed;
unsigned int fbLeftSpeed, fbRightSpeed;

///////////////////  functions  ///////////////////////////
int function_exit()
{
	uint8_t pnStopDriveSystemData[] = {0, 0, 0, 0};
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
		      //new CPacketControlInterface("dds", "/dev/ttyUSB1", 57600);
		      new CPacketControlInterface("dds", "/dev/ttySC0", 57600);

	pmInterface = 
		      //new CPacketControlInterface("pm", "/dev/ttyUSB0", 57600);
		      new CPacketControlInterface("pm", "/dev/ttySC1", 57600);

	printf("---Establish Connection with MCUs---------------------\n");

	if (!ddsInterface->Open())
	{	printf("USB1 not open\n");	return -1;	}

	if (!pmInter_POWER_Een())
	{	printf("USB0 not open\n");	return -1;	}


	printf("Both interfaces are open\n");

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
						printf("dds get non-0 uptime, it should be pm\n");
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

	return 0;
}

int function_step(int targetLeftSpeed, int targetRightSpeed)
{
	if ((targetLeftSpeed != currentLeftSpeed)
	  || (targetRightSpeed != currentRightSpeed))
	{
		currentLeftSpeed = targetLeftSpeed;
		currentRightSpeed = targetRightSpeed;
		//uint8_t temp = reinterpret_cast<uint8_t>(currentLeftSpeed & 255);
		uint8_t tempL = currentLeftSpeed & 255;
		uint8_t tempR = currentRightSpeed & 255;

		uint8_t pnStopDriveSystemData[] = {0, tempL, 0, tempR};
		ddsInterface->SendPacket(
			CPacketControlInterface::CPacket::EType::SET_DDS_SPEED,
			pnStopDriveSystemData,
			sizeof(pnStopDriveSystemData));
	}
	printf("currentspeed : %u, %u\n",currentLeftSpeed,currentRightSpeed);

	printf("Asking speed");
	ddsInterface->SendPacket(CPacketControlInterface::CPacket::EType::GET_DDS_SPEED);
	while (1)
	{
		ddsInterface->ProcessInput();
		if(ddsInterface->GetState() == CPacketControlInterface::EState::RECV_COMMAND)
		{
			printf("received command\n");
			//std::cout << "received command" << std::endl;
			const CPacketControlInterface::CPacket& cPacket = ddsInterface->GetPacket();
			if (cPacket.GetType() == CPacketControlInterface::CPacket::EType::GET_UPTIME)
			{
				printf("packettype: 0x%x\n",(int)cPacket.GetType());
				if(cPacket.GetDataLength() == 4) 
				{
					const uint8_t* punPacketData = cPacket.GetDataPointer();
					uint32_t unUptime = 
						(punPacketData[0] << 24) |
						(punPacketData[1] << 16) |
						(punPacketData[2] << 8)  |
						(punPacketData[3] << 0); 
					printf("uptime: %u\n",unUptime);
				} 
				//break;
			}
			if (cPacket.GetType() == CPacketControlInterface::CPacket::EType::GET_DDS_SPEED)
			{
				printf("packettype: 0x%x\n",(int)cPacket.GetType());
				if(cPacket.GetDataLength() == 4) 
				{
					const uint8_t* punPacketData = cPacket.GetDataPointer();
					printf("speed: %u\n",punPacketData[0]);
					printf("speed: %u\n",punPacketData[1]);
					printf("speed: %u\n",punPacketData[2]);
					printf("speed: %u\n",punPacketData[3]);
					//fbLeftSpeed = punPacketData[1];
				} 

				break;
			}
		}
	}// end of while 1

	return 0;
}
