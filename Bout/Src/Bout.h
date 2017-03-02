#pragma once
#include <string>


#include "../I_Bout.h"
#include "../../Umpire/I_Umpire.h"
#include "Timer.h"
#include <thread>
#include <sstream>

struct pos2d
{
	int x;
	int y;
};

struct 场地范围
{
	pos2d start;
	pos2d end;
};

static void IntToString(int num, std::string& strRes)
{
	stringstream strStream;
	strStream << num;
	strRes = strStream.str();
}

inline bool is范围内(pos2d pos, 场地范围 范围)
{
	////从左往右 ->
	//if (pos.x < 范围.start.x)
	//	return false;
	//if (pos.x > 范围.end.x)
	//	return false;
	//if (pos.y < 范围.start.y)
	//	return false;
	//if (pos.y > 范围.end.y)
	//	return false;
	//return true;

	//从右往左 <-
	if (pos.x > 范围.start.x)
		return false;
	if (pos.x < 范围.end.x)
		return false;
	if (pos.y < 范围.start.y)//Y 仍然是从上忘下
		return false;
	if (pos.y > 范围.end.y)
		return false;
	return true;
}


enum NowState
{
	State_Waiting = 0,
	State_CWSecondServe = 1, // 判断是否是二发，并且
	State_CWOneSideTwiceHit = 2, //发球
	State_NoServe2Hit = 3, //落地
	State_CWOneSideTouchDTouchD = 4, //触球网
	State_ = 5,
	State_Unknown_Error
};

class Bout : public I_Bout
{
public:
	Bout();
	virtual ~Bout();
private: 
	I_BoutEvent* m_event;
	I_TennisVideo* m_tv;
	bool m_is单打;
	//---------------------------------------------
	bool __stdcall start();//启动所有摄像头，捕捉视频并分析
	bool __stdcall stop();
	void * __stdcall init(I_BoutEvent* event, bool is单打, void * dllHandler);
	void __stdcall GroundSize(int Lf, int Ld, int Lw, int Dd, int Ds, int Dw);
	void __stdcall BallPower(int power);
	void __stdcall RunOnce() { ( m_tv == NULL ? NULL : (m_tv->runOnec()) );  }; // TODO: runOnec 貌似拼错了，需要修改？
	bool __stdcall reStartBout(int pos) { return ( m_tv == NULL ?  false : (m_tv->reStartBout(pos)) ); };
	bool __stdcall pause() { m_isPause = true; return (m_tv == NULL ? false : (m_tv->pause())); };//通知底层暂时停止数据处理
	bool __stdcall isPausing() { return m_isPause; }
	bool __stdcall resume() { m_isPause = false; return (m_tv == NULL ? false : (m_tv->resume())); };//通知重新开始处理数据
	void __stdcall ballPower(int power) { return (m_tv == NULL ? false : (m_tv->ballPower(power))); };// 通知底层当前谁发球
	bool __stdcall BasicInit(InitMsg msg) {
		return (m_tv == NULL ?  false : (m_tv->init(msg)));
	}
	//void __stdcall GetAce落点String(std::string Aace, std::string Bace);
	//void __stdcall Get一发落点String(std::string A一发, std::string B一发);
	//void __stdcall Get二发落点String(std::string A二发, std::string B二发);
	//void __stdcall Get击球点String(std::string A击球点, std::string B击球点);


	NowState GetNextState(BallStat prevoisState, BallStat nowState);
	struct Ground
	{
		int Lf;
		int Ld;
		int Lw;
		int Dd;
		int Ds;
		int Dw;
	}m_gound;

	ScorData m_player1, m_player2;
	bool  BallStatDown(Ball ball);
	bool is发球出界(Ball ball);
	bool is发球出界2(Ball ball);
	void 加分(int player);
	void 对方加分();
	void 自己加分();
	void BallHitInspect(Ball ball);
	//同一场地两次击球
	void BallTwoHitA();
	void BallTwoHitB();
	//根据球的上一次坐标，和当前坐标计算方向(1是向A场，2是向B场，3是剩余情况)
	int BallDirection(Ball ball);
	void HandleStat(BallStat stat);
	//落在哪个区域
	int  在哪区(Ball ball);

	int  BallHitArea(Ball ball);
	void BallTwoHitSpect();
	void initAllData();

	场地范围 m_场地A;
	场地范围 m_场地B;

	//现在球是谁打的或者谁发的（1打的就是A场打的，落地点在B场，如果出界就是B得分）
	int m_击球者;
	int m_击球次数;
	BallStat m_stat;
	//当前球的坐标
	Ball m_ball;
	//记录上一次球落地的坐标
	Ball m_ballTouch_down;
	Ball m_发球成功Postion;

	//球权 0为A 1为B
	int m_BallPower;
	//发球所在区域
	int  m_BallServeArea;
	//是否是发球
	bool m_is发球;
	bool m_is发球成功;
	//是否是脏数据
	bool m_is无效数据;

	//是否开启检查球的方向（只有击球的时候检查）
	bool m_inSpectDirection;
	//击球之后球的方向  1是向A场飞  2是向B场飞 3就是错误
	//第一次击球方向
	int m_BallDirection1;
	//第二次击球方向
	int m_BallDirection2;
	//击球的坐标
	Ball m_HitBall;
	//击球所在场地
	int  m_HitBallArea;
	int  m_HitBallNumA;
	int  m_HitBallNumB;
	//是否启动两次击球同一场地检查
	bool m_isSpectTwohit;
	////如果在距离网只有1m的距离以内开启检查判断方向
	//bool m_isSpectDirection;
	//是否接触异物
	bool m_isTouch_net;

	struct BallData
	{
		Ball ball;
		BallStat stat;
	};
	//上次事件
	BallData m_上次Data;
	//第几次发球
	int m_第几次发球;


	// 记录当前球的距离是否大于1100, 等待1s,2s后状态
	bool m_tmpLT;
	Timer m_t;
	int m_isRecord; // 记录是否判定界外落地 0.事件6未开始，1.事件6开始，等待下一个事件，2.事件6开始，下一个事件来到；>=3未定义;
	bool m_isPause; // 是否让底层停止传输数据

	Timer m_tForTimeOut; // 检测OnBall事件是否超时
	BallData m_lastStatWhenout;
};


