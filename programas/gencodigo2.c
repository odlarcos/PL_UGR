
int main () {

  declare {
    int n,d ;
    int curr;
    int ultim ;
    int cuenta ;
    int primero ;
  }
  curr = 2 ;
  ultim = 0 ; 
  cuenta = 0 ;
  primero = 1 ;

  printf("Introduce numero: ") ;
  scanf(n) ;
  printf(n) ;
  curr = 2 ;

  while (curr <= n) {

    d = n / curr ;

    if ( d * curr == n ) {

      if ( curr != ultim ) {
        ultim = curr ;
        cuenta = 1 ;
      } else {
        cuenta = cuenta + 1 ;
      }

      n = n/curr;

    }else {

      if ( cuenta > 0 ) {

        if ( primero == 0) printf("*") ;
        primero = 0 ; printf(curr) ;

        if ( cuenta > 1 ) printf(cuenta) ;
      }

      curr = curr+1;
      cuenta = 0;
    }
  }

  if ( cuenta > 0 ) {
    
    if ( primero == 0 ) {
      printf("*") ;
    }
    primero = 0;
    printf(curr) ;
    if ( cuenta > 1 ) {
      printf(cuenta) ;
    }
  }

  printf("\n") ;
}