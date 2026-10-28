#ifndef READ_DATA_H
#define READ_DATA_H
Color color(int r, int g, int b, int a){
	return (Color){static_cast<unsigned char>(r%256),
	static_cast<unsigned char>(g%256),
	static_cast<unsigned char>(b%256),
	static_cast<unsigned char>(a%256)};
}
int colorSeed = 45;
Color color(string Name){
	string ColorName = Name + "   ";
	return (Color){static_cast<unsigned char>(ColorName[0]*colorSeed%256),
	static_cast<unsigned char>(ColorName[1]*colorSeed%256),
	static_cast<unsigned char>(ColorName[2]*colorSeed%256),
	255};
}
Color cGlobalMessage = (Color){160,200,160,255};
Color cHome = (Color){168,128,108,255};
Color cEdge = BLACK;
Color cToiletOpen  = (Color){108,108,108,255};
Color cToiletClosed = (Color){108,108,108,255};
Color cPublicPlaces = (Color){200,200,200,255};
Color cRoad = WHITE;
Color cText = BLACK;
Color cBackground = WHITE;

void setColor(){
	int r,g,b,a;
	ifstream colorRead;
	colorRead = ifstream("data/color/rgba/global_message.txt");
	colorRead >> r >> g >> b >> a;
	cGlobalMessage = color(r,g,b,a);
	colorRead.close();
	colorRead = ifstream("data/color/rgba/home.txt");
	colorRead >> r >> g >> b >> a;
	cHome = color(r,g,b,a);
	colorRead.close();
	colorRead = ifstream("data/color/rgba/edge.txt");
	colorRead >> r >> g >> b >> a;
	cEdge = color(r,g,b,a);
	colorRead.close();
	colorRead = ifstream("data/color/rgba/toilet_open.txt");
	colorRead >> r >> g >> b >> a;
	cToiletOpen = color(r,g,b,a);
	colorRead.close();
	colorRead = ifstream("data/color/rgba/toilet_closed.txt");
	colorRead >> r >> g >> b >> a;
	cToiletClosed = color(r,g,b,a);
	colorRead.close();
	colorRead = ifstream("data/color/rgba/public_places.txt");
	colorRead >> r >> g >> b >> a;
	cPublicPlaces = color(r,g,b,a);
	colorRead.close();
	colorRead = ifstream("data/color/rgba/road.txt");
	colorRead >> r >> g >> b >> a;
	cRoad = color(r,g,b,a);
	colorRead.close();
	colorRead = ifstream("data/color/rgba/text.txt");
	colorRead >> r >> g >> b >> a;
	cText = color(r,g,b,a);
	colorRead.close();
	colorRead = ifstream("data/color/rgba/background.txt");
	colorRead >> r >> g >> b >> a;
	cBackground = color(r,g,b,a);
	colorRead.close();
	colorRead = ifstream("data/color/color_seed.txt");
	colorRead >> colorSeed;
	colorRead.close();
}

int tEnd = 10000;
int tGlobalMessageDefault = 500;
int fps = 10;
double MIN_HEALTH = 0.05;
bool bBotStop = 1;
double WindowSizeRate = 1;
bool keyboardHelper = 0;
int botMode = 0;
string botName = "bot_default";
void setTime(){
	ifstream read = ifstream("data/time/end.txt");
	read >> tEnd;
	read.close();
	read = ifstream("data/time/global_message.txt");
	read >> tGlobalMessageDefault;
	read.close();
	read = ifstream("data/time/fps.txt");
	read >> fps;
	read.close();
	read = ifstream("data/gamedata/min_health.txt");
	read >> MIN_HEALTH;
	read.close();
	read = ifstream("data/gamedata/debug_mode.txt");
	read >> DEBUGING;
	read.close();
	read = ifstream("data/gamedata/bot_stop.txt");
	read >> bBotStop;
	read.close();
	read = ifstream("data/gamedata/window_size_rate.txt");
	read >> WindowSizeRate;
	read.close();
	read = ifstream("data/gamedata/keyboard_helper.txt");
	read >> keyboardHelper;
	read.close();
	read = ifstream("data/gamedata/bot_mode.txt");
	read >> botMode;
	read.close();
	read = ifstream("data/bot/bot_name.txt");
	read >> botName;
	read.close();
}
#endif