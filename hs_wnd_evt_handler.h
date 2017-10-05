#ifndef __HS_WND_EVT_HANDLER_H__
#define __HS_WND_EVT_HANDLER_H__

#include <stdint.h>

class HSWindowEventHandler
{
public:
	virtual void OnKeyDown(char key_value, uint8_t repeat) = 0;
	virtual void OnKeyUp(char key_value, uint8_t repeat) = 0;
	virtual void OnMouseMotion(int x, int y, int xrel, int yrel) = 0;
	virtual void OnMouseButtonDown(uint8_t button_index, int x, int y) = 0;
	virtual void OnMouseButtonUp(uint8_t button_index, int x, int y) = 0;
	virtual void OnMouseWheel(int x, int y) = 0;
};

#endif