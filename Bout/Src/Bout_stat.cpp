#include "Bout.h"
#include <fstream>
using namespace std;

static const int 底线给事件6的临界坐标 = 11000;
static const int 底线给事件6的直接超时距离 = 20000;
//球的事件在这里处理
void Bout::HandleStat(BallStat stat)
{
	ofstream out("Bout.txt", ios::app);

	if (stat == BallStat_hit)
	{
		switch (m_上次Data.stat)
		{
			case BallStat_serve:
				m_event->onError(L"Error Code 21");
				out << "Error Code 21" << endl;
				break;
			case BallStat_touch_down:
				// 已处理
				break;
			case BallStat_out:
				m_event->onError(L"Error Code 61");
				out << "Error Code 21" << endl;
				break;
			default:
				break;
		}
	}
	else if (stat == BallStat_serve)
	{
		switch (m_上次Data.stat)
		{
		case BallStat_hit:
			m_event->onError(L"Error Code 12");
			out << "Error Code 12" << endl;
			break;
		case BallStat_serve:
			m_event->onError(L"Error Code 22");
			out << "Error Code 22" << endl;
			break;
		case BallStat_touch_down:
			m_event->onError(L"Error Code 32");
			out << "Error Code 32" << endl;
			break;
		case BallStat_touch_net:
			m_event->onError(L"Error Code 42");
			out << "Error Code 42" << endl;
			break;
		case BallStat_out:
			m_event->onError(L"Error Code 62");
			out << "Error Code 62" << endl;
			break;
		case BallStat_in:
			m_event->onError(L"Error Code 92");
			out << "Error Code 92" << endl;
			break;
		case BallStat_hit_guess:
			m_event->onError(L"Error Code 102");
			out << "Error Code 102" << endl;
			break;
		default:
			break;
		}
	}
	else if(stat == BallStat_touch_down)
	{
		switch (m_上次Data.stat)
		{
		case BallStat_out:
			m_event->onError(L"Error Code 63");
			out << "Error Code 63" << endl;
			break;
		default:
			break;
		}
	}
	else if (stat == BallStat_touch_net)
	{
		switch (m_上次Data.stat)
		{
		case BallStat_touch_down:
			m_event->onError(L"Error Code 34");
			out << "Error Code 34" << endl;
			break;
		case BallStat_touch_net:
			m_event->onError(L"Error Code 44");
			out << "Error Code 44" << endl;
			break;
		case BallStat_out:
			m_event->onError(L"Error Code 64");
			out << "Error Code 64" << endl;
			break;
		default:
			break;
		}
	}
	//else if (stat == BallStat_out)
	//{
	//	switch (m_上次Data.stat)
	//	{
	//	case:
	//		break;
	//	case:
	//		break;
	//	case:
	//		break;
	//	case:
	//		break;
	//	default:
	//		break;
	//	}
	//}
	else if (stat == BallStat_in)
	{
		switch (m_上次Data.stat)
		{
		case BallStat_hit:
			m_event->onError(L"Error Code 19");
			out << "Error Code 19" << endl;
			break;
		case BallStat_serve:
			m_event->onError(L"Error Code 29");
			out << "Error Code 29" << endl;
			break;
		case BallStat_touch_down:
			m_event->onError(L"Error Code 39");
			out << "Error Code 39" << endl;
			break;
		case BallStat_touch_net:
			m_event->onError(L"Error Code 49");
			out << "Error Code 49" << endl;
			break;
		case BallStat_in:
			m_event->onError(L"Error Code 99");
			out << "Error Code 99" << endl;
			break;
		case BallStat_hit_guess:
			m_event->onError(L"Error Code 109");
			out << "Error Code 109" << endl;
			break;
		default:
			break;
		}
	}
	else if (stat == BallStat_hit_guess)
	{
		switch (m_上次Data.stat)
		{
		case BallStat_hit:
			m_event->onError(L"Error Code 110");
			out << "Error Code 110" << endl;
			break;
		case BallStat_serve:
			m_event->onError(L"Error Code 210");
			out << "Error Code 210" << endl;
			break;
		case BallStat_touch_down:
			m_event->onError(L"Error Code 310");
			out << "Error Code 310" << endl;
			break;
		case BallStat_touch_net:
			m_event->onError(L"Error Code 410");
			out << "Error Code 410" << endl;
			break;
		case BallStat_in:
			m_event->onError(L"Error Code 910");
			out << "Error Code 910" << endl;
			break;
		case BallStat_hit_guess:
			m_event->onError(L"Error Code 1010");
			out << "Error Code 1010" << endl;
			break;
		default:
			break;
		}
	}
	else
	{
		;
	}

	//if ( m_isRecord == 1 && 
	//	(stat != BallStat_in || stat != BallStat_hit_guess || stat != BallStat_null)
	//    )
	//{
	//	if (out.is_open())
	//	{
	//		out << "isRecord 2" << endl;
	//		out.close();
	//	}
	//	m_isRecord = 2;
	//	//return;
	//}

	// TODO From Joe:
	// 事件6发生后，长时间收不到事件的处理，是否要加入定时器来处理？？？
	double s = m_t.elapsed_second();
	if ( m_isRecord == 1 && 
		(
			(s > 2.0 && !m_tmpLT) 
			||
			(s > 1.0 && m_tmpLT)
		)
		&& m_上次Data.stat == BallStat_out )
	{
		if (out.is_open())
		{
			out << "m_isRecord 1 down to floor out, >=1100: " << m_tmpLT << " second:" << s << endl;
			out.close();
		}

		m_上次Data.stat = m_lastStatWhenout.stat;
		m_上次Data.ball = m_lastStatWhenout.ball;
		m_isRecord = 0;
		// 作为界外落地处理
		m_ball.x = m_场地A.start.x- 底线给事件6的直接超时距离;
		m_ball.y = m_场地A.start.y- 底线给事件6的直接超时距离;
		BallStatDown(m_ball);
		m_ballTouch_down = m_ball;

		return;
	}


	if (stat == BallStat_null)
	{
		return;
	}
	else if (stat == BallStat_hit || stat == BallStat_hit_guess) //击球
	{
		// TODO From Joe:
		// 事件6发生之后在1s内发生了事件11(包含事件1？)的处理，if |x| >= 100 && 1s内出现事件11,则忽略
		if ( m_isRecord == 1 &&
			 s <= 1.0 && 
			 //m_t.elapsed_second() < 1.0 &&
			 m_tmpLT && 
			 m_上次Data.stat == BallStat_out && 
			 stat == BallStat_hit_guess)
		{
			if (out.is_open())
			{
				out << "m_isRecord 1 ignore from 10, >=1100: " << m_tmpLT << " second:" << s << endl;
				out.close();
			}
			// 忽略
			m_isRecord = 0;
			//m_tmpLT = false;
			m_上次Data.stat = m_lastStatWhenout.stat;
			m_上次Data.ball = m_lastStatWhenout.ball;
		}

		if (m_is无效数据)
			return;

		if ((m_ball.x > 0 && m_击球者 == 1) || //在A场地这边第二次击球？
			(m_ball.x < 0 && m_击球者 == 2) )
		{
			////连续2次击球,直接对方得分
			//对方加分();

			//后来改成，连续2次击球,直接自己得分
			自己加分();
			return;
		}
		
		m_击球次数++;
		if (m_is发球)
		{
			m_is发球 = false;
			if (m_第几次发球 == 1)
			{
				if (m_击球者 == 1)
				{
					m_event->on发球Pos(m_击球者, 1, m_发球成功Postion);
				}
				if (m_击球者 == 2)
				{
					m_event->on发球Pos(m_击球者, 1, m_发球成功Postion);
				}
			}
			if (m_第几次发球 == 2)
			{
				if (m_击球者 == 1)
				{
					m_event->on发球Pos(m_击球者, 2, m_发球成功Postion);
				}
				if (m_击球者 == 2)
				{
					m_event->on发球Pos(m_击球者, 2, m_发球成功Postion);
				}
			}
		}

		//如果是击球的话就交换击球者，不管在哪击球的
		if (m_击球者 == 1)
		{
			m_击球者 = 2;
			m_player2.击球次数++;

			//ofstream fout("A击球点.txt", ios::app);
			//if (fout)
			//{
			//	fout << m_ball.x;
			//	fout << ",";
			//	fout << m_ball.y;
			//	fout << ";";
			//	fout.close();
			//}
			//// TODO: 考虑击球点数据的收集
			////if (m_ball.x >= -11887 && m_ball.x <= 0)
			////	m_player1.场内击球次数++;
			////else if (m_ball.x >= -13887 && m_ball.x <= -11887)
			////	m_player1.底线击球次数++;
			////else if (m_ball.x >= -15887 && m_ball.x <= -13887)
			////	m_player1.场外击球次数++;

			//if (m_ball.x >= 0 && m_ball.x <= 11887)
			//	m_player1.场内击球次数++;
			//else if (m_ball.x > 11887 && m_ball.x <= 13887)
			//	m_player1.底线击球次数++;
			//else if (m_ball.x > 13887 && m_ball.x <= 15887)
			//	m_player1.场外击球次数++;

			ofstream fout("B击球点.txt", ios::app);
			if (fout)
			{
				fout << m_ball.x;
				fout << ",";
				fout << m_ball.y;
				fout << ";";
				fout.close();
			}

			// TODO: 考虑击球点数据的收集
			//if (m_ball.x >= -11887 && m_ball.x <= 0)
			//	m_player2.场内击球次数++;
			//else if (m_ball.x >= -13887 && m_ball.x <= -11887)
			//	m_player2.底线击球次数++;
			//else if (m_ball.x >= -15887 && m_ball.x <= -13887)
			//	m_player2.场外击球次数++;

			if (m_ball.x >= -11887 && m_ball.x <= 0)
				m_player2.场内击球次数++;
			else if (m_ball.x >= -13887 && m_ball.x < -11887)
				m_player2.底线击球次数++;
			else if (m_ball.x >= -15887 && m_ball.x < -13887)
				m_player2.场外击球次数++;

		}	
		else if (m_击球者 == 2)
		{
			m_击球者 = 1;
			m_player1.击球次数++;

			//ofstream fout("B击球点.txt", ios::app);
			//if (fout)
			//{
			//	fout << m_ball.x;
			//	fout << ",";
			//	fout << m_ball.y;
			//	fout << ";";
			//	fout.close();
			//}

			//// TODO: 考虑击球点数据的收集
			////if (m_ball.x >= -11887 && m_ball.x <= 0)
			////	m_player2.场内击球次数++;
			////else if (m_ball.x >= -13887 && m_ball.x <= -11887)
			////	m_player2.底线击球次数++;
			////else if (m_ball.x >= -15887 && m_ball.x <= -13887)
			////	m_player2.场外击球次数++;



			//if (m_ball.x >= -11887 && m_ball.x <= 0)
			//	m_player1.场内击球次数++;
			//else if (m_ball.x >= -13887 && m_ball.x < -11887)
			//	m_player1.底线击球次数++;
			//else if (m_ball.x >= -15887 && m_ball.x < -13887)
			//	m_player1.场外击球次数++;


			ofstream fout("A击球点.txt", ios::app);
			if (fout)
			{
				fout << m_ball.x;
				fout << ",";
				fout << m_ball.y;
				fout << ";";
				fout.close();
			}
			// TODO: 考虑击球点数据的收集
			//if (m_ball.x >= -11887 && m_ball.x <= 0)
			//	m_player1.场内击球次数++;
			//else if (m_ball.x >= -13887 && m_ball.x <= -11887)
			//	m_player1.底线击球次数++;
			//else if (m_ball.x >= -15887 && m_ball.x <= -13887)
			//	m_player1.场外击球次数++;

			if (m_ball.x >= 0 && m_ball.x <= 11887)
				m_player1.场内击球次数++;
			else if (m_ball.x > 11887 && m_ball.x <= 13887)
				m_player1.底线击球次数++;
			else if (m_ball.x > 13887 && m_ball.x <= 15887)
				m_player1.场外击球次数++;
		}
			

		m_event->onHitBallPos(m_击球者, m_ball);

		m_inSpectDirection = true;
		m_HitBall = m_ball;
		m_isTouch_net = false;
		//检查击球位置，记录同一场地内是第几次击球
		if (m_HitBallArea == BallHitArea(m_HitBall))
		{//如果上次击球的位置跟这次 击球的位置相同 开启检查
			m_isSpectTwohit = true;
		}
		else
		{
			m_HitBallNumA = 0;
			m_HitBallNumB = 0;
		}
		////球在网1米范围内
		//if (m_HitBallArea != BallHitArea(m_HitBall) && m_HitBall.x > -1000 && m_HitBall.x < 1000)
		//{
		//	m_isSpectDirection = true;
		//}
		m_HitBallArea = BallHitArea(m_HitBall);
		if (m_HitBallArea == 1)
		{
			m_HitBallNumA++;
		}
		if (m_HitBallArea == 2)
		{
			m_HitBallNumB++;
		}

	}
	else if (stat == BallStat_serve)
	{
		//发球
		m_击球者 = m_BallPower;
		m_BallServeArea = 在哪区(m_ball);
		if ((m_BallPower == 1 && m_ball.x > 0) || (m_BallPower == 2 && m_ball.x < 0))
		{
			m_is无效数据 = false;
			m_event->onError(L"比赛进行中"); // 恢复UI层错误显示
			// TODO　From Joe:
			//m_event->on发球(m_ball);
			//on发球(m_ball);
			on发球(m_ball);
			m_event->on发球(m_ball);
			


			//最高发球速度
			if (m_击球者 == 1)
			{
				if (m_ball.speed > m_player1.speedMax)
					m_player1.speedMax = m_ball.speed;

				// m_击球者 = 2;
			}
			if (m_击球者 == 2)
			{
				if (m_ball.speed > m_player2.speedMax)
					m_player2.speedMax = m_ball.speed;

				// m_击球者 = 1;
			}
		}
		else
		{
			m_is无效数据 = true;

			// TODO:检测出了问题，是否需要通知底层已经出问题了
			// TDOD From Joe: 这里需要调整执行逻辑流程
			// stop();
			//pause();
			m_isPause = true;

			m_event->onError(L"无效数据,请检查当前球权跟数据是否匹配");
			//Plug::PlugMessageBox(L"无效数据,请检查当前球权跟数据是否匹配!!!");// 这句会改变执行流程，导致程序崩溃
			return;
		}
		m_is发球 = true;
		m_isTouch_net = false;

		m_第几次发球++;
		if (m_第几次发球 == 1)
		{
			if (m_BallPower == 1)
			{
				m_player1.n1Total++;

				//// 一区外侧
				//if (m_发球成功Postion.x <= 0 && m_发球成功Postion.x >= -6401 && m_发球成功Postion.y <= -2059 && m_发球成功Postion.y >= -4115)
				//{
				//	m_player1.一区外侧次数++;
				//} // 一区内侧
				//else if (m_发球成功Postion.x <= 0 && m_发球成功Postion.x >= -6401 && m_发球成功Postion.y <= 0 && m_发球成功Postion.y >= -2058)
				//{
				//	m_player1.一区内侧次数++;
				//}// 二区外侧
				//else if (m_发球成功Postion.x <= 0 && m_发球成功Postion.x >= -6401 && m_发球成功Postion.y <= 4115 && m_发球成功Postion.y >= 2059)
				//{
				//	m_player1.二区外侧次数++;
				//} // 二区内侧
				//else if (m_发球成功Postion.x <= 0 && m_发球成功Postion.x >= -6401 && m_发球成功Postion.y <= 2058 && m_发球成功Postion.y >= 0)
				//{
				//	m_player1.二区内侧次数++;
				//}
				//else
				//{
				//	;
				//}
			}
			
			if (m_BallPower == 2)
			{
				m_player2.n1Total++;

				//if (m_发球成功Postion.x >= 0 && m_发球成功Postion.x <= 6451 && m_发球成功Postion.y >= 2059 && m_发球成功Postion.y <= 4165)
				//{
				//	m_player2.一区外侧次数++;
				//} // 一区内侧
				//else if (m_发球成功Postion.x >= 0 && m_发球成功Postion.x <= 6451 && m_发球成功Postion.y >= -50 && m_发球成功Postion.y <= 2058)
				//{
				//	m_player2.一区内侧次数++;
				//}// 二区外侧
				//else if (m_发球成功Postion.x >= 0 && m_发球成功Postion.x <= 6451 && m_发球成功Postion.y >= -4165 && m_发球成功Postion.y <= -2059)
				//{
				//	m_player2.二区外侧次数++;
				//} // 二区内侧
				//else if (m_发球成功Postion.x >= 0 && m_发球成功Postion.x <= 6451 && m_发球成功Postion.y >= -2058 && m_发球成功Postion.y <= 50)
				//{
				//	m_player2.二区内侧次数++;
				//}
				//else
				//{
				//	;
				//}
			}

		}
		if (m_第几次发球 == 2)
		{
			if (m_BallPower == 1)
			{
				m_player1.n2Total++;

				//// 一区外侧
				//if (m_发球成功Postion.x <= 0 && m_发球成功Postion.x >= -6451 && m_发球成功Postion.y <= -2059 && m_发球成功Postion.y >= -4165)
				//{
				//	m_player1.一区外侧次数++;
				//} // 一区内侧
				//else if (m_发球成功Postion.x <= 0 && m_发球成功Postion.x >= -6451 && m_发球成功Postion.y <= 50 && m_发球成功Postion.y >= -2058)
				//{
				//	m_player1.一区内侧次数++;
				//}// 二区外侧
				//else if (m_发球成功Postion.x <= 0 && m_发球成功Postion.x >= -6451 && m_发球成功Postion.y <= 4165 && m_发球成功Postion.y >= 2059)
				//{
				//	m_player1.二区外侧次数++;
				//} // 二区内侧
				//else if (m_发球成功Postion.x <= 0 && m_发球成功Postion.x >= -6451 && m_发球成功Postion.y <= 2058 && m_发球成功Postion.y >= -50)
				//{
				//	m_player1.二区内侧次数++;
				//}
				//else
				//{
				//	;
				//}
			}

			if (m_BallPower == 2)
			{
				m_player2.n2Total++;
			}
		}



		//m_tForTimeOut.reset();

	}
	else if (stat == BallStat_out) // 事件6出视野
	{
		// TODO From Joe:
		if (abs(m_ball.x) == 底线给事件6的直接超时距离)
		{
			if (out.is_open())
			{
				out << "底线给事件6的直接超时距离事件 x：" << m_ball.x << " y: " << m_ball.y << " z:" << m_ball.z << endl;
				out.close();
			}
			BallStatDown(m_ball);
			m_ballTouch_down = m_ball;

			m_isRecord = 0;// 此时事件6处理已经结束

			return;
		}
		
		m_isRecord = 1;

		if (abs(m_ball.x) >= 底线给事件6的临界坐标)
			m_tmpLT = true;
		else
			m_tmpLT = false;

		m_t.reset();

		if (out.is_open())
		{
			out << "m_isRecord 1 event from 6 >=1100 " << m_tmpLT << endl;
			out.close();
		}

		m_lastStatWhenout.stat = m_上次Data.stat;
		m_lastStatWhenout.ball = m_上次Data.ball;
		return;
	}
	else if (stat == BallStat_touch_down)
	{
		// 判定为界外落地
		//落地
		BallStatDown(m_ball);
		m_ballTouch_down = m_ball;
	}
	else if (stat == BallStat_touch_net)
	{
		int a = 0;
	}
	else if (stat == BallStat_touch_any)
	{
		//if (m_is发球)
		//{
		//	m_is失误 = true;
		//	m_isTouch_net = true;
		//}
	}
	//else if (stat == BallStat_out)
	//{

	//}
	else if (stat == BallStat_non_uniqueness)
	{
	}
	else if (stat == BallStat_error)
	{
	}
	else if (stat == BallStat_in)
	{
		// TODO From Joe:
		// if |x| < 100 && 2s 内出现事件9,则忽略
		if ( m_isRecord == 1 && 
			 s <= 2.0 &&
			 !m_tmpLT && 
			 m_上次Data.stat == BallStat_out
		   )
		{
			if (out.is_open())
			{
				out << "m_isRecord 1 ignore from 9, >=100: " << m_tmpLT << " second:" << s << endl;
				out.close();
			}
			// 忽略
			m_isRecord = 0;
			m_上次Data.stat = m_lastStatWhenout.stat;
			m_上次Data.ball = m_lastStatWhenout.ball;
			//return;
		}
	}
}
