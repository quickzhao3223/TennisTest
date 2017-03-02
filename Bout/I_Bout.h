///////////////////////////////////////////////////////////////
//
// I_Bout.h                   
// Description:	
//	
///////////////////////////////////////////////////////////////
#pragma once
#include <plug/plug.h>               
//------------------------------------------------------------------------------------------------------------------
#include "../TennisVideo/I_TennisVideo.h"
#include <string>
#include <vector>
#pragma pack(push, 1)

struct ScorData
{
	int score;						//分数
	int ace;						//ace
	int 双误;						//两次失误
	int n1Total;					//一发总次数		（整盘自己一发了多少次）
	int n1进球Num;					//一发进球次数		（一发，发球成功）
	int n1得分Num;					//一发得分次数		（一发，这个回合 自己得分）
	int n2Total;					//二发总次数
	int n2进球Num;					//二发进球次数
	int n2得分Num;					//二发成功次数
	int 致胜分;
	int speedMax;					//最高发球速度
	int speedSum;                   //发球速度之和
	int 击球次数;					//精彩视频里面要统计击球20次以上的得分回合
	//int 网前击球次数;
	int 场内击球次数;
	int 底线击球次数;
	int 场外击球次数;
	// 
	int 一区内侧次数;
	int 一区外侧次数;
	int 二区内侧次数;
	int 二区外侧次数;
	// 二发
	int 一区内侧次数2;
	int 一区外侧次数2;
	int 二区内侧次数2;
	int 二区外侧次数2;


	ScorData& operator+= (const ScorData& other)
	{
		score += other.score;
		ace += other.ace;
		双误 += other.双误;

		n1Total += other.n1Total;
		n1进球Num += other.n1进球Num;
		n1得分Num += other.n1得分Num;
		n2Total += other.n2Total;
		n2进球Num += other.n2进球Num;
		n2得分Num += other.n2得分Num;
		致胜分 += other.致胜分;
		speedSum += other.speedSum;
		场内击球次数 += other.场内击球次数;
		底线击球次数 += other.底线击球次数;
		场外击球次数 += other.场外击球次数;
		一区内侧次数 += other.一区内侧次数;
		一区外侧次数 += other.一区外侧次数;
		二区内侧次数 += other.二区内侧次数;
		二区外侧次数 += other.二区外侧次数;
		一区内侧次数2 += other.一区内侧次数2;
		一区外侧次数2 += other.一区外侧次数2;
		二区内侧次数2 += other.二区内侧次数2;
		二区外侧次数2 += other.二区外侧次数2;

		if (other.speedMax > speedMax)
			speedMax = other.speedMax;
		return *this;
	}
};

#pragma pack(pop)

struct I_BoutEvent
{
	virtual void __stdcall onBallStat(BallStat stat) = 0;
	virtual void __stdcall onBall(Ball ball) = 0;
	virtual void __stdcall onError(const wchar_t* err) = 0;
	virtual void __stdcall onAddScor(int player, ScorData scor1, ScorData scor2, Ball ball) = 0;
	virtual void __stdcall on发球(Ball ball) = 0;
	virtual void __stdcall onBallAce(int power) = 0;
	virtual void __stdcall on发球成功() = 0;
	virtual void __stdcall on发球失败() = 0;
	virtual void __stdcall on发球Pos(int 击球者, int type, Ball ball) = 0;
	virtual void __stdcall onHitBallPos(int 击球者, Ball ball) = 0;
};
struct I_Bout
{
	std::function<void(Ball ball)> onBall;//每10ms输出一次球的信息
	std::function<void(Ball ball)> on发球;//每10ms输出一次球的信息
	virtual void * __stdcall init(I_BoutEvent* event, bool is单打, void * dllHandler) = 0;
	virtual bool __stdcall start() = 0;//启动所有摄像头，捕捉视频并分析
	virtual bool __stdcall stop() = 0;
	virtual void __stdcall GroundSize(int Lf, int Ld, int Lw, int Dd, int Ds, int Dw) = 0;
	virtual void __stdcall BallPower(int power) = 0;
	virtual void __stdcall RunOnce() = 0;
	virtual bool __stdcall reStartBout(int pos) = 0;//开始一个新的回合, pos 发球位置,1,2A场，3,4B场
	virtual bool __stdcall isPausing() = 0;
	virtual bool __stdcall pause() = 0;
	virtual bool __stdcall resume() = 0;
	virtual bool __stdcall BasicInit(InitMsg msg) = 0;
	//virtual void __stdcall GetAce落点String(std::string Aace, std::string Bace) = 0;
	//virtual void __stdcall Get一发落点String(std::string A一发, std::string B一发) = 0;
	//virtual void __stdcall Get二发落点String(std::string A二发, std::string B二发) = 0;
	//virtual void __stdcall Get击球点String(std::string A击球点, std::string B击球点) = 0;
};