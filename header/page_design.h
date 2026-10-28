#ifndef PAGE_DESIGN_H
#define PAGE_DESIGN_H
bool ClickDown(){
	if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) || IsKeyDown(KEY_ENTER) || GetTouchPointCount()!=0)return 1;
	return 0;
}
void GlobalMessage(string _Message, int time = tGlobalMessageDefault){
	BeginDrawing();
	ClearBackground(cGlobalMessage);
	drawText(_Message.c_str(),10,10,50,cText);
	EndDrawing();
	WaitTime(time/1000.0);
	return;
}

//		defined in showmap.h
bool shouldShow = 1;		//to mark whether ShowMap() should be done.
void ShowMap(void);
bool ProcessTextInput(std::string& text, int maxLength = -1)
{
    bool changed = false;
	shouldShow = 1;
	while(!ClickDown()){
		ShowMap();
	    // Handle character input
	    int key = GetCharPressed();
	    while (key > 0)
	    {
		        // Keep only printable ASCII characters (space to ~)
	        if (key >= 32 && key <= 126)
	        {
	            if (maxLength < 0 || (int)text.size() < maxLength)
	            {
	            	if(key == ' ')key = '_';
	                text.push_back((char)key);
	                changed = true;
	            }
	        }
	        key = GetCharPressed();  // consume next character (if multiple pressed in same frame)
	    }
	
	    // Handle backspace
	    if (IsKeyPressed(KEY_BACKSPACE))
	    {
	        if (!text.empty())
	        {
            	text.pop_back();
        	    changed = true;
    	    }
	    }
	    drawText(("Enter text:"+text).c_str(),2000,0,50,cText);
	}
	shouldShow = 1;
	while(ClickDown()){ShowMap();}
    return changed;
}

int choose(const vector<button_t> &vbChoice,bool TopPage = 1){
	while(ClickDown()){ShowMap();};
	if(TopPage)pageY = 0;
	button_t bUp(0,"\t\t/\\",2000,(BUTTON_WIDTH+FONT_SIZE)*21+pageY,400,FONT_SIZE);
	button_t bDown(0,"\t\t\\/",2000,(BUTTON_WIDTH+FONT_SIZE)*22+pageY,400,FONT_SIZE);
	button_t bBack(0,"\t\t<-",2000,(BUTTON_WIDTH+FONT_SIZE)*23+pageY,400,FONT_SIZE);
	button_t bTop(0,"\t\t^^^",2000,(BUTTON_WIDTH+FONT_SIZE)*24+pageY,400,FONT_SIZE);
	button_t bLeft(0,"\t\t<",2000,(BUTTON_WIDTH+FONT_SIZE)*25+pageY,400,FONT_SIZE);
	button_t bRight(0,"\t\t>",2000,(BUTTON_WIDTH+FONT_SIZE)*26+pageY,400,FONT_SIZE);
	
	while(GetCharPressed()!=0){};
	//if not clicked wait for click, showing map and buttons
	while(!ClickDown()){
		ShowMap();
		bUp.show();bDown.show();bBack.show();bTop.show();bLeft.show();bRight.show();
		for(auto bChoice : vbChoice)if(bChoice.onPage())bChoice.show();
		//draw the letters for keyboard input
		if(keyboardHelper)
		for(char cShow = 'A';cShow <= 'T';++cShow)
			drawText(string(1,cShow).c_str(),2500,(BUTTON_WIDTH+FONT_SIZE)*(cShow-'A'+1),FONT_SIZE,cText);
		//input with keyboard is only when it's not clicked down
		int key = GetCharPressed();
		if(key>='a' && key<='z'){
			return key - 'a'+1+(pageY/(BUTTON_WIDTH+FONT_SIZE));
		}
		if(key>='A' && key<='Z'){
			return key - 'A'+1+(pageY/(BUTTON_WIDTH+FONT_SIZE));
		}
	}
	int x = getMouseX();
	int y = getMouseY()+pageY;
	//check for the buttons
	if(bBack.on(x,y)){while(ClickDown()){ShowMap();for(auto bChoice : vbChoice)if(bChoice.onPage())bChoice.show();}return 0;}
	else if(bUp.on(x,y)){pageY = max(pageY-21*(BUTTON_WIDTH+FONT_SIZE),0);}
	else if(bDown.on(x,y)){pageY += 21*(BUTTON_WIDTH+FONT_SIZE);}
	else if(bLeft.on(x,y)){pageX = max(pageX-1500,0);}
	else if(bRight.on(x,y)){pageX += 1500;}
	else if(bTop.on(x,y)){pageY = 0;}
	else for(auto bChoice : vbChoice){
		if(bChoice.on(x,y)){
			shouldShow = 1;
			//cannot return if still clicking.
			//or it wound(might) go into a new round.
			//show map and buttons if still clicking.
			while(ClickDown()){
				bUp.show();bDown.show();bBack.show();bTop.show();bLeft.show();bRight.show();
				ShowMap();
				for(auto bChoiceShow : vbChoice)if(bChoiceShow.onPage())bChoiceShow.show();
			}
			if(DEBUGING)cout<<"Chosen"<<bChoice.number()<<".\n";
			return bChoice.number();
		}
	}
	while(ClickDown()){
		ShowMap();
		for(auto bChoice : vbChoice)if(bChoice.onPage())bChoice.show();
	}
	return choose(vbChoice,0);
}
#endif