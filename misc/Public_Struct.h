/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */


#ifndef PUBLIC_STURCT_H_
#define PUBLIC_STURCT_H_

#include "Log.h"
#include "Time_Value.h"
#include "Date_Time.h"
#include "List.h"
#include "Object_Pool.h"
#include "Misc.h"
#include "Public_Define.h"

struct Account_Info {
	std::string account;
	int agent_num;
	int server_num;

	Account_Info(void);
	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct Cid_Info {
	int32_t gate_cid;			//gate连接game,master的cid
	int32_t game_cid;			//game连接master的cid
	int32_t player_cid;		//player连接gate的cid

	Cid_Info(void) : gate_cid(0), game_cid(0), player_cid(0) {};
	Cid_Info(int32_t gate_cid_, int32_t game_cid_, int32_t player_cid_) : gate_cid(gate_cid_), game_cid(game_cid_), player_cid(player_cid_) {};

	void reset(void) {
		gate_cid = -1;
		game_cid = -1;
		player_cid = -1;
	}
};

struct Ip_Info {
	std::string ip;
	int port;

	Ip_Info(void) { reset(); };

	void reset(void) {
		ip.clear();
		port = 0;
	}
};

struct Saving_Info {
	role_id_t role_id;		// 角色
	Time_Value timestamp;	// 保存时的时间错

	Saving_Info(void) : role_id(0) {}
	Saving_Info(role_id_t role_id_, Time_Value timestamp_) : role_id(role_id_), timestamp(timestamp_) {};
};

struct Close_Info {
	int cid;
	Time_Value timestamp;

	Close_Info(void) : cid(-1), timestamp(Time_Value::zero) { }
	Close_Info(int p_cid, const Time_Value &p_timestamp) : cid(p_cid), timestamp(p_timestamp) { }
};

struct Msg_Info {
	bool is_inited;
	int cid;
	long hash_key;				/// 用于加解密的hash key
	uint32_t msg_serial;				/// 上一条消息序号
	Time_Value msg_timestamp;			/// 上一条消息时间戳
	uint32_t msg_interval_count_;		/// 操作频率统计
	Time_Value msg_interval_timestamp;

	void reset(void) {
		is_inited = false;
		cid = -1;
		hash_key = 0;
		msg_serial = 0;
		msg_timestamp = Time_Value::zero;
		msg_interval_count_ = 0;
		msg_interval_timestamp = Time_Value::zero;
	}
};

struct Recycle_Tick {
	enum {
		NON_RECYCLE,	/// 非回收状态
		RECYCLE,		/// 回收状态
	};

	static const Time_Value recycle_time_;
	static const Time_Value tick_interval_;

	int status_;
	Time_Value last_change_status_ts_;
	Time_Value last_tick_ts_;

	Recycle_Tick(void)
	: status_(NON_RECYCLE),
	  last_change_status_ts_(Time_Value::gettimeofday()),
	  last_tick_ts_(Time_Value::zero)
	{ }

	void reset(void) {
		set(NON_RECYCLE);
		last_tick_ts_ = Time_Value::zero;
	}

	void set(int v) {
		status_ = v;
		last_change_status_ts_ = Time_Value::gettimeofday();
	}
};

struct Gate_Player_Info {
	role_id_t role_id;
	std::string account;

	Gate_Player_Info(void);
	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct Master_Player_Info {
	role_id_t role_id;
	std::string account;
	std::string role_name;
	int level;

	Master_Player_Info(void);
	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct Game_Player_Info {
	role_id_t role_id;
	int agent_num;
	int server_num;
	std::string account;
	std::string role_name;
	int gender; 					/// 0 = 女(female), 1 = 男(male)
	int career; 					///  职业 1-4	([1,4] 荒蛮，玄羽，莲华，妙冰)
	int create_time;				/// 创建角色时刻
	int level;     			   		/// 避免连表查询
	int last_sign_in_time;			/// 最后登录时间
	int last_sign_out_time;			/// 最后登出时间
	std::string ip;					/// IP

	Game_Player_Info(void);
	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	int load(void);
	int save(void);
	void reset(void);
};

struct Player_Data {
	enum{
		NULL_STATUS,
		SUCCESS_LOADED,			/// 加载成功
		SUCCESS_CREATED,		/// 创建成功
		ROLE_NOT_EXIST,			/// 角色不存在
		ROLE_HAS_EXIST,			///	角色已经存在
		ROLE_SAVE_OFFLINE,	/// 角色下线保存
	};

	role_id_t role_id;
	int8_t status;

	Game_Player_Info game_player_info;

	void set_all_detail_change_state(bool is_change) {
	}

	void set_role_id(role_id_t p_role_id) {
		role_id = p_role_id;
	}

	void set_all_role_id(role_id_t p_role_id) {
		role_id = p_role_id;

		game_player_info.role_id = p_role_id;
	}

	Player_Data(void);
	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	int load(void);
	int save(void);
	void reset(void);
};

struct Player_DB_Cache {
	role_id_t role_id;
	std::string account;
	std::string role_name;
	int agent_num;
	int server_num;
	int gender;
	int career;
	int level;

	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	void reset(void);
};

struct DB_Cache {
	typedef boost::unordered_map<role_id_t, Player_DB_Cache> ID_Player_Cache_Map;
	typedef boost::unordered_map<std::string, Player_DB_Cache> Account_Player_Cache_Map;

	const static int player_cache_map_bucket_num = 50000;

	DB_Cache(void)
	: id_player_cache_map(player_cache_map_bucket_num),
	  account_player_cache_map(player_cache_map_bucket_num)
	{ }

	void reset(void) {
		id_player_cache_map.clear();
		account_player_cache_map.clear();
	}

	ID_Player_Cache_Map id_player_cache_map;
	Account_Player_Cache_Map account_player_cache_map;
};

struct Tick_Info {
	const Time_Value server_info_interval_tick;
	Time_Value server_info_last_tick;

	const Time_Value player_interval_tick; /// Game_Player定时器间隔
	Time_Value player_last_tick;

	const Time_Value manager_interval_tick; /// Game_Manager
	Time_Value manager_last_tick;

	const Time_Value saving_scanner_interval_tick;	// 玩家下线保存表的扫描
	Time_Value saving_scanner_last_tick;

	const Time_Value object_pool_interval_tick;
	Time_Value object_pool_last_tick;

	Tick_Info(void)
	: server_info_interval_tick(Time_Value(300, 0)),
	  server_info_last_tick(Time_Value::zero),
	  player_interval_tick(Time_Value(0, 500 * 1000)),
	  player_last_tick(Time_Value::zero),
	  manager_interval_tick(1, 0),
	  manager_last_tick(Time_Value::zero),
	  saving_scanner_interval_tick(20, 0),
	  saving_scanner_last_tick(Time_Value::zero),
	  object_pool_interval_tick(300, 0),
	  object_pool_last_tick(Time_Value::zero)
	{ }
};

#endif /* PUBLIC_STURCT_H_ */