// Object to display

#pragma once

class IDisplayableObject {
public:
	virtual void Display() const = 0;

	virtual ~IDisplayableObject() {};
};