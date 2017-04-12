
public class Main {
 public static void main(String[] args){
  Produs produs=new Produs();
  Consumator consumator=new Consumator(produs);
  Producator producator=new Producator(produs);
  producator.start();
  consumator.start();
 }
}
