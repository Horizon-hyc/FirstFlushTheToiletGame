#ifndef PLAYER_ACTION_H
#define PLAYER_ACTION_H
void action(int num);		//just declaring it to use in rockpaper()

bool rockpaper(){

	vector<int> order = scramble();
	for(auto iPlayer:order){
		if(a[iPlayer].stuckTime>0)a[iPlayer].stuckTime--;
		if(a[iPlayer].invisibleTime>1)a[iPlayer].invisibleTime-=1;
		else if(a[iPlayer].invisibleTime == 1){
			if(a[iPlayer].real){
				a[iPlayer].invisibleTime = 0;
				vbut_t vbChoice = vbut_t();
				vbChoice.push_back(button_t(0,a[iPlayer].name+" revives at:\n"));
				vector<int> viPlace = vector<int>();
				int iPlace = 0;
				for(int i = 1;i<=3*n+4;i++){
					if(p[i].alive){
						iPlace++;
						viPlace.push_back(i);
						vbChoice.push_back(button_t(iPlace,p[i].name));
					}
				}
				while(1){
					iPlace = choose(vbChoice);
					if(iPlace>=1 && iPlace < viPlace.size()){
						a[iPlayer].move(p[iPlace].name);
						break;
					}
					if(DEBUGING)cout<<"Overflow.\n";
				}
			}
			else {
				a[iPlayer].invisibleTime = 0;
				vector<int> viPlace = vector<int>();
				int iPlace = 0;
				for(int i = 1;i<=3*n+4;i++){
					if(p[i].alive){
						iPlace++;
						viPlace.push_back(i);
					}
				}
				iPlace = randInt(viPlace.size());
				if(iPlace>=1 && iPlace < viPlace.size()){
					a[iPlayer].move(p[viPlace.at(iPlace-1)].name);
					break;
				}
			}
		}
	}
	int alivePlayer = 0;
	for(int i = 1;i<=n;i++)if(a[i].hp>MIN_HEALTH)alivePlayer++;
	if(alivePlayer<=1){return 1;}
	int movingPlayerCount = randInt(1,n-1);
	order.resize(movingPlayerCount);
	order.shrink_to_fit();
	string TurnMessage = "Moving players this turn:\n";
	int count = 0;
	for(auto i:order){
		if(a[i].hp>MIN_HEALTH && a[i].invisibleTime==0 && a[i].stuckTime==0){
			count++;
			TurnMessage+=a[i].name+" ";
			if(count % 4 == 0)TurnMessage+="\n";
		}
	}
	if(TurnMessage == "Moving players this turn:\n")TurnMessage+="NOBODY is going to move.";
	GlobalMessage(TurnMessage);
	for(auto i:order){
		if(a[i].hp>MIN_HEALTH && a[i].invisibleTime==0 && a[i].stuckTime==0){
			if(a[i].real)action(i);
			else autoAction(i,botMode);
			if(DEBUGING)cout<<a[i].name<<"'s turn finished.\n";
		}
	}
	GlobalMessage("Turn ended.\n\n\n");
	return 0;
}
void action(int num){
	while(ClickDown()){ShowMap();}
	vbut_t vbMove = vbut_t();
	vbMove.push_back(button_t(0,a[num].name+"'s Move:"));
	vbMove.push_back(button_t(1,"Change doors"));
	vbMove.push_back(button_t(2,"Move to"));
	vbMove.push_back(button_t(3,"Left hand"));
	vbMove.push_back(button_t(4,"Right hand"));
	vbMove.push_back(button_t(5,"Change status"));
	vbMove.push_back(button_t(6,"get things"));
	vbMove.push_back(button_t(7,"Upgrade"));
	vbMove.push_back(button_t(8,"Drag"));
	vbMove.push_back(button_t(9,"RPG"));
	vbMove.push_back(button_t(10,"Flush"));
	vbMove.push_back(button_t(11,"Drink tea"));
	const int moveType = choose(vbMove);
	if(moveType == 0){
        playerstat();
        placestat();
		action(num);	
	}
	if(moveType == 1){
		if(a[num].sleep==asleep){GlobalMessage("You're asleep.\n");action(num);return;}
		const int iHere = findPlaceNumber(a[num].loc);
		int iPlace = 0;
		vbut_t vbChoice = vbut_t();
		vector<int> viPlace = vector<int>();
		if(iHere<=n){
			iPlace++;
			viPlace.push_back(iHere+n);
			vbChoice.push_back(button_t(iPlace,p[iHere+n].name));
		}
		if(iHere==SUPERMARKET){
			for(int i = DRUGSTORE+1;i<=3*n+4;i++){
				iPlace++;
				viPlace.push_back(i);
				vbChoice.push_back(button_t(iPlace,p[i].name));
			}
			iPlace++;
			viPlace.push_back(DRUGSTORE);
			vbChoice.push_back(button_t(iPlace,"drug store"));
		}
		if(p[iHere].alive && p[iHere].door!=none){
			iPlace++;
			viPlace.push_back(iHere);
			vbChoice.push_back(button_t(iPlace,p[iHere].name));
		}
		if(iHere == ROAD){
			for(int i = 1; i<=n;i++){
				iPlace++;
				if(p[i].alive){viPlace.push_back(i);vbChoice.push_back(button_t(iPlace,p[i].name));}
				else {viPlace.push_back(i+n);vbChoice.push_back(button_t(iPlace,p[i+n].name));}
			}
		}
		if(viPlace.size() == 0){GlobalMessage("No doors.\n");action(num);return;}
		iPlace = choose(vbChoice);
		if(iPlace>viPlace.size()||iPlace<=0){if(DEBUGING)cout<<"Overflow.\n";action(num);return;}
		int placeNumber = viPlace.at(iPlace-1);
		if(p[placeNumber].door == closed)p[placeNumber].door = open;
		else if(p[placeNumber].door == open)p[placeNumber].door = closed;
	}
	if(moveType == 2){
		if(a[num].sleep == asleep){GlobalMessage("You're asleep.\n");action(num);return;}
		
		vector<int> viPossiblePlace = vector<int>();
		vbut_t vbChoice = vbut_t();
		vbChoice.push_back(button_t(0,"Possible places to go to:\n"));
		int iPlace = 0;
		for(int i=1;i<=3*n+4;i++){
			if(connected(p[findPlaceNumber(a[num].loc)],p[i])){
				iPlace++;
				viPossiblePlace.push_back(i);
				vbChoice.push_back(button_t(iPlace,p[i].name));
			}
		}		//set a vector of possible places.
		if(viPossiblePlace.size()==0){GlobalMessage("Nowhere to go.\n");action(num);return;}
		int iGoPlace = choose(vbChoice);
		if(iGoPlace > viPossiblePlace.size()||iGoPlace<=0){if(DEBUGING)cout<<"Overflow.\n";action(num);return;}
		int placeNumber = findPlaceNumber(p[viPossiblePlace.at(iGoPlace-1)].name);
		if(placeNumber == -1){GlobalMessage("Invalid input.\n");action(num);return;}
		if(connected(p[findPlaceNumber(a[num].loc)],p[placeNumber]))
			a[num].move( p[viPossiblePlace.at(iGoPlace-1)].name );
	}
	if(moveType == 3){
		if(a[num].sleep == asleep){GlobalMessage("You're asleep.\n");action(num);return;}
		if(a[num].h1 == "oil"){a[num].h1 = "fist";useOil(num);return;}
		vector<int> viPlayer=vector<int>();
		vbut_t vbChoice = vbut_t();
		vbChoice.push_back(button_t(0,"Use "+a[num].h1+" at:"));
		int iPlayer = 0;
		for(int i = 1;i<=playerCount;i++){
			if(a[num].loc == a[i].loc && a[i].hp>MIN_HEALTH && a[i].invisibleTime==0){
				iPlayer++;
				viPlayer.push_back(i);
				vbChoice.push_back(button_t(iPlayer,a[i].name));
			}
		}
		if(viPlayer.size() == 0){GlobalMessage("Nobody to use at.\n");action(num);return;}
		iPlayer = choose(vbChoice);
		if(iPlayer > viPlayer.size()||iPlayer<=0){if(DEBUGING)cout<<"Overflow.\n";action(num);return;}
		string sWeapon;
		if(a[num].h1 == "fist"){
			if(a[num].sleep == awake)sWeapon = "fist_awake";
			else if (a[num].sleep == daydream)sWeapon = "fist_dream";
			else sWeapon = "";
		}
		else sWeapon = a[num].h1;
		if(a[num].h1 == "poison" || a[num].h1 == "invisible" || a[num].h1 == "health"){
			a[num].h1 = "fist";
		}
		else if(viPlayer.at(iPlayer-1) ==num){GlobalMessage("Don't hurt yourself.\n");action(num);return;}
		use(viPlayer.at(iPlayer-1),sWeapon);
	}
	if(moveType == 4){
		if(a[num].sleep == asleep){GlobalMessage("You're asleep.\n");action(num);return;}
		if(a[num].h2 == "oil"){a[num].h2 = "fist";useOil(num);return;}
		vector<int> viPlayer=vector<int>();
		vbut_t vbChoice = vbut_t();
		vbChoice.push_back(button_t(0,"Use "+a[num].h2+" at:\n"));
		int iPlayer = 0;
		for(int i = 1;i<=playerCount;i++){
			if(a[num].loc == a[i].loc && a[i].hp>MIN_HEALTH && a[i].invisibleTime == 0){
				iPlayer++;
				viPlayer.push_back(i);
				vbChoice.push_back(button_t(iPlayer,a[i].name));
			}
		}
		if(viPlayer.size() == 0){GlobalMessage("Nobody to use at.\n");action(num);return;}
		iPlayer = choose(vbChoice);
		if(iPlayer > viPlayer.size()||iPlayer<=0){if(DEBUGING)cout<<"Overflow.\n";action(num);return;}
		string sWeapon;
		if(a[num].h2 == "fist"){
			if(a[num].sleep == awake)sWeapon = "fist_awake";
			else sWeapon = "fist_dream";
		}
		else sWeapon = a[num].h2;
		if(a[num].h2 == "poison" || a[num].h2 == "invisible" || a[num].h2 == "health"){
			a[num].h2 = "fist";
		}
		else if(viPlayer.at(iPlayer-1) ==num){GlobalMessage("Don't hurt yourself.\n");action(num);return;}
		use(viPlayer.at(iPlayer-1),sWeapon);
	}
	if(moveType == 5){
		if(stringSplit(a[num].loc,' ').size()>=2)if(stringSplit(a[num].loc,' ').at(1)=="toilet"){
			GlobalMessage("You're in a toilet.\n");action(num);return;
		}
		vbut_t vbChoice = vbut_t();
		vbChoice.push_back(button_t(0,"Change status into:\n"));
		vbChoice.push_back(button_t(1,"sleeping"));
		vbChoice.push_back(button_t(2,"daydreaming"));
		vbChoice.push_back(button_t(3,"awake"));
		int inStatus = choose(vbChoice);
		if(inStatus>3||inStatus<=0){if(DEBUGING)cout<<"Overflow.\n";action(num);return;}
		a[num].sleep = inStatus -1;
		if(a[num].sleep != awake){
			a[num].h1 = a[num].h2 = "fist";
			a[num].shoulder = "empty";
			a[num].rocket = a[num].leftRocket = a[num].rightRocket = 0;
			a[num].aim = "";
		}
	}
	if(moveType == 6){
		if(a[num].sleep != awake){
			GlobalMessage("You are not awake.\n");
			action(num);
			return;
		}
		vector<string> vsWeapon;
		vbut_t vbChoice = vbut_t();
		int iPlace = findPlaceNumber(a[num].loc);
		int iWeapon = 0;
		if(p[iPlace].name=="drug store" && p[iPlace].door!=closed){
			GlobalMessage("door not closed.\n");action(num);return;
		}
		iWeapon++;vsWeapon.push_back("fist");vbChoice.push_back(button_t(iWeapon,"fist"));
		if(p[iPlace].knife > 0){
			iWeapon++;vsWeapon.push_back("knife");vbChoice.push_back(button_t(iWeapon,"knife"));
		}
		if(p[iPlace].rpg > 0){
			iWeapon++;vsWeapon.push_back("rpg");vbChoice.push_back(button_t(iWeapon,"rpg"));
		}
		if(p[iPlace].rocket > 0 && a[num].shoulder != "empty"){
			iWeapon++;vsWeapon.push_back("rocket");vbChoice.push_back(button_t(iWeapon,"rocket"));
		}
		if(p[iPlace].health > 0){
			iWeapon++;vsWeapon.push_back("health");vbChoice.push_back(button_t(iWeapon,"health"));
		}
		if(p[iPlace].poison > 0){
			iWeapon++;vsWeapon.push_back("poison");vbChoice.push_back(button_t(iWeapon,"poison"));
		}
		if(p[iPlace].oil > 0){
			iWeapon++;vsWeapon.push_back("oil");vbChoice.push_back(button_t(iWeapon,"oil"));
		}
		if(p[iPlace].invisible > 0){
			iWeapon++;vsWeapon.push_back("invisible");vbChoice.push_back(button_t(iWeapon,"invisible"));
		}
		if(vsWeapon.size() == 0){GlobalMessage("Nothing to get.\n");action(num);return;}
		iWeapon = choose(vbChoice);
		if(iWeapon>vsWeapon.size()||iWeapon<=0){if(DEBUGING)cout<<"Overflow.\n";action(num);return;}
		if(vsWeapon.at(iWeapon-1) == "knife"){p[iPlace].knife -= 1;}
		if(vsWeapon.at(iWeapon-1) == "rpg"){p[iPlace].rpg -= 1;}
		if(vsWeapon.at(iWeapon-1) == "rocket"){p[iPlace].rocket -= 1;}
		if(vsWeapon.at(iWeapon-1) == "health"){p[iPlace].health -= 1;}
		if(vsWeapon.at(iWeapon-1) == "poison"){p[iPlace].poison -= 1;}
		if(vsWeapon.at(iWeapon-1) == "oil"){p[iPlace].oil -= 1;}
		if(vsWeapon.at(iWeapon-1) == "invisible"){p[iPlace].invisible -= 1;}
		vbChoice = vbut_t();
		vbChoice.push_back(button_t(0,"Get it to:"));
		vbChoice.push_back(button_t(1,"left"));
		vbChoice.push_back(button_t(2,"right"));
		vbChoice.push_back(button_t(3,"shoulder"));
		int hand = choose(vbChoice);
		if(hand>3||hand<=0){if(DEBUGING)cout<<"Overflow.\n";action(num);return;}
		if(hand == 1){
			if(a[num].h1 == "rocket" && vsWeapon.at(iWeapon-1)!="rocket")a[num].leftRocket=0;
			a[num].h1 = vsWeapon.at(iWeapon-1);
			if(vsWeapon.at(iWeapon-1) == "rocket"){a[num].leftRocket+=2;}
		}
		if(hand == 2){
			if(a[num].h2 == "rocket" && vsWeapon.at(iWeapon-1)!="rocket")a[num].rightRocket=0;
			a[num].h2 = vsWeapon.at(iWeapon-1);
			if(vsWeapon.at(iWeapon-1)=="rocket")a[num].rightRocket+=2;
		}
		if(hand == 3){
			if(vsWeapon.at(iWeapon-1)=="rpg")
				a[num].shoulder = vsWeapon.at(iWeapon-1);
			else {GlobalMessage("Cannot put it on your shoulder.\n");action(num);return;}
		}
	}
	if(moveType == 7){
		if(a[num].h1 != "knife" && a[num].h2 != "knife")
			{GlobalMessage("No knives.\n");action(num);return;}
		if(a[num].h1 == "knife")a[num].h1 = "upgraded knife";
		if(a[num].h2 == "knife")a[num].h2 = "upgraded knife";
	}
	if(moveType == 8){
		if(a[num].sleep == asleep){GlobalMessage("You're asleep.\n");action(num);return;}
		vector<int> viPlayer = vector<int>();
		vbut_t vbChoice = vbut_t();
		int iPlayer = 0;
		vbChoice.push_back(button_t(0,"Possible players to drag:\n"));
		for(int i = 1;i <= playerCount; i++){
			if(a[i].hp>MIN_HEALTH && a[i].invisibleTime ==0 && i != num)
			if(near(p[findPlaceNumber(a[i].loc)],p[findPlaceNumber(a[num].loc)])){
				iPlayer++;
				viPlayer.push_back(i);
				vbChoice.push_back(button_t(iPlayer,a[i].name));
			}
		}
		if(viPlayer.size() == 0){GlobalMessage("Nobody to drag.\n");action(num);return;}
		iPlayer = choose(vbChoice);
		if(iPlayer > viPlayer.size()||iPlayer<=0){if(DEBUGING)cout<<"Overflow.\n";action(num);return;}
		if(a[num].loc != a[viPlayer.at(iPlayer-1)].loc){
			a[viPlayer.at(iPlayer-1)].move( a[num].loc );
			return;
		}
		else {
			vector<int> viPlace = vector<int>();
			vbChoice = vbut_t();
			int iPlace = 0;
			vbChoice.push_back(button_t(0,"Possible places:"));
			for(int i = 1;i<=3*n+4;i++){
				if(near(p[i],p[findPlaceNumber(a[num].loc)])){
					if(p[i].name != a[num].loc){
						iPlace++;
						viPlace.push_back(i);
						vbChoice.push_back(button_t(iPlace,p[i].name));
					}
				}
			}
			if(viPlace.size() == 0){GlobalMessage("Nowhere to drag.\n");action(num);return;}
			iPlace = choose(vbChoice);
			if(iPlace > viPlace.size()||iPlace<=0){if(DEBUGING)cout<<"Overflow.\n";action(num);return;}
			a[viPlayer.at(iPlayer-1)].move( p[viPlace.at(iPlace-1)].name );
		}
	}
	if(moveType == 9){
		if(a[num].shoulder != "rpg" && a[num].shoulder != "rpg_loaded")
		{GlobalMessage("You don't have an rpg.\n");action(num);return;}
		//load the rpg if not loaded
		if(a[num].shoulder == "rpg"){
			bool haveRocket = 0;
			if(a[num].leftRocket!=0){
				a[num].leftRocket -= 1;
				if(a[num].leftRocket == 0)a[num].h1 = "fist";
				haveRocket = 1;
			}
			else if(a[num].rightRocket!=0){
				a[num].rightRocket -= 1;
				if(a[num].rightRocket == 0)a[num].h2 = "fist";
				haveRocket = 1;
			}
			if(!haveRocket){GlobalMessage("Not rockets to load.\n");action(num);return;}
			a[num].rocket = 1;
			a[num].shoulder = "rpg_loaded";
			return;
		}
		if(a[num].shoulder == "rpg_loaded"){
			vbut_t vbChoice = vbut_t();
			vbChoice.push_back(button_t(0,"You can:"));
			vbChoice.push_back(button_t(1,"aim"));
			vbChoice.push_back(button_t(2,"fire"));
			int iAct = choose(vbChoice);
			if(iAct != 1 && iAct != 2){if(DEBUGING)cout<<"Overflow.\n";action(num);return;}
			if(iAct == 1){		//aim
				if(stringSplit(a[num].loc,' ').size()>=2)if(stringSplit(a[num].loc,' ').at(1)=="toilet"){
					GlobalMessage("You're in a toilet, unable to aim.\n");action(num);return;
				}
				vector<int> viPlace;
				vbChoice = vbut_t();
				int iPlace = 0;
				for(int i = 1; i<= 3*n+4;i++){
					if(p[i].alive){
						if(stringSplit(p[i].name,' ').size()>=2)
							if(stringSplit(p[i].name,' ').at(1)=="toilet"
							|| stringSplit(p[i].name,' ').at(1)=="store")
							continue;
						iPlace++;
						viPlace.push_back(i);
						vbChoice.push_back(button_t(iPlace,p[i].name));
					}
				}
				iPlace = choose(vbChoice);
				if(iPlace > viPlace.size()||iPlace<=0){if(DEBUGING)cout<<"Overflow.\n";action(num);return;}
				a[num].aim = p[viPlace.at(iPlace-1)].name;
			}
			if(iAct == 2){		//fire
				if(a[num].rocket == 0){GlobalMessage("Lack rocket.\n");action(num);return;}
				if(a[num].aim == ""){GlobalMessage("Not aimed.\n");action(num);return;}
				a[num].rocket-=1;
				a[num].shoulder = "rpg";
				if(a[num].aim == "supermarket"){
					for(int i = 1;i<=n;i++){
						if(a[i].loc=="supermarket"||a[i].loc=="drug store")
							if(a[i].hp>MIN_HEALTH && a[i].invisibleTime==0)a[i].hp-=1.0;
					}
					p[DRUGSTORE].door=none;
				}
				else {
					for(int i = 1;i<= n;i++){
						if(a[i].loc == a[num].aim)
							if(a[i].hp>MIN_HEALTH && a[i].invisibleTime==0)a[i].hp-=1.0;
					}
					p[findPlaceNumber(a[num].aim)].door=none;
					if(findPlaceNumber(a[num].aim)!=ROAD)
						p[findPlaceNumber(a[num].aim)].alive=0;
					else 
					for(int i = 1;i<= n;i++){
						if(a[i].loc == a[num].aim)
							if(a[i].hp>MIN_HEALTH && a[i].invisibleTime==0)a[i].loc="road";
					}
				}
			}
		}
	}
	if(moveType == 10){
		if(a[num].sleep == asleep){GlobalMessage("You're asleep.\n");action(num);return;}
		if(findPlaceNumber(a[num].loc)<=n){
			if(p[findPlaceNumber(a[num].loc)+n].door != closed)
				{GlobalMessage("Cannot flush.\n");action(num);return;}
			else for(int i=1;i<=n;i++)
				if(a[i].loc==p[findPlaceNumber(a[num].loc)+n].name)
					if(a[i].hp>MIN_HEALTH && a[i].invisibleTime==0)a[i].hp-=1.0;
		}
		else if(findPlaceNumber(a[num].loc) == SUPERMARKET){
			int iPlace = 0;
			vector<int> viPlace = vector<int>();
			vbut_t vbChoice  = vbut_t();
			for(int i = DRUGSTORE+1;i<=3*n+4;i++){
				if(p[i].door==closed){
					iPlace++;
					viPlace.push_back(i);
					vbChoice.push_back(button_t(iPlace,p[i].name));
				}
			}
			iPlace = choose(vbChoice);
			if(iPlace > viPlace.size()||iPlace<=0){if(DEBUGING)cout<<"Overflow.\n";action(num);return;}
			for(int i=1;i<=n;i++)if(a[i].loc==p[viPlace.at(iPlace-1)].name)
				if(a[i].hp>MIN_HEALTH && a[i].invisibleTime==0)a[i].hp-=1.0;
		}
		else if(findPlaceNumber(a[num].loc) == ROAD){
			int iPlace = 0;
			vector<int> viPlace = vector<int>();
			vbut_t vbChoice = vbut_t();
			for(int i = n+1;i<=2*n;i++){
				if(p[i].door==closed && !(p[i-n].alive)){
					iPlace++;
					viPlace.push_back(i);
					vbChoice.push_back(button_t(iPlace,p[i].name));
				}
			}
			iPlace = choose(vbChoice);
			if(iPlace > viPlace.size()||iPlace<=0){if(DEBUGING)cout<<"Overflow.\n";action(num);return;}
			for(int i=1;i<=n;i++)if(a[i].loc==p[viPlace.at(iPlace-1)].name)
				if(a[i].hp>MIN_HEALTH && a[i].invisibleTime==0)a[i].hp-=1.0;
		}
		else {GlobalMessage("Nothing to flush.\n");action(num);return;}
	}
	//moveType == 11 drink tea.
	if(moveType > 11 || moveType < -1){if(DEBUGING)cout<<"Overflow.\n";action(num);return;}
	//return;
}

void useOil(int num){
	const int iHere = findPlaceNumber(a[num].loc);
	vector<int> viPlace = vector<int>();
	vbut_t vbChoice = vbut_t();
	if(iHere<=n)if(p[iHere].door == open)viPlace.push_back(iHere+n);
	if(iHere==SUPERMARKET){
		for(int i = DRUGSTORE+1;i<=3*n+4;i++){
			if(p[i].door == open)
			viPlace.push_back(i);
		}
	}
	if(iHere == ROAD){
		for(int i = 1; i<= n; i++){
			if(!p[i].alive && p[i+n].door == open)viPlace.push_back(i+n);
		}
	}
	if(p[iHere].alive && p[iHere].door!=none)
		if(stringSplit(p[iHere].name,' ').size()>=2)
			if(stringSplit(p[iHere].name,' ').at(1) == "toilet")
				viPlace.push_back(iHere);
	int iPlace = 1;
	for(;iPlace<=viPlace.size();iPlace++)vbChoice.push_back(button_t(iPlace,p[viPlace.at(iPlace-1)].name));
	iPlace = choose(vbChoice);
	if(iPlace>viPlace.size()||iPlace<=0){if(DEBUGING)cout<<"Overflow.\n";action(num);return;}
	p[viPlace.at(iPlace-1)].oiled = 1;
	
}

#endif