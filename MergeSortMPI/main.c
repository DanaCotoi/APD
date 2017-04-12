#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

// Declaram cele 2 functii pentru sortarea vectorului

void merge(int, int, int, int, int);
void mergeSort(int, int, int);


int main(int argc, char* argv){

 int n,c;
 int arr,init_arr;
 int numtasks, size, rank, len;
 char hostname[MPI_MAX_PROCESSOR_NAME];

 // Initializam MPI

 MPI_INIT(&argc, &argv);
 MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
 MPI_Comm_rank(MPI_COMM_WORLD,&rank);

 // Impartim vectorul in mai multe bucati egale

 size = n/numtasks;

 // Trimite fiecare subvector la fiecare proces

 int *sub_arr = malloc(size * sizeof(int));
 MPI_Scatter(init_arr,size,MPI_INT,sub_arr,size,MPI_INT,0,MPI_COMM_WORLD);

 //Efectuam mergeSort pe fiecare proces

 int *tmp_arr = malloc(size * sizeof(int));
 mergeSort(sub_arr,tmp_arr,0,(size - 1));

 // Imbinam subvectorii sortati intr-unul singur

 int *sorted = NULL;
 if(rank == 0){
    sorted = malloc(n * sizeof(int));
 }

 MPI_Gather(sub_arr,size,MPI_INT,sorted,size,MPI_INT,0,MPI_COMM_WORLD);

 // Facem ultimul apel mergeSort

 if(rank == 0){

    int *alt_arr =malloc(n * sizeof(int));
    mergeSort(sorted,alt_arr, 0, (n - 1));
 }

 // Afisam vectorul sortat

 printf("Acesta este vectorul sortat: ");
 for(c = 0; c < n; c++){
    printf("%d",sorted[c]);
 }

 //Finalizam MPI

 MPI_Barrier(MPI_COMM_WORLD);
 MPI_Finalize();

}

 //Functia mergeSort

 void mergeSort(int arr[],int low,int high)
 {
     int mid;

     if(low<high){
        mid=(low+high)/2;
        mergeSort(arr,low,mid);     //Recursivitatea la dreapta
        mergeSort(arr,mid+1,high);  //Recursivitatea la stanga
        merge(arr,low,mid,high);    //Imbinarea celor doi subvectori sortati
     }
 }

 // Functia recursiva merge

 void merge(int arr[],int low,int mid,int high)
 {
     int temp[50];    // Vector folosit pentru imbinare
     int i,m,l,k;
     l=low;            //Inceputul de la prima lista
     i=low;            //Inceputul de la a doua lista
     k=0;
     m=mid+1;

     while((l<=mid) && (m<high))
     {
         if(arr[l]<=arr[m]){
            temp[i]=arr[l];
            l++;
         }
         else{
            temp[i]=arr[m];
            m++;
         }
         i++;
     }
     if(l>mid){
        for(k=m;k<=high;k++){
            temp[i]=arr[k];
            i++;
        }
     }
     else{
        for(k=1;k<=mid;k++){
            temp[i]=arr[k];
            i++;
        }

     }
     for(k=low;k<=high;k++){
        arr[k]=temp[k];
     }

 }

