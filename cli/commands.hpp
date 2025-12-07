#pragma once
#include "args_parser.hpp"
#include "http_client.hpp"

int commandSave(const ParseResult& args, HttpClient& client);
int commandRead(const ParseResult& args, HttpClient& client);