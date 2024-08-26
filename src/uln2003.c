#include <wiringPi.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define CLOCKWISE 1
#define COUNTER_CLOCKWISE 2

void delayMS(int x){
	usleep(x * 1000);
}

void rotate(int* pins, int direction){
	for(int i=0;i<4; i++){
		if(CLOCKWISE == direction){
			for(int j=0;j<4;j++){
				if(j==i){
					digitalWrite(pins[3-j], 1);	//output a high level
				}
				else{
					digitalWrite(pins[3-j], 0);	//output a low level
				}
			}
		}
		else if(COUNTER_CLOCKWISE == direction){
			for(int j=0;j<4;j++){
				if(j==i){
					digitalWrite(pins[j], 1);	//output a high level
				}
				else{
					digitalWrite(pins[j], 0);	//output a low level
				}
			}
		}
		delayMS(4);
	}
}

int main(void){
	int pinA = 26;
	int pinB = 27;
	int pinC = 28;
	int pinD = 29;
	int pins[4] = {pinA, pinB, pinC, pinD};

	if(-1 == wiringPiSetup()){
		printf("wiringPiSetup() error!\n");
		return -1;
	}

	pinMode(pinA, OUTPUT);
	pinMode(pinB, OUTPUT);
	pinMode(pinC, OUTPUT);
	pinMode(pinD, OUTPUT);

	delayMS(50);	// wait for a stable status
	for(int i=0;i<500;i++){
		rotate(pins, CLOCKWISE);
		if(0 == i%100)
			printf("%d\n", i);
	}

	return 0;
}
