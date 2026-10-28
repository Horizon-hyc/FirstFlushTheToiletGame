#ifndef BUTTON_H
#define BUTTON_H
#define BUTTON_WIDTH 3
#define FONT_SIZE 50
int pageY = 0;
int pageX = 0;
class button_t{
private:
	int __number;
	string __name;
	int __x;
	int __y;
	int __delta_x;
	int __delta_y;
public:
	button_t(int n, string s){
		__number = n;
		__name = s;
		__x = 2000;
		__y = n*FONT_SIZE+n*BUTTON_WIDTH;
		__delta_x = 400;
		__delta_y = FONT_SIZE;
	}
	button_t(int n, string s,int x,int y,int dx,int dy){
		__number = n;
		__name = s;
		__x = x;
		__y = y;
		__delta_x = dx;
		__delta_y = dy;
	}
	bool onPage(){
		return (0 <= __y-pageY && __y-pageY <= 20*(BUTTON_WIDTH+FONT_SIZE));
	}
	bool on(int x,int y){
		if(__x <= x)
		if(__y <= y)
		if(__x+__delta_x >= x)
		if(__y+__delta_y >= y)
		return 1;
		return 0;
	}
	int number(){return __number;}
	string name(){return __name;}
	void show(){
		drawRectangle(__x,__y-pageY,__delta_x,__delta_y,cText);
		drawRectangle(__x+BUTTON_WIDTH,__y+BUTTON_WIDTH-pageY,
		__delta_x-2*BUTTON_WIDTH,__delta_y-2*BUTTON_WIDTH,cBackground);
		if(!on(getMouseX(),getMouseY()-pageY))
			drawText(__name.c_str(),__x+2*BUTTON_WIDTH,__y+2*BUTTON_WIDTH-pageY,FONT_SIZE-2*BUTTON_WIDTH,cText);
		else 
			drawText(__name.c_str(),__x+2*BUTTON_WIDTH,__y+2*BUTTON_WIDTH-pageY,FONT_SIZE-2*BUTTON_WIDTH,BLUE);
	
	}
};
#define vbut_t vector<button_t>
#endif