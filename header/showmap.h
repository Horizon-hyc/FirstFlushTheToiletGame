#ifndef SHOWMAP_H
#define SHOWMAP_H
int showPage = 0;
int playerLine = 0;
bool begun = 0;
vector<int> viPlayerList = vector<int>();

#define NOTICEBOARD -n-1
void ShowMap(){
BeginDrawing();
if(!begun){ClearBackground(cBackground);}
//notice board
else if(showPage == NOTICEBOARD){
	
	ClearBackground(cBackground);
	drawRectangle(0,0,150,150,cText);
	drawRectangle(5,5,140,140,cBackground);
	drawText("<",15,15,180,cText);
	drawText("Notice Board",200,10,100,BLUE);
	int countLine = 0;
    playerLine = countLine;
    viPlayerList = vector<int>();
	for(int j = 1;j<=n;j++){
		if(a[j].hp>MIN_HEALTH && a[j].invisibleTime==0)
    	viPlayerList.push_back(j);
	}
	int col = 0;
	for(auto iPlayer:viPlayerList){
		drawRectangle(10+450*(col%4),200+countLine,440,55,color(a[iPlayer].name));
		drawRectangle(15+450*(col%4),205+countLine,430,45,cBackground);
		drawText(a[iPlayer].name.c_str(),20+450*(col%4),205+countLine,50,color(a[iPlayer].name));
		col++;
		if(col%4==0)countLine+=60;	
	}
	
	if(ClickDown()){
		int x = getMouseX();
		int y = getMouseY();
		if((5 <= x && x<= 155)&&(5 <= y && y <= 145)){showPage = 0;EndDrawing();return;}
		else for(int iPlayerNumber = 0;iPlayerNumber<viPlayerList.size();iPlayerNumber++){
			if(15+450*(iPlayerNumber%4) <= x && x <= 455+450*(iPlayerNumber%4)){
				if(205 + playerLine + iPlayerNumber/4*60 <= y && y <= 250 + playerLine + iPlayerNumber/4*60){
					showPage = -viPlayerList.at(iPlayerNumber);
					EndDrawing();return;
				}
			}
		}
	}
}
//main map
else if(showPage == 0){
	
	ClearBackground(cBackground);
	for(int i = 1;i <= 3*n+4;i++){
		if(!p[i].alive)continue;
		vector<int> viPlayer = vector<int>();
		for(int iPlayer = 1; iPlayer <= n; iPlayer++){
			if(a[iPlayer].hp>MIN_HEALTH && a[iPlayer].invisibleTime==0)if(a[iPlayer].loc == p[i].name)
				viPlayer.push_back(iPlayer);
		}
		if(i<=n){
			if(i*100+20-pageX > 0 && i*100+120-pageX <= 2000){
				drawRectangle(i*100+20-pageX,1100,100,100,cEdge);
				drawRectangle(i*100+30-pageX,1110,80,80,cHome);
				if(p[i].door == open)drawRectangle(i*100+30-pageX,1100,80,10,cHome);
				for(auto iPlayer : viPlayer){
					drawCircle(a[iPlayer].x-pageX,a[iPlayer].y,10,color(a[iPlayer].name));
				}
			}
		}
		else if(i<=2*n){
			if((i-n)*100+30-pageX > 0 && (i-n)*100+90-pageX <= 2000){
				drawRectangle((i-n)*100+30-pageX,1210,60,60,cEdge);
				if(p[i].door != closed)
					drawRectangle((i-n)*100+40-pageX,1220,40,40,cToiletOpen);		
				else
					drawRectangle((i-n)*100+40-pageX,1220,40,40,cToiletClosed);	
				for(auto iPlayer : viPlayer){
					drawCircle(a[iPlayer].x-pageX,a[iPlayer].y,10,color(a[iPlayer].name));
				}	
			}
		}
		else if(i==SUPERMARKET){
			drawRectangle(10,200,1700,200,cEdge);
			drawRectangle(20,210,1680,180,cPublicPlaces);
			for(auto iPlayer : viPlayer){
				drawCircle(a[iPlayer].x,a[iPlayer].y,10,color(a[iPlayer].name));
			}
		}
		else if(i==DRUGSTORE){
			drawRectangle(1700,200,100,200,cEdge);
			drawRectangle(1710,210,80,180,cPublicPlaces);
			for(auto iPlayer : viPlayer){
				drawCircle(a[iPlayer].x,a[iPlayer].y,10,color(a[iPlayer].name));
			}
		}
		else if(i==ROAD){
			drawRectangle(20,430,1700,670,cRoad);
			for(auto iPlayer : viPlayer){
				drawCircle(a[iPlayer].x,a[iPlayer].y,10,color(a[iPlayer].name));
			}
		}
		else {
			if((i-DRUGSTORE-6)*100+30-pageX > 0 && (i-DRUGSTORE-6)*100+90-pageX <= 2000){
				drawRectangle((i-DRUGSTORE-6)*100+30-pageX,10,60,60,cEdge);
				if(p[i].door != closed)
					drawRectangle((i-DRUGSTORE-6)*100+40-pageX,20,40,40,cToiletOpen);
				else 
					drawRectangle((i-DRUGSTORE-6)*100+40-pageX,20,40,40,cToiletClosed);
				for(auto iPlayer : viPlayer){
					drawCircle(a[iPlayer].x-pageX,a[iPlayer].y,10,color(a[iPlayer].name));
				}
			}
		}
	}
	if(DEBUGING)drawText((to_string(getMouseX())+" "+to_string(getMouseY())).c_str(),200,200,200,BLACK);
	
	if(ClickDown()){
		const int x = getMouseX();
		const int y = getMouseY();
		if(x>=2000){EndDrawing();return;}
		if(1720 <= x && x <= 2000)if(430 <= y && y <= 1000){showPage = NOTICEBOARD;EndDrawing();return;}
		for(int i = 1; i <= 3*n+4;i++){
			if(i<=n){
				if(1110 <= y && y <= 1190)
				if(i*100+30-pageX <= x && x <= i*100+110-pageX){showPage = i;EndDrawing();return;}
			}
			else if(i<=2*n){
				if(1210 <= y && y <= 1290)
				if((i-n)*100+30-pageX <= x && x <= (i-n)*100+90-pageX ){showPage = i;EndDrawing();return;}
			}
			else if(i==SUPERMARKET){
				if(20 <= x && x <= 1680)if(210 <= y && y <= 390){showPage = i;EndDrawing();return;}
			}
			else if(i==DRUGSTORE){
				if(1710 <= x && x <= 1790)if(210 <= y && y <= 390){showPage = i;EndDrawing();return;}
			}
			else if(i==ROAD){
				if(20 <= x && x <= 1720)if(430 <=y && y<= 1000){showPage = i;EndDrawing();return;}
			}
			else if(20 <= y && y <= 60)if((i-DRUGSTORE-6)*100+40-pageX  <= x && x <= (i-DRUGSTORE-6)*100+80-pageX ){
					showPage = i;EndDrawing();return;
			}
		}		//change pages
	}
}
//places
else if(showPage > 0){
	int i = showPage;
	
	ClearBackground(cBackground);
	drawRectangle(0,0,150,150,cText);
	drawRectangle(5,5,140,140,cBackground);
	drawText("<",15,15,180,cText);
	drawText(p[i].name.c_str(),200,10,100,BLUE);
    if(p[i].oiled)drawText("(oiled).\n",800,210,100,cText);
    if(p[i].door==none)drawText("(no door)",1500,10,100,cText);
	else if(p[i].door==closed)drawText("(closed)",1500,10,100,cText);
	else if(p[i].door==open)drawText("(open)",1500,10,100,cText);
	int countLine = 0;
    if(p[i].knife!=0){drawText(("knife: "+sval(p[i].knife)).c_str(),210,210+countLine,50,cText);countLine+=100;}
    if(p[i].rpg!=0){drawText(("rpg: "+sval(p[i].rpg)).c_str(),210,210+countLine,50,cText);countLine+=100;}
    if(p[i].rocket!=0){drawText(("rocket: "+sval(p[i].rocket)).c_str(),210,210+countLine,50,cText);countLine+=100;}
    if(p[i].health!=0){drawText(("health: "+sval(p[i].health)).c_str(),210,210+countLine,50,cText);countLine+=100;}
    if(p[i].poison!=0){drawText(("poison: "+sval(p[i].poison)).c_str(),210,210+countLine,50,cText);countLine+=100;}
    if(p[i].invisible!=0){drawText(("invisible: "+sval(p[i].invisible)).c_str(),210,210+countLine,50,cText);countLine+=100;}
    if(p[i].oil!=0){drawText(("oil: "+sval(p[i].oil)).c_str(),210,210+countLine,50,cText);countLine+=100;}
    playerLine = countLine;
    viPlayerList = vector<int>();
	for(int j = 1;j<=n;j++){
		if(a[j].hp>MIN_HEALTH && a[j].invisibleTime==0 && a[j].loc==p[i].name)
    	viPlayerList.push_back(j);
	}
	int col = 0;
	for(auto iPlayer:viPlayerList){
		drawRectangle(10+450*(col%4),200+countLine,440,55,color(a[iPlayer].name));
		drawRectangle(15+450*(col%4),205+countLine,430,45,cBackground);
		drawText(a[iPlayer].name.c_str(),20+450*(col%4),205+countLine,50,color(a[iPlayer].name));
		col++;
		if(col%4==0)countLine+=60;
	}
	
	if(ClickDown()){
		int x = getMouseX();
		int y = getMouseY();
		if(x>=2000){EndDrawing();return;}
		if((5 <= x && x<= 155)&&(5 <= y && y <= 145)){showPage = 0;EndDrawing();return;}
		else for(int iPlayerNumber = 0;iPlayerNumber<viPlayerList.size();iPlayerNumber++){
			if(15+450*(iPlayerNumber%4) <= x && x <= 455+450*(iPlayerNumber%4)){
				if(205 + playerLine + iPlayerNumber/4*60 <= y && y <= 250 + playerLine + iPlayerNumber/4*60){
					showPage = -viPlayerList.at(iPlayerNumber);
					EndDrawing();return;
				}
			}
		}
	}
}
//players
else if(showPage < 0){
	int iPlayer = -showPage;
	
	ClearBackground(cBackground);
	drawRectangle(0,0,150,150,cText);
	drawRectangle(5,5,140,140,cBackground);
	drawText("<",15,15,180,cText);
	drawText(a[iPlayer].name.c_str(),200,10,100,color(a[iPlayer].name));
	string sInfo = "";
	if(a[iPlayer].hp<=0){sInfo = a[iPlayer].name + " is dead.\n";}
	else {
		sInfo = sInfo + "    health:"+fraction(a[iPlayer].hp).str()+"\n";
	    sInfo = sInfo +  "    location: "+a[iPlayer].loc+"\n";
	    if(a[iPlayer].sleep == 0)sInfo = sInfo +  "    sleeping"+"\n";
	    else if(a[iPlayer].sleep == 1)sInfo = sInfo +  "    daydreaming"+"\n";
	    else sInfo = sInfo + "    awake"+"\n";
	    
		if(a[iPlayer].h1!="fist")sInfo = sInfo +   "    hand1: " + a[iPlayer].h1 + "\n";
		if(a[iPlayer].h1=="rocket")sInfo = sInfo + "(" + to_string(a[iPlayer].leftRocket) + ")\n";
   	    if(a[iPlayer].h2!="fist")sInfo = sInfo +   "    hand2: " + a[iPlayer].h2 + "\n";
		if(a[iPlayer].h2=="rocket")sInfo = sInfo + "(" + to_string(a[iPlayer].rightRocket) + ")\n";
   	    if(a[iPlayer].shoulder!="empty")sInfo = sInfo + "    shoulder: "+a[iPlayer].shoulder+"\n";
   	    if(a[iPlayer].aim != ""){sInfo = sInfo +   "    (aimed at "+a[iPlayer].aim+")\n";}
   	    if(a[iPlayer].invisibleTime > 0){sInfo = sInfo +   "    Invisible.\n";}
    }
    drawText(sInfo.c_str(),10,200,50,cText);
	if(ClickDown()){
		int x = getMouseX();
		int y = getMouseY();
		if(x>=2000){EndDrawing();return;}
		if(5 <= x && x<= 155)if(5 <= y && y <= 145){showPage = 0;EndDrawing();return;}
	}
}
EndDrawing();
}
#endif