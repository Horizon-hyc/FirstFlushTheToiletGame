

#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <fstream>
using namespace std;
#include "D:/libraries/raylib-4.5.0_win64_mingw-w64/include/raylib.h"
#include "header/debuging.h"
#include "header/read_data.h"
#include "header/my_raylib.h"
#include "header/button.h"
#include "header/page_design.h"

#include "header/fraction_bydog.h"
using namespace frac;

#include "header/basic.h"
#include "header/computer.h"
#include "header/player_action.h"
#include "header/showmap.h"




void playForOnce(){
	initWorld();
	begun = 1;
	vector<button_t> vbN = vector<button_t>();
    vbN.push_back(button_t(0,"Player amount is:"));
    for(int i = 2;i < 1000; i++){
    	vbN.push_back(button_t(i,to_string(i)));
	}
	while(n<2){
   		n = choose(vbN);
   		if(n==0)return;
   	}
   	begun = 1;
   	vbut_t vbRealPlayerAmount = vbut_t();
    vbRealPlayerAmount.push_back(button_t(0,"Real player amount is:"));
    for(int i = 1;i <= n; i++){
    	vbRealPlayerAmount.push_back(button_t(i,to_string(i)));
	}
	while(realPlayerAmount<=0){
   		realPlayerAmount = choose(vbRealPlayerAmount);
	}
	inita(n);
    while(!rockpaper()){};
    string WinningMessage = "Game Ended.\nWinner is ";
    int i = 1;
	for(;i<=n;++i)if(a[i].hp>MIN_HEALTH){break;}
    WinningMessage+=a[i].name;
	if(i == n+1)WinningMessage = "Game Ended.\nThis world seems to be doomed to be destroyed";
    WinningMessage += ".\nCongratulations!\n";
    GlobalMessage(WinningMessage,tEnd);
    begun = 0;
}
void trainBot(){
	if(botMode==0)return;
	if(botMode==1){
		trainable::trainBot();
	}
}
void playOnline(){
	GlobalMessage("NOT OPEN YET!!!");
}
void changeSettings(){
	GlobalMessage("Type \"end\" to continue.\n");
	system("start Change_Data.exe");
	setColor();
	setTime();
}
int main(){
	//initialization
    srand((unsigned)time(0));
    setColor();
    setTime();
    InitWindow(2560*WindowSizeRate,1440*WindowSizeRate,"Toilet Game");
    SetTargetFPS(fps);
    BeginDrawing();
    ClearBackground(cBackground);
    EndDrawing();
    //choose mode
    vbut_t vbMode = vbut_t();
    vbMode.push_back(button_t(0,"Quit"));
    vbMode.push_back(button_t(1,"Play"));
    vbMode.push_back(button_t(2,"Settings"));
    vbMode.push_back(button_t(3,"Play online"));
    vbMode.push_back(button_t(4,"Train bots"));
    while(1){
    int mode = choose(vbMode);
    switch(mode){
    	case 0:{
    		goto endGame;
			break;
		}
		case 1:{
			playForOnce();
			break;
		}
		case 2:{
			changeSettings();
			break;
		}
		case 3:{
			playOnline();
			break;
		}
		case 4:{
			trainBot();
			break;
		}
	}
    
    
	}
	endGame:
		return 0;
    return 0;
}
