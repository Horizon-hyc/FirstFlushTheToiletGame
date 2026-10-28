#ifndef COMPUTER_H
#define COMPUTER_H
//normal rand
#include "rand_bot.h"

//blockable rand
#include "rand_trainable_bot.h"
#define MAX_TRY_STEP 100
void autoAction(const int num, int botMode){
	switch(botMode){
		case 0:{
			randBot::randAction(num,randInt(1,11));
			break;
		}
		case 1:{
			trainable::botTrainable_t bot1(botName);
			int count = 0;
			while(trainable::randAction(num,bot1) && ++count<=MAX_TRY_STEP){};
			bot1.save();
			break;
		}
		default:{
			break;
		}
	}
}
#endif