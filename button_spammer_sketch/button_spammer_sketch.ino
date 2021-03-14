#define MAX_SCORE 2
#define PLAY_TIME 2.0

#define switchPinPlayer1 7
#define switchPinPlayer2 8

#define ledPinPlayer1 12
#define ledPinPlayer2 13
#define ledPinRuller  2

#define BLINKING_SPEED 1000

int previousGameTime = 0;
float lastBlinkingTime = 0;

typedef struct s_player {
  int switchPin;
  int ledPin;
  int score;

  bool clicked;
} t_player;

t_player players[2] = {
  (t_player) {switchPinPlayer1, ledPinPlayer1, 0, false},
  (t_player) {switchPinPlayer2, ledPinPlayer2, 0, false}
};

void declarePins(t_player *player)
{
  pinMode(player->switchPin, INPUT);
  pinMode(player->ledPin, OUTPUT);
}

void setup()
{
  declarePins(&players[0]);
  declarePins(&players[1]);

  pinMode(ledPinRuller, OUTPUT);
}

void loop()
{
  float loopTime = millis() - previousGameTime;

  // Serial.println(loopTime);

  if (loopTime / 1000 < PLAY_TIME) {
    readSwitchPins(&players[0]);
    readSwitchPins(&players[1]);

    displayTimeLeft(loopTime);
  } else {
    digitalWrite(players[0].ledPin, LOW);
    digitalWrite(players[1].ledPin, LOW);
    digitalWrite(ledPinRuller, HIGH);

    // can be transformed into reset function.
    if (players[0].score > players[1].score)
      digitalWrite(players[0].ledPin, HIGH);
    else if (players[0].score < players[1].score)
      digitalWrite(players[1].ledPin, HIGH);
    else {
      digitalWrite(players[0].ledPin, HIGH);
      digitalWrite(players[1].ledPin, HIGH);
    }

    waitForReset();
  }
}

void waitForReset()
{
  while (true) {
    int val1 = digitalRead(players[0].switchPin);
    int val2 = digitalRead(players[1].switchPin);

    if (val1 == HIGH || val2 == HIGH)
      break;
  }

  players[0].score = 0;
  players[1].score = 0;
  lastBlinkingTime = 0;
  previousGameTime = millis();
}

void displayTimeLeft(float loopTime)
{
  static bool state = false;
  float percentage = 100 - (loopTime / (PLAY_TIME * 1000) * 100);
  float blinkingTime = BLINKING_SPEED * (percentage / 100);

  if (loopTime - lastBlinkingTime > blinkingTime) {
    state = !state;
    lastBlinkingTime = loopTime;
    digitalWrite(ledPinRuller, state ? HIGH : LOW);
  }
}

void readSwitchPins(t_player *player)
{
  int val = digitalRead(player->switchPin); // read input value from the current button.
  
  if (val == HIGH && !player->clicked) {

    // updating score.
    player->clicked = true;
    ++player->score;
    digitalWrite(player->ledPin, HIGH);

  } else if (val == LOW) {

    // reset the click.
    player->clicked = false;
    digitalWrite(player->ledPin, LOW);
  }
}
