#pragma once

#include "CoreMinimal.h"
#include "SimpleProtocolsDefinition.h"

// 网关的请求与响应.
DEFINITION_SIMPLE_PROTOCOLS(GateStatusRequests, 2)
DEFINITION_SIMPLE_PROTOCOLS(GateStatusResponses, 3)
DEFINITION_SIMPLE_PROTOCOLS(PlayerRegistInfoRequests, 12)// 是CenterServer向db之间交互.
DEFINITION_SIMPLE_PROTOCOLS(PlayerRegistInfoResponses, 13)// 是CenterServer向db之间交互.
DEFINITION_SIMPLE_PROTOCOLS(PlayerQuitRequests, 14)
DEFINITION_SIMPLE_PROTOCOLS(PlayerQuitResponses, 15)
DEFINITION_SIMPLE_PROTOCOLS(UpdateCharacterDataRequests, 26)// 请求 服务器更新GAS人物属性集
DEFINITION_SIMPLE_PROTOCOLS(UpdateCharacterDataResponses, 27)// 响应 服务器更新GAS人物属性集


DEFINITION_SIMPLE_PROTOCOLS(IdentityReplicationRequests, 28) // 身份覆写协议Request.
DEFINITION_SIMPLE_PROTOCOLS(IdentityReplicationResponses, 29)// 身份覆写协议Response.
