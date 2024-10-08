#ifndef BRISTLE_H
#define BRISTLE_H

#include "Paper.h"
class Bristle
{
public:
	Bristle();
	~Bristle();

	// Public functions
	void updateState();
	void colorPaper(Paper& dest);

	// Getters/Setters
	void setColor(const float value);
	void setQuantity(const float value);
	void setPosition(const float x, const float y);
	float getColor() const;
	float getQuantity() const;
	float getPosition() const;


private:
	float m_color;
	float m_quantity;
	float m_relPosition[2];
};

#endif // BRISTLE_H

