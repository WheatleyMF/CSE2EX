#include "NpcAct.h"

#include "WindowsWrapper.h"

#include "Back.h"
#include "Boss.h"
#include "Frame.h"
#include "Game.h"
#include "Map.h"
#include "MyChar.h"
#include "NpChar.h"
#include "Sound.h"
#include "Triangle.h"

//Sue (being teleported by Misery)
void ActNpc280(NPCHAR *npc)
{
	RECT rcLeft[2] = {
		{112, 32, 128, 48},
		{144, 32, 160, 48},
	};

	RECT rcRight[2] = {
		{112, 48, 128, 64},
		{144, 48, 160, 64},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			npc->x += 0xC00;
			npc->tgt_x = npc->x;
			PlaySoundObject(29, 1);
			// Fallthrough
		case 1:
			if (++npc->act_wait == 64)
			{
				npc->act_no = 2;
				npc->act_wait = 0;
			}

			break;

		case 2:
			npc->ani_no = 0;

			if (npc->flag & 8)
			{
				npc->act_no = 4;
				npc->act_wait = 0;
				npc->ani_no = 1;
				PlaySoundObject(23, 1);
			}

			break;
	}

	if (npc->act_no > 1)
	{
		npc->ym += 0x20;
		if (npc->ym > 0x5FF)
			npc->ym = 0x5FF;

		npc->y += npc->ym;
	}

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];

	if (npc->act_no == 1)
	{
		npc->rect.bottom = npc->rect.top + npc->act_wait / 4;

		if (npc->act_wait / 2 & 1)
			npc->x = npc->tgt_x;
		else
			npc->x = npc->tgt_x + 0x200;
	}
}

//Doctor (red energy form)
void ActNpc281(NPCHAR *npc)
{
	RECT rc = {0, 0, 0, 0};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			break;

		case 10:
			npc->act_no = 11;
			npc->act_wait = 0;
			// Fallthrough
		case 11:
			++npc->act_wait;

			SetNpChar(270, npc->x, npc->y + 0x10000, 0, 0, 2, npc, 0x100);

			if (npc->act_wait > 150)
				npc->act_no = 12;

			break;

		case 20:
			npc->act_no = 21;
			npc->act_wait = 0;
			// Fallthrough
		case 21:
			if (++npc->act_wait > 250)
			{
				DeleteNpCharCode(270, 0);
				npc->act_no = 22;
			}

			break;
	}

	npc->rect = rc;
}

//Mini Undead Core (active)
void ActNpc282(NPCHAR *npc)
{
	RECT tc[3] = {
		{256, 80, 320, 120},
		{256, 0, 320, 40},
		{256, 120, 320, 160},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 20;
			npc->tgt_y = npc->y;

			if (Random(0, 100) & 1)
				npc->ym = -0x100;
			else
				npc->ym = 0x100;
			// Fallthrough
		case 20:
			npc->xm = -0x200;

			if (npc->x < -0x8000)
				npc->cond = 0;

			if (npc->tgt_y < npc->y)
				npc->ym -= 0x10;
			if (npc->tgt_y > npc->y)
				npc->ym += 0x10;

			if (npc->ym > 0x100)
				npc->ym = 0x100;
			if (npc->ym < -0x100)
				npc->ym = -0x100;

			if (gMC.flag & 8 && gMC.y < npc->y - 0x800 && gMC.x > npc->x - 0x3000 && gMC.x < npc->x + 0x3000)
			{
				npc->tgt_y = 0x12000;
				npc->ani_no = 2;
			}
			else if (npc->ani_no != 1)
			{
				npc->ani_no = 0;
			}

			if (gMC.flag & 1 && gMC.x < npc->x - npc->hit.back && gMC.x > npc->x - npc->hit.back - 0x1000 && gMC.hit.bottom + gMC.y > npc->y - npc->hit.top && gMC.y - gMC.hit.top < npc->hit.bottom + npc->y)
			{
				npc->bits &= ~0x40;
				npc->ani_no = 1;
			}
			else if (gMC.flag & 4 && gMC.x > npc->hit.back + npc->x && gMC.x < npc->x + npc->hit.back + 0x1000 && gMC.hit.bottom + gMC.y > npc->y - npc->hit.top && gMC.y - gMC.hit.top < npc->hit.bottom + npc->y)
			{
				npc->bits &= ~0x40;
				npc->ani_no = 1;
			}
			else if (gMC.flag & 2 && gMC.y < npc->y - npc->hit.top && gMC.y > npc->y - npc->hit.top - 0x1000 && gMC.hit.front + gMC.x > npc->x - npc->hit.back && gMC.x - gMC.hit.back < npc->hit.front + npc->x)
			{
				npc->bits &= ~0x40;
				npc->ani_no = 1;
			}
			else if (gMC.flag & 8 && gMC.y > npc->y + npc->hit.bottom - 0x800 && gMC.y < npc->y + npc->hit.bottom + 0x1800 && gMC.hit.front + gMC.x > npc->x - npc->hit.back - 0x800 && gMC.x - gMC.hit.back < npc->x + npc->hit.front + 0x800)
			{
				npc->bits &= ~0x40;
				npc->ani_no = 1;
			}
	}

	npc->x += npc->xm;
	npc->y += npc->ym;

	npc->rect = tc[npc->ani_no];
}

//Misery (transformed)
void ActNpc283(NPCHAR *npc)
{
	RECT rcLeft[11] = {
		{0, 64, 32, 96},
		{32, 64, 64, 96},
		{64, 64, 96, 96},
		{96, 64, 128, 96},
		{128, 64, 160, 96},
		{160, 64, 192, 96},
		{192, 64, 224, 96},
		{224, 64, 256, 96},
		{0, 0, 0, 0},
		{256, 64, 288, 96},
		{288, 64, 320, 96},
	};

	RECT rcRight[11] = {
		{0, 96, 32, 128},
		{32, 96, 64, 128},
		{64, 96, 96, 128},
		{96, 96, 128, 128},
		{128, 96, 160, 128},
		{160, 96, 192, 128},
		{192, 96, 224, 128},
		{224, 96, 256, 128},
		{0, 0, 0, 0},
		{256, 96, 288, 128},
		{288, 96, 320, 128},
	};

	if (npc->act_no < 100 && (gBoss[0].cond == 0 || npc->life < 400))
		npc->act_no = 100;

	int x;
	int y;
	int direct;
	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->y -= 0x1000;
			PlaySoundObject(29, 1);
			// Fallthrough
		case 1:
			if (++npc->act_wait / 2 & 1)
				npc->ani_no = 9;
			else
				npc->ani_no = 0;

			break;

		case 10:
			npc->act_no = 11;
			npc->ani_no = 9;
			break;

		case 20:
			gSuperXpos = 0;
			npc->act_no = 21;
			npc->act_wait = 0;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			// Fallthrough
		case 21:
			npc->xm = 7 * npc->xm / 8;
			npc->ym = 7 * npc->ym / 8;

			if (++npc->ani_wait > 20)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 1)
				npc->ani_no = 0;

			if (++npc->act_wait > 100)
				npc->act_no = 30;

			if (gMC.x > npc->x)
				npc->direct = 2;
			else
				npc->direct = 0;

			break;

		case 30:
			npc->act_no = 31;
			npc->act_wait = 0;
			npc->ani_no = 2;
			npc->count2 = npc->life;
			// Fallthrough
		case 31:
			if (++npc->ani_wait > 1)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 3)
				npc->ani_no = 2;

			if (npc->flag & 8)
				npc->ym = -0x200;

			if (gBoss[0].x < npc->x)
				npc->xm -= 0x20;
			else
				npc->xm += 0x20;

			if (gMC.y < npc->y)
				npc->ym -= 0x10;
			else
				npc->ym += 0x10;

			if (npc->xm > 0x200)
				npc->xm = 0x200;
			if (npc->xm < -0x200)
				npc->xm = -0x200;

			if (npc->ym > 0x200)
				npc->ym = 0x200;
			if (npc->ym < -0x200)
				npc->ym = -0x200;

			if (gMC.x < npc->x)
				npc->direct = 0;
			else
				npc->direct = 2;

			if (++npc->act_wait > 150 && (npc->life < npc->count2 - 20 || gSuperXpos))
			{
				gSuperXpos = 0;
				npc->act_no = 40;
			}

			if (gBoss[0].ani_no && npc->act_wait > 250)
				npc->act_no = 50;

			break;

		case 40:
			npc->act_no = 41;
			npc->act_wait = 0;
			npc->xm = 0;
			npc->ym = 0;

			if (gMC.x < npc->x)
				npc->direct = 0;
			else
				npc->direct = 2;

			PlaySoundObject(103, 1);

			if (gMC.y < 0x14000)
				npc->count2 = 290;
			else
				npc->count2 = 289;
			// Fallthrough
		case 41:
			if (++npc->act_wait / 2 & 1)
				npc->ani_no = 4;
			else
				npc->ani_no = 5;

			if (npc->act_wait % 6 == 1)
			{
				if ( npc->count2 == 289 )
				{
					x = npc->x + (Random(-0x40, 0x40) * 0x200);
					y = npc->y + (Random(-0x20, 0x20) * 0x200);
				}
				else
				{
					x = npc->x + (Random(-0x20, 0x20) * 0x200);
					y = npc->y + (Random(-0x40, 0x40) * 0x200);
				}

				if (x < 0x4000)
					x = 0x4000;
				if (x > (gMap.width - 2) * 0x2000)
					x = (gMap.width - 2) * 0x2000;

				if (y < 0x4000)
					y = 0x4000;
				if (y > (gMap.length - 2) * 0x2000)
					y = (gMap.length - 2) * 0x2000;

				PlaySoundObject(39, 1);
				SetNpChar(npc->count2, x, y, 0, 0, 0, 0, 0x100);
			}

			if (npc->act_wait > 50)
			{
				npc->act_no = 42;
				npc->act_wait = 0;

				if (gMC.x < npc->x)
					npc->direct = 0;
				else
					npc->direct = 2;
			}

			break;

		case 42:
			++npc->act_wait;
			npc->ani_no = 6;

			if (npc->act_wait > 50)
			{
				npc->ym = -0x200;

				if (npc->direct == 0)
					npc->xm = 0x200;
				else
					npc->xm = -0x200;

				npc->act_no = 30;
			}

			break;

		case 50:
			npc->act_no = 51;
			npc->act_wait = 0;
			npc->xm = 0;
			npc->ym = 0;

			if (gMC.x < npc->x)
				npc->direct = 0;
			else
				npc->direct = 2;

			PlaySoundObject(103, 1);
			// Fallthrough
		case 51:
			if (++npc->act_wait / 2 & 1)
				npc->ani_no = 4;
			else
				npc->ani_no = 5;

			if (gMC.equip & 0x20)
			{
				if (npc->act_wait % 10 == 1)
				{
					if (npc->direct == 0)
					{
						x = npc->x + 0x1400;
						y = npc->y;

						switch (npc->act_wait / 6 % 4)
						{
							case 0:
								direct = 0xD8;
								break;
							case 1:
								direct = 0xEC;
								break;
							case 2:
								direct = 0x14;
								break;
							case 3:
								direct = 0x28;
								break;
						}
					}
					else
					{
						x = npc->x - 0x1400;
						y = npc->y;

						switch (npc->act_wait / 6 % 4)
						{
							case 0:
								direct = 0x58;
								break;
							case 1:
								direct = 0x6C;
								break;
							case 2:
								direct = 0x94;
								break;
							case 3:
								direct = 0xA8;
								break;
						}
					}

					PlaySoundObject(39, 1);
					SetNpChar(301, x, y, 0, 0, direct, 0, 0x100);
				}
			}
			else if (npc->act_wait % 24 == 1)
			{
				if (npc->direct == 0)
				{
					x = npc->x + 0x1400;
					y = npc->y;

					switch (npc->act_wait / 6 % 4)
					{
						case 0:
							direct = 0xD8;
							break;
						case 1:
							direct = 0xEC;
							break;
						case 2:
							direct = 0x14;
							break;
						case 3:
							direct = 0x28;
							break;
					}
				}
				else
				{
					x = npc->x - 0x1400;
					y = npc->y;

					switch (npc->act_wait / 6 % 4)
					{
						case 0:
							direct = 0x58;
							break;
						case 1:
							direct = 0x6C;
							break;
						case 2:
							direct = 0x94;
							break;
						case 3:
							direct = 0xA8;
							break;
					}
				}

				PlaySoundObject(39, 1);
				SetNpChar(301, x, y, 0, 0, direct, 0, 0x100);
			}

			if (npc->act_wait > 50)
			{
				npc->act_no = 42;
				npc->act_wait = 0;
				if (gMC.x < npc->x)
					npc->direct = 0;
				else
					npc->direct = 2;
			}

			break;

		case 99:
			npc->xm = 0;
			npc->ym = 0;
			npc->ani_no = 9;
			npc->bits &= ~0x20;
			break;

		case 100:
			npc->act_no = 101;
			npc->ani_no = 9;
			npc->damage = 0;
			npc->bits &= ~0x20;
			npc->bits |= 8;
			npc->ym = -0x200;
			npc->shock += 50;
			npc->hit.bottom = 0x1800;
			++gBoss[0].ani_no;
			// Fallthrough
		case 101:
			npc->ym += 0x20;

			if (npc->y > 0x1B000 - npc->hit.bottom)
			{
				npc->y = 0x1B000 - npc->hit.bottom;
				npc->act_no = 102;
				npc->ani_no = 10;
				npc->xm = 0;
				npc->ym = 0;
			}

			break;
	}

	npc->y += npc->ym;

	if (npc->shock)
		npc->x += npc->xm / 2;
	else
		npc->x += npc->xm;

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];
}

//Sue (transformed)
void ActNpc284(NPCHAR *npc)
{
	RECT rcLeft[13] = {
		{0, 128, 32, 160},
		{32, 128, 64, 160},
		{64, 128, 96, 160},
		{96, 128, 128, 160},
		{128, 128, 160, 160},
		{160, 128, 192, 160},
		{192, 128, 224, 160},
		{224, 128, 256, 160},
		{0, 0, 0, 0},
		{256, 128, 288, 160},
		{288, 128, 320, 160},
		{224, 64, 256, 96},
		{208, 32, 224, 48},
	};

	RECT rcRight[13] = {
		{0, 160, 32, 192},
		{32, 160, 64, 192},
		{64, 160, 96, 192},
		{96, 160, 128, 192},
		{128, 160, 160, 192},
		{160, 160, 192, 192},
		{192, 160, 224, 192},
		{224, 160, 256, 192},
		{0, 0, 0, 0},
		{256, 160, 288, 192},
		{288, 160, 320, 192},
		{224, 96, 256, 128},
		{208, 48, 224, 64},
	};

	if (npc->act_no < 100 && (gBoss[0].cond == 0|| npc->life < 500))
		npc->act_no = 100;

	unsigned char deg;

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->y -= 0x800;
			PlaySoundObject(29, 1);
			npc->count2 = npc->life;
			// Fallthrough
		case 1:
			if (++npc->act_wait / 2 & 1)
			{
				npc->view.top = 0x2000;
				npc->view.back = 0x2000;
				npc->view.front = 0x2000;
				npc->ani_no = 11;
			}
			else
			{
				npc->view.top = 0x600;
				npc->view.back = 0x1000;
				npc->view.front = 0x1000;
				npc->ani_no = 12;
			}

			if (npc->act_wait > 50)
				npc->act_no = 10;

			break;

		case 10:
			npc->act_no = 11;
			npc->ani_no = 11;
			npc->view.top = 0x2000;
			npc->view.back = 0x2000;
			npc->view.front = 0x2000;
			DeleteNpCharCode(257, 1);
			break;

		case 20:
			npc->act_no = 21;
			npc->act_wait = 0;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			npc->damage = 0;
			npc->bits |= 0x20;
			npc->bits &= ~8;
			// Fallthrough
		case 21:
			npc->xm = 7 * npc->xm / 8;
			npc->ym = 7 * npc->ym / 8;

			if (++npc->ani_wait > 20)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 1)
				npc->ani_no = 0;

			if (++npc->act_wait > 80)
				npc->act_no = 30;

			if (gMC.x > npc->x)
				npc->direct = 2;
			else
				npc->direct = 0;

			if (npc->life < npc->count2 - 50)
			{
				npc->count2 = npc->life;
				gSuperXpos = 10;
			}

			break;

		case 30:
			npc->act_no = 31;
			npc->act_wait = 0;
			npc->ani_no = 2;
			npc->xm = 0;
			npc->ym = 0;
			// Fallthrough
		case 31:
			if (++npc->act_wait > 16)
			{
				++npc->count1;
				npc->count1 %= 4;

				switch (npc->count1)
				{
					case 1:
					case 3:
						npc->act_no = 34;
						break;
					case 0:
						npc->act_no = 32;
						break;
					case 2:
						npc->act_no = 32;
						break;
				}
			}

			break;

		case 32:
			npc->act_no = 33;
			npc->act_wait = 0;
			npc->bits &= ~0x20;

			if (gMC.x < npc->x)
				npc->tgt_x = gMC.x - 0x14000;
			else
				npc->tgt_x = gMC.x + 0x14000;

			npc->tgt_y = gMC.y;

			deg = GetArktan(npc->x - npc->tgt_x, npc->y - npc->tgt_y);
			npc->xm = 3 * GetCos(deg);
			npc->ym = 3 * GetSin(deg);
			npc->bits &= ~8;

			if (npc->x < (gMap.width * 0x2000) / 2 && npc->xm > 0)
			{
				if (npc->y < (gMap.length * 0x2000) / 2 && npc->ym > 0)
					npc->bits |= 8;
				if (npc->y > (gMap.length * 0x2000) / 2 && npc->ym < 0)
					npc->bits |= 8;
			}
			if (npc->x > (gMap.width * 0x2000) / 2 && npc->xm < 0)
			{
				if (npc->y < (gMap.length * 0x2000) / 2 && npc->ym > 0)
					npc->bits |= 8;
				if (npc->y > (gMap.length * 0x2000) / 2 && npc->ym < 0)
					npc->bits |= 8;
			}

			if (npc->xm > 0)
				npc->direct = 2;
			else
				npc->direct = 0;
			// Fallthrough
		case 33:
			if (++npc->act_wait / 2 & 1)
				npc->ani_no = 3;
			else
				npc->ani_no = 8;

			if (npc->act_wait > 50 || npc->flag & 5)
				npc->act_no = 20;

			break;

		case 34:
			npc->act_no = 35;
			npc->act_wait = 0;
			npc->damage = 4;

			npc->tgt_x = gMC.x;
			npc->tgt_y = gMC.y;

			deg = GetArktan(npc->x - npc->tgt_x, npc->y - npc->tgt_y);
			npc->ym = 3 * GetSin(deg);
			npc->xm = 3 * GetCos(deg);
			npc->bits &= ~8;

			if (npc->x < (gMap.width * 0x2000) / 2 && npc->xm > 0)
			{
				if (npc->y < (gMap.length * 0x2000) / 2 && npc->ym > 0)
					npc->bits |= 8;
				if (npc->y > (gMap.length * 0x2000) / 2 && npc->ym < 0)
					npc->bits |= 8;
			}

			if (npc->x > (gMap.width * 0x2000) / 2 && npc->xm < 0)
			{
				if (npc->y < (gMap.length * 0x2000) / 2 && npc->ym > 0)
					npc->bits |= 8;
				if (npc->y > (gMap.length * 0x2000) / 2 && npc->ym < 0)
					npc->bits |= 8;
			}

			if (npc->xm > 0)
				npc->direct = 2;
			else
				npc->direct = 0;
			// Fallthrough
		case 35:
			if (++npc->act_wait > 20 && npc->shock)
			{
				npc->act_no = 40;
			}
			else if (npc->act_wait > 50 || npc->flag & 5)
			{
				npc->act_no = 20;
			}

			if (++npc->ani_wait > 1)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 7)
				npc->ani_no = 4;

			if (npc->act_wait % 5 == 1)
				PlaySoundObject(109, 1);

			break;

		case 40:
			npc->act_no = 41;
			npc->act_wait = 0;
			npc->ani_no = 2;
			npc->damage = 0;
			npc->bits &= ~8;
			// Fallthrough
		case 41:
			npc->xm = 7 * npc->xm / 8;
			npc->ym = 7 * npc->ym / 8;

			if (++npc->act_wait > 6)
			{
				npc->act_no = 42;
				npc->act_wait = 0;
				npc->ym = -0x200;

				if (npc->direct == 0)
					npc->xm = 0x200;
				else
					npc->xm = -0x200;
			}

			break;

		case 42:
			npc->ani_no = 9;

			if (npc->flag & 8)
			{
				npc->act_no = 43;
				npc->act_wait = 0;
				npc->ani_no = 2;

				if (gMC.x > npc->x)
					npc->direct = 2;
				else
					npc->direct = 0;
			}

			npc->ym += 0x20;
			if (npc->ym > 0x5FF)
				npc->ym = 0x5FF;

			break;

		case 43:
			if (++npc->act_wait > 16)
				npc->act_no = 20;

			break;

		case 99:
			npc->xm = 0;
			npc->ym = 0;
			npc->ani_no = 9;
			npc->bits &= ~0x20;
			break;

		case 100:
			npc->act_no = 101;
			npc->ani_no = 9;
			npc->damage = 0;
			npc->bits &= ~0x20;
			npc->bits |= 8;
			npc->ym = -0x200;
			npc->shock += 50;
			++gBoss[0].ani_no;
			// Fallthrough
		case 101:
			npc->ym += 0x20;

			if (npc->y > 0x1B000 - npc->hit.bottom)
			{
				npc->y = 0x1B000 - npc->hit.bottom;
				npc->act_no = 102;
				npc->ani_no = 10;
				npc->xm = 0;
				npc->ym = 0;
			}

			break;
	}

	npc->y += npc->ym;

	if (npc->shock)
		npc->x += npc->xm / 2;
	else
		npc->x += npc->xm;

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];
}

//Undead Core spiral projectile
void ActNpc285(NPCHAR *npc)
{
	RECT rc = {232, 104, 248, 120};

	if (npc->x < 0 || npc->x > gMap.width * 0x2000)
	{
		VanishNpChar(npc);
		return;
	}

	unsigned char deg;

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->tgt_x = npc->x;
			npc->tgt_y = npc->y;
			npc->count1 = npc->direct / 8;
			npc->direct &= 7;
			// Fallthrough
		case 1:
			npc->count1 += 24;
			npc->count1 &= 0xFF;

			deg = npc->count1;

			if (npc->act_wait < 128)
				++npc->act_wait;

			if (npc->direct == 0)
				npc->xm -= 21;
			else
				npc->xm += 21;

			npc->tgt_x += npc->xm;

			npc->x = npc->tgt_x + 4 * GetCos(deg);
			npc->y = npc->tgt_y + 6 * GetSin(deg);

			SetNpChar(286, npc->x, npc->y, 0, 0, 0, 0, 0x100);
	}

	npc->rect = rc;
}

//Undead Core spiral shot trail
void ActNpc286(NPCHAR *npc)
{
	RECT rc[3] = {
		{232, 120, 248, 136},
		{232, 136, 248, 152},
		{232, 152, 248, 168},
	};

	if (++npc->ani_wait > 0)
	{
		npc->ani_wait = 0;
		++npc->ani_no;
	}

	if (npc->ani_no > 2)
		npc->cond = 0;
	else
		npc->rect = rc[npc->ani_no];
}

//Orange smoke
void ActNpc287(NPCHAR *npc)
{
	RECT rcLeft[7] = {
		{0, 224, 16, 240},
		{16, 224, 32, 240},
		{32, 224, 48, 240},
		{48, 224, 64, 240},
		{64, 224, 80, 240},
		{80, 224, 96, 240},
		{96, 224, 112, 240},
	};

	if (npc->act_no == 0)
	{
		npc->xm = Random(-4, 4) * 0x200;
		npc->act_no = 1;
	}
	else
	{
		npc->xm = 20 * npc->xm / 21;
		npc->ym = 20 * npc->ym / 21;

		npc->x += npc->xm;
		npc->y += npc->ym;
	}

	if (++npc->ani_wait > 1)
	{
		npc->ani_wait = 0;
		++npc->ani_no;
	}

	if (npc->ani_no > 6)
		npc->cond = 0;
	else
		npc->rect = rcLeft[npc->ani_no];
}

//Undead Core exploding rock
void ActNpc288(NPCHAR *npc)
{
	RECT rc[5] = {
		{232, 72, 248, 88},
		{232, 88, 248, 104},
		{232, 0, 256, 24},
		{232, 24, 256, 48},
		{232, 48, 256, 72},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->xm = -0x200;
			// Fallthrough
		case 1:
			if (npc->direct == 1)
			{
				npc->ym -= 0x20;
				if (npc->ym < -0x5FF)
					npc->ym = -0x5FF;

				if (npc->flag & 2)
					npc->act_no = 2;
			}
			else if (npc->direct == 3)
			{
				npc->ym += 0x20;
				if (npc->ym > 0x5FF)
					npc->ym = 0x5FF;

				if (npc->flag & 8)
					npc->act_no = 2;
			}

			if (++npc->ani_wait > 3)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 1)
				npc->ani_no = 0;

			break;

		case 2:
			PlaySoundObject(44, 1);
			npc->act_no = 3;
			npc->act_wait = 0;
			npc->bits |= 8;
			npc->ym = 0;

			if (gMC.x < npc->x)
				npc->xm = -0x400;
			else
				npc->xm = 0x400;

			npc->view.back = 0x1800;
			npc->view.front = 0x1800;
			npc->view.top = 0x1800;
			npc->view.bottom = 0x1800;
			// Fallthrough
		case 3:
			if (++npc->ani_no > 4)
				npc->ani_no = 2;

			if (++npc->act_wait % 4 == 1)
			{
				if (npc->direct == 1)
					SetNpChar(287, npc->x, npc->y, 0, 0x400, 0, 0, 0x100);
				else
					SetNpChar(287, npc->x, npc->y, 0, -0x400, 0, 0, 0x100);
			}

			if (npc->x < 0x2000 || npc->x > (gMap.width * 0x2000) - 0x2000)
				npc->cond = 0;

			break;
	}

	npc->x += npc->xm;
	npc->y += npc->ym;

	npc->rect = rc[npc->ani_no];
}

//Critter (orange, Misery)
void ActNpc289(NPCHAR *npc)
{
	RECT rcLeft[3] = {
		{160, 32, 176, 48},
		{176, 32, 192, 48},
		{192, 32, 208, 48},
	};

	RECT rcRight[3] = {
		{160, 48, 176, 64},
		{176, 48, 192, 64},
		{192, 48, 208, 64},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->ani_no = 2;

			if (gMC.x < npc->x)
				npc->direct = 0;
			else
				npc->direct = 2;
			// Fallthrough
		case 1:
			if (++npc->act_wait > 16)
			{
				npc->act_no = 10;
				npc->view.top = 0x1000;
				npc->view.bottom = 0x1000;
				npc->damage = 2;
				npc->bits |= 0x20;
			}

			break;

		case 10:
			if (npc->flag & 8)
			{
				npc->act_no = 11;
				npc->ani_no = 0;
				npc->act_wait = 0;
				npc->xm = 0;

				if (gMC.x < npc->x)
					npc->direct = 0;
				else
					npc->direct = 2;
			}

			break;

		case 11:
			if (++npc->act_wait > 10)
			{
				if (++npc->count1 > 4)
					npc->act_no = 12;
				else
					npc->act_no = 10;

				PlaySoundObject(30, 1);
				npc->ym = -0x600;

				if (npc->direct == 0)
					npc->xm = -0x200;
				else
					npc->xm = 0x200;

				npc->ani_no = 2;
			}

			break;

		case 12:
			npc->bits |= 8;

			if (npc->y > gMap.length * 0x2000)
				VanishNpChar(npc);

			break;
	}

	if (npc->act_no >= 10)
		npc->ym += 0x40;

	if (npc->ym > 0x5FF)
		npc->ym = 0x5FF;

	npc->x += npc->xm;
	npc->y += npc->ym;

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];

	if (npc->act_no == 1)
	{
		npc->rect.top += 8 - npc->act_wait / 2;
		npc->rect.bottom -= npc->act_wait / 2 + 8;
		npc->view.top = (npc->act_wait * 0x200) / 2;
		npc->view.bottom = (npc->act_wait * 0x200) / 2;
	}
}

//Bat (Misery)
void ActNpc290(NPCHAR *npc)
{
	RECT rcLeft[3] = {
		{112, 32, 128, 48},
		{128, 32, 144, 48},
		{144, 32, 160, 48},
	};

	RECT rcRight[3] = {
		{112, 48, 128, 64},
		{128, 48, 144, 64},
		{144, 48, 160, 64},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->ani_no = 2;

			if (gMC.x < npc->x)
				npc->direct = 0;
			else
				npc->direct = 2;
			// Fallthrough
		case 1:
			if (++npc->act_wait > 16)
			{
				npc->act_no = 10;
				npc->view.top = 0x1000;
				npc->view.bottom = 0x1000;
				npc->damage = 2;
				npc->bits |= 0x20;
				npc->tgt_y = npc->y;
				npc->ym = 0x400;
			}

			break;

		case 10:
			if (++npc->ani_wait > 2)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 2)
				npc->ani_no = 0;

			if (npc->tgt_y > npc->y)
				npc->ym += 0x40;
			else
				npc->ym -= 0x40;

			if (npc->direct == 0)
				npc->xm -= 0x10;
			else
				npc->xm += 0x10;

			if (npc->x < 0 || npc->y < 0 || npc->x > gMap.width * 0x2000 || npc->y > gMap.length * 0x2000)
			{
				VanishNpChar(npc);
				return;
			}

			break;
	}

	npc->x += npc->xm;
	npc->y += npc->ym;

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];

	if (npc->act_no == 1)
	{
		npc->rect.top += 8 - npc->act_wait / 2;
		npc->rect.bottom -= npc->act_wait / 2 + 8;
		npc->view.top = (npc->act_wait * 0x200) / 2;
		npc->view.bottom = (npc->act_wait * 0x200) / 2;
	}
}

//Mini Undead Core (inactive)
void ActNpc291(NPCHAR *npc)
{
	RECT tc[2] = {
		{256, 80, 320, 120},
		{256, 0, 320, 40},
	};

	if (npc->act_no == 0)
	{
		npc->act_no = 20;

		if (npc->direct == 2)
		{
			npc->bits &= ~0x40;
			npc->ani_no = 1;
		}
	}

	npc->rect = tc[npc->ani_no];
}

//Quake
void ActNpc292(NPCHAR *npc)
{
	(void)npc;

	SetQuake(10);
}

//Undead Core giant energy shot
void ActNpc293(NPCHAR *npc)
{
	RECT rect[2] = {
		{240, 200, 280, 240},
		{280, 200, 320, 240},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			// Fallthrough
		case 1:
			if (++npc->ani_no > 1)
				npc->ani_no = 0;

			SetNpChar(4, npc->x + (Random(0, 0x10) * 0x200), npc->y + (Random(-0x10, 0x10) * 0x200), 0, 0, 0, 0, 0x100);

			npc->x -= 0x1000;

			if (npc->x < -0x4000)
				npc->cond = 0;

			break;
	}

	npc->rect = rect[npc->ani_no];
}

//Quake + falling block generator
void ActNpc294(NPCHAR *npc)
{
	switch (npc->act_no)
	{
		case 0:
			if (gMC.x < (gMap.width - 6) * 0x2000)
			{
				npc->act_no = 1;
				npc->act_wait = 0;
			}

			break;

		case 1:
			++npc->act_wait;

			if (gMC.equip & 0x20)
			{
				npc->x = gMC.x + 0x8000;

				if (npc->x < 0x34000)
					npc->x = 0x34000;
			}
			else
			{
				npc->x = gMC.x + 0xC000;

				if (npc->x < 0x2E000)
					npc->x = 0x2E000;
			}

			if (npc->x > (gMap.width - 10) * 0x2000)
				npc->x = (gMap.width - 10) * 0x2000;

			if (npc->act_wait > 24)
			{
				int x;

				if (gMC.equip & 0x20)
					x = npc->x + (Random(-14, 14) * 0x2000);
				else
					x = npc->x + (Random(-11, 11) * 0x2000);

				const int y = gMC.y - 0x1C000;

				if (Random(0, 10) & 1)
					SetNpChar(279, x, y, 0, 0, 0, 0, 0x100);
				else
					SetNpChar(279, x, y, 0, 0, 2, 0, 0x100);

				npc->act_wait = Random(0, 15);
			}

			break;
	}
}

//Cloud
void ActNpc295(NPCHAR *npc)
{
	RECT rc[4] = {
		{0, 0, 208, 64},
		{32, 64, 144, 96},
		{32, 96, 104, 0x80},
		{104, 96, 144, 0x80},
	};
	
	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->ani_no = npc->direct % 4;
			switch (npc->direct)
			{
				case 0:
					npc->ym = -1000;
					npc->view.back = 0xD000;
					npc->view.front = 0xD000;
					break;
				case 1:
					npc->ym = -0x800;
					npc->view.back = 0x7000;
					npc->view.front = 0x7000;
					break;
				case 2:
					npc->ym = -0x400;
					npc->view.back = 0x4000;
					npc->view.front = 0x4000;
					break;
				case 3:
					npc->ym = -0x200;
					npc->view.back = 0x2800;
					npc->view.front = 0x2800;
					break;
				case 4:
					npc->xm = -0x400;
					npc->view.back = 0xD000;
					npc->view.front = 0xD000;
					break;
				case 5:
					npc->xm = -0x200;
					npc->view.back = 0x7000;
					npc->view.front = 0x7000;
					break;
				case 6:
					npc->xm = -0x100;
					npc->view.back = 0x4000;
					npc->view.front = 0x4000;
					break;
				case 7:
					npc->xm = -0x80;
					npc->view.back = 0x2800;
					npc->view.front = 0x2800;
					break;
			}

			break;

		case 1:
			npc->x += npc->xm;
			npc->y += npc->ym;
			if (npc->x < -0x8000)
				npc->cond = 0;
			if (npc->y < -0x4000)
				npc->cond = 0;
			break;
	}

	npc->rect = rc[npc->ani_no];
}

//Cloud generator
void ActNpc296(NPCHAR *npc)
{
	if (++npc->act_wait > 16)
	{
		npc->act_wait = Random(0, 16);
		int dir = Random(0, 100) & 3;
		
		int pri;
		if (npc->direct)
		{
			switch (dir)
			{
				case 0:
					pri = 0x80;
					break;
				case 1:
					pri = 0x55;
					break;
				case 2:
					pri = 0x40;
					break;
				case 3:
					pri = 0x00;
					break;
			}
			
			SetNpChar(295, npc->x, npc->y + (Random(-7, 7) * 0x2000), 0, 0, dir + 4, 0, pri);
		}
		else
		{
			switch (dir)
			{
				case 0:
					pri = 0x180;
					break;
				case 1:
					pri = 0x80;
					break;
				case 2:
					pri = 0x40;
					break;
				case 3:
					pri = 0x00;
					break;
			}
			
			SetNpChar(295, npc->x + (Random(-10, 10) * 0x2000), npc->y, 0, 0, dir, 0, pri);
		}
	}
}

//Sue in dragon's mouth
void ActNpc297(NPCHAR *npc)
{
	RECT rc = {112, 48, 0x80, 64};

	npc->x = npc->pNpc->x + 0x2000;
	npc->y = npc->pNpc->y + 0x1000;

	npc->rect = rc;
}

//Doctor (opening)
void ActNpc298(NPCHAR *npc)
{
	RECT rc[8] = {
		{72, 0x80, 88, 160},
		{88, 0x80, 104, 160},
		{104, 0x80, 120, 160},
		{72, 0x80, 88, 160},
		{120, 0x80, 136, 160},
		{72, 0x80, 88, 160},
		{104, 160, 120, 192},
		{120, 160, 136, 192},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->y -= 0x1000;
			// Fallthrough

		case 1:
			npc->ani_no = 0;
			break;

		case 10:
			npc->act_no = 11;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			npc->count1 = 0;
			// Fallthrough

		case 11:
			if (++npc->ani_wait > 6)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 1)
			{
				npc->ani_no = 0;

				if (++npc->count1 > 7)
				{
					npc->ani_no = 0;
					npc->act_no = 1;
				}
			}

			break;

		case 20:
			npc->act_no = 21;
			npc->ani_no = 2;
			npc->ani_wait = 0;
			// Fallthrough

		case 21:
			if (++npc->ani_wait > 10)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if ( npc->ani_no > 5 )
				npc->ani_no = 2;

			npc->x += 0x100;

			break;

		case 30:
			npc->ani_no = 6;
			break;

		case 40:
			npc->act_no = 41;
			npc->ani_no = 6;
			npc->ani_wait = 0;
			npc->count1 = 0;
			// Fallthrough

		case 41:
			if (++npc->ani_wait > 6)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 7)
			{
				npc->ani_no = 6;

				if (++npc->count1 > 7)
				{
					npc->ani_no = 6;
					npc->act_no = 30;
				}
			}

			break;
	}

	npc->rect = rc[npc->ani_no];
}

//Balrog/Misery (opening)
void ActNpc299(NPCHAR *npc)
{
	RECT rc[2] = {
		{0, 0, 48, 48},
		{48, 0, 96, 48},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;

			if (npc->direct == 0)
			{
				npc->ani_no = 1;
				npc->act_wait = 25;
				npc->y -= 1600;
			}
			else
			{
				npc->ani_no = 0;
				npc->act_wait = 0;
			}

			break;
	}

	if (++npc->act_wait / 50 % 2)
		npc->y += 0x40;
	else
		npc->y -= 0x40;

	npc->rect = rc[npc->ani_no];
}
