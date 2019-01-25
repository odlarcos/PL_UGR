

int main () {

    declare {
        int n,d ;
        int curr;
        int ultim ;
        int cuenta ;
        int primero ;
    }

    int fibonnaci(int n){

        declare{
            int aux1;
            int aux2;
        }

        if( n == 0){
            return 0;
        }else if( n == 1){
            return 1;

        }else{
            aux1 = fibonnaci(n-1);
            aux2 = fibonnaci(n-2);

            return aux1+aux2;
        }
    }

    int fibonnaciN(int n){
        
        declare{
            int i, result;
        }

        i = 0;

        while( i <= n){
            result = fibonnaci(i);
            printf(result);
            i = i+1;
        }
        return 0;
    }

    scanf(n);

    d = fibonnaciN(n);

}