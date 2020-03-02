
float cf = 19.5; // calibration factor
int ffs0 = A0; // FlexiForce sensor is connected analog pin A0
int ffs1 = A1; // FlexiForce sensor is connected analog pin A1
int ffs2 = A2; // FlexiForce sensor is connected analog pin A2
int ffs3 = A3; // FlexiForce sensor is connected analog pin A3
int ffs4 = A4; // FlexiForce sensor is connected analog pin A4
int ffs5 = A5; // FlexiForce sensor is connected analog pin A5
int ffsdata[6] = {0, 0, 0, 0, 0, 0};
float vout[6] = {0, 0, 0, 0, 0, 0};
void setup()
{
  Serial.begin(9600);
  pinMode(ffs0, INPUT);
  pinMode(ffs1, INPUT);
  pinMode(ffs2, INPUT);
  pinMode(ffs3, INPUT);
  pinMode(ffs4, INPUT);
  pinMode(ffs5, INPUT);
}
  void loop()
  {
    ffsdata[0] = analogRead(ffs0);
    ffsdata[1] = analogRead(ffs1);
    ffsdata[2] = analogRead(ffs2);
    ffsdata[3] = analogRead(ffs3);
    ffsdata[4] = analogRead(ffs4);
    ffsdata[5] = analogRead(ffs5);
    for (int i = 0; i < 3; i++) {
      vout[i] = ((ffsdata[i])*100); // 1023.0;
      vout[i] = vout[i] * cf;
      Serial.print("Flexi Force sensor ");
      Serial.print(i);
      Serial.print(": ");
      Serial.print(vout[i], 3);
      Serial.println("");
    }
    float totalPalm = vout[0] + vout[1] + vout[2]+1;// + vout[3];
//    float totalTop = vout[4] + vout[5];
//    Serial.print("Total Palm Reading: ");
//    Serial.print(totalPalm, 3);
//    Serial.println("");
//    Serial.print("Total Top of Hand Reading: ");
//    Serial.print(totalTop, 3);
//    Serial.println("");
    delay(100);
  }
