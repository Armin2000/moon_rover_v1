//Left motor side
#define MIN1 5 //A motor | Forward
#define MIN2 4 //A motor | Backward
#define MINA 16 //PWM

//Right motor side
#define MIN3 14 //B motor | Forward
#define MIN4 13 //B motor | Backward
#define MINB 2 //PWM

void Forward()
  {
   digitalWrite(MIN1,HIGH);
   digitalWrite(MIN2,LOW);
   
   digitalWrite(MIN3,HIGH);
   digitalWrite(MIN4,LOW);
  }

void Backward()
  {
   digitalWrite(MIN1,LOW);
   digitalWrite(MIN2,HIGH);

   digitalWrite(MIN3,LOW);
   digitalWrite(MIN4,HIGH);
  }  

void Left()
{
  digitalWrite(MIN1,HIGH);
  digitalWrite(MIN2,LOW);

  digitalWrite(MIN3,LOW);
  digitalWrite(MIN4,HIGH); 
}

void Right()
{
  digitalWrite(MIN1,LOW);
  digitalWrite(MIN2,HIGH);

  digitalWrite(MIN3,HIGH);
  digitalWrite(MIN4,LOW); 
}

void Stop()
  {
   digitalWrite(MIN1,LOW);
   digitalWrite(MIN2,LOW);  

   digitalWrite(MIN3,LOW);
   digitalWrite(MIN4,LOW);    
  }
