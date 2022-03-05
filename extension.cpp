/**
 * vim: set ts=4 :
 * =============================================================================
 * SourceMod Sample Extension
 * Copyright (C) 2004-2008 AlliedModders LLC.  All rights reserved.
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * As a special exception, AlliedModders LLC gives you permission to link the
 * code of this program (as well as its derivative works) to "Half-Life 2," the
 * "Source Engine," the "SourcePawn JIT," and any Game MODs that run on software
 * by the Valve Corporation.  You must obey the GNU General Public License in
 * all respects for all other code used.  Additionally, AlliedModders LLC grants
 * this exception to all derivative works.  AlliedModders LLC defines further
 * exceptions, found in LICENSE.txt (as of this writing, version JULY-31-2007),
 * or <http://www.sourcemod.net/license.php>.
 *
 * Version: $Id$
 */

#include "extension.h"
#include <hiredis/hiredis.h>

/**
 * @file extension.cpp
 * @brief Implement extension code here.
 */

redisContext* connect = new redisContext();
redisReply* reply = new redisReply();

bool Connect(const char* ip, int port) {
	connect = redisConnect(ip, port);
	if (connect != NULL && connect->err) {
		return false;
	}
	return true;
}

bool isConnect() {
	return connect != NULL && !connect->err;
}

char* Get(const char* key) {
	reply = (redisReply*)redisCommand(connect, "get '%s'", key);
	char* str = reply->str;
	freeReplyObject(reply);
	return str;
}

void* Set(const char* key, const char* value) {
	redisCommand(connect, "set '%s' '%s'", key, value);
}

void* Del(const char* key) {
	redisCommand(connect, "del '%s'", key);
}

//Start

Sample g_Sample;		/**< Global singleton for extension's main interface */

SMEXT_LINK(&g_Sample);

cell_t Redis_Connect(IPluginContext* pContext, const cell_t* params) {
	return Connect(const_cast<char*>(reinterpret_cast<char*>(params[1])), params[2]);
};

cell_t Redis_Set(IPluginContext* pContext, const cell_t* params) {
	if (!isConnect()) {
		return pContext->ThrowNativeError("Redis is not connect");
	}
	Set(const_cast<char*>(reinterpret_cast<char*>(params[1])), const_cast<char*>(reinterpret_cast<char*>(params[2])));
};

cell_t Redis_Get(IPluginContext* pContext, const cell_t* params) {
	if (!isConnect()) {
		return pContext->ThrowNativeError("Redis is not connect");
	}
	char* result = Get(const_cast<char*>(reinterpret_cast<char*>(params[1])));
	pContext->StringToLocalUTF8(params[2], params[3], result, 0);
};

cell_t Redis_Del(IPluginContext* pContext, const cell_t* params) {
	if (!isConnect()) {
		return pContext->ThrowNativeError("Redis is not connect");
	}
	Del(const_cast<char*>(reinterpret_cast<char*>(params[1])));
}

const sp_nativeinfo_t MyNatives[] = {
	{"Redis_Connect", Redis_Connect},
	{"Redis_Set", Redis_Set},
	{"Redis_Get", Redis_Get},
	{"Redis_Del", Redis_Del},
};

void Sample::SDK_OnAllLoaded() {
	sharesys->AddNatives(myself, MyNatives);
};