/*
 * Mouse.h
 *
 *  Created on: 16-Oct-2016
 *      Author: amu
 */

#ifndef __MOUSE_H__
#define __MOUSE_H__

namespace core {

class Mouse {
public:
	Mouse(void);
	~Mouse(void);

	void move(int x, int y);

	inline int getX(void) const { return mX; }
	inline int getY(void) const { return mY; }

private:
	void moveMouse(int x, int y);

private:
	int mX;
	int mY;
};

} /* end namespace core */

#endif /* __MOUSE_H__ */
