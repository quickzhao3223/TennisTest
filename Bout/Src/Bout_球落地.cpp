#include "Bout.h"
#include <ctime>

std::string Aace球落点;
std::string A一发落点;
std::string A二发落点;
std::string A击球点;
std::string Bace球落点;
std::string B一发落点;
std::string B二发落点;
std::string B击球点;

//void Bout::GetAce落点String(std::string aace, std::string bace)
//{
//	//aace = Aace球落点;
//	//bace = Bace球落点;
//
//	ifstream fin("Aace球落点.txt");
//	aace = "";
//	std::string strT="";
//	if (fin.is_open()) 
//	{
//		fin >> strT;
//		aace += strT;
//		while (!fin.eof())
//		{
//			aace += strT;
//			fin >> strT;
//		}
//		fin.close();
//	}
//
//	ifstream fin2("Bace球落点.txt");
//	bace = "";
//	strT = "";
//	if (fin2.is_open())
//	{
//		fin2 >> strT;
//		bace += strT;
//		while (!fin2.eof())
//		{
//			bace += strT;
//			fin2 >> strT;
//		}
//		fin2.close();
//	}
//}
//void Bout::Get一发落点String(std::string a一发, std::string b一发)
//{
//	//a一发 = A一发落点;
//	//b一发 = B一发落点;
//
//	ifstream fin("A一发落点.txt");
//	a一发 = "";
//	std::string strT = "";
//	if (fin.is_open())
//	{
//		fin >> strT;
//		a一发 += strT;
//		while (!fin.eof())
//		{
//			a一发 += strT;
//			fin >> strT;
//		}
//		fin.close();
//	}
//
//	ifstream fin2("B一发落点.txt");
//	b一发 = "";
//	strT = "";
//	if (fin2.is_open())
//	{
//		fin2 >> strT;
//		b一发 += strT;
//		while (!fin2.eof())
//		{
//			b一发 += strT;
//			fin2 >> strT;
//		}
//		fin2.close();
//	}
//}
//void Bout::Get二发落点String(std::string a二发, std::string b二发)
//{
//	//a二发 = A二发落点;
//	//b二发 = B二发落点;
//
//	ifstream fin("A二发落点.txt");
//	a二发 = "";
//	std::string strT = "";
//	if (fin.is_open())
//	{
//		fin >> strT;
//		a二发 += strT;
//		while (!fin.eof())
//		{
//			a二发 += strT;
//			fin >> strT;
//		}
//		fin.close();
//	}
//
//	ifstream fin2("B二发落点.txt");
//	b二发 = "";
//	strT = "";
//	if (fin2.is_open())
//	{
//		fin2 >> strT;
//		b二发 += strT;
//		while (!fin2.eof())
//		{
//			b二发 += strT;
//			fin2 >> strT;
//		}
//		fin2.close();
//	}
//
//}
//void Bout::Get击球点String(std::string a击球点, std::string b击球点)
//{
//	//a击球点 = A击球点;
//	//b击球点 = B击球点;
//
//	ifstream fin("A击球点.txt");
//	a击球点 = "";
//	std::string strT = "";
//	if (fin.is_open())
//	{
//		fin >> strT;
//		a击球点 += strT;
//		while (!fin.eof())
//		{
//			a击球点 += strT;
//			fin >> strT;
//		}
//		fin.close();
//	}
//	
//	ifstream fin2("B击球点.txt");
//	b击球点 = "";
//	strT = "";
//	if (fin2.is_open())
//	{
//		fin2 >> strT;
//		b击球点 += strT;
//		while (!fin2.eof())
//		{
//			b击球点 += strT;
//			fin2 >> strT;
//		}
//		fin2.close();
//	}
//}


//球的落地事件
bool Bout::BallStatDown(Ball ball)
{
	//Point2D p(ball.x, ball.y);
	string strBallInfo;

	//发球的判断=======================================================================
	if (m_is发球)
	{//如果是发球
		if (m_is发球成功)
		{
			//if (m_上次Data.stat == BallStat_touch_down) //已经第二次以上落地球了
			//{
			//	if (BallTouchDownArea(m_ballTouch_down) == BallTouchDownArea(ball))//第二以上触地只要还在对方场地内，自己就加分
			//	{//还在对方场地内

			//		return true;
			//	}
			//}
			if (m_BallPower == 1)
			{
				m_player1.ace++;
				m_event->onBallAce(1);

				//m_player1.ace球落点.push_back(p);
				
				//IntToString(ball.x, strBallInfo);
				//Aace球落点 += strBallInfo;
				//Aace球落点 += ",";
				//IntToString(ball.y, strBallInfo);
				//Aace球落点 += strBallInfo;
				//Aace球落点 += ";";

				ofstream fout("Aace球落点.txt", ios::app);
				if(fout)
				{
					fout << m_发球成功Postion.x;
					fout << ",";
					fout << m_发球成功Postion.y;
 					fout << ";";
					fout.close();
				}

			}
			if (m_BallPower == 2)
			{
				m_player2.ace++;
				m_event->onBallAce(2);

				//m_player2.ace球落点.push_back(p);

				//IntToString(ball.x, strBallInfo);
				//Bace球落点 += strBallInfo;
				//Bace球落点 += ",";
				//IntToString(ball.y, strBallInfo);
				//Bace球落点 += strBallInfo;
				//Bace球落点 += ";";

				ofstream fout("Bace球落点.txt", ios::app);
				{
					fout << m_发球成功Postion.x;
					fout << ",";
					fout << m_发球成功Postion.y;
					fout << ";";
					fout.close();
				}

			}
			m_event->on发球Pos(m_击球者, 0, m_发球成功Postion);

			自己加分();
			return true;
		}

		bool 出界 = is发球出界(ball);
		//bool 出界 = is发球出界2(ball);

		bool is发球成功 = false;
		if (!出界)
		{
			if (m_上次Data.stat == BallStat_touch_net)//没出界，没触网
			{
				//无效,因触网等原因导致无效发球
				m_is无效数据 = true;//后面还要判断双误，不能返回
				m_第几次发球--;//本次发球计数无效
			}
			else
				is发球成功 = true;
		}

		if(is发球成功)
		{
			m_event->on发球成功();
			m_发球成功Postion = ball;
		
			m_is发球成功 = true;
		}
		else
		{
			m_event->on发球失败();//发球出界
			//无效,因触网等原因导致无效发球
			m_is无效数据 = true;//后面还要判断双误，不能返回
		}

		if (m_第几次发球 == 1)
		{
			if (is发球成功)
			{
				if (m_击球者 == 1)
				{
					m_player1.n1进球Num++;

					// 一区外侧
					if (m_发球成功Postion.x <= 0 && m_发球成功Postion.x >= -6401 && m_发球成功Postion.y <= -2059 && m_发球成功Postion.y >= -4115)
					{
						m_player1.一区外侧次数++;
					} // 一区内侧
					else if (m_发球成功Postion.x <= 0 && m_发球成功Postion.x >= -6401 && m_发球成功Postion.y <= 0 && m_发球成功Postion.y >= -2058)
					{
						m_player1.一区内侧次数++;
					}// 二区外侧
					else if (m_发球成功Postion.x <= 0 && m_发球成功Postion.x >= -6401 && m_发球成功Postion.y <= 4115 && m_发球成功Postion.y >= 2059)
					{
						m_player1.二区外侧次数++;
					} // 二区内侧
					else if (m_发球成功Postion.x <= 0 && m_发球成功Postion.x >= -6401 && m_发球成功Postion.y <= 2058 && m_发球成功Postion.y >= 0)
					{
						m_player1.二区内侧次数++;
					}
					//m_player1.一发落点.push_back(p);

					//IntToString(ball.x, strBallInfo);
					// += strBallInfo;
					//A一发落点 += ",";
					//IntToString(ball.y, strBallInfo);
					//A一发落点 += strBallInfo;
					//A一发落点 += ";";

					ofstream fout("A一发落点.txt", ios::app);
					if (fout)
					{
						fout << m_发球成功Postion.x;
						fout << ",";
						fout << m_发球成功Postion.y;
						fout << ";";
						fout.close();
					}

					m_event->on发球Pos(m_击球者, 1, m_发球成功Postion);
				}
				if (m_击球者 == 2)
				{
					m_player2.n1进球Num++;

					if (m_发球成功Postion.x >= 0 && m_发球成功Postion.x <= 6401 && m_发球成功Postion.y >= 2059 && m_发球成功Postion.y <= 4115)
					{
						m_player2.一区外侧次数++;
					} // 一区内侧
					else if (m_发球成功Postion.x >= 0 && m_发球成功Postion.x <= 6401 && m_发球成功Postion.y >= 0 && m_发球成功Postion.y <= 2058)
					{
						m_player2.一区内侧次数++;
					}// 二区外侧
					else if (m_发球成功Postion.x >= 0 && m_发球成功Postion.x <= 6401 && m_发球成功Postion.y >= -4115 && m_发球成功Postion.y <= -2059)
					{
						m_player2.二区外侧次数++;
					} // 二区内侧
					else if (m_发球成功Postion.x >= 0 && m_发球成功Postion.x <= 6401 && m_发球成功Postion.y >= -2058 && m_发球成功Postion.y <= 0)
					{
						m_player2.二区内侧次数++;
					}
					//m_player2.一发落点.push_back(p);

					//IntToString(ball.x, strBallInfo);
					// += strBallInfo;
					//B一发落点 += ",";
					//IntToString(ball.y, strBallInfo);
					//B一发落点 += strBallInfo;
					//B一发落点 += ";";

					ofstream fout("B一发落点.txt", ios::app);
					if (fout)
					{
						fout << m_发球成功Postion.x;
						fout << ",";
						fout << m_发球成功Postion.y;
						fout << ";";
						fout.close();
					}
					m_event->on发球Pos(m_击球者, 1, m_发球成功Postion);
				}
			}
		}
		if (m_第几次发球 == 2)
		{
			if (is发球成功)
			{
				if (m_击球者 == 1)
				{
					m_player1.n2进球Num++;

					if (m_发球成功Postion.x <= 0 && m_发球成功Postion.x >= -6451 && m_发球成功Postion.y <= -2059 && m_发球成功Postion.y >= -4165)
					{
						m_player1.一区外侧次数2++;
					} // 一区内侧
					else if (m_发球成功Postion.x <= 0 && m_发球成功Postion.x >= -6451 && m_发球成功Postion.y <= 50 && m_发球成功Postion.y >= -2058)
					{
						m_player1.一区内侧次数2++;
					}// 二区外侧
					else if (m_发球成功Postion.x <= 0 && m_发球成功Postion.x >= -6451 && m_发球成功Postion.y <= 4165 && m_发球成功Postion.y >= 2059)
					{
						m_player1.二区外侧次数2++;
					} // 二区内侧
					else if (m_发球成功Postion.x <= 0 && m_发球成功Postion.x >= -6451 && m_发球成功Postion.y <= 2058 && m_发球成功Postion.y >= -50)
					{
						m_player1.二区内侧次数2++;
					}
					//m_player1.二发落点.push_back(p);

					//IntToString(ball.x, strBallInfo);
					// += strBallInfo;
					//A二发落点 += ",";
					//IntToString(ball.y, strBallInfo);
					//A二发落点 += strBallInfo;
					//A二发落点 += ";";

					ofstream fout("A二发落点.txt", ios::app);
					if (fout)
					{
						fout << m_发球成功Postion.x;
						fout << ",";
						fout << m_发球成功Postion.y;
						fout << ";";
						fout.close();
					}
					m_event->on发球Pos(m_击球者, 2, m_发球成功Postion);

				}
				if (m_击球者 == 2)
				{
					m_player2.n2进球Num++;

					// 一区外侧
					if (m_发球成功Postion.x >= 0 && m_发球成功Postion.x <= 6451 && m_发球成功Postion.y >= 2059 && m_发球成功Postion.y <= 4165)
					{
						m_player2.一区外侧次数2++;
					} // 一区内侧
					else if (m_发球成功Postion.x >= 0 && m_发球成功Postion.x <= 6451 && m_发球成功Postion.y >= -50 && m_发球成功Postion.y <= 2058)
					{
						m_player2.一区内侧次数2++;
					}// 二区外侧
					else if (m_发球成功Postion.x >= 0 && m_发球成功Postion.x <= 6451 && m_发球成功Postion.y >= -4165 && m_发球成功Postion.y <= -2059)
					{
						m_player2.二区外侧次数2++;
					} // 二区内侧
					else if (m_发球成功Postion.x >= 0 && m_发球成功Postion.x <= 6451 && m_发球成功Postion.y >= -2058 && m_发球成功Postion.y <= 50)
					{
						m_player2.二区内侧次数2++;
					}
					//m_player2..push_back(p);

					//IntToString(ball.x, strBallInfo);
					//B二发落点 += strBallInfo;
					//B二发落点 += ",";
					//IntToString(ball.y, strBallInfo);
					//B二发落点 += strBallInfo;
					//B二发落点 += ";";

					ofstream fout("B二发落点.txt", ios::app);
					if (fout)
					{
						fout << m_发球成功Postion.x;
						fout << ",";
						fout << m_发球成功Postion.y;
						fout << ";";
						fout.close();
					}
					m_event->on发球Pos(m_击球者, 2, m_发球成功Postion);
				}
			}
			else
			{
				//第二次发球还不成功就立即算分
				if (m_击球者 == 1)
				{
					m_player1.双误++;
				}
				if (m_击球者 == 2)
				{
					m_player2.双误++;
				}
				对方加分();
			}
			return true;
		}
		return true;
	}

	//击球的判断==========================================================================================
	if (m_击球者 == 1)
	{
		//m_player1.击球点.push_back(p);

		//IntToString(ball.x, strBallInfo);
		// += strBallInfo;
		//A击球点 += ",";
		//IntToString(ball.y, strBallInfo);
		//A击球点 += strBallInfo;
		//A击球点 += ";";

		//ofstream fout("A击球点.txt",ios::app);
		//if (fout)
		//{
		//	fout << ball.x;
		//	fout << ",";
		//	fout << ball.y;
		//	fout << ";";
		//	fout.close();
		//}


		// TODO: 考虑击球点数据的收集
		//if (ball.x >= 0 && ball.x <= 11887)
		//	m_player1.场内击球次数++;
		//else if (ball.x >= 11887 && ball.x <= 13887)
		//	m_player1.底线击球次数++;
		//else if (ball.x >= 13887 && ball.x <= 15887)
		//	m_player1.场外击球次数++;

		if (m_上次Data.stat == BallStat_touch_down) //已经第二次以上落地球了
		{
			//判断落点是否在同侧
			if ((m_上次Data.ball.x * ball.x) < 0)
			{
				ofstream out("Bout.txt", ios::app);
				time_t now = time(0);
				tm* localtm = localtime(&now);
				m_event->onError(L"Error Code 33");
				out << "Error Code 33 " << asctime(localtm) << endl;
				out.close();
				//Error Code 33
				//return false;
			}
			//不管落到哪里都是自己加分，因为对方根本没碰到球
			m_player1.致胜分++;
			自己加分();
			return true;
		}

		pos2d pos;
		pos.x = ball.x;
		pos.y = ball.y;
		if (is范围内(pos, m_场地A))
		{
			对方加分();//自己失误，对方得分，不算制胜分，普通得分
			return true;
		}
		else if (is范围内(pos, m_场地B) == false)
		{
			//也没落在对方场地，应该出界了
			对方加分();//自己失误，对方得分，不算制胜分，普通得分
			return true;
		}

	}
	if (m_击球者 == 2)
	{
		//m_player2.击球点.push_back(p);

		//IntToString(ball.x, strBallInfo);
		//B击球点 += strBallInfo;
		//B击球点 += ",";
		//IntToString(ball.y, strBallInfo);
		// += strBallInfo;
		//B击球点 += ";";

		//ofstream fout("B击球点.txt", ios::app);
		//if (fout)
		//{
		//	fout << ball.x;
		//	fout << ",";
		//	fout << ball.y;
		//	fout << ";";
		//	fout.close();
		//}

		//// TODO: 考虑击球点数据的收集
		//if (ball.x >= -11887 && ball.x <= 0)
		//	m_player2.场内击球次数++;
		//else if (ball.x >= -13887 && ball.x <= -11887)
		//	m_player2.底线击球次数++;
		//else if (ball.x >= -15887 && ball.x <= -13887)
		//	m_player2.场外击球次数++;

		if (m_上次Data.stat == BallStat_touch_down) //已经第二次以上落地球了
		{
			//判断落点是否在同侧
			if ((m_上次Data.ball.x * ball.x) < 0)
			{
				ofstream out("Bout.txt", ios::app);
				time_t now = time(0);
				tm* localtm = localtime(&now);
				m_event->onError(L"Error Code 33");
				out << "Error Code 33 " << asctime(localtm) << endl;
				out.close();

				//return false;
			}

			//不管落到哪里都是自己加分，因为对方根本没碰到球
			m_player2.致胜分++;
			自己加分();
			return true;
		}

		pos2d pos;
		pos.x = ball.x;
		pos.y = ball.y;
		if (is范围内(pos, m_场地B))
		{
			//落在自己场地
			对方加分(); //自己失误，对方得分，不算制胜分，普通得分
			return true;
		}
		else if (is范围内(pos, m_场地A) == false)
		{
			//也没落在对方场地，应该出界了
			对方加分(); //自己失误，对方得分，不算制胜分，普通得分
			return true;
		}
	}
	return true;
}
