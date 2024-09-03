// Define pins for the ultrasonic sensor
const int trigPin = 9;
const int echoPin = 10;

// Variables for storing duration, distance, and user inputs
long duration;
float distance;
const int arraySize = 3;  // Size of the array to store user inputs
float userInputs[arraySize];  // Array to store user inputs
float adjustedDistances[arraySize];  // Array to store adjusted distances

const float minValue = 0.1;  // Minimum valid input value in cm
const float maxValue = 400.0; // Maximum valid input value in cm
const unsigned long inputTimeout = 30000; // Timeout in milliseconds (30 seconds)

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Set pin modes
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Prompt the user to enter distance values
  Serial.println("Enter three positive distance values between 0.1 cm and 400 cm:");
}

void loop() {
  for (int i = 0; i < arraySize; i++) {
    Serial.print("Enter distance value ");
    Serial.print(i + 1);
    Serial.print(": ");

    unsigned long startTime = millis();
    while (millis() - startTime < inputTimeout) {
      if (Serial.available() > 0) {
        userInputs[i] = Serial.parseFloat();

        // Check if the input is within the valid range
        if (userInputs[i] >= minValue && userInputs[i] <= maxValue) {
          Serial.print("User input value ");
          Serial.print(i + 1);
          Serial.print(": ");
          Serial.println(userInputs[i]);

          // Measure the distance using the ultrasonic sensor
          digitalWrite(trigPin, LOW);
          delayMicroseconds(2);
          digitalWrite(trigPin, HIGH);
          delayMicroseconds(10);
          digitalWrite(trigPin, LOW);

          duration = pulseIn(echoPin, HIGH);
          distance = (duration / 2.0) * 0.0344;
          adjustedDistances[i] = distance + userInputs[i];

          Serial.print("Distance to target is: ");
          Serial.print(distance);
          Serial.println(" cm");

          Serial.print("Distance to target (adjusted by user input) is: ");
          Serial.print(adjustedDistances[i]);
          Serial.println(" cm");

          break; // Exit the input loop for this index
        } else {
          Serial.println("Invalid input. Please enter a number between 0.1 cm and 400 cm.");
          Serial.flush(); // Clear any remaining input
          startTime = millis(); // Restart the timer for the next input attempt
        }
      }
    }

    if (millis() - startTime >= inputTimeout) {
      Serial.println("Input timeout. Please restart the program to enter values.");
      return; // Exit loop if timeout occurs
    }
  }

  // Print all adjusted distances after all inputs have been entered
  Serial.println("Summary of adjusted distances:");
  for (int i = 0; i < arraySize; i++) {
    Serial.print("Adjusted distance ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(adjustedDistances[i]);
    Serial.println(" cm");
  }

  // Wait before allowing another round of inputs
  delay(5000);  // Delay to allow the user to read the output before the next loop
}
