#include "recorder.h"

CMovementRecorder MovementRecorder;

CMovementRecorder::CMovementRecorder()
{
	Ticks = 0;
	FindStart = false;

	Movements = new CRecord[MaxRecordTicks];
	DrawPath = new Vector[MaxRecordTicks];
	TempCmd = new SRecord[MaxRecordTicks];


	Situation = NOTHING;
	AutoPlayer = false;
}

float  GetSpeedByDistance(float distance)
{
	if (distance > 8.5)
		return 250;
	if (distance > 5.5)
		return 100;
	if (distance > 3.5)
		return 70;
	if (distance > .5)
		return 30;

	return 20;
}

void CMovementRecorder::SmootherAngles(C_BasePlayer* LocalPlayer)
{
	if (Situation != PLAYING) return;

	if (ConVars.setangleyinm->GetBool() && !(ConVars.angle_y_in_m->m_cValue[0] == 'm'))
	{
		QAngle Angle = Movements[Ticks + 1].viewangles;
		NormalizeAngles(Angle);

		static QAngle Interpolated;
		InterpolateAngles(Interpolated, Angle, Interpolated, 1.0f - ConVars.correctangle_inm->GetFloat());
		NormalizeAngles(Interpolated);

		QAngle FirstSet = Movements[Ticks].viewangles;
		{
			QAngle Set = Interpolated - QAngle(0.0f, ConVars.angle_y_in_m->GetFloat(), 0.0f);
			float Angle = AngleNormalize(ConVars.angle_y_in_m->GetFloat());
			if ((Angle > 90.0f || Angle < -90.0f) && ConVars.surf_style->GetBool())
			{
				Set.x = 89.0f;
			}


			PredictionSDK->SetViewAngles(Set);
			PredictionSDK->SetLocalViewAngles(Set);
			EngineSDK->SetViewAngles(Set);
			Interpolated = Interpolated;
		}
	}

}

void CMovementRecorder::RecordMovement(CUserCmd*Cmd, C_BasePlayer*LocalPlayer)
{
	static bool init[3];

	QAngle OldAngleBeforeR = Cmd->viewangles;

	int OldButtons = Cmd->buttons;

	if (Macro.Save)
	{
		Macro.SaveMacro(Movements, DrawPath, TickEnd, szDirFileDemosDll(Macro.CurrentName).c_str());
		Macro.Save = false;
	}

	if (Macro.Load)
	{
		Macro.ReadMacro(Movements, DrawPath, TickEnd, szDirFileDemosDll(Macro.CurrentName).c_str());
		Macro.Load = false;
	}

	if (Macro.Load2)
	{
		Macro.ReadMacro2(TempCmd, TickEnd, DrawPath[0], szDirFileDemosDll(Macro.CurrentName).c_str());
		for (int i = 0; i < MaxRecordTicks; i++)
		{
			Movements[i].buttons = TempCmd[i].buttons;
			Movements[i].upmove = TempCmd[i].up;
			Movements[i].forwardmove = TempCmd[i].forward;
			Movements[i].sidemove = TempCmd[i].side;
			Movements[i].viewangles = TempCmd[i].view;
		}

		Macro.Load2 = false;
	}

	if (Situation == STARTPOS)
		FindStart = true;

	if (FindStart)
	{
		static bool doonce = false;
		static int dooncetimer = 0;
		if (FindStart == 1)
		{
			Vector MyPos = LocalPlayer->GetAbsOrigin();
			if ((MyPos.x <= (DrawPath[0].x + 250)) && (MyPos.x >= (DrawPath[0].x - 250)) && (MyPos.y <= (DrawPath[0].y + 250)) && (MyPos.y >= (DrawPath[0].y - 250)))
			{
				FindStart = 2;
				dooncetimer = 0;
				doonce = true;
			}
			else
			{
				dooncetimer++;
				if (doonce)
				{
					EngineSDK->ClientCmd("say !tele");
					doonce = false;
				}
				if (dooncetimer == 150)
				{
					FindStart = 0;
					dooncetimer = 0;
					doonce = true;
				}
			}
		}

		if (FindStart == 2)
		{
			static QAngle qCurrentView; static QAngle qDelta;
			EngineSDK->GetViewAngles(qCurrentView);
			qDelta = Movements[0].viewangles - qCurrentView;

			if (qDelta[0] > 180.f)  qDelta[0] -= 360.f;
			if (qDelta[1] > 180.f)  qDelta[1] -= 360.f;
			if (qDelta[0]<-180.f) qDelta[0] += 360.f;
			if (qDelta[1]<-180.f) qDelta[1] += 360.f;
			Cmd->viewangles = qCurrentView + qDelta / 20.f;
			if (Cmd->viewangles[0] >  180.f) Cmd->viewangles[0] -= 360.f;
			if (Cmd->viewangles[1] >  180.f) Cmd->viewangles[1] -= 360.f;
			if (Cmd->viewangles[0] < -180.f) Cmd->viewangles[0] += 360.f;
			if (Cmd->viewangles[1] < -180.f) Cmd->viewangles[1] += 360.f;

			EngineSDK->SetViewAngles(Cmd->viewangles);

			Vector MyPos = LocalPlayer->GetAbsOrigin();
			QAngle AimAngle;
			VectorAngles(DrawPath[0] - MyPos, AimAngle);
			AimAngle.z = 0;

			Vector vDeltaOrigin = DrawPath[0] - MyPos;
			float distance = sqrtf(vDeltaOrigin.Length());
			Cmd->forwardmove = GetSpeedByDistance(distance);

			if (Cmd->forwardmove < 51)
				Cmd->buttons |= IN_SPEED;

			Cmd->sidemove = 0;


			Vector Direction(Cmd->forwardmove, Cmd->sidemove, Cmd->upmove);
			float Velocity = Direction.Length();
			QAngle Angles;
			VectorAngles(Direction, Angles);
			float delta = AngleNormalize(Cmd->viewangles.y - AimAngle.y);
			float yaw = DEG2RAD(AngleNormalize(delta + Angles.y));

			Cmd->forwardmove = (cosf(yaw) * Velocity);
			Cmd->sidemove = (sinf(yaw) * Velocity);

			if ((MyPos.x <= (DrawPath[0].x + .01)) && (MyPos.x >= (DrawPath[0].x - .01)) && (MyPos.y <= (DrawPath[0].y + .01)) && (MyPos.y >= (DrawPath[0].y - .01)))
			{
				Vector tPlayerVelocity = LocalPlayer->m_vecAbsVelocity;
				float PlayerVelocity = tPlayerVelocity.Length();
				if (PlayerVelocity <= 5)
				{
					if (qDelta.x < .2 && qDelta.x >(.2 * -1.00f) && qDelta.y < .2 && qDelta.y >(.2 * -1.00f))
					{
						FindStart = 0;
						Situation = PLAYING;
					}
					else
						FindStart = 3;
				}
			}
		}

		if (FindStart == 3)
		{
			static QAngle qCurrentView;
			EngineSDK->GetViewAngles(qCurrentView);
			QAngle qDelta = Movements[0].viewangles - qCurrentView;

			if (qDelta[0] > 180)  qDelta[0] -= 360;
			if (qDelta[1] > 180)  qDelta[1] -= 360;
			if (qDelta[0]<-180) qDelta[0] += 360;
			if (qDelta[1]<-180) qDelta[1] += 360;
			Cmd->viewangles = qCurrentView + qDelta / 20;
			if (Cmd->viewangles[0] >  180) Cmd->viewangles[0] -= 360;
			if (Cmd->viewangles[1] >  180) Cmd->viewangles[1] -= 360;
			if (Cmd->viewangles[0] < -180) Cmd->viewangles[0] += 360;
			if (Cmd->viewangles[1] < -180) Cmd->viewangles[1] += 360;

			EngineSDK->SetViewAngles(Cmd->viewangles);
			if (qDelta.x < .2 && qDelta.x >(.2 * -1.00f) && qDelta.y < .2 && qDelta.y >(.2 * -1.00f))
			{
				FindStart = 0;
				Situation = PLAYING;
			}
		}

	}

	if (Situation == RECORDING)
	{
		if (!init[0])
		{
			FindStart = false;

			if (ConVars.rerecord->GetInt() < 1)
			{
				Ticks = 0;
			}

			init[0] = true;
		}


		DrawPath[Ticks] = LocalPlayer->GetAbsOrigin();

		Movements[Ticks].buttons = Cmd->buttons;
		Movements[Ticks].upmove = Cmd->upmove;
		Movements[Ticks].sidemove = Cmd->sidemove;
		Movements[Ticks].forwardmove = Cmd->forwardmove;
		Movements[Ticks].viewangles = Cmd->viewangles;
		Movements[Ticks].weaponsubtype = Cmd->weaponsubtype;
		Movements[Ticks].weaponselect = Cmd->weaponselect;

		TickEnd = Ticks;

		++Ticks;
	}
	else
	{
		init[0] = false;
	}

	if (Situation == PLAYING)
	{
		if (!init[1])
		{
			Ticks = 0;
			FindStart = false;
			init[1] = true;
		}

		if (ConVars.rerecord->GetInt() > 0)
		{
			if (Ticks >= ConVars.rerecord->GetInt())
			{
				Situation = RECORDING;
			}
		}

		if ((Ticks >= TickEnd) && AutoPlayer)
		{
			Situation = STARTPOS;
		}

		if ((Ticks >= TickEnd) && !AutoPlayer)
		{
			Situation = NOTHING;
		}

		DrawPath[Ticks] = LocalPlayer->GetAbsOrigin();

		Cmd->buttons = Movements[Ticks].buttons;
		Cmd->upmove = Movements[Ticks].upmove;
		Cmd->sidemove = Movements[Ticks].sidemove;
		Cmd->forwardmove = Movements[Ticks].forwardmove;
		Cmd->viewangles = Movements[Ticks].viewangles;
		Cmd->weaponsubtype = Movements[Ticks].weaponsubtype;
		Cmd->weaponselect = Movements[Ticks].weaponselect;

		Vector vecMove((float)Cmd->forwardmove, (float)Cmd->sidemove, (float)Cmd->upmove);
		float speed = (float)sqrtf(vecMove.x * vecMove.x + vecMove.y * vecMove.y);
		QAngle angMove, qRealView(Cmd->viewangles);
		VectorAngles(vecMove, angMove);
		angMove = QAngle((float)angMove.x, (float)angMove.y, 0.0f);
		QAngle First;

		Cmd->viewangles.y -= ConVars.angle_y_in_m->GetFloat();

		float Angle = AngleNormalize(ConVars.angle_y_in_m->GetFloat());
		if ((Angle > 90.0f || Angle < -90.0f) && ConVars.surf_style->GetBool())
		{
			Cmd->viewangles.x = 89.0f;
		}

		if (ConVars.angle_y_in_m->m_cValue[0] == 'm')
			Cmd->viewangles = OldAngleBeforeR;

		NormalizeAngles(Cmd->viewangles);

		float yaw = (float)DEG2RAD((float)AngleNormalize((float)AngleNormalize(Cmd->viewangles.y - qRealView.y) + angMove.y));
		Cmd->forwardmove = (float)cosf(yaw) * speed;
		Cmd->sidemove = (float)sinf(yaw) * speed;

		++Ticks;
	}
	else
	{
		init[1] = false;
	}

	if (Situation == NOTHING)
	{
		if (!init[2])
		{
			Ticks = 0;
			FindStart = false;
			init[2] = true;
		}

	}
	else
	{
		init[2] = false;
	}

	if (Situation != PLAYING && Situation != RECORDING)
		ConVars.rerecord->SetValue("0");
}