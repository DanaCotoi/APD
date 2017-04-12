public class Consumator extends Thread{
 private Produs copieProdus;
 public Consumator(Produs produs){
  copieProdus=produs;
 }
 public void run(){
  for(int i=0;i<10;i++){
   copieProdus.get(); //incearca sa "consume" 10 produse
   try{
    sleep(500);
   }
   catch(Exception ex){
    System.out.println("Exceptie Consumator");
   }
  }
 }
}