/**
 * 大厅协议.
 */
#pragma once
#include "SimpleProtocolsDefinition.h"

DEFINITION_SIMPLE_PROTOCOLS(CharacterAppearanceRequests, 4)
DEFINITION_SIMPLE_PROTOCOLS(CharacterAppearanceResponses, 5)
DEFINITION_SIMPLE_PROTOCOLS(CheckCharacterNameRequests, 6)
DEFINITION_SIMPLE_PROTOCOLS(CheckCharacterNameResponses, 7)
DEFINITION_SIMPLE_PROTOCOLS(CreateCharacterRequests, 8)
DEFINITION_SIMPLE_PROTOCOLS(CreateCharacterResponses, 9)
DEFINITION_SIMPLE_PROTOCOLS(LoginToDSServerRequests, 10)
DEFINITION_SIMPLE_PROTOCOLS(LoginToDSServerResponses, 11)
DEFINITION_SIMPLE_PROTOCOLS(DeleteCharacterRequests, 20)
DEFINITION_SIMPLE_PROTOCOLS(DeleteCharacterResponses, 21)
DEFINITION_SIMPLE_PROTOCOLS(EditorCharacterRequests, 22)
DEFINITION_SIMPLE_PROTOCOLS(EditorCharacterResponses, 23)