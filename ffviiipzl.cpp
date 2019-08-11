/* Copyright (c) Ateeq Sharfuddin 2000. */

#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <tchar.h>

#include "resource.h"

#include "tile.h"
#include "gmat.h"

static const int SZ  = 3L;

static HINSTANCE g_hInst=NULL;
static HWND hMain=NULL;

static Tile Game[5][5];
static int i, j, l, m;

static HWND    btn[SZ][SZ]=	
{
   {NULL,NULL,NULL},
	{NULL,NULL,NULL},
	{NULL,NULL,NULL}
};

static HBITMAP hbmp[SZ+2][SZ+2]=
{
   {NULL, NULL, NULL, NULL, NULL},
	{NULL, NULL, NULL, NULL, NULL},
	{NULL, NULL, NULL, NULL, NULL},
	{NULL, NULL, NULL, NULL, NULL},
	{NULL, NULL, NULL, NULL, NULL}
};

static gMat gMatrix[SZ][SZ];
static bool tMat[SZ][SZ];
static bool GameWon = false;


void Clear_tMat(void);
void LoadBMPs(void);
void Rnd(void);
void GuardianBlocks(void);
void PlayingBlocks(void);
void MakeButtons(void);
void Populate(void);
void Swap(int a, int b, char ltrb);
long FAR PASCAL MainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
long FAR PASCAL ViewProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void Uninit(void);

void Clear_tMat(void)
{
   ZeroMemory(tMat, sizeof(tMat));
}

void LoadBMPs()
{
	int bmpr = IDB_BMP00;
	int btnr = IDC_BU00;

	for (i=0; i < SZ; i++) 
   {
		for (j=0; j < SZ; j++)
      {
			hbmp[i+1][j+1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(bmpr++));
			btn[i][j]=GetDlgItem(hMain,btnr++);
		}
	}

}

void Rnd(void)
{
	int l, m;
   l = m = -1;
		
	for (i = 0; i < SZ; i++)
   {
		for (j = 0; j < SZ; j++)
      {
			do
         {
				l = (int) rand()%SZ;
				m = (int) rand()%SZ;
			} while (tMat[m][l] == true);
			tMat[m][l] = true;
			gMatrix[j][i].SetXY(l, m);
		}
	}

}


void GuardianBlocks(void)
{
	for (i = 0; i < SZ+2; i++)
   {
		Game[0][i].SetTile(false, -1, -1);
		Game[i][0].SetTile(false, -1, -1);
		Game[SZ+1][i].SetTile(false, -1, -1);
		Game[i][SZ+1].SetTile(false, -1, -1);
	}
}

void PlayingBlocks(void)
{
	for (i = 1; i < SZ+1; i++)
   {
		for (j = 1; j < SZ+1; j++)
      {
			Game[i][j].SetTile(false, i-1, j-1);
		}
	}

	Game[SZ][SZ].fIsEmpty = true;
}

void MakeButtons(void)
{
	int e=0, f=0;
	for (i = 0; i < SZ; i++) {
		for (j = 0; j < SZ; j++) {
			if (i == SZ-1 && j == SZ-1){}
			else {
				e = gMatrix[i][j].nXPos;
				f = gMatrix[i][j].nYPos;
				Game[i+1][j+1].hbmpImage = hbmp[f+1][e+1];
				SendMessage(btn[i][j], BM_SETIMAGE, IMAGE_BITMAP, (long)Game[i+1][j+1].hbmpImage);
			}
		}
	}
}


void Populate(void)
{
   int i = 0, j = 0;
	int count = 0;

	for (i = 1; i < SZ+1; i++) {
		for (j = 1; j < SZ+1; j++) {
			if (!Game[i][j].fIsEmpty) {
            if (Game[i][j+1].fIsEmpty) { Game[i][j].fRight = true; }
            else { Game[i][j].fRight = false; }

            if (Game[i+1][j].fIsEmpty) { Game[i][j].fBottom = true; }
            else { Game[i][j].fBottom = false; }

            if (Game[i][j-1].fIsEmpty) { Game[i][j].fLeft = true; }
            else { Game[i][j].fLeft = false; }

            if (Game[i-1][j].fIsEmpty) { Game[i][j].fTop = true; }
            else { Game[i][j].fTop = false; }
			}
			else
         {
				Game[i][j+1].fLeft = true;
				Game[i][j+1].fTop =  Game[i][j+1].fRight = Game[i][j+1].fBottom = false;

            Game[i+1][j].fTop = true;
				Game[i+1][j].fLeft = Game[i+1][j].fRight = Game[i+1][j].fBottom = false;

				Game[i][j-1].fRight = true;
            Game[i][j-1].fLeft = Game[i][j-1].fTop = Game[i][j-1].fBottom = false;

				Game[i-1][j].fBottom = true;
            Game[i-1][j].fLeft = Game[i-1][j].fTop = Game[i-1][j].fRight = false;
			}
		}
	}

   ZeroMemory(tMat, sizeof(tMat));

	for (i = 1; i < SZ+1; i++)
   {
		for (j = 1; j < SZ+1; j++)
      {
			if (Game[j][i].nXPos == gMatrix[i-1][j-1].nXPos &&
				 Game[j][i].nYPos == gMatrix[i-1][j-1].nYPos)
			{
				tMat[j-1][i-1]=true;
			}
			else
         {
				tMat[j-1][i-1]=false;
			}
		}
	}


	for (i = 0; i < SZ; i++)
   {
      for (j = 0; j < SZ; j++) if (tMat[i][j]) count++;
	}

	if (count == SZ*SZ-1)
   {
		for (i = 0; i < SZ; i++)
      {
			for (j = 0; j < SZ; j++)
         {
				Game[i+1][j+1].hbmpImage = hbmp[i+1][j+1];
				SendMessage(btn[i][j],BM_SETIMAGE,IMAGE_BITMAP,
                        (long)Game[i+1][j+1].hbmpImage);
			}
		}

		MessageBox(hMain,
				   _T("Congratulations!!!\n") \
				   _T("You have solved this puzzle.\n") \
				   _T("Now STOP bothering me and find something more challenging"),
				   _T("Woo-hoo!"),
				   MB_OK|MB_ICONINFORMATION);
		GameWon = true;
	}

}

void Swap(int i, int j, char ltrb)
{
	HBITMAP temp=NULL;
	int y1, y2;
	int x1, x2;

	y1 = i;
	x1 = j;
	if (ltrb == 'L')
   {
		y2 = i;
		x2 = j-1;
	}

	else if (ltrb == 'T')
   {
		y2 = i-1;
		x2 = j;
	}
	else if (ltrb == 'R')
   {
		y2 = i;
		x2 = j+1;
	}
	else if (ltrb == 'B')
   {
		y2 = i+1;
		x2 = j;
	}

	temp = Game[y2+1][x2+1].hbmpImage;
	Game[y2+1][x2+1].hbmpImage = Game[y1+1][x1+1].hbmpImage;
	Game[y1+1][x1+1].hbmpImage = temp;

	Game[y2+1][x2+1].fIsEmpty = false;
	Game[y1+1][x1+1].fIsEmpty = true;

	gMatrix[y2][x2].nXPos = gMatrix[y1][x1].nXPos;
	gMatrix[y2][x2].nYPos = gMatrix[y1][x1].nYPos;

	gMatrix[y1][x1].nXPos = -2;
	gMatrix[y1][x1].nYPos = -2;


	Populate();

	SendMessage(btn[y1][x1],BM_SETIMAGE, IMAGE_BITMAP, (long)Game[y1+1][x1+1].hbmpImage);
	SendMessage(btn[y2][x2],BM_SETIMAGE, IMAGE_BITMAP, (long)Game[y2+1][x2+1].hbmpImage);

	temp = NULL;
}

int PASCAL WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   int nShowCmd)
{
	MSG msg;
	
	if (hPrevInstance)
		return FALSE;

	g_hInst = hInstance;
	

	hMain = CreateDialog(g_hInst,MAKEINTRESOURCE(IDD_MAIN),NULL,(DLGPROC)MainProc);	

	LoadBMPs();
	Clear_tMat();
	srand((unsigned)time(NULL));
	Rnd();
	GuardianBlocks();
	PlayingBlocks();
	Populate();
	MakeButtons();


	ShowWindow(hMain,nShowCmd);
	UpdateWindow(hMain);

	HACCEL haccel = LoadAccelerators(g_hInst,MAKEINTRESOURCE(IDR_ACCELERATOR));
	
	while (GetMessage(&msg,NULL,0,0))
   {
		if (!TranslateAccelerator(hMain,haccel,&msg))
      {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	Uninit();

	return msg.wParam;
}


long FAR PASCAL MainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_INITDIALOG:
		return TRUE;
		break;

	case WM_CLOSE:
		Uninit();
		EndDialog(hWnd, TRUE);
		PostQuitMessage(0);
		return TRUE;
		break;		
	case WM_COMMAND:
		if (!GameWon) {
			if (HIWORD(wParam)==BN_CLICKED)
         {			
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						if ((HWND)lParam == btn[i][j])
                  {
							if (Game[i+1][j+1].fLeft)
                     {
								Swap(i, j, 'L');
								Game[i+1][j+1].fLeft = false;
							}
							else if (Game[i+1][j+1].fTop)
                     {
								Swap(i, j, 'T');
								Game[i+1][j+1].fTop = false;
							}
							else if (Game[i+1][j+1].fRight)
                     {
								Swap(i, j, 'R');
								Game[i+1][j+1].fRight = false;
							}
							else if (Game[i+1][j+1].fBottom)
                     {
								Swap(i, j, 'B');
								Game[i+1][j+1].fBottom = false;
							}
						}
					}
				}
			}
		}

		switch (LOWORD(wParam))
      {
		case IDH_VIEWIMAGE:			
			DialogBox(NULL,MAKEINTRESOURCE(IDD_IMAGE),hWnd,(DLGPROC)ViewProc);
			break;
		case IDH_ABOUT:
			MessageBox(hWnd,
				_T("FFVIII Puzzle (Win32 Version)\nCreated by Sharfuddin\n2000\0"),
				_T("About FFVIII Puzzle..."),
				MB_OK|MB_ICONINFORMATION);			
			break;
		case IDF_RESTART:
			GameWon = false;
			Uninit();
			LoadBMPs();
			Clear_tMat();
			Rnd();
			GuardianBlocks();
			PlayingBlocks();
			Populate();
			MakeButtons();
			SendMessage(btn[SZ-1][SZ-1],BM_SETIMAGE, IMAGE_BITMAP, (long)NULL);
			break;
		case IDF_EXIT:
		case IDCANCEL:
			Uninit();
			EndDialog(hWnd,TRUE);
			PostQuitMessage(0);
			return TRUE;
			break;
		}

	}


	return FALSE;
}

long FAR PASCAL ViewProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HBITMAP bmpraijin;
	HWND hwndframe;
	switch (uMsg) {
	case WM_INITDIALOG:
		bmpraijin = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BMPRAIJIN));
		hwndframe = GetDlgItem(hWnd,IDC_BMPRAIJIN);
		SendMessage(hwndframe,STM_SETIMAGE,IMAGE_BITMAP,(long)bmpraijin);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
		case IDCANCEL:
			EndDialog(hWnd,TRUE);
			break;
		}
		break;
	}
	return FALSE;
}

void Uninit()
{
	unsigned int i, j;
	for (i = 0; i < SZ; i++)
   {
		for (j = 0; j < SZ; j++)
      {
			DeleteObject(hbmp[i][j]);
			hbmp[i][j]=NULL;
		}
	}

	for (i = 0; i < SZ+2; i++)
   {
		for (j = 0; j < SZ+2; j++)
      {
			DeleteObject(Game[i][j].hbmpImage);
			Game[i][j].hbmpImage=NULL;
		}
	}
}