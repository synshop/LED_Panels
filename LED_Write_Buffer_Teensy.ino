
//Define Pins
#define LEDPIN_D1 7
#define LEDPIN_D2 8
#define LEDPIN_OE 9
#define LEDPIN_LAT 10
#define LEDPIN_A0 11
#define LEDPIN_A1 12
#define LEDPIN_CLK 13



//declare buffers for screen bitmap and output
uint8_t screenBuffer[64][16][3];
uint8_t outputBuffer[2][4][384];


elapsedMillis bench;
int cycleCount;
volatile uint8_t curFrame;

void setup() {

  curFrame = 0;

  //initialize buffers with zero values
  memset(screenBuffer,0,sizeof(screenBuffer));
  memset(outputBuffer,0,sizeof(outputBuffer));
   
  //setup pins
  pinMode(LEDPIN_D1, OUTPUT);
  pinMode(LEDPIN_D2, OUTPUT);
  pinMode(LEDPIN_OE, OUTPUT);
  pinMode(LEDPIN_LAT, OUTPUT); 
  pinMode(LEDPIN_A0, OUTPUT);
  pinMode(LEDPIN_A1, OUTPUT);
  pinMode(LEDPIN_CLK, OUTPUT);

  //initialize pins and turn off display
  digitalWriteFast(LEDPIN_OE, HIGH);
  digitalWriteFast(LEDPIN_LAT, LOW);
  digitalWriteFast(LEDPIN_CLK, LOW);
  digitalWriteFast(LEDPIN_D1, LOW);
  digitalWriteFast(LEDPIN_D2, LOW);
  digitalWriteFast(LEDPIN_A0, LOW);
  digitalWriteFast(LEDPIN_A1, LOW);
  
  //wait for display to initialize
  delay(10);

  for(int x = 0; x < 64; x++) {
    for(int y = 0; y < 16; y++) {
      if(x % 1 == 0) {screenBuffer[x][y][0] = (x % 64);}
    }
  }

    loadFrame();
  
}

void loop() {

    outputFrame(); 
    curFrame++;
    if(curFrame > 64) {
      curFrame = 0;
    }
}


//send data to display
void clockData(uint8_t pin1, uint8_t pin2) {
  digitalWriteFast(LEDPIN_D1, pin1);
  digitalWriteFast(LEDPIN_D2, pin2);
  digitalWriteFast(LEDPIN_CLK, HIGH);
  digitalWriteFast(LEDPIN_D1, LOW);
  digitalWriteFast(LEDPIN_D2, LOW);
  digitalWriteFast(LEDPIN_CLK, LOW);
}



//update outputBuffer from frameBuffer
void loadFrame() {

 int outputBufferIndex = 0;

  //load data from screen buffer into output buffer
  for(int half = 1; half >=0; half--) {
    for(int line = 3; line >=0; line--) {
      for(int block = 7; block >= 0; block--) {
        for(int color = 2; color >= 0; color--) {
          for(int pixel = 7; pixel >=0; pixel--) {
           outputBuffer[half][line][outputBufferIndex] = screenBuffer[(block * 8) + pixel][(half * 8) + (line + 4)][color];
           outputBufferIndex++;
           }
           for(int pixel = 7; pixel >=0; pixel--) {
             outputBuffer[half][line][outputBufferIndex] = screenBuffer[(block * 8) + pixel][(half * 8) + line][color];
             outputBufferIndex++;
          }
        }
      }
    outputBufferIndex = 0;
    }
  }


}

void outputFrame() {
    for(int line = 0; line < 4; line++) {

    //turn off display
    digitalWriteFast(LEDPIN_OE, HIGH);
    //select active line
    if((line + 1) % 4 == 0 ) {
      digitalWriteFast(LEDPIN_A0, LOW);
      digitalWriteFast(LEDPIN_A1, LOW);
    }
    else if((line + 1) % 4 == 3){
      digitalWriteFast(LEDPIN_A0, HIGH);
      digitalWriteFast(LEDPIN_A1, LOW);
    }
    else if((line+ 1) % 4 == 2) {
      digitalWriteFast(LEDPIN_A0, LOW);
      digitalWriteFast(LEDPIN_A1, HIGH);
    }
    else {
      digitalWriteFast(LEDPIN_A0, HIGH);
      digitalWriteFast(LEDPIN_A1, HIGH);
    }
    //send data from output buffer
    for(int lbIndex = 0; lbIndex < 384; lbIndex++) {
      clockData((outputBuffer[0][line][lbIndex] > curFrame), (outputBuffer[1][line][lbIndex] > curFrame));
    }
    
  //latch data and turn on display
  digitalWriteFast(LEDPIN_LAT, HIGH);
  digitalWriteFast(LEDPIN_LAT, LOW);
  
  //delayMicroseconds(30);
  digitalWriteFast(LEDPIN_OE, LOW);
  delayMicroseconds(50);
  digitalWriteFast(LEDPIN_OE, HIGH);
  }
}

