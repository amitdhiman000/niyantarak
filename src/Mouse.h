/* Copyright 2016 Amit Dhiman
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
* Dated : 24-02-2017
* Author : Amit Dhiman <amitdhiman000@gmail.com>
* Description : Mouse class
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
