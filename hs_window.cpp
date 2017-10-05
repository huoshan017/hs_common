#include "hs_window.h"
#include <stdio.h>
#include <time.h>

HSWindow::HSWindow() : window_(NULL), render_(NULL), quit_(false)
{
	memset(&event_, 0, sizeof(event_));
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
	}
}

HSWindow::~HSWindow()
{
	SDL_Quit();
}

bool HSWindow::Create(const char* title, int width, int height)
{
	window_ = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
	if (!window_) {
		fprintf(stderr, "Couldn't create %dx%d window: %s\n", width, height, SDL_GetError());
		return false;
	}

	render_ = SDL_CreateRenderer(window_, 0, SDL_RENDERER_ACCELERATED);

	return true;
}

void HSWindow::Quit()
{
	quit_ = true;
}

void HSWindow::GetWindowSize(int* w, int* h)
{
	SDL_GetWindowSize(window_, w, h);
}

void HSWindow::SetWindowSize(int w, int h)
{
	SDL_SetWindowSize(window_, w, h);
}

void HSWindow::DoEvent(long delta, HSWindowEventHandler* handler)
{
	if (!handler)
		return;

	if (!quit_) {
		SDL_WaitEvent(&event_);
		switch (event_.type) {
		case SDL_KEYDOWN:
			handler->OnKeyDown(event_.key.keysym.sym, event_.key.repeat);
			break;
		case SDL_KEYUP:
			handler->OnKeyUp(event_.key.keysym.sym, event_.key.repeat);
			break;
		case SDL_MOUSEMOTION:
			handler->OnMouseMotion(event_.motion.x, event_.motion.y, event_.motion.xrel, event_.motion.yrel);
			break;
		case SDL_MOUSEBUTTONDOWN:
			handler->OnMouseButtonDown(event_.button.button, event_.button.x, event_.button.y);
			break;
		case SDL_MOUSEBUTTONUP:
			handler->OnMouseButtonUp(event_.button.button, event_.button.x, event_.button.y);
			break;
		case SDL_MOUSEWHEEL:
			handler->OnMouseWheel(event_.wheel.x, event_.wheel.y);
			break;
		case SDL_QUIT:
			quit_ = true;
			break;
		default:
			break;
		}
	}
}

void HSWindow::Run()
{
	static long last_tick = 0;
	while (!quit_) {
		long now_tick = clock();
		if (!last_tick)
			last_tick = now_tick;

		long elpased = now_tick - last_tick;
		DoEvent(elpased, NULL);
	}
}

void HSWindow::RenderClear()
{
	SDL_RenderClear(render_);
}

void HSWindow::Present(long delta)
{
	SDL_RenderPresent(render_);
}

void HSWindow::SetRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	SDL_SetRenderDrawColor(render_, r, g, b, a);
}

void HSWindow::SetLogicalSize(int w, int h)
{
	SDL_RenderSetLogicalSize(render_, w, h);
}

void HSWindow::GetLogicalSize(int* w, int* h)
{
	SDL_RenderGetLogicalSize(render_, w, h);
}

void HSWindow::DrawPoint(int x, int y)
{
	SDL_RenderDrawPoint(render_, x, y);
}

void HSWindow::DrawPoints(const SDL_Point* points, int count)
{
	SDL_RenderDrawPoints(render_, points, count);
}

void HSWindow::DrawLine(int x1, int y1, int x2, int y2)
{
	SDL_RenderDrawLine(render_, x1, y1, x2, y2);
}

void HSWindow::DrawLines(const SDL_Point* points, int count)
{
	SDL_RenderDrawLines(render_, points, count);
}

void HSWindow::DrawRect(const SDL_Rect* rect)
{
	SDL_RenderDrawRect(render_, rect);
}

void HSWindow::DrawRects(const SDL_Rect* rects, int count)
{
	SDL_RenderDrawRects(render_, rects, count);
}

void HSWindow::FillRect(const SDL_Rect* rect)
{
	SDL_RenderFillRect(render_, rect);
}

void HSWindow::FillRects(const SDL_Rect* rects, int count)
{
	SDL_RenderFillRects(render_, rects, count);
}