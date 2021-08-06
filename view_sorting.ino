#define ARRAY_SIZE 5
#define MAX_VALUE 7
#define MAX_BITS 3
#define CHANGED_BUZZ 0
#define SORTED_BUZZ 1
#define MOVEMENT_INTV 400

// structure that represents a number in the array
typedef struct Number {
  unsigned char pins[MAX_BITS];
  unsigned char pinsState[MAX_BITS];
  unsigned char value;
} Number;

Number n0 = {{11, 12, 13}, -1};
Number n1 = {{8, 9, 10}, -1};
Number n2 = {{5, 6, 7}, -1};
Number n3 = {{2, 3, 4}, -1};
Number n4 = {{A0, A1, A2}, -1};

// array of numbers that will be presented using LEDs connected to an Arduino UNO board
typedef struct NumberArray {
  Number a[ARRAY_SIZE] = {n0, n1, n2, n3, n4};
} NumberArray;

// clears an array of Number structs
void clearNumArray(Number arr[]) {
  for (char i = 0; i < ARRAY_SIZE; i++)
    arr[i].value = -1;
}

// blinks all the LEDs associated to a Number variable
//fix this function
void blinkNum(Number num) { // I don't think this is worth a for or while loop.
  unsigned char *pinArray = num.pins;
  unsigned char *pinStateArray = num.pinsState;
  digitalWrite(pinArray[0], LOW);
  digitalWrite(pinArray[1], LOW);
  digitalWrite(pinArray[2], LOW);
  delay(50);
  digitalWrite(pinArray[0], HIGH);
  digitalWrite(pinArray[1], HIGH);
  digitalWrite(pinArray[2], HIGH);
  delay(50);
  digitalWrite(pinArray[0], LOW);
  digitalWrite(pinArray[1], LOW);
  digitalWrite(pinArray[2], LOW);
  delay(50);
  digitalWrite(pinArray[0], HIGH);
  digitalWrite(pinArray[1], HIGH);
  digitalWrite(pinArray[2], HIGH);
  
  if (pinStateArray[0] == 1)
    digitalWrite(pinArray[0], HIGH);
  else
    digitalWrite(pinArray[0], LOW);

  if (pinStateArray[1] == 1)
    digitalWrite(pinArray[1], HIGH);
  else
    digitalWrite(pinArray[1], LOW);

  if (pinStateArray[2] == 1)
    digitalWrite(pinArray[2], HIGH);
  else
    digitalWrite(pinArray[2], LOW);
}

// buzz the buzzer
void buzz(char buzzType) {
  if (buzzType == CHANGED_BUZZ) {
    tone(A3, 500);
    delay(250);
    noTone(A3);
  } else {
    tone(A3, 500);
    delay(800);
    noTone(A3);
  }
}

// gets the bits of a number and sets the outpút pin's value accordingly
void setPinsValues(Number num) {
  unsigned char value = num.value;
  for (char i = 0; i < MAX_BITS; i++) { //the issue is located here. Check the pins
    if (bitRead(value, i) == 1) {
      digitalWrite(num.pins[i], HIGH);
      num.pinsState[i] = 1;
    } else {
      digitalWrite(num.pins[i], LOW);
      num.pinsState[i] = 0;
    }
  }
}

// sets the LED pattern for every number in the array
void setLedPattern(NumberArray numArr) {
  for (char i = 0; i < ARRAY_SIZE; i++) {
    setPinsValues(numArr.a[i]);
  }
}

// sorting algorithm
void insertion_sort(int arr[], int s) {
  for (int i = 1; i < s; i++) {
    int key = arr[i];
    int j = i - 1;
    while (j > -1 && arr[j] > key) {
      arr[j + 1] = arr[j];
      j--;
    }
    arr[++j] = key;
  }
}

// modified sorting algorithm
void insertionSortLed(Number arr[]) {
  for (char i = 1; i < ARRAY_SIZE; i++) {
    unsigned char key = arr[i].value;
    char j = i - 1;
    while (j > -1 && arr[j].value > key) {
      arr[j + 1].value = arr[j].value;
      setPinsValues(arr[j]);
      setPinsValues(arr[j + 1]);
      j--;
      buzz(CHANGED_BUZZ);
      delay(MOVEMENT_INTV);
    }
    arr[++j].value = key;
    setPinsValues(arr[j]);
    //blinkNum(arr[j]);
    buzz(SORTED_BUZZ);
    delay(MOVEMENT_INTV);
  }
}

void printNumArray(Number arr[]) {
  for (char i = 0; i < ARRAY_SIZE; i++) {
    Serial.print(arr[i].value);
    Serial.print(" ");
  }
}

// starts the sorting process
void start(NumberArray numArr) {
  setLedPattern(numArr);
  delay(3000);
  insertionSortLed(numArr.a);
}

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT); //array[0] bit 2
  pinMode(12, OUTPUT); //array[0] bit 1
  pinMode(11, OUTPUT); //array[0] bit 0
  pinMode(10, OUTPUT); //array[1] bit 2
  pinMode(9, OUTPUT); //array[1] bit 1
  pinMode(8, OUTPUT); //array[1] bit 0
  pinMode(7, OUTPUT); //...
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT); //buzzer anode
  pinMode(A4, INPUT_PULLUP); //button anode
}

void loop() {
  if (digitalRead(A4) == LOW) {
    NumberArray numArr;
    Number *arr = numArr.a;
    clearNumArray(arr);
    for (char i = 0; i < ARRAY_SIZE; i++) {
      unsigned char num = (unsigned char)random(0, 7);
      while (
           num == arr[0].value || num == arr[1].value || num == arr[2].value ||
           num == arr[3].value || num == arr[4].value
         ) {
          num = (unsigned char)random(0, 7);
      }
      arr[i].value = num;
    }
    start(numArr);
  }
}
