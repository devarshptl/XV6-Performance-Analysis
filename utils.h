void panic(char *msg);

long now();

unsigned int xorbuf(unsigned int *buffer, int size);

unsigned int intxorbuf(unsigned int *buffer, int size, unsigned int cur);

unsigned char bytexorbuf(unsigned char *buffer, int size, unsigned char cur);