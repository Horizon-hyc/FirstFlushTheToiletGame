#ifndef BASIC_H
#define BASIC_H
#define closed 1
#define none 0
#define open 2
#define asleep 0
#define daydream 1
#define awake 2
#define ROAD 2*n+1
#define SUPERMARKET 2*n+2
#define DRUGSTORE 2*n+3
#define playerCount n
#define MAX_PLAYER_COUNT 5000
#define MAX_PLACE_COUNT 50000
#define MAX_INFINITY 500000
int n = 0;
int realPlayerAmount;
class player
{
public:
	bool real;
	
    double hp;
    string name;
    int sleep;
    bool poisoned;
    bool healed;
    int invisibleTime;
    int stuckTime;
    
    string h1, h2, shoulder;
    int leftRocket = 0;
    int rightRocket = 0;
    int rocket;
    string aim;
    string loc;
    
    void move(string);
    
    int x;
    int y;
};

struct place
{
	int rpg;int knife;int rocket;int health;int poison;int oil;int invisible;
	int door;
	string name;
	bool alive;
	bool oiled;
	place() = default;//do not delete this.although it seems useless.
	place(string initName,int initDoorMode){
		knife=0;rpg=0;rocket=0;health=0;poison=0;oil=0;invisible=0;oiled=0;
		door = initDoorMode;
		name = initName;
		alive = 1;
	}	//to define a standard place.
};
std::vector<std::string> stringSplit(const std::string& str, char delim) {
    std::size_t previous = 0;
    std::size_t current = str.find_first_of(delim);
    std::vector<std::string> elems = vector<std::string>();
    if(current >= str.length() - 1){elems.push_back(str);return elems;}
    while (current != std::string::npos) {
        if (current > previous) {
            elems.push_back(str.substr(previous, current - previous));
        }
        previous = current + 1;
        current = str.find_first_of(delim, previous);
    }
    if (previous != str.size()) {
        elems.push_back(str.substr(previous));
    }
    return elems;
}		//copied from zhihuzhuanlan, just a way to split strings


int randInt(int num){
	if(num == 1)return 1;
    return rand() % num + 1;
}
int randInt(int minNum,int maxNum){
    return minNum-1+randInt(maxNum-minNum+1);
}

player a[MAX_PLAYER_COUNT];
place p[MAX_PLAYER_COUNT];
int findPlaceNumber(const string _placeName){
	for(int i=0;i<MAX_PLACE_COUNT;i++)if(p[i].name == _placeName)return i;
	return -1;
}

void initWorld(){
	n = 0;
	realPlayerAmount = 0;
	setColor();
	setTime();
	for(auto &plyr : a)plyr = player();
	for(auto &plc : p)plc = place();
}

const bool connected(place place1,place place2){
	if(!place1.alive || !place2.alive){return 0;}
	if(place1.name == place2.name)return 0;
	const vector<string> vName1 = stringSplit(place1.name,' ');
	const vector<string> vName2 = stringSplit(place2.name,' ');
	if(place1.name=="drug store"){
		if(place1.door==closed){return 0;}//closed drugstores are connected to nothing
		if(place2.name=="supermarket")return 1;//no need to consider closed drugstores
		return 0;
	}
	if(place2.name=="drug store"){
		if(place2.door==closed){return 0;}
		if(place1.name=="supermarket")return 1;
		return 0;
	}		//the same as above.no need to consider drugstores afterwards
	if(place1.name=="supermarket"){
		if(place1.door==closed){
			if(vName2.size()>=2)if(vName2.at(0) == "supermarket" && vName2.at(1) == "toilet"
			&& (!place2.door == closed))return 1;
			return 0;
		}
		else {
			if(place2.name=="road")return 1;
			if(vName2.size()>=2)if(vName2.at(0) == "supermarket" && vName2.at(1) == "toilet"
			&& place2.door != closed)return 1;
			if(vName2.size()>=2)if(vName2.at(1) == "home" && place2.door != closed)return 1;
			return 0;
		}
	}
	if(place2.name=="supermarket"){
		if(place2.door==closed){
			if(vName1.size()>=2)if(vName1.at(0) == "supermarket" && vName1.at(1) == "toilet"
			&& (!place1.door == closed))return 1;
			return 0;
		}
		else {if(place1.name=="road")return 1;
			if(vName1.size()>=2)if(vName1.at(0) == "supermarket" && vName1.at(1) == "toilet"
			&& place1.door != closed)return 1;
			if(vName1.size()>=2)if(vName1.at(1) == "home" && place1.door != closed)return 1;
			return 0;
		}
	}		//no need to consider supermarket afterwards.
	if(place1.name == "road")if(vName2.size()>=2)
			if(vName2.at(1) == "toilet" && vName2.at(0)!= "supermarket"
			&& !p[findPlaceNumber(vName2.at(0)+" home")].alive)return (place2.door!=closed);
	if(place2.name == "road")if(vName1.size()>=2)
			if(vName1.at(1) == "toilet" && vName1.at(0)!= "supermarket"
			&& !p[findPlaceNumber(vName1.at(0)+" home")].alive)return (place1.door!=closed);
	if(place1.name == "road")return (vName2.at(1) == "home" && place2.door != closed);
	if(place2.name == "road")return (vName1.at(1) == "home" && place1.door != closed);
	if(vName1.at(1) == "home"){
		if(place1.door == closed){
			if(vName2.at(0) == vName1.at(0) && place2.door != closed)return 1;
			return 0;
		}
		else {
			if(vName2.at(0) == vName1.at(0) && place2.door != closed)return 1;
			if(vName2.at(1) == "home" && place2.door != closed)return 1;
			return 0;
		}
	}
	if(vName2.at(1) == "home"){
		if(place2.door == closed){
			if(vName1.at(0) == vName2.at(0) && place1.door != closed)return 1;
			return 0;
		}
		else {
			if(vName1.at(0) == vName2.at(0) && place1.door != closed)return 1;
			if(vName1.at(1) == "home" && place1.door != closed)return 1;
			return 0;
		}
	}		//only toilets left
			//if its the toilet of supermarket, then it is already returned at supermarket check.
	if(vName1.at(1) == "toilet")return (vName2.at(1) == "home" && vName2.at(0) == vName1.at(0));
	if(vName2.at(1) == "toilet")return (vName1.at(1) == "home" && vName1.at(0) == vName2.at(0));
	return 1;
}

const bool near(place place1,place place2){
	if(place1.name == place2.name)return 1;
	const vector<string> vName1 = stringSplit(place1.name,' ');
	const vector<string> vName2 = stringSplit(place2.name,' ');
	if(place1.name == "supermarket")if(vName2.size()>=2)
		if(vName2.at(1) == "home")return 0;
	if(place2.name == "supermarket")if(vName1.size()>=2)
		if(vName1.at(1) == "home")return 0;
	if(vName1.size()>=2)if(vName2.size()>=2)
		if(vName1.at(1) == "home" && vName2.at(1) == "home")return 0;
	return connected(place1,place2);
}

void player::move(const string sLoc){
	loc = sLoc;
	vector<string> vsLoc = stringSplit(sLoc,' ');
	if(vsLoc.size()>=2)if(vsLoc.at(1) == "toilet"){
		sleep = awake;
		h1 = h2 = "fist";
		leftRocket = rightRocket = 0;
		shoulder = "empty";
		aim = "";
		rocket = 0;
	}
	int iPlace =  findPlaceNumber(sLoc);
	if(p[iPlace].oiled)stuckTime++;
	if(iPlace <= n){
		x = randInt(iPlace*100+30,iPlace*100+110);
		y = randInt(1110,1190);
	}
	else if(iPlace <= 2*n){
		x = randInt((iPlace-n)*100+40,(iPlace-n)*100+80);
		y = randInt(1220,1260);
	}
	else if(iPlace == SUPERMARKET){
		x = randInt(20,1700);
		y = randInt(210,390);
	}
	else if(iPlace == DRUGSTORE){
		x = randInt(1710,1780);
		y = randInt(210,390);
	}
	else if(iPlace == ROAD){
		x = randInt(20,1720);
		y =	randInt(430,1000);
	}
	else {
		x = randInt((iPlace-DRUGSTORE-6)*100+40,(iPlace-DRUGSTORE-6)*100+80);
		y = randInt(20,60);
	}
}

void use(int i,string weapon){
	if(weapon == "fist_awake"){a[i].hp-=1.0/3;return;}
	if(weapon == "fist_dream"){a[i].hp-=0.5;return;}
	if(weapon == "knife"){a[i].hp-=0.5;return;}
	if(weapon == "upgraded knife"){a[i].hp-=1.0;return;}
	if(weapon == "poison"){
		if(!a[i].poisoned){
			a[i].hp/=3.0;
			a[i].poisoned=1;
			return;
		}
	}
	if(weapon == "health"){
		if(!a[i].healed){
			a[i].hp+=0.5;
			if(a[i].hp>1)a[i].hp = 1;
			a[i].healed = 1;
			return;
		}
	}
	if(weapon == "invisible"){a[i].invisibleTime=4;return;}
	return;
}
int num;
void inita(int n){
	for(int i = 1; i <= n; i++)
    {
        
        if(i<= realPlayerAmount){
			GlobalMessage("Please enter your name, player "+to_string(i)+".\n");
			if(!ProcessTextInput(a[i].name,10))a[i].name = "Player_"+to_string(i);
		}
        else a[i].name = "computer_"+to_string(i-realPlayerAmount);
        a[i].real = (i<=realPlayerAmount);
        a[i].hp = 1.0;
        a[i].sleep = 0;
        a[i].poisoned = 0;
        a[i].invisibleTime = 0;
        a[i].stuckTime = 0;
        a[i].h1 = a[i].h2 = "fist";
        a[i].leftRocket =0;
        a[i].rightRocket = 0;
        a[i].rocket = 0;
        a[i].shoulder= "empty";
        a[i].loc = a[i].name + "'s home";
    }
    int i = 1;
	for(;i<=n;i++){
		p[i]=place(a[i].name+"\'s home",open);
		p[i].knife=1;
	}		//init homes, only difference is the knife.
	for(;i<=2*n;i++){
		p[i]=place(a[i-n].name+"\'s toilet",open);		//i-n is the relative person
	}		//init toilets, standard place.
	p[ROAD] = place("road",none);		//init road.
	p[SUPERMARKET] = place("supermarket",none);
	p[SUPERMARKET].rocket=p[SUPERMARKET].rpg=p[SUPERMARKET].knife=2*MAX_INFINITY;
	p[SUPERMARKET].health=0;		//init supermarket
	p[DRUGSTORE] = place("drug store",closed);
	p[DRUGSTORE].health=p[DRUGSTORE].poison=2*MAX_INFINITY;
	p[DRUGSTORE].oil=p[DRUGSTORE].invisible=2*MAX_INFINITY;
	int j = 1;
	for(i = DRUGSTORE+1;i<=3*n+4;i++)		//i<=3*n+4 so that everyone can have a toilet to stay.
	{
		p[i] = place("supermarket toilet "+to_string(j),open);
		j++;
	}
	for(int i = 1;i <= n; i++)a[i].move(a[i].name+"'s home");
}

void useOil(int num);
string sval(int _number){
	if(_number>MAX_INFINITY)return "Infinity";
	return to_string(_number);
}
vector<int> scramble(){
	vector<int> output;int temp;
	for(int i=1;i<=n;i++){output.push_back(i);}
	for(int i=0;i<n*n;i++){
		int i1=rand()%n,i2=rand()%n;
		temp = output[i2];
		output[i2]=output[i1];
		output[i1]=temp;
	}
	return output;
}		//randomly sramble 1~n


void playerstat(){
    for(int i = 1; i <= n; i++)
    {
    	if(a[i].hp <= 0){/*cout << a[i].name << " is dead.\n";*/continue;}
        cout << a[i].name;
		cout <<"    health:" << fraction(a[i].hp).str() << endl;
        cout << "    location: " << a[i].loc << endl;
        
        if(a[i].sleep == 0)cout << "    sleeping" << endl;
        else if(a[i].sleep == 1)cout << "    daydreaming" << endl;
        else cout << "    awake" << endl;
        
        if(a[i].h1!="fist")cout << "    hand1: " << a[i].h1 << endl;
        if(a[i].h1=="rocket")cout<<"	(" << a[i].leftRocket<<")\n";
        if(a[i].h2!="fist")cout << "    hand2: " << a[i].h2 << endl;
        if(a[i].h1=="rocket")cout<<"	(" << a[i].rightRocket<<")\n";
        if(a[i].shoulder!="empty")cout<<"    shoulder: "<<a[i].shoulder<<endl;
        if(a[i].aim != ""){cout << "    (aimed at "<<a[i].aim<<")\n";}
        if(a[i].rocket!=0){cout << "    (with rocket " << a[i].rocket<<")\n";}
        if(a[i].invisibleTime > 0){cout << "    Invisible.\n";}
    }
}

void placestat(){
    for(int i = 1; i <= 3*n+4; i++)
    {
    	if(p[i].alive==0){
    		//cout<<p[i].name<<" does not exist"<<endl;
    		continue;
		}
        cout << p[i].name;
        if(p[i].oiled)cout<<"(oiled).\n";
        if(p[i].door==none)cout<<"(no door)";
		else if(p[i].door==closed)cout<<"(closed)";
		else if(p[i].door==open)cout<<"(open)";
		cout<<"\n";
        if(p[i].knife!=0)cout << "    knife: " << sval(p[i].knife) << endl;
        if(p[i].rpg!=0)cout << "    rpg: " << sval(p[i].rpg) << endl;
        if(p[i].rocket!=0)cout<<"    rocket: "<< sval(p[i].rocket) <<endl;
        if(p[i].health!=0)cout << "    health: " << sval(p[i].health) << endl;
        if(p[i].poison!=0)cout << "    poison: " << sval(p[i].poison) << endl;
        if(p[i].invisible!=0)cout<<"    invisible: "<<sval(p[i].invisible) <<endl;
        if(p[i].oil!=0)cout<<"    oil: "<<sval(p[i].oil) <<endl;
    }
}
#endif