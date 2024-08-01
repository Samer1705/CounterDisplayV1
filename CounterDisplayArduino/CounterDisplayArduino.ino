#define SEGA D12
#define SEGB D11
#define SEGC D10
#define SEGD D9
#define SEGE D8
#define SEGF D7
#define SEGG D6
#define DIGIT1_EN D5
#define DIGIT2_EN D4
#define DIGIT3_EN D3
#define SEG_TYPE 1

#if (SEG_TYPE == 1) {
int digit[10][7] = {
  { 1, 1, 1, 1, 1, 1, 0 },  //0
  { 0, 1, 1, 0, 0, 0, 0 },  //1
  { 1, 1, 0, 1, 1, 0, 1 },  //2
  { 1, 1, 1, 1, 0, 0, 1 },  //3
  { 0, 1, 1, 0, 0, 1, 1 },  //4
  { 1, 0, 1, 1, 0, 1, 1 },  //5
  { 1, 0, 1, 1, 1, 1, 1 },  //6
  { 1, 1, 1, 0, 0, 1, 0 },  //7
  { 1, 1, 1, 1, 1, 1, 1 },  //8
  { 1, 1, 1, 1, 0, 1, 1 }   //9
};
}
#else {
  
}
void setup() {
  // put your setup code here, to run once:
  pinMode(SEGA, OUTPUT);
  pinMode(SEGB, OUTPUT);
  pinMode(SEGC, OUTPUT);
  pinMode(SEGD, OUTPUT);
  pinMode(SEGE, OUTPUT);
  pinMode(SEGF, OUTPUT);
  pinMode(SEGG, OUTPUT);
  pinMode(DIGIT1_EN, OUTPUT);
  pinMode(DIGIT2_EN, OUTPUT);
  pinMode(DIGIT3_EN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
}
