#pragma once

class vec2
{
public:
	vec2() { _x = 0; _y = 0; }
	vec2(int x, int y) { _x = x; _y = y; }
	void set(int x, int y) { _x = x; _y = y; }

	int _x, _y;

private:

};


