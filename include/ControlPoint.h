#ifndef CONTROLPOINT_H
#define CONTROLPOINT_H

class ControlPoint
{
public:
	ControlPoint();
	~ControlPoint();

private:
	m_x;
	m_y;
	m_pressure;
};

ControlPoint::ControlPoint(float x, float y, float pressure)
	: m_x(x),
	  m_y(y),
	  m_pressure(pressure)
{
}

ControlPoint::~ControlPoint()
{
};

#endif // CONTROLPOINT_H
