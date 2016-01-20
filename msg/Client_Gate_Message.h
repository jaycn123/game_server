/*
 * ClientToGate.h
 *
 *  Created on: 2016年1月6日
 *      Author: zhangyalei
 */

#ifndef MSG_CLIENT_GATE_MESSAGE_H_
#define MSG_CLIENT_GATE_MESSAGE_H_

#include <cstring>
#include "Block_Buffer.h"

//心跳
struct MSG_111000 {
	int32_t client_time;

	MSG_111000(void);
	void serialize(Block_Buffer& w) const;
	int deserialize(Block_Buffer& r);
	void reset(void);
};

//心跳(返回)
struct MSG_511000 {
	int32_t client_time;
	int32_t server_time;

	MSG_511000(void) ;
	void serialize(Block_Buffer& w) const;
	int deserialize(Block_Buffer& r);
	void reset(void);
};

//客户端登录消息
struct MSG_111001 {
	std::string account;
	std::string session;		//登录验证的seesion

	MSG_111001(void);
	void serialize(Block_Buffer& w) const;
	int deserialize(Block_Buffer& r);
	void reset(void);
};

//客户端登录消息(返回)
struct MSG_511001 {
	std::string account;

	MSG_511001(void) ;
	void serialize(Block_Buffer& w) const;
	int deserialize(Block_Buffer& r);
	void reset(void);
};

#endif /* MSG_CLIENT_GATE_MESSAGE_H_ */