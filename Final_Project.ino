#include <Adafruit_CircuitPlayground.h>
const int mSwitch = 7;
const int lButton = 5;
const int rButton = 4;
bool play_Music = true;
bool game_State = false;
int A_four = 440;
int losses_Player_Left;
int losses_Player_Right;
int lButton_time;
int lPrevious_Press;
int rButton_time;
int rPrevious_Press;


float midi[127];
int song[27][2] = {
    {69, 100},{127, 100},{64, 100},{62, 100},{61, 100},{127, 200},{61, 100},{62, 100},{64, 100},{66, 100},{68, 100},{64, 100},{127, 300},{66, 100},{127, 100},{64, 100},{62,100},
    {61, 100}, {127,100},{61,100},{127,100},{59,100},{127,100},{61,100},{127,100},{62,100},{127,200}
  };
void main_Menu_Music(){
  if(play_Music){
  for(int i = 0; i < sizeof(song) / sizeof(song[0]); i++) // Calculate how many rows are in the array using: sizeof(song) / sizeof(song[0])
  {
    if(!play_Music){
      return;
    }
    CircuitPlayground.playTone(midi[song[i][0]], song[i][1]);
    delay(1);
  }
  }
}

void setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();
  CircuitPlayground.clearPixels();
  pinMode(lButton, INPUT_PULLDOWN);
  pinMode(rButton, INPUT_PULLDOWN);
  pinMode(mSwitch, INPUT_PULLUP);
  CircuitPlayground.setBrightness(25);
  attachInterrupt(digitalPinToInterrupt(lButton), buttonLPressed, FALLING);
  attachInterrupt(digitalPinToInterrupt(rButton), buttonRPressed, FALLING);
  attachInterrupt(digitalPinToInterrupt(mSwitch), mSwitchChanged, CHANGE);
  generateMIDI();
  Serial.print("Flip the Switch to start!");
}

void game_Start(void){
if(game_State){
  CircuitPlayground.clearPixels();
  delay(200);
for (int i = 0; i < 5; ++i){
CircuitPlayground.setPixelColor(i, 160, 32, 240);
CircuitPlayground.setPixelColor(9-i, 160, 32, 240);
delay(500);
}
CircuitPlayground.clearPixels();
delay(1000);
for(int i = 0; i < 10; ++i){
CircuitPlayground.setPixelColor(i, 255, 0, 0);
}
delay(random(500,2500));
for(int i = 0; i < 10; ++i){
  CircuitPlayground.setPixelColor(i, 0, 255, 0);
}
int color_Change = millis();
delay(3000);
  int right_time = rButton_time - color_Change;
  int left_time = lButton_time - color_Change;
  Serial.print("Left Players Reaction Time:   ");
  Serial.println(left_time);
  Serial.print("Right Players Reaction Time:   ");
  Serial.println(right_time);
  if((left_time < 0) && (right_time <0)){
    Serial.println("No Winner");
  }
  else if((left_time < 0) && (right_time > 0)){
    ++losses_Player_Left;
    if(left_time > -2500){
      Serial.println("Left Player was too antsy Right Players Wins Round by Default");
      score_Board();
    }
    else{
    Serial.println("Right Players Wins Round by Default");
    score_Board();
    }
  }
  else if((left_time > 0) && (right_time < 0)){
    ++losses_Player_Right;
    if(right_time > -2500){
      Serial.println("Right Player was too antsy Left Players Wins Round by Default");
      score_Board();
    }
    else{
    Serial.println("Left Players Wins Round by Default");
    score_Board();
    }
  }
  else if (left_time > right_time){
    ++losses_Player_Left;
    Serial.println("Right Player Wins Round");
    score_Board();
  }
  else{
    ++losses_Player_Right;
    Serial.println("Left Player Wins Round");
    score_Board();
  }
}
}

void score_Board(void){
  CircuitPlayground.clearPixels();
  for (int i = 0; i < losses_Player_Right; ++i){
  CircuitPlayground.setPixelColor(9-i, 0, 0, 255);
  delay(500);
  }
  for (int i = 0; i < losses_Player_Left; ++i){
  CircuitPlayground.setPixelColor(i, 255, 0, 0);
  delay(500);
  }
  delay(3000);
  if(losses_Player_Left == 5){
    game_Over_Player_Left();
  }
  if(losses_Player_Right == 5){
    game_Over_Player_Right();
  }
}

void game_Over_Player_Left(void){
Serial.println("Right Player wins!!!");
for(int i = 0; i < 3; ++i){
  flash_Red();
}
reset();
}
void game_Over_Player_Right(void){
Serial.println("Left Player wins!!!");
for(int i = 0; i < 3; ++i){
  flash_Blue();
}
reset();
}

void reset(){
game_State = !game_State;
play_Music = !play_Music;
losses_Player_Right = 0;
losses_Player_Left = 0;
}

void flash_Red(){
  for(int i = 0; i < 10; ++i){
CircuitPlayground.setPixelColor(i, 255, 0, 0);
}
delay(1000);
CircuitPlayground.clearPixels();
delay(1000);
}
void flash_Blue(){
  for(int i = 0; i < 10; ++i){
CircuitPlayground.setPixelColor(i, 0, 0, 255);
}
delay(1000);
CircuitPlayground.clearPixels();
delay(1000);
}

void loop(){

  main_Menu_Music();
  game_Start();
}

void buttonLPressed(){
  lButton_time = millis();
  if(lButton_time - lPrevious_Press > 2500){
lButton_time = millis();
lPrevious_Press = lButton_time;
  }
  else{
    lButton_time = lPrevious_Press;
  }
}

void buttonRPressed(){
  rButton_time = millis();
  if(rButton_time - rPrevious_Press > 2500){
rButton_time = millis();
rPrevious_Press = rButton_time;
  }
  else{
    rButton_time = rPrevious_Press;
  }
}

void mSwitchChanged(){
  play_Music = !play_Music;
  game_State = !game_State;
}

void generateMIDI()
{
  for (int x = 0; x < 127; ++x)
  {
    midi[x] = (A_four / 32.0) * pow(2.0, ((x - 9.0) / 12.0));
    Serial.println(midi[x]);
  }
}