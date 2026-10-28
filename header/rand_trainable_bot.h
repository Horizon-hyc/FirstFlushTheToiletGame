#ifndef RAND_TRAINABLE_BOT_H
#define RAND_TRAINABLE_BOT_H
namespace trainable{
#define PROBLEM_AMOUNT 20
enum problem{
	NON_ATTACK,
	DRINK_TEA,
	GO_SLEEPING,
	GO_DAYDREAMING,
	INTO_TOILET,
};

class botTrainable_t{
public:
	botTrainable_t(){
		for(auto &rate : blockRate)rate=(rand()%RAND_MAX)*1.0/RAND_MAX;
	}
	botTrainable_t(string botName){
		name = botName;
		read();
	}
	bool block(problem actProblem) const {
		return (rand() / (double)RAND_MAX) <= blockRate[actProblem];
	}
	void save() const {
		ofstream saveFile = ofstream("data/bot/rates/"+name+".txt");
		for(auto rate:blockRate)saveFile << rate << endl;
		saveFile.close();
	}
	void read() {
    	ifstream reader("data/bot/rates/" + name + ".txt");
    	if (!reader) {
    	    for (auto &rate : blockRate) rate = 0.9;
    	    return;
    	}
    	for (auto &rate : blockRate)
    	    if (!(reader >> rate)) break;
	}
private:
	string name;
	double blockRate[PROBLEM_AMOUNT];
};

bool randUseOil(int);
bool randAction(const int num, const botTrainable_t &bot){
	const int moveType = randInt(1,10);
	if(DEBUGING)cout<<moveType<<endl;
	//basic move checks
	if(moveType == 11){if(bot.block(DRINK_TEA))return 1;}
	if(a[num].sleep!=asleep){
		bool peopleAround = 0;
		for(auto plyr : a)if(plyr.loc == a[num].loc && plyr.name != a[num].name)peopleAround = 1;
		if(peopleAround)if(moveType != 3 && moveType != 4){if(bot.block(NON_ATTACK))return 1;}
	}
	if(moveType == 1){
		if(a[num].sleep==asleep){return 1;}
		const int iHere = findPlaceNumber(a[num].loc);
		vector<int> viPlace = vector<int>();
		if(iHere<=n)viPlace.push_back(iHere+n);
		if(iHere==SUPERMARKET){
			for(int i = DRUGSTORE+1;i<=3*n+4;i++){
				viPlace.push_back(i);
			}
			viPlace.push_back(DRUGSTORE);
		}
		if(p[iHere].alive && p[iHere].door!=none)viPlace.push_back(iHere);
		if(iHere == ROAD){
			for(int i = 1; i<=n;i++){
				if(p[i].alive)viPlace.push_back(i);
				else viPlace.push_back(i+n);
			}
		}
		if(viPlace.size() == 0){return 1;}
		int iPlace = 1;
		iPlace = randInt(viPlace.size());
		int placeNumber = viPlace.at(iPlace-1);
		if(p[placeNumber].door == closed)p[placeNumber].door = open;
		else if(p[placeNumber].door == open)p[placeNumber].door = closed;
	}
	if(moveType == 2){
		if(a[num].sleep == asleep){return 1;}
		vector<int> viPossiblePlace;
		int iPlace = 0;
		for(int i=1;i<=3*n+4;i++){
			if(connected(p[findPlaceNumber(a[num].loc)],p[i])){
				iPlace++;
				viPossiblePlace.push_back(i);
			}
		}		//set a vector of possible places.
		if(viPossiblePlace.size()==0){return 1;}
		int iGoPlace;
		iGoPlace = randInt(viPossiblePlace.size());
		int placeNumber = findPlaceNumber(p[viPossiblePlace.at(iGoPlace-1)].name);
		if(placeNumber == -1){return 1;}
		if(stringSplit(p[placeNumber].name,' ').size()>=2)
			if(stringSplit(p[placeNumber].name,' ').at(1)=="toilet")
				if(bot.block(INTO_TOILET))return 1;
		if(connected(p[findPlaceNumber(a[num].loc)],p[placeNumber]))
			a[num].move( p[viPossiblePlace.at(iGoPlace-1)].name );
	}
	if(moveType == 3){
		if(a[num].sleep == asleep){return 1;}
		if(a[num].h1 == "oil"){a[num].h1 = "fist";return randUseOil(num);}
		vector<int> viPlayer=vector<int>();
		int iPlayer = 0;
		for(int i = 1;i<=playerCount;i++){
			if(a[num].loc == a[i].loc && a[i].hp>MIN_HEALTH && a[i].invisibleTime==0){
				iPlayer++;
				viPlayer.push_back(i);
			}
		}
		if(viPlayer.size() == 0){return 1;}
		iPlayer = randInt(viPlayer.size());
		if(iPlayer > viPlayer.size()){return 1;}
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
		else if(viPlayer.at(iPlayer-1) ==num){return 1;}
		use(viPlayer.at(iPlayer-1),sWeapon);
	}
	if(moveType == 4){
		if(a[num].sleep == asleep){return 1;}
		if(a[num].h2 == "oil"){a[num].h2 = "fist";return randUseOil(num);}
		vector<int> viPlayer=vector<int>();
		int iPlayer = 0;
		for(int i = 1;i<=playerCount;i++){
			if(a[num].loc == a[i].loc && a[i].hp>MIN_HEALTH && a[i].invisibleTime == 0){
				iPlayer++;
				viPlayer.push_back(i);
			}
		}
		if(viPlayer.size() == 0){return 1;}
		iPlayer = randInt(viPlayer.size());
		if(iPlayer > viPlayer.size()){return 1;}
		string sWeapon;
		if(a[num].h2 == "fist"){
			if(a[num].sleep == awake)sWeapon = "fist_awake";
			else sWeapon = "fist_dream";
		}
		else sWeapon = a[num].h2;
		if(a[num].h2 == "poison" || a[num].h2 == "invisible" || a[num].h2 =="health"){
			a[num].h2 = "fist";
		}
		else if(viPlayer.at(iPlayer-1) ==num){return 1;}
		use(viPlayer.at(iPlayer-1),sWeapon);
	}
	if(moveType == 5){
		if(stringSplit(a[num].loc,' ').size()>=2)if(stringSplit(a[num].loc,' ').at(1)=="toilet"){
			return 1;
		}
		int inStatus;
		inStatus = randInt(3);
		if(inStatus>3){return 1;}
		if(inStatus == asleep + 1)if(bot.block(GO_SLEEPING)){return 1;}
		if(a[num].h1 != "fist" || a[num].h2 != "fist")if(inStatus == daydream + 1)if(bot.block(GO_DAYDREAMING)){return 1;}
		a[num].sleep = inStatus -1;
		if(a[num].sleep != awake){
			a[num].h1 = a[num].h2 = "fist";
			a[num].shoulder = "empty";
			a[num].leftRocket = 0;
			a[num].rightRocket = 0;
			a[num].rocket = 0;
			a[num].aim = "";
		}
	}
	if(moveType == 6){
		if(a[num].sleep != awake){
			return 1;
		}
		vector<string> vsWeapon;
		int iPlace = findPlaceNumber(a[num].loc);
		int iWeapon = 0;
		if(p[iPlace].name=="drug store" && p[iPlace].door!=closed){
			return 1;
		}
		if(p[iPlace].knife > 0){
			iWeapon++;vsWeapon.push_back("knife");
		}
		if(p[iPlace].rpg > 0){
			iWeapon++;vsWeapon.push_back("rpg");
		}
		if(p[iPlace].rocket > 0 && a[num].shoulder != "empty"){
			iWeapon++;vsWeapon.push_back("rocket");
		}
		if(p[iPlace].health > 0){
			iWeapon++;vsWeapon.push_back("health");
		}
		if(p[iPlace].poison > 0){
			iWeapon++;vsWeapon.push_back("poison");
		}
		if(p[iPlace].oil > 0){
			iWeapon++;vsWeapon.push_back("oil");
		}
		if(p[iPlace].invisible > 0){
			iWeapon++;vsWeapon.push_back("invisible");
		}
		if(vsWeapon.size() == 0){return 1;}
		iWeapon = randInt(vsWeapon.size());
		if(iWeapon>vsWeapon.size()){return 1;}
		if(vsWeapon.at(iWeapon-1) == "knife"){p[iPlace].knife -= 1;}
		if(vsWeapon.at(iWeapon-1) == "rpg"){p[iPlace].rpg -= 1;}
		if(vsWeapon.at(iWeapon-1) == "rocket"){p[iPlace].rocket -= 1;}
		if(vsWeapon.at(iWeapon-1) == "health"){p[iPlace].health -= 1;}
		if(vsWeapon.at(iWeapon-1) == "poison"){p[iPlace].poison -= 1;}
		if(vsWeapon.at(iWeapon-1) == "oil"){p[iPlace].oil -= 1;}
		if(vsWeapon.at(iWeapon-1) == "invisible"){p[iPlace].invisible -= 1;}
		int hand;
		hand = randInt(3);
		if(hand>3){return 1;}
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
			else {return 1;}
		}
	}
	if(moveType == 7){
		if(a[num].h1 != "knife" && a[num].h2 != "knife"){return 1;}
		if(a[num].h1 == "knife")a[num].h1 = "upgraded knife";
		if(a[num].h2 == "knife")a[num].h2 = "upgraded knife";
	}
	if(moveType == 8){
		if(a[num].sleep == asleep){return 1;}
		vector<int> viPlayer = vector<int>();
		int iPlayer = 0;
		for(int i = 1;i <= playerCount; i++){
			if(a[i].hp>MIN_HEALTH && a[i].invisibleTime ==0 && i != num)
			if(near(p[findPlaceNumber(a[i].loc)],p[findPlaceNumber(a[num].loc)])){
				iPlayer++;
				viPlayer.push_back(i);
			}
		}
		if(viPlayer.size() == 0){return 1;}
		iPlayer = randInt(viPlayer.size());
		if(iPlayer > viPlayer.size()){return 1;}
		if(a[num].loc != a[viPlayer.at(iPlayer-1)].loc){
			a[viPlayer.at(iPlayer-1)].move( a[num].loc );
			return 0;
		}
		else {
			vector<int> viPlace = vector<int>();
			int iPlace = 0;
			for(int i = 1;i<=3*n+4;i++){
				if(near(p[i],p[findPlaceNumber(a[num].loc)])){
					if(p[i].name != a[num].loc){
						iPlace++;
						viPlace.push_back(i);
					}
				}
			}
			if(viPlace.size() == 0){return 1;}
			iPlace = randInt(viPlace.size());
			if(iPlace > viPlace.size()){return 1;}
			a[viPlayer.at(iPlayer-1)].move( p[viPlace.at(iPlace-1)].name );
		}
	}
	if(moveType == 9){
		if(a[num].shoulder != "rpg" && a[num].shoulder != "rpg_loaded")
		{return 1;}
		if(a[num].shoulder == "rpg"){
			bool haveRocket = 0;
			if(a[num].leftRocket!=0){
				a[num].leftRocket-=1;
				haveRocket = 1;
			}
			else if(a[num].rightRocket!=0){
				a[num].rightRocket -= 1;
				haveRocket = 1;
			}
			if(!haveRocket){return 1;}
			a[num].rocket = 1;
			a[num].shoulder = "rpg_loaded";
			return 0;
		}
		if(a[num].shoulder == "rpg_loaded"){
			int iAct;
			iAct = randInt(2);
			if(iAct != 1 && iAct != 2){return 1;}
			if(iAct == 1){
				vector<int> viPlace;
				int iPlace = 0;
				for(int i = 1; i<= 3*n+4;i++){
					if(p[i].alive){
						if(stringSplit(p[i].name,' ').size()>=2)
							if(stringSplit(p[i].name,' ').at(1)=="toilet"
							|| stringSplit(p[i].name,' ').at(1)=="store")
							continue;
						iPlace++;
						viPlace.push_back(i);
					}
				}
				if(viPlace.size()==0){return 1;}
				iPlace = randInt(viPlace.size());
				if(iPlace > viPlace.size()){return 1;}
				a[num].aim = p[viPlace.at(iPlace-1)].name;
			}
			if(iAct == 2){
				if(a[num].rocket == 0){return 1;}
				if(a[num].aim == ""){return 1;}
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
		if(a[num].sleep==asleep){return 1;}
		if(findPlaceNumber(a[num].loc)<=n){
			if(p[findPlaceNumber(a[num].loc)+n].door != closed)
				{return 1;}
			else for(int i=1;i<=n;i++)
				if(a[i].loc==p[findPlaceNumber(a[num].loc)+n].name)
					if(a[i].hp>MIN_HEALTH && a[i].invisibleTime==0)a[i].hp-=1.0;
		}
		else if(findPlaceNumber(a[num].loc) == SUPERMARKET){
			int iPlace = 0;
			vector<int> viPlace = vector<int>();
			for(int i = DRUGSTORE+1;i<=3*n+4;i++){
				if(p[i].door==closed){
					iPlace++;
					viPlace.push_back(i);
				}
			}
			if(viPlace.size()==0){return 1;}
			iPlace = randInt(viPlace.size());
			if(iPlace > viPlace.size()){return 1;}
			for(int i=1;i<=n;i++)if(a[i].loc==p[viPlace.at(iPlace-1)].name)
				if(a[i].hp>MIN_HEALTH && a[i].invisibleTime==0)a[i].hp-=1.0;
		}
		else if(findPlaceNumber(a[num].loc) == ROAD){
			int iPlace = 0;
			vector<int> viPlace = vector<int>();
			for(int i = n+1;i<=2*n;i++){
				if(p[i].door==closed && !(p[i-n].alive)){
					iPlace++;
					viPlace.push_back(i);
				}
			}
			if(viPlace.size()==0){return 1;}
			iPlace = randInt(viPlace.size());
			if(iPlace > viPlace.size()){return 1;}
			for(int i=1;i<=n;i++)if(a[i].loc==p[viPlace.at(iPlace-1)].name)
				if(a[i].hp>MIN_HEALTH && a[i].invisibleTime==0)a[i].hp-=1.0;
		}
		else {return 1;}
	}
	//moveType == 11 drink tea.
	if(moveType > 11 || moveType < -1){return 1;}
	if(bBotStop){
		ShowMap();
		vbut_t vbGoOn = vbut_t();
		vbGoOn.push_back(button_t(0,"Go on."));
		choose(vbGoOn);
	}
	return 0;
}
//need repair.
//changed return values
bool randUseOil(int num){
	const int iHere = findPlaceNumber(a[num].loc);
	vector<int> viPlace = vector<int>();
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
	if(viPlace.size()==0){return 1;}
	int iPlace = 1;
	iPlace = randInt(viPlace.size());
	if(iPlace>viPlace.size()){return 1;}
	p[viPlace.at(iPlace-1)].oiled = 1;
	return 0;
}
bool rockpaper(double surviveRate){
	return 1;
};
void trainBot(){
	vector<botTrainable_t> vbotTrainee(11,botTrainable_t(botName));
	//change into buttons later
	n = 10;
	realPlayerAmount = 0;
	double surviveRate = 0.5;
	inita(n);
    while(!rockpaper(surviveRate)){};
}
}
#endif