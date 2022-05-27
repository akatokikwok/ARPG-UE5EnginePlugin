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