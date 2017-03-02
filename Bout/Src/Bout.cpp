#include "Bout.h"

PLUG_COMPONENT_AUTO_REG(Bout)//DO NOT EDIT THIS

//------------------------------------------------------------------------------------------------------------------
Bout::Bout()
{
	m_击球者 = 1;
	m_is单打 = true;

	// TODO From Joe:
	m_tmpLT = false;
	m_t.reset();
	m_isRecord = 0;
	m_isPause = false;
}

Bout::~Bout()
{
}

I_TennisVideo* GetInterfaceImp()
{
	auto path = Plug::GetCurrentPath();
	HMODULE hDLL = LoadLibrary((path + L"Tennis_DLL.dll").c_str());
	if (hDLL != nullptr)
	{
		auto p = (GetInterface)GetProcAddress(hDLL, "getInterface");
		if (p != nullptr)
		{
			return p();
		}
	}
	return nullptr;
}

BOOL IsFileExist(LPCWSTR pwhPath)
{
	if (!pwhPath && *pwhPath == L' ')
		return FALSE;
	DWORD dwFileAttr = GetFileAttributesW(pwhPath);
	if ((DWORD)(-1) == dwFileAttr)
		return FALSE;
	return TRUE;
}
BOOL TryDeleteFile(LPCWSTR pszFile)
{
	if (!pszFile)
		return FALSE;

	// 文件不存在
	if (!IsFileExist(pszFile))
		return FALSE;

	if (!::DeleteFileW(pszFile))
	{
		if (::MoveFileEx(pszFile, NULL, MOVEFILE_DELAY_UNTIL_REBOOT))
		{
			// 文件暂时无法删除，待重启后删除
			return TRUE;
		}
		else
		{
			// 文件删除操作失败:[%s]"
			return TRUE;
		}
	}
	return TRUE;
}


//------------------------------------------------------------------------------------------------------------------
void * __stdcall Bout::init(I_BoutEvent* event, bool is单打, void * dllHandler)
{
	TryDeleteFile(L"Aace球落点.txt");
	TryDeleteFile(L"Bace球落点.txt");
	TryDeleteFile(L"A一发落点.txt");
	TryDeleteFile(L"B一发落点.txt");
	TryDeleteFile(L"A二发落点.txt");
	TryDeleteFile(L"B二发落点.txt");
	TryDeleteFile(L"A击球点.txt");
	TryDeleteFile(L"B击球点.txt");

	m_is单打 = is单打;
	m_event = event;
	
	//m_tv->onBallStat = [this](BallStat stat)
	//{
	//};
	// 判空处理
	if (dllHandler != NULL)
	{
		m_tv = (I_TennisVideo*) dllHandler;
	} else {
		// TODO: Do Global Log
		m_tv = GetInterfaceImp();
		if (m_tv == NULL)
		{
			::MessageBox(NULL, L"错误提示，请检查摄像头驱动是否已经安装!", L"错误提示", MB_OK);
			return NULL;
		}

	}
		
	m_tv->onBall = [this](BallStat stat, Ball ball)
	{
		if (stat != BallStat_serve)//不发球，就检查是否无效数据
		{
			if (m_is无效数据)
				return;
		}

		//以下处理坐标问题
		ball.x *= 10;
		ball.y *= 10;
		ball.z *= 10;

		m_event->onBall(ball);
		if (onBall)
			onBall(ball);
		m_event->onBallStat(stat);


		//坐标系转换
		//__int64 e = m_ball.timestamp - ball.timestamp;
		//e = e / 1000;
		m_ball = ball;
		m_stat = stat;
		HandleStat(m_stat);

		if (stat != BallStat_null)
		{
			m_上次Data.ball = m_ball;
			m_上次Data.stat = stat;
		}

		//如果击球则开启方向的检查(击球事件过了的下一个坐标了)
		if (m_inSpectDirection)
		{
			m_inSpectDirection = false;
			if (m_BallDirection1 == 0)
			{
				m_BallDirection1 = BallDirection(ball);
				return;
			}
			if (m_BallDirection2 == 0)
			{
				m_BallDirection2 = BallDirection(ball);
				return;
			}
			m_BallDirection1 = m_BallDirection2;
			m_BallDirection2 = BallDirection(ball);
		}
		if (m_isSpectTwohit)
		{
			BallTwoHitSpect();
			m_isSpectTwohit = false;
		}
		//if (m_isSpectDirection&&m_BallDirection2 != 0)
		//{
		//	if (m_BallDirection1 == m_BallDirection2)
		//	{
		//		自己加分();
		//	}
		//}
	};
	m_tv->onError = [this](const wchar_t* err)
	{
		m_event->onError(err);
	};

	initAllData();//新的一回合，数据重置有效
	
	return m_tv;
}
//开启摄像头，开始发送事件坐标
bool __stdcall Bout::start()
{
	return ( m_tv == NULL ?  false : (m_tv->start()) );
	//return hr;
}
bool __stdcall Bout::stop()
{
	(m_tv == NULL ?  NULL : (m_tv->stop()) );
	//delete m_tv;
	m_tv = NULL;
	return true;

	//m_tv->stop();
	//return true;
}
//具体球场的大小 ，现在有默认值，在构造会赋值，以后有了具体数据 在这里赋值
void __stdcall Bout::GroundSize(int Lf, int Ld, int Lw, int Dd, int Ds, int Dw)
{
	m_gound.Dd = Dd;											//单人场地y轴范围
	m_gound.Ds = Ds;                                            //双人场地y轴范围
	m_gound.Dw = Dw;											//场地摄像头y轴范围
	m_gound.Lf = Lf;											//单人场地x轴范围
	m_gound.Ld = Ld;											//双人场地y轴范围
	m_gound.Lw = Lw;											//场地摄像头y轴范围


																////从左往右 ->
																//m_场地A.start.x = 0 - Ld;
																//m_场地A.start.y = 0 - Dd;
																//m_场地A.end.x = 0;
																//m_场地A.end.y = 0 + Dd;

																//m_场地B.start.x = 0;
																//m_场地B.start.y = 0 - Dd;
																//m_场地B.end.x = 0 + Ld;
																//m_场地B.end.y = 0 + Dd;

																//从右往左 <-
	if (m_is单打)
	{
		m_场地A.start.x = Ld;
		m_场地A.start.y = 0 - Dd;
		m_场地A.end.x = 0;
		m_场地A.end.y = 0 + Dd;

		m_场地B.start.x = 0;
		m_场地B.start.y = 0 - Dd;
		m_场地B.end.x = 0 - Ld;
		m_场地B.end.y = 0 + Dd;
	}
	else
	{
		m_场地A.start.x = Ld;
		m_场地A.start.y = 0 - Ds;
		m_场地A.end.x = 0;
		m_场地A.end.y = 0 + Ds;

		m_场地B.start.x = 0;
		m_场地B.start.y = 0 - Ds;
		m_场地B.end.x = 0 - Ld;
		m_场地B.end.y = 0 + Ds;
	}

}
////飞出视野之外，计算1秒之后 不在等待 开始计算分数
//void __stdcall TennisVideo::Time()
//{
//	对方加分();
//}


bool Bout::is发球出界2(Ball ball)
{
	//(m_player1.score+ m_player2.score)
	auto bout = GetPlug(Umpire);
	if (bout->是否抢七())
	{
		// 抢七局： 如果发球人是A，则总分为n=0+4k(k=0,1,2...)时，应该落入1区不是出界，否则出界
		//                           总分为n=1+4k(k=0,1,2...)时，应该落入2区不是出界，否则出界
		//          如果发球人是B，则总分为n=1+4k(k=0,1,2...)时，应该落入1区不是出界，否则出界
		//                           总分为n=3+4k(k=0,1,2...)时，应该落入2区不是出界，否则出界
		if ((m_player1.score + m_player2.score) % 4 == 0)
		{
			// 落入1区则界内，否则出界
			if (ball.x >= 0 && ball.x <= 11887 && ball.y >= 0 && ball.y <= 4115)
			{
				return false;
			}
			return true;
		}
		else if ((m_player1.score + m_player2.score) % 4 == 1)
		{
			// 落入2区则界内，否则出界
			if (ball.x >= 0 && ball.x <= 11887 && ball.y <= 0 && ball.y >= -4115)
			{
				return false;
			}
			else
			{
				return false;
			}
		}
		else if ((m_player1.score + m_player2.score) % 4 == 2)
		{
			// 落入1区则界内，否则出界
			if (ball.x >= 0 && ball.x <= 11887 && ball.y >= 0 && ball.y <= 4115)
			{
				return false;
			}
			return true;
		}
		else if ((m_player1.score + m_player2.score) % 4 == 3)
		{
			// 落入2区则界内，否则出界
			if (ball.x >= 0 && ball.x <= 11887 && ball.y <= 0 && ball.y >= -4115)
			{
				return false;
			}
			else
			{
				return false;
			}
		}
	}
	else
	{

		// 非抢七局： 如果发球人是A，则总分为偶数时，应该落入1区不是出界，否则出界
		//                             总分为奇数时，应该落入2区不是出界，否则出界
		if ((m_player1.score + m_player2.score) % 2 == 0)
		{
			// 落入1区则界内，否则出界
			if (ball.x >= 0 && ball.x <= 11887 && ball.y >= 0 && ball.y <= 4115)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else
		{
			// 落入2区则界内，否则出界
			if (ball.x >= 0 && ball.x <= 11887 && ball.y <= 0 && ball.y >= -4115)
			{
				return false;
			}
			else
			{
				return false;
			}
		}

	}





	return true;
}
//是否落在界外true是落在界外
bool Bout::is发球出界(Ball ball)
{
	switch (m_BallServeArea)
	{
	case 1:
		if (ball.x > 0 || ball.y > 0 || ball.x < -m_gound.Lf || ball.y < -m_gound.Dd)
		{
			return true;
		}
		break;
	case 2:
		if (ball.x > 0 || ball.y < 0 || ball.x < -m_gound.Lf || ball.y > m_gound.Dd)
		{
			return true;
		}
		break;
	case 3:
		if (ball.x < 0 || ball.y > 0 || ball.x > m_gound.Lf || ball.y < -m_gound.Dd)
		{
			return true;
		}
		break;
	case 4:
		if (ball.x < 0 || ball.y < 0 || ball.x > m_gound.Lf || ball.y > m_gound.Dd)
		{
			return true;
		}
		break;
	case 5:
		if (ball.x > 0 || ball.y < -m_gound.Dd || ball.y > m_gound.Dd)
		{
			return true;
		}
		break;
	case 6:
		if (ball.x < 0 || ball.y < -m_gound.Dd || ball.y > m_gound.Dd)
		{
			return true;
		}
		break;
	default:
		break;
	}
	////将来或许还有双人 边界范围不一样，暂时 只有一套单人
	//if (ball.x > 0 && ball.y > 0)
	//{
	//	if (ball.x > m_gound.Ld || ball.y > m_gound.Dd)
	//	{
	//		return true;
	//	}	
	//}
	//if (ball.x > 0 && ball.y < 0)
	//{
	//	if (ball.x > m_gound.Ld || ball.y < m_gound.Dd)
	//	{
	//		return true;
	//	}
	//}
	//if (ball.x <0 && ball.y>0)
	//{
	//	if (ball.x < m_gound.Ld || ball.y > m_gound.Dd)
	//	{
	//		return true;
	//	}
	//}
	//if (ball.x < 0 && ball.y < 0)
	//{
	//	if (ball.x < m_gound.Ld || ball.y < m_gound.Dd)
	//	{
	//		return true;
	//	}
	//}
	return false;

}

//根据球的上一次坐标，和当前坐标计算方向(1是向A场，2是向B场，3是剩余情况)
int Bout::BallDirection(Ball ball)
{
	int a = 0;
	if (ball.x < m_ball.x)
	{
		a = 1;
	}
	if (ball.x > m_ball.x)
	{
		a = 2;
	}
	if (ball.x == m_ball.x)
	{
		a = 3;
	}
	//传来的坐标单位是cm，实际球场坐标单位是mm
	m_ball.x = ball.x;
	m_ball.y = ball.y;
	m_ball.z = ball.z;
	return a;
}
//上一个方向 跟击球完的下一个方向比较 
void Bout::BallHitInspect(Ball ball)
{
	if (m_BallDirection1 != BallDirection(ball))
	{//如果不相同 说明玩家击球并且改变了方向  检查球的位置是否还在同一场地内，如果 还在 说明 对方过网击球
		BallHitArea(m_HitBall);
		//if (m_HitBallNumA>=2)
		//if (m_HitBallNumB>=2)
	}
	else
	{//如果方向相同就需要进一步检查是否同一个人击球了两次   检查球的位置是否还在同一场地内 ，或者在对方场地内一米 

	}
}

int Bout::BallHitArea(Ball ball)
{
	//is范围内()
	//if (ball.x < 0 && -m_gound.Dd < ball.y < m_gound.Dd)
	pos2d pos;
	pos.x = ball.x;
	pos.y = ball.y;
	if (is范围内(pos, m_场地A))
	{
		return  1;
	}
	//if (ball.x > 0 && -m_gound.Dd < ball.y < m_gound.Dd)
	if (is范围内(pos, m_场地B))
	{
		return  2;
	}
	return 3;
}

void Bout::BallTwoHitA()
{//A场地连续两次击球
	if (-1000 < m_HitBall.x && m_HitBall.x < 0 && m_BallDirection2 == 1)
	{//在对方场内击球
		自己加分();
	}
	if (m_HitBall.x < 0 && m_BallDirection2 == 2)
	{//连续两次击球
		对方加分();
	}
}
void Bout::BallTwoHitB()
{//B场地连续两次击球
	if (m_HitBall.x > 0 && m_HitBall.x < 1000 && m_BallDirection2 == 2)
	{
		对方加分();
	}
	if (m_HitBall.x > 0 && m_BallDirection2 == 1)
	{//连续两次击球
		自己加分();
	}
}
void Bout::BallTwoHitSpect()
{
	if (m_HitBallNumA >= 2)
	{
		BallTwoHitA();
	}
	if (m_HitBallNumB >= 2)
	{
		BallTwoHitB();
	}
}

void __stdcall Bout::BallPower(int power)
{
	m_BallPower = power;
}


const static NowState StateTable[11][11] = { State_Unknown_Error };

NowState Bout::GetNextState(BallStat prevoisState, BallStat nowState)
{
	return StateTable[nowState][prevoisState];
}