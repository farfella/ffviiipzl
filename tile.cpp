#include "tile.h"

Tile::Tile
   (
   )
{
	fIsEmpty = fLeft = fRight = fTop = fBottom = false;
	hbmpImage = NULL;
   nXPos = nYPos = 0;
}

Tile::Tile
   (
   bool fIsEmpty, 
   int  nXPos, 
   int  nYPos
   )
{
	this->fIsEmpty = fIsEmpty;
	this->nXPos = nXPos;
	this->nYPos = nYPos;
}

void Tile::SetTile
   (
   bool fIsEmpty,
   int nXPos,
   int nYPos
   )
{
	this->fIsEmpty= fIsEmpty;
	this->nXPos = nXPos;
	this->nYPos = nYPos;
}