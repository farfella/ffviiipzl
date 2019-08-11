#ifndef _GMAT_H_
#define _GMAT_H_

class gMat {
public:
	int nXPos;      // x-pos
	int nYPos;      // y-pos

	gMat
      (
      );

	gMat
      (
      int nXPos,
      int nYPos
      );

	void SetXY
      (
      int nXPos,
      int nYPos
      );
};

#endif // _GMAT_H_