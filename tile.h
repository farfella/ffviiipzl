#ifndef TILE_H_
#define TILE_H_

#include <windows.h>

class Tile
{
public:
	HBITMAP  hbmpImage;  // (HBITMAP) image
	int      nXPos;      // (int) x-position
	int      nYPos;      // (int) y-position
	bool     fLeft;      // (bool) left
	bool     fRight;     // (bool) right
	bool     fTop;       // (bool) top
	bool     fBottom;    // (bool) bottom
	bool     fIsEmpty;   // (bool) self

	Tile
      (
      );

	Tile
      (
      bool bIsEmpty, 
      int  nXPos, 
      int  nYPos
      );

	void SetTile
      (
      bool  nIsEmpty,
      int   nXPos,
      int   nYPos
      );
};

#endif // TILE_H_