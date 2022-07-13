// Copyright (C) RenZhai.2021.All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Channel/SimpleChannel.h"
#include "Stream/SimpleIOStream.h"
#include "SimpleNetChannelType.h"
#include "SimpleNetManage.h"

template<uint32 ProtocolsType>
class FSimpleProtocols{};

#define DEFINITION_SIMPLE_BUFFER  \
TArray<uint8> Buffer; \
FSimpleIOStream Stream(Buffer);

#define DEFINITION_SIMPLE_HEARD(ProtocolsName) \
FSimpleBunchHead Head; \
Head.ProtocolsNumber = (uint32)SP_##ProtocolsName;\
Head.ParamNum = (uint8)FRecursionMessageInfo::GetBuildParams(Params...); \
Head.ChannelID = InChannel->GetGuid(); \
Stream << Head;

class SIMPLENETCHANNEL_API FRecursionMessageInfo
{
public:
	template<typename ...ParamTypes>
	static int32 GetBuildParams(ParamTypes &...Param)
	{
		return sizeof...(Param);
	}

	template<typename ...ParamTypes>
	static void BuildSendParams(FSimpleIOStream& InStream, ParamTypes &...Param){}
	
	//Input parameters to the stream recursively
	template<class T,typename ...ParamTypes>
	static void BuildSendParams(FSimpleIOStream& InStream, T& FirstParam, ParamTypes &...Param)
	{
		InStream << FirstParam;
		BuildSendParams(InStream, Param...);
	}

	template<typename ...ParamTypes>
	static void BuildReceiveParams(FSimpleIOStream& InStream, ParamTypes &...Param) {}

	template<class T, typename ...ParamTypes>
	static void BuildReceiveParams(FSimpleIOStream& InStream, T& FirstParam, ParamTypes &...Param)
	{
		InStream >> FirstParam;
		BuildReceiveParams(InStream, Param...);
	}
};

#define DEFINITION_PROTOCOLS(ProtocolsName,ProtocolsNumber,bForceSend) \
enum{SP_##ProtocolsName = ProtocolsNumber}; \
template<> class SIMPLENETCHANNEL_API FSimpleProtocols<ProtocolsNumber> \
{ \
public: \
	template<typename ...ParamTypes> \
	static void Send(FSimpleChannel* InChannel,ParamTypes &...Params) \
	{ \
		DEFINITION_SIMPLE_BUFFER \
		DEFINITION_SIMPLE_HEARD(ProtocolsName) \
		FRecursionMessageInfo::BuildSendParams(Stream, Params...); \
		InChannel->Send(Buffer,bForceSend);\
	} \
	template<typename ...ParamTypes> \
	static void Receive(FSimpleChannel* InChannel,ParamTypes &...Params) \
	{ \
		DEFINITION_SIMPLE_BUFFER \
		if (InChannel->Receive(Buffer)) \
		{ \
			Stream.Seek(sizeof(FSimpleBunchHead)); \
			FRecursionMessageInfo::BuildReceiveParams(Stream,Params...); \
		} \
	} \
};

#define DEFINITION_SIMPLE_PROTOCOLS(ProtocolsName,ProtocolsNumber) DEFINITION_PROTOCOLS(ProtocolsName,ProtocolsNumber,false)
#define DEFINITION_SIMPLE_PROTOCOLS_FORCE(ProtocolsName,ProtocolsNumber) DEFINITION_PROTOCOLS(ProtocolsName,ProtocolsNumber,true)

#if/* PLATFORM_IOS ||*/ PLATFORM_ANDROID
#define SIMPLE_PROTOCOLS_SEND(InProtocols,args...) FSimpleProtocols<InProtocols>::Send(Channel,##args);
#define SIMPLE_PROTOCOLS_RECEIVE(InProtocols,args...) FSimpleProtocols<InProtocols>::Receive(Channel,##args);

//Make sure simplemannage is the connection side
#define SIMPLE_CLIENT_SEND(SimpleManage,InProtocols,args...) \
if (SimpleManage && SimpleManage->GetController()) \
{ \
	if (FSimpleChannel* SimpleChannel_SIMPLE = SimpleManage->GetChannel()) \
	{ \
		FSimpleProtocols<InProtocols>::Send(SimpleChannel_SIMPLE,##args); \
	} \
}

//Make sure simplemannage is the connection side
#define SIMPLE_CLIENT_RECV(SimpleManage,InProtocols,args...) \
if (SimpleManage && SimpleManage->GetController()) \
{ \
	if (FSimpleChannel* SimpleChannel_SIMPLE = SimpleManage->GetChannel()) \
	{ \
		FSimpleProtocols<InProtocols>::Receive(SimpleChannel_SIMPLE,##args); \
	} \
}

//To ensure simplemannage, the listening server must call our listening port through other terminals to send data to other remote terminals
#define SIMPLE_SERVER_SEND(SimpleManage,InProtocols,AddrInfo,args...) \
if (SimpleManage && SimpleManage->GetController()) \
{ \
	if (FSimpleChannel* SimpleChannel_SIMPLE = SimpleManage->GetChannel(AddrInfo.Addr.IP,AddrInfo.Addr.Port,AddrInfo.ChannelID)) \
	{ \
		FSimpleProtocols<InProtocols>::Send(SimpleChannel_SIMPLE,##args); \
	} \
}
//To ensure simplemannage, the listening server must call our listening port through other terminals to send data to other remote terminals
#define SIMPLE_SERVER_RECV(SimpleManage,InProtocols,AddrInfo,args...) \
if (SimpleManage && SimpleManage->GetController()) \
{ \
	if (FSimpleChannel* SimpleChannel_SIMPLE = SimpleManage->GetChannel(AddrInfo.IP,AddrInfo.Port,AddrInfo.ChannelID)) \
	{ \
		FSimpleProtocols<InProtocols>::Receive(SimpleChannel_SIMPLE,##args); \
	} \
}

#define SIMPLE_SERVER_MULTICAST_SEND(SimpleManage,InProtocols,InType,Code,args...) \
if (SimpleManage)\
{\
	SimpleManage->MulticastByPredicate<InProtocols,InType>(Code,##args);\
}

#else
#define SIMPLE_PROTOCOLS_SEND(InProtocols,...) FSimpleProtocols<InProtocols>::Send(Channel,__VA_ARGS__);
#define SIMPLE_PROTOCOLS_RECEIVE(InProtocols,...) FSimpleProtocols<InProtocols>::Receive(Channel,__VA_ARGS__);

//Make sure simplemannage is the connection side
#define SIMPLE_CLIENT_SEND(SimpleManage,InProtocols,...) \
if (SimpleManage && SimpleManage->GetController()) \
{ \
	if (FSimpleChannel* SimpleChannel_SIMPLE = SimpleManage->GetChannel()) \
	{ \
		FSimpleProtocols<InProtocols>::Send(SimpleChannel_SIMPLE,__VA_ARGS__); \
	} \
}

//Make sure simplemannage is the connection side
#define SIMPLE_CLIENT_RECV(SimpleManage,InProtocols,...) \
if (SimpleManage && SimpleManage->GetController()) \
{ \
	if (FSimpleChannel* SimpleChannel_SIMPLE = SimpleManage->GetChannel()) \
	{ \
		FSimpleProtocols<InProtocols>::Receive(SimpleChannel_SIMPLE,__VA_ARGS__); \
	} \
}

//To ensure simplemannage, the listening server must call our listening port through other terminals to send data to other remote terminals
#define SIMPLE_SERVER_SEND(SimpleManage,InProtocols,AddrInfo,...) \
if (SimpleManage && SimpleManage->GetController()) \
{ \
	if (FSimpleChannel* SimpleChannel_SIMPLE = SimpleManage->GetChannel(AddrInfo.Addr.IP,AddrInfo.Addr.Port,AddrInfo.ChannelID)) \
	{ \
		FSimpleProtocols<InProtocols>::Send(SimpleChannel_SIMPLE,__VA_ARGS__); \
	} \
}
//To ensure simplemannage, the listening server must call our listening port through other terminals to send data to other remote terminals
#define SIMPLE_SERVER_RECV(SimpleManage,InProtocols,AddrInfo,...) \
if (SimpleManage && SimpleManage->GetController()) \
{ \
	if (FSimpleChannel* SimpleChannel_SIMPLE = SimpleManage->GetChannel(AddrInfo.IP,AddrInfo.Port,AddrInfo.ChannelID)) \
	{ \
		FSimpleProtocols<InProtocols>::Receive(SimpleChannel_SIMPLE,__VA_ARGS__); \
	} \
}

//广播 需要指定SimpleManage 协议 和 对象类型，以及 判断是不是要广播这个对象
#define SIMPLE_SERVER_MULTICAST_SEND(SimpleManage,InProtocols,InType,Code,...) \
if (SimpleManage)\
{\
	SimpleManage->MulticastByPredicate<InProtocols,InType>(Code,__VA_ARGS__);\
}

//例如
//GetManage()->MulticastByPredicate<SP_ChatResponses,UMMOARPGCenterServerObject>(
//	A,PlayerID, ContentString, Index);

#endif // PLATFORM_WINDOWS

//SIMPLE_PROTOCOLS_SEND(MMM_Test, 12, TEXT("Hello"), 10.f);
//SIMPLE_PROTOCOLS_SEND(SP_Hello, TEXT("asdasd"), 12, 25);