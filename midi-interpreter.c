#include <stdio.h>
#define true 1
#define false 0

short readByte(void* b) {
	if (fread(b, 1, 1, stdin) <= 0)
		return false;
	return true;
}

void channelVoiceType(char b) {
	short type = (b & 0x70) >> 4;
	switch (type) {
		case 0:
			printf("note off");
			break;
		case 1:
			printf("note on");
			break;
		case 2:
			printf("poly key pressure");
			break;
		case 3:
			printf("control change");
			break;
		case 4:
			printf("program change");
			break;
		case 5:
			printf("channel pressure");
			break;
		case 6:
			printf("pitch bend");
			break;
	}
	putchar('\n');
}

void interpretStatus(unsigned char b) {
	short channel = b & 0x0f;
	printf("Channel %2d, ", channel);
	channelVoiceType(b);
}

void interpretDataByte(unsigned char b) {
	printf("%d\n", b);
}

short int readMessage() {
	unsigned char b;
	if (!readByte(&b))
		return false;

	if (b >= 0x80)
		interpretStatus(b);
	else
		interpretDataByte(b);
	return true;
}

int main() {
	printf("Listening for MIDI...\n");

	while (readMessage())
		;
}
