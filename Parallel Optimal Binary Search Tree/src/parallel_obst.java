import java.util.Random;
import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;

class Worker extends Thread{
    int workerid, totalWorkers;
    int cost[][];
    int w[];
    int n;
    CyclicBarrier barrier;
    public Worker(int workerid, int cost[][], int w [], int totalWorkers, int n, CyclicBarrier barrier){
        this.workerid = workerid;
        this.cost = cost;
        this.w = w;
        this.totalWorkers = totalWorkers;
        this.n=n;
        this.barrier = barrier;
    }

    @Override
    public void run() {
        int s=1,j,totalCost,i,k,c;
        while (s<n){

            try {
                barrier.await();
            } catch (InterruptedException e) {
                e.printStackTrace();
            } catch (BrokenBarrierException e) {
                e.printStackTrace();
            }

            for(i=workerid;i<=n-s;i=i+totalWorkers){
                j=i+s;
                cost[i][j]=Integer.MAX_VALUE;
                totalCost = get_frequency(i,j);
                for(k=i;k<=j;k++){
                    c = cost[i][k-1] + cost[k+1][j]+totalCost;
                    if(c<cost[i][j])
                        cost[i][j]=c;

                }
            }
            s++;
        }

    }
    private int get_frequency(int start, int end){
        if(start==0)
            return w[end];
        return w[end]-w[start-1];
    }


}


public class parallel_obst  {

    public static void frequencySum(int w[], int frequency[]){
        int sum = 0;
        for(int i=1;i<=frequency.length;i++){
            sum=sum+frequency[i-1];
            w[i]=sum;
        }
    }

    public static void generateFrequency(int frequency[]){
        int low=0,high=100;
        Random r = new Random();
        for(int i=0;i<frequency.length;i++){
            frequency[i]=r.nextInt(high-low)+low;
        }
    }

    public static void main(String[] args) {
        int n=Integer.parseInt(args[0]),numThreads=Integer.parseInt(args[1]);
        int []frequency = new int[n];
        generateFrequency(frequency);
        int [][]cost = new int[n+2][n+2];
        int []w = new int [n+1];
        int i,j;
        frequencySum(w,frequency);
        for(i=1;i<=n;i++)
            cost[i][i] = frequency[i-1];
        CyclicBarrier barrier = new CyclicBarrier(numThreads);
        Worker workers[] = new Worker[numThreads];
        long t1  = System.nanoTime();
        for(i=0;i<numThreads;i++){
            workers[i]=new Worker(i+1,cost,w,numThreads,n,barrier);
            workers[i].start();
        }

        for(i=0;i<numThreads;i++){
            try {
                workers[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        long t2 = System.nanoTime();

     /*   for(i=0;i<cost.length;i++) {
            for (j = 0; j < cost.length; j++)
                System.out.print(cost[i][j]+" ");
            System.out.println();
        }*/
        System.out.println("Parallel Cost="+cost[1][n]);
        System.out.println("Parallel Time = "+(t2-t1)/Math.pow(10,9)+" seconds");
        System.out.println("In Serial");
        serial_obst s =new serial_obst(n,frequency);
        t1=System.nanoTime();
        s.perform_serial_obst();
        t2=System.nanoTime();
        System.out.println("Serial Cost = "+s.getCost());
        System.out.println("Serial Time = "+(t2-t1)/Math.pow(10,9)+" seconds");



    }
}
