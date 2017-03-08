#define DEBUG 1

const uint32_t TickFrequency = 200;

volatile boolean l;
volatile uint32_t WorkEnabled = 0;
uint32_t LastTime = 0, LastEndOfWorkTime = 0;
uint32_t TickWorkTime = 0, TickTotalTime = 0;
uint32_t TickCount = 0;
uint32_t i;


//TC1 ch 0
void TC3_Handler()
{
  TC_GetStatus(TC1, 0);
  
  /*Serial.print("Temps = ");
  Serial.print(micros());
  Serial.print(" ms\r");*/

  // Update Tick Count
  TickCount++;

  // Add work time
  TickWorkTime += (!WorkEnabled) ? (LastEndOfWorkTime - LastTime) :(micros() - LastTime);
  
  // Add total time
  TickTotalTime += (micros() - LastTime);

  // Check if a second has passed
  if (!(TickCount % TickFrequency))
  {
    digitalWrite(LED_BUILTIN, l = !l);
    
    // Calculate and Print %
#if DEBUG
    Serial.print("Current Time = ");
    Serial.println(micros());

    Serial.print("Work Enabled count = ");
    Serial.println(WorkEnabled);

    Serial.print("Tick Work Time = ");
    Serial.println(TickWorkTime);

    Serial.print("Tick Total Time = ");
    Serial.println(TickTotalTime);
#endif
    Serial.print(100.0*((float)TickWorkTime / (float)TickTotalTime));
    Serial.println("%");

    // Reset work time and total time
    TickWorkTime = 0;
    TickTotalTime = 0;
  }

  // Enable work
  WorkEnabled++;

  // Set lastTime value
  LastTime = micros();
}

void startTimer(Tc *tc, uint32_t channel, IRQn_Type irq, uint32_t frequency) 
{
  pmc_set_writeprotect(false);
  pmc_enable_periph_clk((uint32_t)irq);
  TC_Configure(tc, channel, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK3);
  uint32_t rc = VARIANT_MCK/32/frequency; //32 because we selected TIMER_CLOCK3 above
  TC_SetRA(tc, channel, rc/2); //50% high, 50% low
  TC_SetRC(tc, channel, rc);
  TC_Start(tc, channel);
  tc->TC_CHANNEL[channel].TC_IER=TC_IER_CPCS;
  tc->TC_CHANNEL[channel].TC_IDR=~TC_IER_CPCS;
  NVIC_EnableIRQ(irq);
}

void setup()
{
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(9600);
  while (!Serial) 
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  startTimer(TC1, 0, TC3_IRQn, TickFrequency); //TC1 channel 0, the IRQ for that channel and the desired frequency
}

void loop()
{
  // Wait for work to be enable
  while (!WorkEnabled);
  
  // Do some work
  for(i=0; i<15000; i++);
  
  // Register time at end of work 
  LastEndOfWorkTime = micros();

  // Reduce work enable
  WorkEnabled--;
}
