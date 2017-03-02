#include "Bout.h"
#include <fstream>
using namespace std;

//落在哪个区域
int  Bout::在哪区(Ball ball)
{
	if ((ball.x > 0 && ball.y > 0))//左下角
	{
		return 1;
	}
	if (ball.x > 0 && ball.y < 0)//左上角
	{
		return  2;
	}
	if (ball.x < 0 && ball.y>0)//右下角
	{
		return 3;
	}
	if (ball.x < 0 && ball.y < 0)//右上角
	{
		return 4;
	}

	// y为0
	if(m_BallPower == 1)
	{
		return 5;
	}
	else if (m_BallPower == 2)
	{
		return 6;
	}

	return 7;
}

void Bout::initAllData()
{
	m_is发球 = false;
	m_is无效数据 = false;
	m_inSpectDirection = false;
	m_HitBallArea = 0;
	m_HitBallNumA = 0;
	m_HitBallNumB = 0;
	m_BallDirection1 = 0;
	m_BallDirection2 = 0;
	m_isSpectTwohit = false;
	//m_isSpectDirection = false;
	m_isTouch_net = false;
	m_击球次数 = 0;
	m_第几次发球 = 0;
	m_is发球成功 = false;
	memset(&m_player1, 0, sizeof(m_player1));
	memset(&m_player2, 0, sizeof(m_player2));
}

void Bout::加分(int player)
{
	if (player == 1)
	{
		m_player1.score++;
		if (m_BallPower == 1)
		{
			if(m_第几次发球 == 1)
				m_player1.n1得分Num++;
			if (m_第几次发球 == 2)
				m_player1.n2得分Num++;
		}
	}
	if (player == 2)
	{
		m_player2.score++;
		if (m_BallPower == 2)
		{
			if (m_第几次发球 == 1)
				m_player2.n1得分Num++;
			if (m_第几次发球 == 2)
				m_player2.n2得分Num++;
		}
	}

	//ofstream out("Bout.txt", ios::app);
	//out << "m_ball.timeStamp: " << m_ball.timestamp << "  player: " << player << endl;
	m_event->onAddScor(player, m_player1, m_player2, m_ball); // TODO: Think about the moment of onAddScor call.
	//out.close();

	initAllData();

	//分都加了，改结束了
	m_击球者 = 0;

	m_is无效数据 = true;//一个回合结束，必须发球开始
}
//对方加分
void Bout::对方加分()
{
	int player = 0;
	if (m_击球者 == 1)
		player = 2;
	else if (m_击球者 == 2)
		player = 1;

	加分(player);
}
//自己加分
void Bout::自己加分()
{
	加分(m_击球者);
}