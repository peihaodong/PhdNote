#pragma once

#ifdef PHD_NOTE
#define PHD_NOTE_API __declspec(dllexport)
#else
#define PHD_NOTE_API __declspec(dllimport)
#endif

//
#include "CAdoLx.h"
#include "PhdSocket.h"
#include "PhdRegedit.h"
#include "PhdExcel.h"
#include "PhdIni.h"
#include "PhdXml.h"
#include "PhdConver.h"

#ifndef PHD_NOTE
//�൱����vs��������� ����������

#pragma comment(lib,"PhdNote.lib")

#endif