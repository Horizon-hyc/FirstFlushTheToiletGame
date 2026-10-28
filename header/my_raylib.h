#ifndef MY_RAYLIB_H
#define MY_RAYLIB_H
void drawText(const char* text, int x, int y, int size, Color textColor){
	DrawText(text,x*WindowSizeRate, y*WindowSizeRate, size*WindowSizeRate,textColor);
}
void drawRectangle(int x, int y, int dx, int dy, Color textColor){
	DrawRectangle(x*WindowSizeRate, y*WindowSizeRate, dx*WindowSizeRate, dy*WindowSizeRate,textColor);
}
void drawCircle(int x, int y, int size, Color textColor){
	DrawCircle(x*WindowSizeRate, y*WindowSizeRate, size*WindowSizeRate,textColor);
}
int getMouseX(){
	return GetMouseX()/WindowSizeRate;
}
int getMouseY(){
	return GetMouseY()/WindowSizeRate;
}
#endif