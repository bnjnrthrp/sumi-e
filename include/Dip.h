#ifndef DIP_H
#define DIP_H

#include "Brush.h"

class Dip
{
public:
	Dip();
	~Dip();

	void dipBrush(Brush& brush);

private:
	float m_trajectory[3];

};

#endif // DIP_H

