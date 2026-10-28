#ifndef FRACTION_BYDOG_H
#define FRACTION_BYDOG_H
#include<cstdlib>
#include<string>

namespace frac{
int _MAX_FRACTION_TRY = 20;
int _MAX_FRACTION_ACCEPT = 1<<12;
	class fraction{
public:
	int numerator,denominator;
	fraction(){numerator = denominator = 0;}
	fraction(double _decimal_number){
		if (_decimal_number < 0){
			numerator = -fraction(-_decimal_number).numerator;
			denominator = -fraction(-_decimal_number).denominator;
		}
		if(abs(_decimal_number) >= _MAX_FRACTION_ACCEPT){fraction();return;}
		int num[_MAX_FRACTION_TRY];
		for(int __i=0;__i<_MAX_FRACTION_TRY;__i++){num[__i]=0;}
		int __i = 0;
		for(;__i<_MAX_FRACTION_TRY;__i++){
			num[__i] = (int)_decimal_number;
			_decimal_number -= num[__i];
			_decimal_number = 1/_decimal_number;
			if(_decimal_number >= _MAX_FRACTION_ACCEPT)break;
		}
		if(__i == _MAX_FRACTION_TRY){fraction();}
		int _numerator=1,_denominator=num[__i];
		for(int __j=__i-1;__j>=0;__j--){
			_numerator += _denominator*num[__j];
			int _temp = _numerator;
			_numerator = _denominator;
			_denominator = _temp;
		}
		if(_numerator >= 0)
		{
			denominator = _numerator;
			numerator = _denominator;
			return;
		}
		else {
			numerator = - _denominator;
			denominator = - _numerator;
			return;
		}
	}
	int frac1(){return numerator;}
	int frac2(){return denominator;}
	const std::string str(){
		if(denominator == 1)return std::to_string(numerator);
		return std::to_string(numerator)+"/"+std::to_string(denominator);
	}
	const double value(){if(denominator == 0)return 0;return numerator*1.0/denominator;}
};
}
#endif