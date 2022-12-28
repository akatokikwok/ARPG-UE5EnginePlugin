#pragma once
#include "SimpleProtocolsDefinition.h"

DEFINITION_SIMPLE_PROTOCOLS(UpdateLoginCharacterInfoRequests, 18)
DEFINITION_SIMPLE_PROTOCOLS(UpdateLoginCharacterInfoResponses, 19)
DEFINITION_SIMPLE_PROTOCOLS(GetCharacterDataRequests, 24)// GAS人物属性集请求
DEFINITION_SIMPLE_PROTOCOLS(GetCharacterDataResponses, 25)// GAS人物属性集回复

DEFINITION_SIMPLE_PROTOCOLS(CharacterUpgradeLevelRequests, 30)// 升级
DEFINITION_SIMPLE_PROTOCOLS(CharacterUpgradeLevelResponses, 31)
DEFINITION_SIMPLE_PROTOCOLS(UpdateAttributeRequests, 32)// 更新人物属性集响应
DEFINITION_SIMPLE_PROTOCOLS(UpdateAttributeaResponses, 33)

DEFINITION_SIMPLE_PROTOCOLS(UpdateSkillAssemblyRequests, 36)// 技能装配
DEFINITION_SIMPLE_PROTOCOLS(UpdateSkillAssemblyResponses, 37)// 