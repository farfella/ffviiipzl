#include "gmat.h"

gMat::gMat
   (
   )
{
	nXPos = nYPos = 0;
}

gMat::gMat
   (
   int nXPos, 
   int nYPos
   )
{
	this->nXPos = nXPos;
   this->nYPos = nYPos;
}

void gMat::SetXY
   (
   int nXPos, 
   int nYPos
   )
{
	this->nXPos = nXPos;
   this->nYPos = nYPos;
}

