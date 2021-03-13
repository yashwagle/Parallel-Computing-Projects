#include <stdio.h>
#include <mpi.h>
#include<math.h>
#include <stdlib.h>
#include<time.h>

#define RMAX 100
/* @author Yash Wagle yw5262*/

void generate_bitonic_sequence(int arr[], int n){
  int i;
  for(i=0;i<n/2;i++)
    arr[i] = i;
  for(i=n/2;i<n;i++)
    arr[i] = n -i;
}


void get_random_array(int arr[], int n){
  int i;
  for(i=0;i<n;i++)
    arr[i] = rand()%RMAX;
}


int main (argc, argv)
     int argc;
     char *argv[];
{
  const long datasize = atoi(argv[1]);
  long print_array = atoi(argv[2]);
  int rank, size;
  int n= datasize;
  int arr[datasize];
clock_t start_time,end_time;
  int currelement;
  int i,steps,nextProcess,nextElement,j,perProcess,temp,start,currentIteration,sign,power;
  int sendbuf,recievebuf;
  MPI_Status Stat;
  double cpu_time_used;
  MPI_Init (&argc, &argv);	/* starts MPI */
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);	/* get current process id */
  MPI_Comm_size (MPI_COMM_WORLD, &size);	/* get number of processes */
  const int subarraySize = datasize/size;
  int rbuf[subarraySize];
   get_random_array(arr,n); // generate the random array


    start_time = clock();
    // scatter the array to the processes
      MPI_Scatter( arr, datasize/size, MPI_INT, rbuf, datasize/size, MPI_INT, 0, MPI_COMM_WORLD);
      perProcess = datasize/size;
      start = 1;
      // Get total number of steps in each iteration
      steps = floor(log10(datasize)/log10(2));
      // convert random sequence to bitonic
      while(start<steps){
        currentIteration = start;
        power = pow(2,start);

        while(currentIteration>0){
        // for each element in the current process
        for(i=0;i<perProcess;i++){
            currelement = rank*perProcess + i;
            // get the next element
            nextElement = currelement ^ (1<<(currentIteration-1));
            // get the next process
            nextProcess = nextElement/perProcess;
            // get whether to use a -ve circuit or a +ve circuit
            sign = pow(-1,currelement/power );
        //  printf("Rank = %d current element = %d nextElement = %d nextProcess=%d currentIteration = %d sign = %d \n",rank,currelement,nextElement,nextProcess,currentIteration,sign );
            // the other element is present in  other process
            if(rank!=nextProcess){
            /*
            If current rank is lesser than the next process
            then first it will send data before receiving it
            */
              if(nextProcess>rank){
                  sendbuf = rbuf[i];
                  // Send the data to the next process
                MPI_Send(&sendbuf,1,MPI_INT, nextProcess,0,MPI_COMM_WORLD);
                // Recieve data from the next process
                MPI_Recv(&recievebuf,1,MPI_INT,nextProcess,0,MPI_COMM_WORLD,&Stat);
                if(sign<0){
                // if negative circuit keep the greater value
                // since this will be the upper output
                  if(recievebuf>rbuf[i]){
                      rbuf[i] = recievebuf;
                  }
                }
                if(sign>0){
                // if positive circuit keep the lesser value
                // since this will be the upper output
                  if(recievebuf<rbuf[i]){
                      rbuf[i] = recievebuf;
                  }
                }
              }
              else{
              /*
              if the current rank is greater than the next process
              then it will first receive before sending it
              */
                sendbuf = rbuf[i];
              //  printf("Waiting to recieve on rank =%d with nextProcess=%d \n",rank,nextProcess );
              // Receive Data
                MPI_Recv(&recievebuf,1,MPI_INT,nextProcess,0,MPI_COMM_WORLD,&Stat);
            //    printf("Waiting to send on rank =%d with nextProcess=%d \n",rank,nextProcess );
                          // Send Data
                MPI_Send(&sendbuf,1,MPI_INT, nextProcess,0,MPI_COMM_WORLD);

                if(sign<0){
               // if positive circuit keep the greater value
                  if(recievebuf<rbuf[i]){
                    rbuf[i] = recievebuf;
                  }
                }
                if(sign>0){
                  if(recievebuf>rbuf[i]){
                    rbuf[i] = recievebuf;
                  }
                }
              }

            }
            else{
            // The next element belongs to the same process
              nextElement = nextElement - rank*perProcess;
              if(rbuf[i]>rbuf[nextElement] && i<nextElement && sign>0){
                  temp = rbuf[i];
                  rbuf[i] = rbuf[nextElement];
                  rbuf[nextElement] = temp;
              }
              else if(rbuf[i]<rbuf[nextElement] && i<nextElement && sign<0){
                temp = rbuf[i];
                rbuf[i] = rbuf[nextElement];
                rbuf[nextElement] = temp;
              }
            }

        }
        currentIteration--;
      }
      start++;
      }
    // convert bitonic sequence to sorted sequence
    steps = floor(log10(datasize)/log10(2));
    perProcess = datasize/size;
    for(i=steps;i>0;i--){
        for(j=0;j<datasize/size;j++){
          currelement = rank*perProcess + j;
          nextElement = currelement ^ (1<<(i-1));
          nextProcess = nextElement/perProcess;

            if(nextProcess!=rank){
                 if(nextProcess>rank){
                 /*similar logic if current rank is lesser than the next process
                 then first it will send data before receiving it*/
                  sendbuf = rbuf[j];
                  MPI_Send(&sendbuf,1,MPI_INT, nextProcess,0,MPI_COMM_WORLD);

                   MPI_Recv(&recievebuf,1,MPI_INT,nextProcess,0,MPI_COMM_WORLD,&Stat);
                   // only positive circuit so the process will take the smaller value
                   if(recievebuf<sendbuf)
                       rbuf[j]=recievebuf;
                    }
                else{
                    /* the next process has rank less than the current process*/
                    sendbuf = rbuf[j];
                    MPI_Recv(&recievebuf,1,MPI_INT,nextProcess,0,MPI_COMM_WORLD,&Stat);
                    MPI_Send(&sendbuf,1,MPI_INT, nextProcess,0,MPI_COMM_WORLD);

                    if(recievebuf>sendbuf)
                        rbuf[j] = recievebuf;
                    }
            }
            else{
              nextElement = nextElement - rank*perProcess;
              if(rbuf[j]>rbuf[nextElement] && j<nextElement){
                  temp = rbuf[j];
                  rbuf[j] = rbuf[nextElement];
                  rbuf[nextElement] = temp;
              }


            }


        }



    }

  MPI_Gather(rbuf,perProcess,MPI_INT,arr,perProcess,MPI_INT,0,MPI_COMM_WORLD);
  end_time = clock();
   cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;

  if(rank==0){
    printf("time required %lf\n", cpu_time_used );
    if(print_array==1){
    printf("Final array\n" );
    for(i=0;i<datasize;i++)
      printf("%d ",arr[i] );
    }
  }

  MPI_Finalize();
  return 0;
}
