#pragma once

#ifdef PHD_NOTE
#define PHD_NOTE_API __declspec(dllexport)
#else
#define PHD_NOTE_API __declspec(dllimport)
#endif

//
#include "PhdIni.h"
#include "PhdExcel.h"
#include "PhdSocket.h"
#include "PhdRegedit.h"
#include "CAdoLx.h"
#include "PhdXml.h"
#include "PhdConver.h"

#ifndef PHD_NOTE
//�൱����vs��������� ����������

#pragma comment(lib,"PhdNote.lib")

#endif