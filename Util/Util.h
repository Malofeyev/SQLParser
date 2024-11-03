#pragma once

#include <Expr/Expr.h>
#include <Expr/SQLCommand.h>
#include <Json/Json.h>

JsonNode* convertToJson(Expr* expr);

JsonNode* convertToJson(SQLCommand* cmd);

