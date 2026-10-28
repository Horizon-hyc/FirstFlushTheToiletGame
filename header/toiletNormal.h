#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <fstream>
using namespace std;

#include "fraction_bydog.h"
using namespace _frac;

//#include "check_functions_bydog.h"

#define closed 1
#define none 0
#define open 2
#define asleep 0
#define daydream 1
#define awake 2
#define ROAD 2*n+1
#define SUPERMARKET 2*n+2
#define DRUGSTORE 2*n+3
int n;
#define playerCount n
#define MAX_PLAYER_COUNT 1000
#define MAX_PLACE_COUNT 5000
#define MAX_INFINITY 50000
class player
{
public:
    double hp;
    string name;
    int sleep;
    bool poisoned;
    bool healed;
    int invisibleTime;
    int stuckTime;
    
    string h1, h2, shoulder;
    int rocket;
    string aim;
    string loc;
    
    void move(string);
};

struct place
{
	int rpg;int knife;int rocket;int health;int poison;int oil;int invisible;
	int door;
	string name;
	bool alive;
	bool oiled;
	place(){}//do not delete this.although it seems useless.
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
    return rand() % num + 1;
}
int randInt(int minNum,int maxNum){
    return rand() % (maxNum-minNum+1) + minNum;
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

player a[MAX_PLAYER_COUNT];
place p[MAX_PLAYER_COUNT];
int findPlaceNumber(const string _placeName){
	for(int i=0;i<MAX_PLACE_COUNT;i++)if(p[i].name == _placeName)return i;
	return -1;
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
		else {
			if(vName1.size()>=2)if(vName1.at(0) == "supermarket" && vName1.at(1) == "toilet"
			&& place1.door != closed)return 1;
			if(vName1.size()>=2)if(vName1.at(1) == "home" && place1.door != closed)return 1;
			return 0;
		}
	}		//no need to consider supermarket afterwards.
	if(place1.name == "road")if(vName2.size()>=2)
			if(vName2.at(1) == "toilet" && vName2.at(0)!= "supermarket"
			&& !p[findPlaceNumber(vName2.at(0)+" home")].alive)return 1;
	if(place2.name == "road")if(vName1.size()>=2)
			if(vName1.at(1) == "toilet" && vName1.at(0)!= "supermarket"
			&& !p[findPlaceNumber(vName1.at(0)+" home")].alive)return 1;
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

const bool near(place place1, place place2){
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
		shoulder = "empty";
		aim = "";
		rocket = 0;
	}
	if(p[findPlaceNumber(sLoc)].oiled)stuckTime++;
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
        cout << "player " << i << " name:";
        cin >> a[i].name;
        a[i].hp = 1.0;
        a[i].sleep = 0;
        a[i].poisoned = 0;
        a[i].invisibleTime = 0;
        a[i].h1 = a[i].h2 = "fist";
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
	for(i = DRUGSTORE+1;i<=3*n+4;i++)		//i<3*n+4 so that everyone can have a toilet to stay.
	{
		p[i] = place("supermarket toilet "+to_string(j),open);
		j++;
	}
}
namespace Normal{
void useOil(int num);
void playerstat()
{
    for(int i = 1; i <= n; i++)
    {
    	if(a[i].hp <= 0){cout << a[i].name << " is dead.\n";continue;}
        cout << a[i].name << ": " << endl;
        cout << "    health: " << fraction(a[i].hp).str() << endl;
        cout << "    location: " << a[i].loc << endl;
        
        if(a[i].sleep == 0)
        {
            cout << "    sleeping" << endl;
        }
        else if(a[i].sleep == 1)
        {
            cout << "    daydreaming" << endl;
        }
        else
        {
            cout << "    awake" << endl;
        }
        
        cout << "    hand1: " << a[i].h1 << endl;
        cout << "    hand2: " << a[i].h2 << endl;
        cout<<"    shoulder: "<<a[i].shoulder<<endl;
        if(a[i].invisibleTime > 0){cout << "Invisible.\n";}
    }
}
string sval(int _number){
	if(_number>MAX_INFINITY)return "Infinity";
	return to_string(_number);
}
void placestat()
{
    for(int i = 1; i <= 3*n+4; i++)
    {
    	if(p[i].alive==0){
    		cout<<"    does not exist"<<endl;
    		continue;
		}
        cout << p[i].name << ": " << endl;
        if(p[i].oiled)cout<<"With oil.\n";
        cout << "    door: ";
        if(p[i].door==none){
        	cout<<"none"<<endl;
		}
		else if(p[i].door==closed){
        	cout<<"closed"<<endl;
		}
		else if(p[i].door==open){
        	cout<<"open"<<endl;
		}
        cout << "    knife: " << sval(p[i].knife) << endl;
        cout << "    rpg: " << sval(p[i].rpg) << endl;
        cout<<"    rocket: "<< sval(p[i].rocket) <<endl;
        cout << "    health: " << sval(p[i].health) << endl;
        cout << "    poison: " << sval(p[i].poison) << endl;
        cout<<"    invisible: "<<sval(p[i].invisible) <<endl;
        cout<<"    oil: "<<sval(p[i].oil) <<endl;
    }
}
void action(int num);		//just declaring it to use in rockpaper()

void rockpaper(){
	/*num=randInt(n);
	cout<<"rock paper scissors shoot!"<<endl;
	cout<<a[num].name<<" won!";*/		//this part does not count the turns.

	vector<int> order = scramble();
	for(auto iPlayer:order){
		if(a[iPlayer].stuckTime>0)a[iPlayer].stuckTime--;
		if(a[iPlayer].invisibleTime>1)a[iPlayer].invisibleTime-=1;
		else if(a[iPlayer].invisibleTime == 1){
			a[iPlayer].invisibleTime = 0;
			cout << a[iPlayer].name << " revives at:\n";
			vector<int> viPlace;
			int iPlace;
			for(int i = 1;i<=3*n+4;i++){
				if(p[i].alive){
					iPlace++;
					viPlace.push_back(i);
					cout<<iPlace<<":"<<p[i].name<<"\n";
				}
			}
			while(1){
				cin>>iPlace;
				if(iPlace>=1 && iPlace < viPlace.size()){
					a[iPlayer].move(p[viPlace.at(iPlace)].name);
					break;
				}
				cout<<"Overflow.\n";
			}
		}
	}
	int movingPlayerCount = randInt(1,n-1);
	order.resize(movingPlayerCount);
	order.shrink_to_fit();
	for(auto i:order)
		if(a[i].hp > 0 && a[i].invisibleTime==0 && a[i].stuckTime==0)
			action(i);
	cout<<"Turn ended.\n";
	return;
}
void action(int num){
	/* from here on, its for checking*/
	cout << a[num].name << "'s turn.(type -1 for help).\n";
	int moveType;
	cin >> moveType;
	if(moveType == -1){
		cout<<"This is what you can do now:\n";
		cout << " 0:show the state of everything (it won't skip your turn).\n"
		<< " 1:change the door mode of a place.\n"
		<< " 2:go to a place.\n"
		<< " 3:use your left hand (" << a[num].h1 << ").\n"
		<< " 4:use your right hand(" << a[num].h2 << ").\n"
		<< " 5:change your state.\n"
		<< " 6:get something from your place.\n"
		<< " 7:upgrade your knives.\n"
		<< " 8:drag others.\n"
		<< " 9:deal with your rpg.\n"
		<< "10:flush toilets.\n";
	}
	if(moveType == 0){
        playerstat();
        placestat();
        action(num);
	}
	if(moveType == 1){/*
		string placeName;
		getline(cin,placeName);
		int placeNumber = findPlaceNumber(placeName);
		if(placeNumber == -1){cout << "Invalid input\n";action(num);return;}
		if(p[placeNumber].door == closed)p[placeNumber].door = open;
		else if(p[placeNumber].door == open)p[placeNumber].door = closed;
		cout << "Changed the door of "<<p[placeNumber].name
		<<" to "<<p[placeNumber].door<<".\n";*/
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
		int iPlace = 1;
		for(;iPlace<=viPlace.size();iPlace++)
			cout<<iPlace<<":"<<p[viPlace.at(iPlace-1)].name<<endl;
		cin>>iPlace;
		if(iPlace>viPlace.size()){cout<<"Overflow.\n";action(num);return;}
		int placeNumber = viPlace.at(iPlace-1);
		if(p[placeNumber].door == closed)p[placeNumber].door = open;
		else if(p[placeNumber].door == open)p[placeNumber].door = closed;
	}
	if(moveType == 2){
		if(a[num].sleep == asleep){cout << "You're asleep.\n";action(num);return;}
		cout << "Possible places to go to:\n";
		vector<int> viPossiblePlace;
		int iPlace = 0;
		for(int i=1;i<=3*n+4;i++){
			if(connected(p[findPlaceNumber(a[num].loc)],p[i])){
				iPlace++;
				viPossiblePlace.push_back(i);
				cout <<iPlace<<":"<< p[i].name <<"\n";
			}
		}		//set a vector of possible places.
		if(viPossiblePlace.size()==0){cout<<"Nowhere to go.\n";action(num);return;}
		int iGoPlace;
		cin>>iGoPlace;
		if(iGoPlace > viPossiblePlace.size()){cout<<"Overflow.\n";action(num);return;}
		int placeNumber = findPlaceNumber(p[viPossiblePlace.at(iGoPlace-1)].name);
		if(placeNumber == -1){cout << "Invalid input.\n";action(num);return;}
		if(connected(p[findPlaceNumber(a[num].loc)],p[placeNumber]))
			a[num].move( p[viPossiblePlace.at(iGoPlace-1)].name );
	}
	if(moveType == 3){
		if(a[num].h1 == "oil"){a[num].h1 = "fist";useOil(num);return;}
		cout << "Possible players to use your " << a[num].h1 <<" at:\n";
		vector<int> viPlayer=vector<int>();
		int iPlayer = 0;
		for(int i = 1;i<=playerCount;i++){
			if(a[num].loc == a[i].loc && a[i].hp > 0 && a[i].invisibleTime==0){
				iPlayer++;
				viPlayer.push_back(i);
				cout <<iPlayer<<":"<< a[i].name <<".\n";
			}
		}
		if(viPlayer.size() == 0){cout << "Nobody to use at.\n";action(num);return;}
		cin >> iPlayer;
		if(iPlayer > viPlayer.size()){cout<<"Overflow.\n";action(num);return;}
		string sWeapon;
		if(a[num].h1 == "fist"){
			if(a[num].sleep == awake)sWeapon = "fist_awake";
			else if (a[num].sleep == daydream)sWeapon = "fist_dream";
			else sWeapon = "";
		}
		else sWeapon = a[num].h1;
		use(viPlayer.at(iPlayer-1),sWeapon);
		if(a[num].h1 == "poison" || a[num].h1 == "invisible" || a[num].h1 == "health"){
			a[num].h1 = "fist";
		}
	}
	if(moveType == 4){
		if(a[num].h2 == "oil"){a[num].h2 = "fist";useOil(num);return;}
		cout << "Possible players to use your " << a[num].h2 <<" at:\n";
		vector<int> viPlayer=vector<int>();
		int iPlayer = 0;
		for(int i = 1;i<=playerCount;i++){
			if(a[num].loc == a[i].loc && a[i].hp > 0 && a[i].invisibleTime == 0){
				iPlayer++;
				viPlayer.push_back(i);
				cout <<iPlayer<<":"<< a[i].name <<".\n";
			}
		}
		if(viPlayer.size() == 0){cout << "Nobody to use at.\n";action(num);return;}
		cin >> iPlayer;
		if(iPlayer > viPlayer.size()){cout<<"Overflow.\n";action(num);return;}
		string sWeapon;
		if(a[num].h2 == "fist"){
			if(a[num].sleep == awake)sWeapon = "fist_awake";
			else sWeapon = "fist_dream";
		}
		else sWeapon = a[num].h2;
		use(viPlayer.at(iPlayer-1),sWeapon);
		if(a[num].h2 == "poison" || a[num].h2 == "invisible" || a[num].h2 =="health"){
			a[num].h2 = "fist";
		}
	}
	if(moveType == 5){
		cout << "change your state into:\n"
		<< "1:sleeping.\n"
		<< "2:daydreaming.\n"
		<< "3:awake.\n";
		int inState;
		cin >> inState;
		if(inState>3){cout<<"Overflow.\n";action(num);return;}
		a[num].sleep = inState -1;
		if(a[num].sleep != awake){
			a[num].h1 = a[num].h2 = "fist";
			a[num].shoulder = "empty";
			a[num].rocket = 0;
			a[num].aim = "";
		}
	}
	if(moveType == 6){
		if(a[num].sleep != awake){
			cout<<"You can't do it. Try another thing.\n";
			action(num);
			return;
		}
		vector<string> vsWeapon;
		int iPlace = findPlaceNumber(a[num].loc);
		int iWeapon = 0;
		if(p[iPlace].knife > 0){
			iWeapon++;vsWeapon.push_back("knife");cout <<iWeapon<<":"<<"knife.\n";
		}
		if(p[iPlace].rpg > 0){
			iWeapon++;vsWeapon.push_back("rpg");cout <<iWeapon<<":"<<"rpg.\n";
		}
		if(p[iPlace].rocket > 0){
			iWeapon++;vsWeapon.push_back("rocket");cout <<iWeapon<<":"<<"rocket.\n";
		}
		if(p[iPlace].health > 0){
			iWeapon++;vsWeapon.push_back("health");cout <<iWeapon<<":"<<"health.\n";
		}
		if(p[iPlace].poison > 0){
			iWeapon++;vsWeapon.push_back("poison");cout <<iWeapon<<":"<<"poison.\n";
		}
		if(p[iPlace].oil > 0){
			iWeapon++;vsWeapon.push_back("oil");cout <<iWeapon<<":"<<"oil.\n";
		}
		if(p[iPlace].invisible > 0){
			iWeapon++;vsWeapon.push_back("invisible");cout <<iWeapon<<":"<<"invisible.\n";
		}
		if(vsWeapon.size() == 0){cout << "Nothing to get.\n";action(num);return;}
		cin >> iWeapon;
		if(iWeapon>vsWeapon.size()){cout<<"Overflow.\n";action(num);return;}
		if(vsWeapon.at(iWeapon-1) == "knife"){p[iPlace].knife -= 1;}
		if(vsWeapon.at(iWeapon-1) == "rpg"){p[iPlace].rpg -= 1;}
		if(vsWeapon.at(iWeapon-1) == "rocket"){p[iPlace].rocket -= 1;}
		if(vsWeapon.at(iWeapon-1) == "health"){p[iPlace].health -= 1;}
		if(vsWeapon.at(iWeapon-1) == "poison"){p[iPlace].poison -= 1;}
		if(vsWeapon.at(iWeapon-1) == "oil"){p[iPlace].oil -= 1;}
		if(vsWeapon.at(iWeapon-1) == "invisible"){p[iPlace].invisible -= 1;}
		int hand;
		cout << "get it to your:\n"
		<< "1:left hand.\n"
		<< "2:right hand.\n"
		<< "3:shoulder.\n";
		cin >> hand;
		if(hand>3){cout<<"Overflow.\n";action(num);return;}
		if(hand == 1)a[num].h1 = vsWeapon.at(iWeapon-1);
		if(hand == 2)a[num].h2 = vsWeapon.at(iWeapon-1);
		if(hand == 3){
			if(vsWeapon.at(iWeapon-1)=="rpg")
				a[num].shoulder = vsWeapon.at(iWeapon-1);
			else if(vsWeapon.at(iWeapon-1)=="rocket")
				a[num].rocket+=2;
			else {cout<<"Cannot put it on your shoulder.\n";action(num);return;}
		}
	}
	if(moveType == 7){
		if(a[num].h1 == "knife")a[num].h1 = "upgraded knife";
		if(a[num].h2 == "knife")a[num].h2 = "upgraded knife";
	}
	if(moveType == 8){
		if(a[num].sleep == asleep){cout << "You're asleep.\n";action(num);return;}
		vector<int> viPlayer = vector<int>();
		int iPlayer = 0;
		cout << "Possible players to drag:\n";
		for(int i = 1;i <= playerCount; i++){
			if(a[i].hp>0 && a[i].invisibleTime ==0)
			if(near(p[findPlaceNumber(a[i].loc)],p[findPlaceNumber(a[num].loc)])){
				iPlayer++;
				viPlayer.push_back(i);
				cout << iPlayer << ":" << a[i].name << "\n";
			}
		}
		if(viPlayer.size() == 0){cout << "Nobody to drag.\n";action(num);return;}
		cin >> iPlayer;
		if(iPlayer > viPlayer.size()){cout<<"Overflow.\n";action(num);return;}
		if(a[num].loc != a[viPlayer.at(iPlayer-1)].loc){
			a[viPlayer.at(iPlayer-1)].move( a[num].loc );
			return;
		}
		else {
			vector<int> viPlace = vector<int>();
			int iPlace = 0;
			cout << "Possible places to go:\n";
			for(int i = 1;i<3*n+4;i++){
				if(near(p[i],p[findPlaceNumber(a[num].loc)])){
					if(p[i].name != a[num].loc){
						iPlace++;
						viPlace.push_back(i);
						cout << iPlace <<":"<<p[i].name<<"\n";
					}
				}
			}
			if(viPlace.size() == 0){cout << "Nowhere to drag.\n";action(num);return;}
			cin >> iPlace;
			if(iPlace > viPlace.size()){cout<<"Overflow.\n";action(num);return;}
			a[viPlayer.at(iPlayer-1)].move( p[viPlace.at(iPlace-1)].name );
		}
	}
	if(moveType == 9){
		if(a[num].shoulder != "rpg" && a[num].shoulder != "rpg_loaded")
		{cout<<"You don't have an rpg.\n";action(num);return;}
		if(a[num].shoulder == "rpg"){a[num].shoulder = "rpg_loaded";return;}
		if(a[num].shoulder == "rpg_loaded"){
			cout<<"You can:\n1:aim\n2:fire.\n";
			int iAct;
			cin >> iAct;
			if(iAct != 1 && iAct != 2){cout << "Overflow.\n";action(num);return;}
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
						cout << iPlace << ":" << p[i].name<<"\n";
					}
				}
				cin >> iPlace;
				if(iPlace > viPlace.size()){cout<<"Overflow.\n";action(num);return;}
				a[num].aim = p[viPlace.at(iPlace-1)].name;
			}
			if(iAct == 2){
				if(a[num].rocket == 0){cout<<"Lack rocket.\n";action(num);return;}
				a[num].rocket-=1;
				if(a[num].aim == "supermarket"){
					for(int i = 1;i<=n;i++){
						if(a[i].loc=="supermarket"||a[i].loc=="drug store")
							if(a[i].hp>0 && a[i].invisibleTime==0)a[i].hp-=1.0;
					}
					p[DRUGSTORE].door=none;
				}
				else {
					for(int i = 1;i<= n;i++){
						if(a[i].loc == a[num].aim)
							if(a[i].hp>0 && a[i].invisibleTime==0)a[i].hp-=1.0;
					}
					p[findPlaceNumber(a[num].aim)].door=none;
					p[findPlaceNumber(a[num].aim)].alive=0;
					
				}
			}
		}
	}
	if(moveType == 10){
		if(findPlaceNumber(a[num].loc)<=n){
			if(p[findPlaceNumber(a[num].loc)+n].door != closed)
				{cout<<"Cannot flush.\n";action(num);return;}
			else for(int i=1;i<=n;i++)
				if(a[i].loc==p[findPlaceNumber(a[num].loc)+n].name)
					if(a[i].hp > 0 && a[i].invisibleTime==0)a[i].hp-=1.0;
		}
		if(findPlaceNumber(a[num].loc) == SUPERMARKET){
			int iPlace = 0;
			vector<int> viPlace = vector<int>();
			for(int i = DRUGSTORE+1;i<=3*n+4;i++){
				if(p[i].door==closed){
					iPlace++;
					viPlace.push_back(i);
					cout<<iPlace<<":"<<p[i].name<<"\n";
				}
			}
			cin >> iPlace;
			if(iPlace > viPlace.size()){cout<<"Overflow.\n";action(num);return;}
			for(int i=1;i<=n;i++)if(a[i].loc==p[viPlace.at(iPlace-1)].name)
				if(a[i].hp > 0 && a[i].invisibleTime==0)a[i].hp-=1.0;
		}
		if(findPlaceNumber(a[num].loc) == DRUGSTORE){
			int iPlace = 0;
			vector<int> viPlace = vector<int>();
			for(int i = n+1;i<=2*n;i++){
				if(p[i].door==closed && !(p[i-n].alive)){
					iPlace++;
					viPlace.push_back(i);
					cout<<iPlace<<":"<<p[i].name<<"\n";
				}
			}
			cin >> iPlace;
			if(iPlace > viPlace.size()){cout<<"Overflow.\n";action(num);return;}
			for(int i=1;i<=n;i++)if(a[i].loc==p[viPlace.at(iPlace-1)].name)
				if(a[i].hp > 0 && a[i].invisibleTime==0)a[i].hp-=1.0;
		}
	}
	if(moveType > 10 || moveType < -1){cout<<"Overflow.\n";action(num);return;}
	/*end for checking*/
	//if(a[i].loc=="supermarket"||a[i].loc==
}

void useOil(int num){
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
	int iPlace = 1;
	for(;iPlace<=viPlace.size();iPlace++)cout<<iPlace<<":"<<p[viPlace.at(iPlace-1)].name<<"\n";
	cin>>iPlace;
	if(iPlace>viPlace.size()){cout<<"Overflow.\n";action(num);return;}
	p[viPlace.at(iPlace-1)].oiled = 1;
	
}
}
