#ifndef __HS_WINDOW_H__
#define __HS_WINDOW_H__

#include "SDL.h"
#include "hs_wnd_evt_handler.h"

class HSWindow
{
public:
	HSWindow();
	~HSWindow();

	bool Create(const char* title, int width, int height);
	void Quit();
	bool IsRun() const { return !quit_; }
	void GetWindowSize(int* w, int* h);
	void SetWindowSize(int w, int h);

	void DoEvent(long delta, HSWindowEventHandler* handler);
	void Run();
	void RenderClear();
	void Present(long delta);

	void SetLogicalSize(int w, int h);
	void GetLogicalSize(int* w, int* h);
	void SetRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

	void DrawPoint(int x, int y);
	void DrawPoints(const SDL_Point* points, int count);
	void DrawLine(int x1, int y1, int x2, int y2);
	void DrawLines(const SDL_Point* lines, int count);
	void DrawRect(const SDL_Rect* rect);
	void DrawRects(const SDL_Rect* rect, int count);
	void FillRect(const SDL_Rect* rect);
	void FillRects(const SDL_Rect* rects, int count);

private:
	SDL_Window* window_;
	SDL_Renderer* render_;
	SDL_Event event_;
	bool quit_;
};

#endif