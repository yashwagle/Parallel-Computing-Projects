/* @author Yash Wagle yw5262*/
#include <stdio.h>
#include <mpi.h>
#include<math.h>
#include <stdlib.h>
#include<time.h>

#define RMAX 100

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
   get_random_array(arr,n); /* generate the random array */


    start_time = clock();
      MPI_Scatter( arr, datasize/size, MPI_INT, rbuf, datasize/size, MPI_INT, 0, MPI_COMM_WORLD);
      perProcess = datasize/size;
      start = 1;

      steps = floor(log10(datasize)/log10(2));
      while(start<steps){
        currentIteration = start;
        power = pow(2,start);
        while(currentIteration>0){
        for(i=0;i<perProcess;i++){
            currelement = rank*perProcess + i;
            nextElement = currelement ^ (1<<(currentIteration-1));
            nextProcess = nextElement/perProcess;
            sign = pow(-1,currelement/power );
        //  printf("Rank = %d current element = %d nextElement = %d nextProcess=%d currentIteration = %d sign = %d \n",rank,currelement,nextElement,nextProcess,currentIteration,sign );

            if(rank!=nextProcess){
              if(nextProcess>rank){
                  sendbuf = rbuf[i];
                  //  printf("Waiting to send on rank =%d with nextProcess=%d \n",rank,nextProcess );
                MPI_Send(&sendbuf,1,MPI_INT, nextProcess,0,MPI_COMM_WORLD);
              //  printf("Waiting to recieve on rank =%d with nextProcess=%d \n",rank,nextProcess );

                MPI_Recv(&recievebuf,1,MPI_INT,nextProcess,0,MPI_COMM_WORLD,&Stat);
                if(sign<0){
                  if(recievebuf>rbuf[i]){
                      rbuf[i] = recievebuf;
                  }
                }
                if(sign>0){
                  if(recievebuf<rbuf[i]){
                      rbuf[i] = recievebuf;
                  }
                }
              }
              else{
                sendbuf = rbuf[i];
              //  printf("Waiting to recieve on rank =%d with nextProcess=%d \n",rank,nextProcess );
                MPI_Recv(&recievebuf,1,MPI_INT,nextProcess,0,MPI_COMM_WORLD,&Stat);
            //    printf("Waiting to send on rank =%d with nextProcess=%d \n",rank,nextProcess );
                MPI_Send(&sendbuf,1,MPI_INT, nextProcess,0,MPI_COMM_WORLD);

                if(sign<0){
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

    steps = floor(log10(datasize)/log10(2));
    perProcess = datasize/size;
    for(i=steps;i>0;i--){
        for(j=0;j<datasize/size;j++){
          currelement = rank*perProcess + j;
          nextElement = currelement ^ (1<<(i-1));
          nextProcess = nextElement/perProcess;

            if(nextProcess!=rank){
                 if(nextProcess>rank){
              sendbuf = rbuf[j];
              MPI_Send(&sendbuf,1,MPI_INT, nextProcess,0,MPI_COMM_WORLD);

              MPI_Recv(&recievebuf,1,MPI_INT,nextProcess,0,MPI_COMM_WORLD,&Stat);
              if(recievebuf<sendbuf)
                rbuf[j]=recievebuf;
              }
              else{
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
