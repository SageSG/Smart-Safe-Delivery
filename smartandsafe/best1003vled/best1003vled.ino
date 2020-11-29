////Smart & Safe Delivery////

const int trigPin = 9;
const int echoPin = 10;
const int buzz = 8;
const int led = 5;
const int success = 3;
long duration;
int distance;
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(led, OUTPUT);
  pinMode(success, OUTPUT);
  Serial.begin(9600);
}
// the loop function runs over and over again forever
void loop()
{
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;

  // Prints the distance on the Serial Monitor
  if (distance >= 400 || distance <= 2)
  {
    Serial.print("Distance = ");
    Serial.println("No Object In Range");
  }
  else
  {
    Serial.print("Distance = ");
    Serial.print(distance);
    Serial.println(" cm");
    delay(250);
    
  }

  //Collision detected
  if (distance <= 20)
  {
    digitalWrite(success,LOW);
    digitalWrite(led, HIGH);
    tone(buzz, 2000);
    delay(100);
    noTone(buzz);
    delay(100);
    
    tone(buzz, 2000);
    delay(100);
    noTone(buzz);
    delay(100);
    
    tone(buzz, 2000);
    delay(100);
    noTone(buzz);
    
    tone(buzz, 2000);
    delay(100);
    noTone(buzz);
    delay(100);
  }
  else
  {
    digitalWrite(led,LOW);
    digitalWrite(success, HIGH);
  }
}
