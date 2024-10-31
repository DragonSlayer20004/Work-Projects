#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "wave.h"

/* Function Declaration */
short* reverseWave(short* channel, int sampleSize);
short* changeSpeed(double speedChange, short* channel, int sampleSize);
short* fadeOutWave(double volumeChange, short* channel, int sampleSize);
short* fadeInWave(double volumeChange, short* channel, int sampleSize);
void changeVolume(double volumeChange, short* channel, int sampleSize);
short* addEcho(double delay, double scale, short* original, int sampleSize);
short charToShort();
void shortToChar(short number);
short clamp(double number);

/* Defines */
#define SAMPLE_RATE 44100

// argc: number of command line arguments
// argv: array of strings (contains the command line arguments)
int main(int argc, char **argv){
	//Create Wave
	WaveHeader header;
	readHeader(&header);
	
	//Copies the header infofrom input file into workable struct
	int sampleSize = header.dataChunk.size / (header.formatChunk.channels * (header.formatChunk.bitsPerSample / 8));
	
	short* leftChannel = malloc(sizeof(short) * sampleSize);
	short* rightChannel = malloc(sizeof(short) * sampleSize);
	
	//Error Checking for valid wave file.	
	if(strstr("RIFF", header.ID) != NULL){
		fprintf(stderr, "File is not a RIFF file\n");
		return -1;
	}
	
	if(strstr("fmt", header.formatChunk.ID) != NULL || header.formatChunk.size != 16 || header.formatChunk.compression != 1){
		fprintf(stderr, "Format chunk is corrupted\n");
		return -1;
	}
	
	if(strstr("data", header.dataChunk.ID) != NULL){
		fprintf(stderr, "Data chunk is corrupted\n");
		return -1;
	}
	
	if(header.formatChunk.channels != 2){
		fprintf(stderr, "File is not stereo\n");
		return -1;
	}
	
	if(header.formatChunk.sampleRate != SAMPLE_RATE){
		fprintf(stderr, "File does not use 44,100Hz sample rate\n");
		return -1;
	}
	
	if(header.formatChunk.bitsPerSample != 16){
		fprintf(stderr, "File does not have 16-bit samples\n");
		return -1;
	}
	//end of error checking
	
	if(leftChannel == NULL || rightChannel == NULL){
		fprintf(stderr, "Program out of memory");
	}
	
	//Copies the sound from input file into workable array of shorts
	for(int i = 0; i < sampleSize; i++){
		leftChannel[i] = charToShort();
		rightChannel[i] = charToShort();
	}
	
	//Loops over all string fields
	for(int i = 1; i < argc; i++){
		char* choice = argv[i];
		
		//Grabs the second letter in a statement thus ignoring '-' character.
		switch(choice[1])
		{
			case 'r': {
				//reverse
				leftChannel = reverseWave(leftChannel, sampleSize);
				rightChannel = reverseWave(rightChannel, sampleSize);
				break;
			}
			case 's': {
				//speed
				//Take the string number and put it to int
				double change = atof(argv[i+1]);
				if(change <= 0){
					fprintf(stderr,"A positive number must be supplied for the speed change\n");
					return -1;
				}	
				leftChannel = changeSpeed(change, leftChannel, sampleSize);
				rightChannel = changeSpeed(change, rightChannel, sampleSize);
				sampleSize = sampleSize / change;
				i++; //Skips over number change
				break;
			}
			case 'f': {
				//flip channels
				short* temp = rightChannel;
				rightChannel = leftChannel;
				leftChannel = temp;
				break;
			}
			case 'o': {
				//fade out
				//Take the string number and put it to int
				double change = atof(argv[i+1]);
				if(change <= 0){
					fprintf(stderr,"A positive number must be supplied for the fade in and fade out time\n");
					return -1;
				}
				leftChannel = fadeOutWave(change, leftChannel, sampleSize);
				rightChannel = fadeOutWave(change, rightChannel, sampleSize);
				i++; //Skips over number change
				break;
			}
			case 'i': {
				//fade in
				//Take the string number and put it to int
				double change = atof(argv[i+1]);
				if(change <= 0){
					fprintf(stderr,"A positive number must be supplied for the fade in and fade out time\n");
					return -1;
				}
				leftChannel = fadeInWave(change, leftChannel, sampleSize);
				rightChannel = fadeInWave(change, rightChannel, sampleSize);
				i++; //Skips over number change
				break;
			}
			case 'v': {
				//volume
				//Take the string number and put it to int
				double change = atof(argv[i+1]);
				if(change <= 0){
					fprintf(stderr,"A positive number must be supplied for the volume scale\n");
					return -1;
				}
				changeVolume(change, leftChannel, sampleSize);
				changeVolume(change, rightChannel, sampleSize);
				i++; //Skips over number change
				break;
			}
			case 'e': {
				//echo
				//Take the string number and put it to int
				double delay = atof(argv[i + 1]);
				double scale = atof(argv[i + 2]); 
				if(delay <= 0 || scale <= 0){
					fprintf(stderr,"Positive numbers must be supplied for the echo delay and scale parameters\n");
					return -1;
				}
				leftChannel = addEcho(delay, scale, leftChannel, sampleSize);
				rightChannel = addEcho(delay, scale, rightChannel, sampleSize);
				
				sampleSize += SAMPLE_RATE * delay;
				i+=2; //Skips over number change
				break;
			}
			default: {
				//If it gets here it means there was an invalid command.
				fprintf(stderr, "Usage: wave [[-r][-s factor][-f][-o delay][-i delay][-v scale][-e delay scale] < input > output\n");
				return -1;
			}
		}
	}
	
	//Takes everything that was done and outputs it to a new wave file.
	header.dataChunk.size = sampleSize * (header.formatChunk.channels * (header.formatChunk.bitsPerSample / 8));
	header.size = 36 + header.dataChunk.size;
	writeHeader(&header);
	for(int i = 0; i < sampleSize; i++){
		shortToChar(leftChannel[i]);
		shortToChar(rightChannel[i]);
	}
	
	return 0;
}

/*
	Switches the first half of the channel with the second half.
*/
short* reverseWave(short* channel, int sampleSize){
    int index = sampleSize;
	for(int i = 0; i < sampleSize / 2; ++i){
		short temp = channel[i];
		channel[i] = channel[index];
		channel[index] = temp;
		index--;
	}
	return channel;
}

/*
	Changes the speed of a channel by the inputted amount. Speedchange > 1 results in smaller file size while SpeedChange < 1 results in larger file size.
*/
short* changeSpeed(double speedChange, short* channel, int sampleSize){
    int size = sampleSize / speedChange;
    short* newChannel = malloc(sizeof(short) * size);
	if(newChannel == NULL){
		fprintf(stderr, "Program out of memory");
	}
	
	for (int i = 0; i < size; i++){
	    newChannel[i] = channel[(int)(i * speedChange)];
	}
	
	//Free old channels and replace them with the new ones
	free(channel);
	return newChannel;
}

/*
	Fades out the sound based on the length.
*/
short* fadeOutWave(double length, short* channel, int sampleSize){
	int fadestart = (length * SAMPLE_RATE);
	
	if(fadestart > sampleSize){
		for(int i = fadestart - sampleSize; i < fadestart; i++){
			double volumeChange = (1.0 - (i / (double)fadestart)) * (1.0 - (i / (double)fadestart));
			channel[sampleSize - fadestart + i] *= volumeChange;
		}	
	} else{
		for(int i = 0; i < fadestart; i++){
			double volumeChange = (1.0 - (i / (double)fadestart)) * (1.0 - (i / (double)fadestart));
			channel[sampleSize - fadestart + i] *= volumeChange;
		}
	}
	return channel;
}

/*
	Fades in the sound based on the length.
*/
short* fadeInWave(double length, short* channel, int sampleSize){
	int fadestart = (length * SAMPLE_RATE);
	
	for(int i = 0; i < fadestart && i < sampleSize; i++){
		double volumeChange = (((double)i / (double)fadestart)) * (((double)i / (double)fadestart));
		channel[i] = channel[i] * volumeChange;
	}
	return channel;
}

/*
	Uses the clamp method to ensure that the volume remains within short range and doesn't overflow.
*/
void changeVolume(double volumeChange, short* channel, int sampleSize){
	for(int i = 0; i < sampleSize; ++i){
		channel[i] = clamp(channel[i] * volumeChange);
	}
}

/*
	Adds in the echo after every few seconds based on the delay. Scale determines how loud the echo is.
*/
short* addEcho(double delay, double scale, short* original, int sampleSize){
	int delayLength = SAMPLE_RATE * delay;
	short* echo = calloc(sampleSize + delayLength, sizeof(short));
	//Error Check for calloc out of memory
	if(echo == NULL){
		fprintf(stderr, "Program out of memory");
	}
	
	for(int i = 0; i < sampleSize; ++i){
		echo[i] = original[i];
	}
	
	for(int i = 0; i < sampleSize; ++i){
		echo[delayLength + i] = clamp(echo[delayLength + i] + original[i] * scale); 
	}
	
	free(original);
	return echo;
}

/**
	Takes in two chars and combines them into one short
*/
short charToShort(){
	int firstLetter = getchar();
	int secondLetter = getchar();
	
	short newShort = (secondLetter << 8) + firstLetter;
	
	if(firstLetter == EOF || secondLetter == EOF){
	fprintf(stderr, "File size does not match size in header\n");
		exit(-1);
	}
	return newShort;
}

/**
	Takes a short and breaks it apart into two different chars.
*/
void shortToChar(short number){
	char firstLetter;
	char secondLetter;
	
	firstLetter = number;
	secondLetter = (number >> 8);
	
	putchar(firstLetter);
	putchar(secondLetter);
}

/**
	Clamp method to keeps shorts contained
*/
short clamp(double number){
	if(number > SHRT_MAX)
		return SHRT_MAX;
	else if(number < SHRT_MIN)
		return SHRT_MIN;
	else
		return number; 
}




