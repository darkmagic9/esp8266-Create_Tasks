#define RED     6
#define YELLOW  5
#define SW1     7
#define SW2     8

QueueHandle_t BlinkQueue;

void setup()
{
  Serial.begin(9600);
  BlinkQueue =  xQueueCreate(5, sizeof(int32_t));


  xTaskCreate(vSenderTask, "Sender Task 1", 100, SW1, 1, NULL);
  xTaskCreate(vSenderTask, "Sender Task 1", 100, SW2, 1, NULL);
  xTaskCreate(vReceiverTask, "Receiver Task", 100, NULL, 1, NULL);

}

void vSenderTask(void *pvParameters)
{
  BaseType_t qStatus;
  int32_t valueToSend = 0;
  int SW = (int32_t)pvParameters;
  pinMode(SW, INPUT);
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);
  while (1)
  {
    if (digitalRead(SW) == HIGH)
      valueToSend = SW;
    else
      valueToSend = 0;
    if (valueToSend != 0) {
      qStatus = xQueueSend(BlinkQueue, &valueToSend, xTicksToWait);
      if (qStatus !=  pdPASS)
      {
        Serial.println("Could not send  to the queue");
      }
      vTaskDelay(100);
    }
  }
}

void vReceiverTask(void *pvParameters)
{
  int32_t valueReceived;
  BaseType_t qStatus;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);
  pinMode(RED, OUTPUT); digitalWrite(RED, HIGH);
  pinMode(YELLOW, OUTPUT); digitalWrite(YELLOW, HIGH);
  while (1)
  {
    qStatus = xQueueReceive(BlinkQueue, &valueReceived, xTicksToWait);
    if (qStatus  != pdPASS)
    {
      Serial.println("Could not receive from queue ");
    }
    else
    {
      Serial.print("Received value  : ");
      Serial.println(valueReceived);
      if (valueReceived == 7) {
        digitalWrite(RED, LOW);
        vTaskDelay(100);
        digitalWrite(RED, HIGH);
      } else if (valueReceived == 8) {
        digitalWrite(YELLOW, LOW);
        vTaskDelay(100);
        digitalWrite(YELLOW, HIGH);
      } else {
        vTaskDelay(1);
      }
    }
  }
}
void loop() {}
