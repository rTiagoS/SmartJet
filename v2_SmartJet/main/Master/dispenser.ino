/* Atuadores ------------------------------------ */

void ligaBomba()
{
  // Liga o pino da bomba
  digitalWrite(pinoBomba, HIGH);
}

void desligaBomba()
{
  // Desliga o pino da bomba
  digitalWrite(pinoBomba, LOW);
}


/* Sensor de Aproximidade ---------------------  */
void sensorAproximidade()
/*  Caso seja detectado uma mão, a bomba é acionada por alguns segundos. A calibração 
 *  desse tempo é importante para expelir uma quantidade suficiente, em ml, do líquido.
 *  Informações são enviadas ao broker. 
 *  Existe uma trégua de alguns segundos para um novo acionamento.
 */
{
  delay(800); // Aguarda um tempo para confirmar que tem uma mão aguardando jato de gel.
  
  
  if(digitalRead(pinoProximidade) == LOW) // Se uma mão for detectada, então:
  {
    if (qtde_Acionamentos <= 6)
    {
      ligaBomba(); // Aciona a bomba por um tempo máximo definido.
      delay(1500); // Mantém a bomba acionada por um tempo.
      enviaBuffer(qtde_Acionamentos);
      
      desligaBomba();
      qtde_Acionamentos +=1;
      delay(1000);
    }   
  }
} // Fim da função sensorAproximidade().
