import java.util.Random;
import java.util.TreeSet;

public class serial_obst {
    TreeSet<>
    int n;
    int cost[][];
    int frequency[];
    int w[];
   public serial_obst(int n,int frequency[]){
        this.n=n;
        this.frequency =frequency;
        this.w = new int[n+1];
        this.cost = new int[n+2][n+2];
   }

    public void frequencySum(){
        int sum = 0;
        for(int i=1;i<=frequency.length;i++){
            sum=sum+frequency[i-1];
            w[i]=sum;
        }
    }

    public static int get_frequency(int w[],int start, int end){
        if(start==0)
            return w[end];
        return w[end]-w[start-1];
    }

    public void perform_serial_obst(){
       int s,i,j,k,totalCost,c;
       frequencySum();
        for(i=1;i<=n;i++)
            cost[i][i] = frequency[i-1];
        for (s=1;s<n;s++){
            for(i=1;i<=n-s;i++){
                j = i + s;
                cost[i][j]=Integer.MAX_VALUE;
                totalCost = get_frequency(w,i,j);
                for(k=i;k<=j;k++){
                    //      System.out.println(i+" "+j+" "+k);
                    c=cost[i][k-1]+cost[k+1][j]+totalCost;
                    //     System.out.println(c);
                    if(c<cost[i][j]){
                        cost[i][j]=c;
                    }
                }

            }

        }
    }

    public void displayMatrix() {
        for (int i = 0; i < cost.length; i++) {
            for (int j = 0; j < cost.length; j++)
                System.out.print(cost[i][j] + " ");
            System.out.println();
        }
    }

    public int getCost(){
       return cost[1][n];
    }

    public static void generateFrequency(int frequency[]){
        int low=0,high=100;
        Random r = new Random();
        for(int i=0;i<frequency.length;i++){
            frequency[i]=r.nextInt(high-low)+low;
        }
    }


    public static void main(String[] args) {
        int n=Integer.parseInt(args[0]);
        int frequency[] = new int [n];
        generateFrequency(frequency);
        serial_obst s= new serial_obst(n,frequency);
        s.perform_serial_obst();
        System.out.println("Serial Cost = "+s.getCost());









    }
}
