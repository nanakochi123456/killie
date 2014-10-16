/*----------------------------------------------------------------------------
    "killerNT.h"
        Kill Process for WindowsNT

     Microsoft C/C++ 32bit Compiler ver11.00  Progremmed by S.Tsuneoka

    "killtyping.h"
	    kill Process for 歌謡タイピング劇場
    Modified by ななこっち★
    (Microsoft C 32bit Compiler ver14.00/Visual Studio 2005)
----------------------------------------------------------------------------*/
#if 0

#define LISTBUFFERSIZE	(MAX_PATH * 2)
#define MAXPROCESS		65536

#define MSGDISPATCH(msg, fn)	case msg: return fn(hWnd, wParam, lParam)


#define IDC_STATIC		-1

#define IDM_PROCLIST	1000

#define IDM_DELETE		100
#define IDM_REFRESH		101
#define IDM_EXIT		102
#define IDM_PROCID		200
#endif

#define	TEXT_SIZE	1024
#define	MAX_PROCESS	65536

#define	KILL_IE			1
#define	KILL_FIREFOX	1
#define	KILL_CHROME		1
#define	KILL_OPERA		1
#define	KILL_SAFARI		1
#define	KILL_LUNASCAPE	1
#define	KILL_SLEIPNIR	1

#define	TITLE_IE		"internet explorer"
#define	CLASS_IE		"ieframe"
#define	PROC_IE			"iexplore.exe"
#define	TITLE_FIREFOX	"mozilla firefox"
#define	CLASS_FIREFOX	"mozilla"
#define	PROC_FIREFOX	"firefox.exe"
#define	TITLE_CHROME	"google chrome"
#define	CLASS_CHROME	"chrome"
#define	PROC_CHROME		"chrome.exe"
#define	NAME_OPERA		"opera"
#define	PROC_OPERA		"opera.exe"
#define	CLASS_SAFARI	"1c03b488-d53b-4a81-97f8-754559640193"
#define	PROC_SAFARI		"safari.exe"
#define	NAME_LUNASCAPE	"lunascape"
#define	PROC_LUNASCAPE	"luna.exe"
#define	NAME_SLEIPNIR	"sleipnir"
#define	PROC_SLEIPNIR	"sleipnir.exe"

char *strcasestr(const char *s, const char *find);
int strncasecmp(const char *s1, const char *s2, size_t n);

#include "resource.h"

#pragma once
