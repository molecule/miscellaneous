
int timer0_count=0;

// Timer0 compare interrupt service routine
SIGNAL(TIMER0_COMPA_vect)
{
  // Toggle LED
  if(timer0_count++>1000)
  {
    timer0_count=0;
    digitalWrite(13, digitalRead(13)^1);
  }
}


