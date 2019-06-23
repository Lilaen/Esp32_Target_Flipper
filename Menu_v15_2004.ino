#include <LiquidCrystal_I2C.h>
#include <Bounce2.h>

/********************************************************/
// Config of Pins, Board: NodE MCU ESP-32S v1.1 101010
/********************************************************/
#define BUTTON_PIN1 16  //Input pinnumber of Button #1 from left
#define BUTTON_PIN2 17  //Input pinnumber of Button #2 from left
#define BUTTON_PIN3 18  //Input pinnumber of Button #3 from left
#define BUTTON_PIN4 19  //Input pinnumber of Button #4 from left

#define BayA 26         //Input pinnumber for triggering Bay A Relay
#define BayB 27         //Input pinnumber for triggering Bay B Relay
/********************************************************/
// Config of LCD/ I2C 20x4
/********************************************************/
//Pins LCD,     GPIO 21 = SCL, GPIO 22 = SDA
// set the LCD number of columns and rows
int lcdColumns = 20;    //Shoud be 20
int lcdRows = 4;        //Shoud be 4
int lcdAddress = 0x27;  // if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(lcdAddress, lcdColumns, lcdRows);
/********************************************************/
// End Config of hardware
/********************************************************/

/********************************************************/
// Setup Arrays for Away/Facing/Rep
/********************************************************/
// Delay for targets
const int DelayTime = 0; //Extra delay Time in Millisec 290ms
/********************************************************/
// Times for Away [7s]
int ArrAWAY[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
int vAWAY = 6;
int AWAY = ArrAWAY[vAWAY];
int tAWAY = AWAY;
/********************************************************/
// Times for Facing [7s]=6
int ArrFACE[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 15, 20, 60, 150, 300, 360, 480};
int vFACE = 2;
int FACE = ArrFACE[vFACE];
int tFACE = FACE;
/********************************************************/
// Repetitions [5x]
int ArrREP[] = {1, 5, 6, 99};
int vREP = 1;
int REP = ArrREP[vREP];
int tREP = REP;
int RepCounter = 0;
int time_now;
/********************************************************/
// Bays
char* ArrBAY[] = {"A", "A&B", "B"}; //ArrBAY = A&B
int   vBAY = 1;
char* BAY = ArrBAY[vBAY];           //BAY = A&B, Will controll, Outputs.
char* FaceEdgeState = "Edge";
char* ProgramState  = "OverView";  //"OverView"
/********************************************************/
// End of Arrays for Away/Facing/Rep
/********************************************************/
int BonunceDelay = 50;  // interval in ms
int Val;
int tVal;
unsigned long buttonPressTimeStamp; //To be removed from final

// Instantiate a Bounce object
Bounce DebouncerButton_1 = Bounce();
Bounce DebouncerButton_2 = Bounce();
Bounce DebouncerButton_3 = Bounce();
Bounce DebouncerButton_4 = Bounce();

void setup() {

  Serial.begin(115200);
  Serial.println ("Starting");

  Serial.print("setup() running on core ");
  Serial.println(xPortGetCoreID());

  // Setup the button with an internal pull-up :
  pinMode(BUTTON_PIN1, INPUT_PULLUP);
  pinMode(BUTTON_PIN2, INPUT_PULLUP);
  pinMode(BUTTON_PIN3, INPUT_PULLUP);
  pinMode(BUTTON_PIN4, INPUT_PULLUP);

  // After setting up the button, setup the Bounce instance :
  DebouncerButton_1.attach(BUTTON_PIN1);  DebouncerButton_1.interval(BonunceDelay);
  DebouncerButton_2.attach(BUTTON_PIN2);  DebouncerButton_2.interval(BonunceDelay);
  DebouncerButton_3.attach(BUTTON_PIN3);  DebouncerButton_3.interval(BonunceDelay);
  DebouncerButton_4.attach(BUTTON_PIN4);  DebouncerButton_4.interval(BonunceDelay);

  //Setup the LED :
  pinMode(BayA, OUTPUT);
  pinMode(BayB, OUTPUT);
  TT_Face();

  // initialize LCD
  lcd.init();
  lcd.backlight();

  // 16x2 LCD Dummy Menu
  MainOverView();

}

void loop() {
  // Serial.println ("Loop");

  // Update the Bounce instance :
  DebouncerButton_1.update();
  DebouncerButton_2.update();
  DebouncerButton_3.update();
  DebouncerButton_4.update();

  // Call code if Bounce fell (transition from HIGH to LOW) :
  if (DebouncerButton_1.fell()) {
    Serial.print ("BTN #1 - ");
    BTN1_Pressed();
  }
  if (DebouncerButton_2.fell()) {
    Serial.print ("BTN #2 - ");
    BTN2_Pressed();
  }
  if (DebouncerButton_3.fell()) {
    Serial.print ("BTN #3 - ");
    BTN3_Pressed();
  }
  if (DebouncerButton_4.fell()) {
    Serial.print ("BTN #4 - ");
    BTN4_Pressed();
  }

}



/*
 * * * * * * * * * * * * *
   - - Setup Of functions
 * * * * * * * * * * * * *
*/


//Button 1 Functions
void BTN1_Pressed() {
  if (ProgramState == "OverView") {
    Serial.println("Start");
    ProgramState = "Running";
    TT_Start();
  }
  else if (ProgramState == "Running") {
    Serial.println("Stop");
    MainOverView();
    RepCounter = REP + 1;
    ProgramState = "OverView";
    time_now = millis();
  }
  else if (ProgramState != "OverView" && ProgramState != "Running") {
    Serial.println("Exit");
    MainOverView();
    ProgramState = "OverView";
  }
  //return;
}
//Button 2 Functions  Menu/Enter
void BTN2_Pressed() {
  if (ProgramState == "OverView") {
    SetupMenu();
  }
  else if (ProgramState == "Menu_Away") {
    //MainOverView();
    ProgramState = "Menu_Face";
    Serial.println(ProgramState);
  }
  else if (ProgramState == "Menu_Face") {
    //MainOverView();
    ProgramState = "Menu_Rep";
    Serial.println(ProgramState);
  }
  else if (ProgramState == "Menu_Rep") {
    //MainOverView();
    ProgramState = "Menu_Away";
    Serial.println(ProgramState);
  }
  //return;
}
//Button 3 Functions
void BTN3_Pressed() {
  if (ProgramState == "OverView") {
    Serial.print("BaySel");
    BaySel();
  }
  else if (ProgramState == "Menu_Away" || ProgramState == "Menu_Face" || ProgramState == "Menu_Rep" ) {
    Serial.print("Inc ");
    Val++;
    Serial.println(Val);
  }
  //return;
}
//Button 4 Functions
void BTN4_Pressed() {
  if (ProgramState == "OverView") {
    Serial.print ("Face-Edge: ");
    FaceEdge();
  }
  else if (ProgramState == "Menu_Away" || ProgramState == "Menu_Face" || ProgramState == "Menu_Rep" ) {
    Serial.print ("Dec ");
    Val--;
    Serial.println(Val);
  }
  //return;
}

//Lcd  Main Over View
void MainOverView() {
  Serial.println ("lcd.clear()");
  lcd.clear();

  lcd.setCursor(11, 0);  //Wifi
  lcd.print("|WiFi:"); lcd.setCursor(18, 0); lcd.print("OK");
  lcd.setCursor(11, 1);  //Wifi
  lcd.print("|"); lcd.print("TargetCtr");

  lcd.setCursor(0, 0);  //Away
  lcd.print("Away:"); lcd.setCursor(6, 0); lcd.print(AWAY); lcd.print("s");
  lcd.setCursor(0, 1);  //Face
  lcd.print("Face:"); lcd.setCursor(6, 1); lcd.print(FACE); lcd.print("s");
  lcd.setCursor(0, 2);  //Rep
  lcd.print("Rep :"); lcd.setCursor(6, 2); lcd.print(REP); lcd.print("x");

  lcd.setCursor(11, 2);  //Bay
  lcd.print("|Bay:"); lcd.setCursor(17, 2); lcd.print(BAY);

  lcd.setCursor(0, 3);  //BTN Labels

  if (ProgramState == "Menu_Away" || ProgramState == "Menu_Face" || ProgramState == "Menu_Rep" ) {
    lcd.print("Exit |Enter|Inc|Dec");
  }
  else {
    lcd.print("Start|Setup|Bay|");
    lcd.setCursor(16, 3);  //BTN Face/Edge Label
    lcd.print(FaceEdgeState);
  }
}


//Lcd  Rep Cont
void Lcd_Rep_Cont() {
  lcd.setCursor   (12, 3);
  lcd.print       (RepCounter);
  lcd.print       (" of ");
  lcd.print       (REP);
  //  lcd.print       ("  ");
  Serial.print    (RepCounter);
  Serial.print    (" / ");
  Serial.println  (REP);
  //return;
}
// Trigger Face
void TT_Face() {
  if (BAY == "A")  {
    digitalWrite(BayA, HIGH);
  }
  if (BAY == "B")  {
    digitalWrite(BayB, HIGH);
  }
  if (BAY == "A&B") {
    digitalWrite(BayA, HIGH);
    digitalWrite(BayB, HIGH);
  }
  FaceEdgeState = "Edge";
  if (ProgramState != "Running") {
    lcd.setCursor(16, 3);
    lcd.print(FaceEdgeState);
    Serial.println  (FaceEdgeState);
  }
}
// Trigger Edge
void TT_Edge() {
  if (BAY == "A")  {
    digitalWrite(BayA, LOW);
  }
  if (BAY == "B")  {
    digitalWrite(BayB, LOW);
  }
  if (BAY == "A&B") {
    digitalWrite(BayA, LOW);
    digitalWrite(BayB, LOW);
  }
  FaceEdgeState = "Face";
  if (ProgramState != "Running") {
    lcd.setCursor(16, 3);
    lcd.print(FaceEdgeState);
    Serial.println  (FaceEdgeState);
  }
}

// Flip From Face to Edge
void FaceEdge() {
  if (ProgramState != "Running") {
    if (FaceEdgeState == "Edge") {
      TT_Edge();
      return;
    }
    else if (FaceEdgeState == "Face"); {
      TT_Face();
      return;
    }
  }
}

// Target Timer Start
void TT_Start() {
  ProgramState = "Running";

  Serial.println  ("AWAY / Face / Rep");
  Serial.print    (AWAY);
  Serial.print    (" / ");
  Serial.print    (FACE);
  Serial.print    (" / ");
  Serial.println  (REP);

  TT_Edge();
  RepCounter = 0;
  lcd.setCursor   (0, 3);
  lcd.print       ("                    "); // Clear Line #4
  lcd.setCursor(0, 3);  //BTN Labels
  lcd.print("Stop | Rep:");
  while (RepCounter < REP) {
    time_now = millis();
    RepCounter = RepCounter + 1;
    Lcd_Rep_Cont();
    time_now = (time_now + (AWAY * 1000) + DelayTime);
    do {
      loop();
    } while (millis() < time_now);
    if (ProgramState == "Running") {
      TT_Face();
      time_now = millis();
      time_now = (time_now + (FACE * 1000) + DelayTime);
    }
    do {
      loop();
    } while (millis() < time_now);
    if (ProgramState == "Running") {
      TT_Edge();
    }
  }
  Serial.println("Squence Ended");
  ProgramState = "OverView";
  MainOverView();
  //return;
}

// Select Bays (BTN 3 Triggers)
void BaySel() {
  vBAY++;
  if (vBAY > ((sizeof(ArrBAY) / sizeof(int)) - 1)) {
    vBAY = 0;
  }
  BAY = ArrBAY[vBAY];
  lcd.setCursor(17, 2); lcd.print("   ");
  lcd.setCursor(17, 2); lcd.print(BAY);
  Serial.println("Bay");
  //return;
}


void SetupMenu() {
  ProgramState = "Menu_Away";
  MainOverView();
  Menu_Away();
  MainOverView();
}

// Logic for Menu Away
void Menu_Away() {
  if (ProgramState != "OverView") {
    ProgramState = "Menu_Away";
    if (ProgramState != "OverView") {
      // lcd.setCursor(0, 3);  //BTN Labels
      // lcd.print("Exit |Enter|Inc|Dec");

      lcd.setCursor(5, 0); lcd.print(">"); //Sets a > In front of Away
      Val = vAWAY;
      tVal = Val;
      while (ProgramState == "Menu_Away") {
        do {
          loop();
        } while (Val == tVal && ProgramState == "Menu_Away");
        if (Val < 0) {
          Val = ((sizeof(ArrAWAY) / sizeof(int)) - 1); // IF Val goes under 0
        }
        if (Val > ((sizeof(ArrAWAY) / sizeof(int)) - 1)) {
          Val = 0; // IF Val goes under 0
        }
        tAWAY = ArrAWAY[Val];
        tVal = Val;
        Serial.print ("New Val "); Serial.println(Val);
        lcd.setCursor(6, 0); lcd.print(tAWAY); lcd.print("s ");
        // LCD WIFI Status refresh.
      }
      if (ProgramState != "OverView") {
        vAWAY = Val;
        AWAY = ArrAWAY[vAWAY];
        lcd.setCursor(5, 0); lcd.print(" "); //Removes > In front of Away
        Menu_Face();
      }
    }
  }
  //  MainOverView();
}// End Menu Away

// Logic for Menu Face
void Menu_Face() {
  if (ProgramState != "OverView") {
    ProgramState == "Menu_Face";
    if (ProgramState != "OverView") {

      lcd.setCursor(5, 1); lcd.print(">"); //Sets a > In front of Face
      Val = vFACE;
      tVal = Val;
      while (ProgramState == "Menu_Face") {
        do {
          loop();
        } while (Val == tVal && ProgramState == "Menu_Face");
        if (Val < 0) {
          Val = ((sizeof(ArrFACE) / sizeof(int)) - 1); // IF Val goes under 0
        }
        if (Val > ((sizeof(ArrFACE) / sizeof(int)) - 1)) {
          Val = 0; // IF Val goes under 0
        }
        tFACE = ArrFACE[Val];
        tVal = Val;
        Serial.print ("New Val "); Serial.println(Val);
        lcd.setCursor(6, 1); lcd.print(tFACE); lcd.print("s  "); lcd.setCursor(11, 1); lcd.print("|");
        // LCD WIFI Status refresh.
      }
      if (ProgramState != "OverView") {
        vFACE = Val;
        FACE = ArrFACE[vFACE];
        lcd.setCursor(5, 1); lcd.print(" "); //Removes > In front of Face
        Menu_Rep();
      }
    }
  }
} // End Menu Face

// Logic for Menu Rep
void Menu_Rep() {
  if (ProgramState != "OverView") {
    ProgramState = "Menu_Rep";
    if (ProgramState != "OverView") {

      lcd.setCursor(5, 2); lcd.print(">"); //Sets a > In front of Rep
      Val = vREP;
      tVal = Val;
      while (ProgramState == "Menu_Rep") {
        do {
          loop();
        } while (Val == tVal && ProgramState == "Menu_Rep");
        if (Val < 0) {
          Val = ((sizeof(ArrREP) / sizeof(int)) - 1); // IF Val goes under 0
        }
        if (Val > ((sizeof(ArrREP) / sizeof(int)) - 1)) {
          Val = 0; // IF Val goes over max 0
        }
        tREP = ArrREP[Val];
        tVal = Val;
        lcd.setCursor(6, 2); lcd.print(tREP); lcd.print("x ");
        Serial.print ("New Val "); Serial.println(Val);
        // LCD WIFI Status refresh.
      }
      if (ProgramState != "OverView") {
        vREP = Val;
        REP = ArrREP[vREP];
        lcd.setCursor(5, 2); lcd.print(" "); //Removes > In front of Rep
        Menu_Away();
      }
    }
  }
}  // End Menu Rep
