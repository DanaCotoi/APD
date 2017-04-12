public class Produs {
 private boolean available=false;
 private int container=-1;
 public synchronized void set(int valoare){
  while(available==true){
   try{
    wait(5000); //blocheaza firul solicitant daca avem deja creat un obiect
    //Thread.sleep(50);
   }
   catch(Exception ex){
    System.out.println("Eroare set");
   }
  }
  available=true;//pentru ca, consumatorul sa poata consuma
  container=valoare;
  System.out.println("Setare produs: "+container);
  notifyAll();//deblocheaza toate firele, in cazul de fata ofera o sansa consumatorului
 }
 public synchronized int get(){
  while(available==false){
   try{
    wait(5000);//blocheaza firul solicitant daca nu avem creat un obiect
    //Thread.sleep(50);
   }
   catch(Exception ex){
    System.out.println("Eroare get");
   }
  }
  available=false;//pentru ca, producatorul sa poata sa produca 
  System.out.println("Consumare produs: "+container);
  notifyAll();//deblocheaza toate firele, in cazul de fata ofera o sansa producatorului
  return container;
 }
}