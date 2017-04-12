public class Producator extends Thread {
 private Produs copieProdus;
 public Producator(Produs produs){ //constructor
  copieProdus=produs;  
 }
 public void run(){
  for(int i=0;i<10;i++){ //incearca sa "creeeze" 10 produse
   copieProdus.set(i);
   try{
    sleep(500);
   }
   catch(Exception ex){
    System.out.println("Exceptie producator");
   }
  }
 }
}